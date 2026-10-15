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
 * ------------------------------------------------------------------
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mac_fsm.h"

#define PARAM_TYPE_BOOLEAN 0x00
#define PARAM_TYPE_INTEGER 0x01
#define PARAM_TYPE_STRING 0x02

struct parameter {
  char *name;
  char short_arg;
  char *long_arg;
  uint8_t type;
  void *value;
};

struct parameter parameters[] = {
  {
    .name = "all",
    .short_arg = 'a',
    .long_arg = "all",
    .type = PARAM_TYPE_BOOLEAN,
  },
  {
    .name = "compress",
    .short_arg = 'z',
    .long_arg = "compress",
    .type = PARAM_TYPE_BOOLEAN,
  }
};

struct input {
  int argc;
  char **argv;
};

state_t *parser_short_callback(input_t *i, output_t *o, state_t *s) {
  printf("short arg parser\n");
  return fsm_status_ok(s);
}

state_t *parser_long_callback(input_t *i, output_t *o, state_t *s) {
  printf("long arg parser\n");
  return fsm_status_ok(s);
}

state_t parser_short_state = {
  .name = "parser_short",
  .handler = parser_short_callback,
  .handler_enter = NULL,
  .handler_error = NULL,
  .handler_ok = NULL,
  .data = NULL,
  .data_length = 0,
};

state_t parser_long_state = {
  .name = "parser_long",
  .handler = parser_long_callback,
  .handler_enter = NULL,
  .handler_error = NULL,
  .handler_ok = NULL,
  .data = NULL,
  .data_length = 0,
};

state_t *parser_callback(input_t *i, output_t *o, state_t *s) {
  struct input *input = i->input;
  int *counter = s->data;

  if (*counter >= (input->argc-1))
    return fsm_status_ok(s);

  size_t len = strnlen(input->argv[*counter], 256);
  
  if (*counter==0 && len>0) {
    printf("program: %s\n", input->argv[*counter]);
  }

  if (len>2 && input->argv[*counter][0] == '-' && input->argv[*counter][1] == 'i') {
    printf("found long argument: %s\n", input->argv[*counter]);
    return fsm_transition(s, &parser_long_state);
  }

  if (len>1 && input->argv[*counter][0] == '-') {
    printf("found short argument: %s\n", input->argv[*counter]);
    return fsm_transition(s, &parser_short_state);
  }

  *counter+=1;
  return fsm_status_continue(s);
}

state_t parser_state =  {
  .name = "parser",
  .handler = parser_callback,
  .handler_enter = NULL,
  .handler_error = NULL,
  .handler_ok = NULL,
  .data = NULL,
  .data_length = 0,
};



int parameter_search_short(char c, struct parameter **parameters, struct parameter *param) {
  return 0;
}

int main(int argc, char **argv) {
  struct input input;
  input.argc = argc;
  input.argv = argv;

  input_t i;
  fsm_input_init(&i);
  i.input = &input;
  i.input_length = sizeof(struct input);

  int counter = 0;
  parser_state.data = &counter;
  parser_state.data_length = sizeof(counter);

  fsm_loop(&i, NULL, &parser_state);
  return 0;
}
