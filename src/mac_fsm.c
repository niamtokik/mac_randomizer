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
 * DRAFT: mac_fsm: flexible minimalist mealy-like machine implementation.
 *
 *
 * This code implements a mealy-like finite state machine mainly used to parse
 * mac addresses (and other specific input) instead of reusing regexp
 * libraries.
 *
 * The states are statically created using C data-structure. The states embed
 * handlers to deal with the input and the stored data.
 *
 * When the FSM is in a state it can change to another state by returning a
 * pointer to a new (or existing) state.
 *
 * ## Continuous Execution
 *
 * ## Step by Step
 *
 * ## Debugging and Tracing
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "mac.h"

int fsm_state_init(state_t *s) {
  bzero(s, sizeof(state_t));
  return 0;
}

int fsm_input_init(input_t *si) {
  bzero(si, sizeof(input_t));
  return 0;
}

int fsm_output_init(output_t *so) {
  bzero(so, sizeof(output_t));
  return 0;
}

void fsm_state_ok(state_t *s) {
  s->status = OK;
  s->message = NULL;
}

void fsm_state_continue(state_t *s) {
  s->status = CONTINUE;
  s->message = NULL;
}

void fsm_state_error(char *msg, state_t *s) {
  s->status = ERROR;
  s->message = msg;
}

void fsm_state_handler(struct __state *(*handler)(input_t *, output_t *, struct __state *), state_t *s) {
  // if (s->handler != handler) set_state_enter(s);
  s->handler = handler;
}

int fsm_init(input_t *i, output_t *o, state_t *s) {
  if (fsm_state_init(s)<0) return -1;
  if (fsm_input_init(i)<0) return -1;
  if (fsm_output_init(o)<0) return -1;
  return 0;
}

int fsm_check(input_t *i, output_t *o, state_t *s) {
  if (s == NULL) return -1;
  if (s->handler == NULL) return -1;
  return 0;
}

int fsm_loop(input_t *i, output_t *o, state_t *s) {
  for (;;) {
    if (fsm_check(i, o, s) <0) return -1; 
    state_t *ret_state = s->handler(i, o, s);
    if (fsm_check(i, o, s) <0) return -1; 
    if (ret_state->status == ERROR) return -1;
    if (ret_state->status == OK) return 0;
    if (ret_state->status == CONTINUE) {
      s = ret_state;
    }
  }
  return -1;
}

int fsm_start(input_t *i, output_t *o, state_t *s) {
  return fsm_loop(i, o, s);
}
