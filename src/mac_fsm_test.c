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

struct state_data sd = {
  .data = (int *)(255),
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
  int data;
  size_t length = sd->length;
  memcpy(&data, &sd->data, length);
  printf("enter: data:%d length:%zu\n", data, length);
}

struct state *input1(char c, struct state *s, struct state_data *sd) {
  printf("input1: %c\n", c);
  return &s2;
}

struct state *input2(char c, struct state *s, struct state_data *sd) {
  printf("input2: %c\n", c);
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
 * Main test suite.
 ********************************************************************/
int main() {
  MU_RUN_SUITE(test_fsm_suite);
  MU_REPORT();
  return MU_EXIT_CODE;
}
