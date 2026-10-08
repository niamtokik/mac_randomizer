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
#include "minunit.h"
#include "mac.h"

// Notes:
// -----
//
// a generator/producer function is required to return the appropriate
// kind of data. The FSM is agnostic and does not care about the kind
// of data passed, only the state handlers care about that.
//
//   size_t generator(void *data) {
//     data = 't';
//     return sizeof(char);
//   }
//
// here a draft of the idea:
//
//   size_t (g)(void *) = generator;
//   struct state *s;
//   struct state_data *d;
//   fsm_start(g, state, state_data);

/*********************************************************************
 * generic fsm use case
 ********************************************************************/
void enter(struct state *s, struct state_data *);
struct state *input1(char, struct state *, struct state_data *);
struct state *input2(char, struct state *, struct state_data *);

int counter = 0;
struct state_data sd = {
  .data = (int *)&counter,
  .length = sizeof(int),
};

struct state s1 = {
  .name = "counter",
  .debug = 0x0,
  .status = 0x0,
  .handler_enter = enter,
  .handler_input = input1,
};

struct state s2 = {
  .name = "counter2",
  .debug = 0x0,
  .status = 0x0,
  .handler_enter = enter,
  .handler_input = input2,
};

void enter(struct state *s, struct state_data *sd) {
  printf("enter: data:%d length:%zu\n", *(int *)(sd->data), sd->length);
}

struct state *input1(char c, struct state *s, struct state_data *sd) {
  (*(int*)(sd->data))++;
  printf("input1: data:%d length:%zu\n", *(int *)(sd->data), sd->length);
  return &s2;
}

struct state *input2(char c, struct state *s, struct state_data *sd) {
  (*(int*)(sd->data))++;
  printf("input2: data:%d length:%zu\n", *(int *)(sd->data), sd->length);
  if (c==0) s->status=1;
  return s;
}

MU_TEST(test_fsm) {
  int ret = fsm_start("test", 5, &s1, &sd);
  mu_assert(ret == 0, "fsm issue");
}

MU_TEST_SUITE(test_fsm_suite) {
  MU_RUN_TEST(test_fsm);
}

/*********************************************************************
 * DRAFT: mac address parsing use case
 ********************************************************************/
// an undefined input, usually checking the first character to parse
// and then analysis the rest of the content until a valid separator
// is found.
struct state *mac_input_undefined(char, struct state*, struct state_data*);
struct state mac_input_undefined_s = {
  .name = "undefined",
  .status = 0x0,
  .handler_input = mac_input_undefined,
};

// an ieee input, supporting only dash separator: xx-xx-xx-xx-xx-xx
struct state *mac_input_ieee(char, struct state*, struct state_data*);
struct state mac_input_ieee_s = {
  .name = "ieee",
  .status = 0x0,
  .handler_input = mac_input_undefined,
};

// an ietf input, supporting only column separator: xx:xx:xx:xx:xx:xx
struct state *mac_input_ietf(char, struct state*, struct state_data*);
struct state mac_input_ietf_s = {
  .name = "ietf",
  .status = 0x0,
  .handler_input = mac_input_undefined,
};

// a cisco input, supporting only dot separator: xxxx.xxxx.xxx
struct state *mac_input_cisco(char, struct state*, struct state_data*);
struct state mac_input_cisco_s = {
  .name = "cisco",
  .status = 0x0,
  .handler_input = mac_input_undefined,
};

// this is an example of a parser implementation using the fsm.
//
// the state_data should contain:
//   - buffer for the mac address
//   - position of the cursor
struct state *mac_input_undefined(char c, struct state *s, struct state_data *sd) {
  // not sure where the position should be defined, it can be in the state
  // itself, or defined in the state data. if it's in the state data, everytime
  // a char is given by the fsm, the handler must increment the position.
  int position = s->position;

  // the buffer containing the decoded mac_address
  uint8_t *mac_address = sd->data->mac_address;

  // the length of the buffer (it should be also compatible with eui64)
  size_t *mac_address_length = sd->data->mac_address_length;

  // check if the first char is a valid hex digit
  if (position==0 && is_not_hex(c)) s->status=1;

  // this is a dash. continue on ieee parser
  if (is_sep_dash(c) && position==2) 
    return mac_input_ieee;

  // this is a column, continue on ietf parser
  if (is_sep_column(c) && position==2)
    return mac_input_ietf;

  // this is a dot, continue on cisco parser
  if (is_sep_dot(c) && position==5)
    return mac_input_cisco;

  // we are not sure if it's valid until no separator
  // was found in the string, update the mac address
  // buffer if everything looks good.
  if (is_hex(c) && position>0 && position<5) {
    return s;
  }
}

/*********************************************************************
 * Main test suite.
 ********************************************************************/
int main() {
  MU_RUN_SUITE(test_fsm_suite);
  MU_REPORT();
  return MU_EXIT_CODE;
}
