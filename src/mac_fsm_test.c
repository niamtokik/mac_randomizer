/* Copyright 2026 Mathieu Kerjouan
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "minunit.h"
#include "mac.h"
#include "mac_test_helper.h"

/*********************************************************************
 * test handler
 * ------------
 *
 * A simple test to check the function callbacks.
 *
 ********************************************************************/
state_t *dumb_callback(input_t *i, output_t *o, state_t *s) {
  int32_t *input = i->input;
  switch (*input) {
    case 0x01: return fsm_status_error("error", s);
    case 0x02: return fsm_status_unknown(s);
    default: return fsm_status_ok(s);
  }
}

MU_TEST(test_fsm_handlers) {
  input_t si;
  output_t so;
  state_t s;
  fsm_init(&si, &so, &s);

  // input initialization
  int32_t i;
  si.input = &i;
  si.input_length = sizeof(int32_t);

  // handler initialization
  fsm_state_handler(dumb_callback, &s);

  // set the input to 1 (error)
  int ret = 0;
  i = 1;
  ret = fsm_loop(&si, &so, &s);
  mu_assert(ret == -1, "should return an error");
  mu_assert(s.status == ERROR, "should be set to error");

  // set the input to 2 (unknown)
  i = 2;
  ret = fsm_loop(&si, &so, &s);
  mu_assert(ret == -1, "should return an unkonwn state");
  mu_assert(s.status == UNKNOWN, "should be set to unknown");

  // set the input to 0 (ok)
  i = 0;
  ret = fsm_loop(&si, &so, &s);
  mu_assert(ret == 0, "should be okay");
  mu_assert(s.status == OK, "should be OK");

  // TODO: check EVAL_ERROR status
  // TODO: check transition
}

MU_TEST_SUITE(test_fsm_handlers_suite) {
  MU_RUN_TEST(test_fsm_handlers);
}

/*********************************************************************
 * fsm counter
 * -----------
 *
 * a simple counter incremented on every computation.
 *
 ********************************************************************/
state_t *counter(input_t *i, output_t *o, state_t *s) {
  int *input = i->input;
  int *output = o->output;
  *input += 1;
  *output = *input;
  if (*input <255) return fsm_status_continue(s);
  if (*input == 255) return fsm_status_ok(s);
  return fsm_status_error("undefined", s);
}

MU_TEST(test_fsm_counter) {
  // allocate input, output and state on the stack, then
  // sanitize them using fsm_init() function.
  input_t si;
  output_t so;
  state_t s;
  fsm_init(&si, &so, &s);

  // set input value
  int data_input = 0;
  si.input= &data_input;
  si.input_length = sizeof(int);

  // set output value
  int output = 0;
  so.output = &output;
  so.output_length = sizeof(int);

  // set the handler
  fsm_state_handler(counter, &s);

  // start the fsm
  int ret = fsm_loop(&si, &so, &s);

  mu_assert(ret == 0, "fsm_start return code issue");
  mu_assert(data_input == 255, "counter issue");
  mu_assert(*(int *)si.input == 255, "input issue");
  mu_assert(*(int *)so.output == 255, "output issue");
  mu_assert(s.status == OK, "status issue");
}

MU_TEST_SUITE(test_fsm_counter_suite) {
  MU_RUN_TEST(test_fsm_counter);
}

/*********************************************************************
 * file descriptor test
 * --------------------
 *
 * the idea is to always ensure a file descriptor is being closed
 * after its used by a teardown function in case of success or by
 * calling the error callback function in case of error.
 *
 ********************************************************************/
struct input_fd {
  char *filename;
  int fd;
};

state_t *fd_handler_init(input_t *i, output_t *o, state_t *s) {
  struct input_fd *ifd = i->input;
  ifd->fd = open("/dev/random", O_RDONLY | O_NONBLOCK);
  return fsm_status_continue(s);
}

state_t *fd_handler(input_t *i, output_t *o, state_t *s) {
  struct input_fd *ifd = i->input;
  char c[32];
  bzero(c, sizeof(char)*32);

  ssize_t ret = read(ifd->fd, c, 32);
  if (ret<0) return fsm_status_error("fd error", s);

  for (int j=0; j<32; j++)
    _debug("random byte: %d,", c[j]);

  return fsm_status_ok(s);
}

enum fsm_status fd_handler_error(input_t *i, output_t *o, state_t *s) {
  struct input_fd *ifd = i->input;
  close(ifd->fd);
  return ERROR;
}

MU_TEST(test_fsm_file_descriptor) {
  input_t si;
  output_t so;
  state_t s;
  fsm_init(&si, &so, &s);

  // initialize input
  struct input_fd ifd;
  bzero(&ifd, sizeof(struct input_fd));
  si.input = &ifd;
  si.input_length = sizeof(struct input_fd);

  // initialize state
  fsm_state_handler(fd_handler, &s);
  fsm_state_handler_error(fd_handler_error, &s);

  // start the fsm
  fsm_start(fd_handler_init, &si, &so, &s);

  // TODO:
  // to be moved into a teardown callback
  close(ifd.fd);
}

MU_TEST_SUITE(test_fsm_file_descriptor_suite) {
  MU_RUN_TEST(test_fsm_file_descriptor);
}

/*********************************************************************
 * fsm ping pong
 * -------------
 *
 * a ping/pong test to check the state transition.
 *
 ********************************************************************/
state_t *ping(input_t *, output_t *, state_t *);
state_t *pong(input_t *, output_t *, state_t *);

state_t *ping(input_t *i, output_t *o, state_t *s) {
  _debug("%s", "ping");
  fsm_state_handler(pong, s);
  return fsm_status_continue(s);
}

state_t *pong(input_t *i, output_t *o, state_t *s) {
  _debug("%s", "pong");
  return fsm_status_ok(s);
}

MU_TEST(test_fsm_pingpong) {
  input_t si;
  output_t so;
  state_t s;
  fsm_init(&si, &so, &s);

  fsm_state_handler(ping, &s);
  int ret = fsm_loop(&si, &so, &s);
  mu_assert(ret == 0, "fsm_start return code issue");
  mu_assert(s.status == OK, "status issue");
}

MU_TEST_SUITE(test_fsm_pingpong_suite) {
  MU_RUN_TEST(test_fsm_pingpong);
}

/*********************************************************************
 * DRAFT: fsm mac simple parser
 * ----------------------------
 *
 * A mac address parser using the fsm. the code present here will
 * then be merged in the main library, this is a sandbox.
 *
 ********************************************************************/
// should be used to initialize the data, invoked with fsm_state()
// function.
state_t *tokenizer_init(input_t *i, output_t *o, state_t *s) {
  return fsm_status_continue(s);
}

struct data {
  int input_cursor;
  int output_cursor;
};

// main handler called to parse every
state_t *tokenizer(input_t *i, output_t *o, state_t *s) {
  _debug("--- fsm computation ---");

  // dereference input
  size_t string_length = i->input_length;
  char *string = i->input;
  _debug("input_length:'%zu', input(value):'%s'", string_length, string);

  // derefrence output
  size_t output_length = o->output_length;
  uint8_t *mac_address = o->output;
  _debug("output_length:'%d', output(address):'%p'", output_length, mac_address);

  // dereferece data from state
  struct data *data = s->data;
  int *position = &data->input_cursor;
  int *output_cursor = &data->output_cursor;
  _debug("input_cursor::%d", *position);
  _debug("output_cursor:%d", *output_cursor);

  // just for debugging purpose
  _debug("string_char:'%c'", string[*position]);
  _debug("output_data:'%x'", mac_address[*position]);
  _mac_print(mac_address);

  // NOTE: in fact, we can easily check every position with
  // a switch/case. But I don't want something "smart", i want
  // something stupid that can be understood by anybody.
  if (*position>string_length)
    return fsm_status_error("overflow", s);

  if (*position==0 && ! is_char_digit(string[*position]))
    return fsm_status_error("bad starting char (0)", s);

  if (*position==16 && is_char_digit(string[*position])) {
    _debug("digit found (final): %c", string[*position]);
    uint8_t nibble = 0x00;
    char_to_uint8n(string[*position], &nibble);
    nibble_to_uint8(nibble, &mac_address[*output_cursor/2]);
    return fsm_status_ok(s);
  }

  if (*position==2 && ! is_char_separator(string[*position]))
    return fsm_status_error("not a separator", s);

  if (*position==5 && ! is_char_separator(string[*position]))
    return fsm_status_error("not a separator", s);

  if (*position==8 && ! is_char_separator(string[*position]))
    return fsm_status_error("not a separator", s);

  if (*position==11 && ! is_char_separator(string[*position]))
    return fsm_status_error("not a separator", s);

  if (*position==14 && ! is_char_separator(string[*position]))
    return fsm_status_error("not a separator", s);

  // just stop the computation here for now
  if (is_char_digit(string[*position])) {
    _debug("digit found: %c", string[*position]);
    uint8_t nibble = 0x00;
    char_to_uint8n(string[*position], &nibble);
    nibble_to_uint8(nibble, &mac_address[*output_cursor/2]);
    *output_cursor += 1;
  }

  *position += 1;
  return fsm_status_continue(s);
}

MU_TEST(test_fsm_tokenizer) {
  // allocate data structure on the stack
  input_t si;
  output_t so;
  state_t s;

  // cleanup the data structure
  fsm_init(&si, &so, &s);

  // configure the input
  // the string is directly set to a mac address
  char *string = "01:23:45:67:89:ab";
  size_t string_length = strlen(string);
  si.input = string;
  si.input_length = string_length;

  // configure the output
  uint8_t mac_address[6];
  bzero(mac_address, sizeof(uint8_t)*6);
  so.output = mac_address;
  so.output_length = sizeof(uint8_t)*6;

  // configure state's data. the idea is to isolate the
  // input from the output during the computation, and
  // put everything in state-'s data.
  struct data data = {
    .input_cursor = 0,
    .output_cursor = 0,
  };
  s.data = &data;
  s.data_length = sizeof(struct data);

  // this is the default state hander, it will be called
  // on each computation.
  fsm_state_handler(tokenizer, &s);

  // start the tokenizer
  int ret = fsm_start(tokenizer_init, &si, &so, &s);
  _debug("message: %s", s.message);
  mu_assert(ret == 0, "ret issue");
  mu_assert(s.message == NULL, "message should not be set");

  uint8_t *out = so.output;
  _mac_print(out);
  mu_assert(out[0] == 0x01, "mac parser error (0)");
  mu_assert(out[1] == 0x23, "mac parser error (1)");
  mu_assert(out[2] == 0x45, "mac parser error (2)");
  mu_assert(out[3] == 0x67, "mac parser error (3)");
  mu_assert(out[4] == 0x89, "mac parser error (4)");
  mu_assert(out[5] == 0xab, "mac parser error (5)");
}

MU_TEST_SUITE(test_fsm_tokenizer_suite) {
  MU_RUN_TEST(test_fsm_tokenizer);
}

/*********************************************************************
 * fsm mac tokenizer + parser
 * --------------------------
 *
 * Another mac address parser.
 ********************************************************************/
struct tokenizer_output {
  // the position of the input cursor;
  int cursor_position;

  // a mac address can't be greater than 23 bytes,
  //   eui48-min: 16 bytes (cisco)
  //   eui64-max: 23 bytes (ieee/ietf)
  // if greater than this value, it should fail.
  struct token tokens[23];
};

// The first function to be called when starting
// the fsm
// state_t *tokenizer_handler_init(input_t *i, output_t *o, state_t *s) { }

// TODO: this is a draft.
// In this tokenizer, only the authorized alphabets (tokens)
// are generated. The syntax is not checked yet.
state_t *tokenizer_handler(input_t *i, output_t *o, state_t *s) {
  // derefrence output
  // size_t output_length = o->output_length;
  struct tokenizer_output *to = o->output;
  struct token *token_output = &to->tokens[to->cursor_position];
  _debug("%d\n", to->cursor_position);

  if (to->cursor_position>23) {
    fsm_status_error("can't be greater than 23 bytes", s);
    return s;
  }

  // dereference input
  size_t string_length = i->input_length;
  char *string = i->input;
  char *c = &string[to->cursor_position];
  _debug("%c %zu\n", *c, string_length);

  // check if it's a digit
  if (is_char_digit(*c)) {
    struct token token;
    mac_token(&token, to->cursor_position, MAC_TOKEN_DIGIT, *c);
    memcpy(token_output, &token, sizeof(struct token));
  }

  to->cursor_position += 1;

  // just stop for now
  fsm_status_ok(s);
  return s;
}

// tokenizer (decoder) procedure
// -----------------------------
//
// 1. read char from input
// 2. increment input cursor (if any)
// 3. check if the character is valid
// 4. creates a new token containing the raw value and
//    the binary value (for digit only)
// 5. returns the token list
// state_t *tokenizer2(input_t *i, output_t *o, state_t *s) { }

// parser (decoder) procedure
// --------------------------
//
// undefined-1. read token from input (previously created via the tokenizer
// undefined-2. check if the first token is valid (must be a digit)
// undefined-3. check if the second token is valid (must be a digit)
// undefined-4. check if the third token is valid
// undefined-4.1. if token is a separator (":"), it's IETF format
// undefined-4.2. if token is a separator ("-") it's IEEEformat
// undefined-4.3. if token is a digit, it's probably a cisco format, but
//                we are not sure yet
// undefined-4. check if the fourth token is a digit
// undefined-5. check if the fifth token is a separator ("."),
//              then this is a cisco format
//
// ieee-1. check if the digits are correctly separated by ("-").
//         group of 2 digits.
//         max size 17 characters (EUI48), 23 chars (EUI64)
//
// ietf-1. check if the digits are correctly separated by (":").
//         group of 2 digits.
//         max size 17 characters (EUI48), 23 chars (EUI64)
//
// cisco-1. check if the digits are correctly separated by (".").
//          group of 4 digits.
//          max size 16 characters (EUI48), 19 chars (EUI64)
//
// state t* mac_undefined(input_t *i, output_t *o, state_t *) {}

// parser (encoder) procedure
// --------------------------
//
// required: format desired (e.g. cisco, ieee, or ietf)
//           type of mac address (e.g. eui48 or eui64)

/*********************************************************************
 * fsm argument parser
 * -------------------
 *
 * The fsm can be used to parser arguments from the command line and
 * convert them into an internal data structure.
 ********************************************************************/
struct ouptut_args {
  char *program_name;
  uint8_t flag_all;
  uint8_t flag_compress;
  uint8_t flag_data;
  char *flag_file;
  char *flag_config;
  char **extra;
};


typedef struct args_s {
  char short_arg;
  char *long_arg;
  char *documentation;
  state_t *state;
} args_s;

// callback parsers
// state_t * program_arg();
// state_t * short_arg();
// state_t * long_arg();
// state_t * param_arg();

args_s args[] = {
  {
    .short_arg = 'a',
    .long_arg = "all",
    .documentation = "",
    .state = NULL,
    // .default_arg = 0,
    // .default_arg_size = sizeof(uint8_t),
  },
  {
    .short_arg = 'z',
    .long_arg = "compress",
    .documentation = "",
    .state = NULL,
  }
};

MU_TEST(test_fsm_arguments) {
  input_t si;
  output_t so;
  state_t s;
  fsm_init(&si, &so, &s);

  char *argv[] = {
    "/path/to/my/program", // program path
    "-a", // all flag
    "-zd", // compress flag and debug
    "--data", // data flag
    "--file", // file flag
    "/path/to/file", // file parameter
    "-c", // config flag
    "/path/to/config", // config parameter
    "--port=8080", // port configuration 
    "--", // stop parsing
    "extra_args",
  };
  int argc = 10;
  _debug("%s", argv[0]);
  _debug("%d", argc);

}

MU_TEST_SUITE(test_fsm_arguments_suite) {
  MU_RUN_TEST(test_fsm_arguments);
}

/*********************************************************************
 * Main test suite.
 ********************************************************************/
int main() {
  MU_RUN_SUITE(test_fsm_handlers_suite);
  MU_RUN_SUITE(test_fsm_file_descriptor_suite);
  MU_RUN_SUITE(test_fsm_counter_suite);
  MU_RUN_SUITE(test_fsm_pingpong_suite);
  MU_RUN_SUITE(test_fsm_tokenizer_suite);
  MU_RUN_SUITE(test_fsm_arguments_suite);
  MU_REPORT();
  return MU_EXIT_CODE;
}
