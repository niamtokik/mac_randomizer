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

/*********************************************************************
 * fsm counter
 ********************************************************************/
state_t *counter(input_t *i, output_t *o, state_t *s) {
  int *input = i->input;
  int *output = o->output;
  // printf("t: i:%d o:%d\n", *input, *output);
  *input += 1;
  *output = *input;
  if (*input <255) fsm_state_continue(s);
  if (*input == 255) fsm_state_ok(s);
  return s;
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
  int ret = fsm_start(&si, &so, &s);

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
 * fsm ping pong
 ********************************************************************/
state_t *ping(input_t *, output_t *, state_t *);
state_t *pong(input_t *, output_t *, state_t *);

state_t *ping(input_t *i, output_t *o, state_t *s) {
  printf("ping\n");
  fsm_state_handler(pong, s);
  fsm_state_continue(s);
  return s;
}

state_t *pong(input_t *i, output_t *o, state_t *s) {
  printf("pong\n");
  fsm_state_ok(s);
  return s;
}

MU_TEST(test_fsm_pingpong) {
  input_t si;
  output_t so;
  state_t s;
  fsm_init(&si, &so, &s);

  fsm_state_handler(ping, &s);
  int ret = fsm_start(&si, &so, &s);
  mu_assert(ret == 0, "fsm_start return code issue");
  mu_assert(s.status == OK, "status issue");
}

MU_TEST_SUITE(test_fsm_pingpong_suite) {
  MU_RUN_TEST(test_fsm_pingpong);
}

/*********************************************************************
 * Main test suite.
 ********************************************************************/
int main() {
  MU_RUN_SUITE(test_fsm_counter_suite);
  MU_RUN_SUITE(test_fsm_pingpong_suite);
  MU_REPORT();
  return MU_EXIT_CODE;
}
