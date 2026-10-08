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
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "mac.h"

// data living alongside the state. It is mostly used to contain
// the final parsed data for example, let consider it has the
// "output" of the fsm
typedef struct state_data {
  void *data;
  size_t length;
} state_data;

// exported state
typedef struct state {
  // a state can have a name to help identify it during
  // code execution.
  char *name;

  // a state can be set with different information to help
  // debugging it.
  int debug;

  // a status is returned with its returned and checked
  // by the fsm. If the the status is different than 0,
  // then this is an error.
  //   status<0: error
  //   status==0: end
  //   status==1: run
  int status;

  // the handler_enter function is executed when
  // a transition occurs, S0 -> S1.handler_enter() -> S1
  void (*handler_enter)(struct state*, struct state_data*);

  // then handler_input function is executed when the
  // state is set. S0.handler_input()
  struct state *(*handler_input)(char, struct state*, struct state_data*);
} state;

// internal state for the fsm
struct __fsm_state {
  int counter;
};

// exported functions.
int fsm_state_init(state *);
int fsm_start(char *, size_t, struct state *, struct state_data *);

// initialize a state
int fsm_state_init(state *s) {
  bzero(s, sizeof(state));
  return 0;
}

// check if the handler_input is not NULL
int fsm_state_is_valid_handler_input(state *s) {
  if (s->handler_input) return 0;
  return 1;
}

// check if the handler_enter is not NULL
int fsm_state_is_valid_handler_enter(state *s) {
  if (s->handler_enter) return 0;
  return 1;
}

// start the finite state machine
int fsm_start(char *input, size_t length, struct state *s, struct state_data *d) {
  // TODO: to check
  struct state_data *current_data = d;

  // TODO: to check
  struct state *current_state = s;

  int i;
  for(i=0; i<length; i++) {
    // extract the character from the input
    char c = input[i];

    // simple guards to avoid messing with the fsm.
    if (current_state == NULL) return -1;
    if (current_state->status<0) return -1;
    if (current_state->status==0) return 0;
    if (current_state->handler_input == NULL) return -1;

    // execute the input handler with the data from the 
    // string.
    struct state *ret_state;
    ret_state = current_state->handler_input(c, current_state, current_data);

    // the returned state is null, this is an error
    if (ret_state == NULL) return -1;

    // if the returned state has a different address than the
    // current state and if handler_enter is not null, then
    // enter handler is executed.
    if (ret_state != current_state && ret_state->handler_enter)
      ret_state->handler_enter(ret_state, current_data);

    // finally, the returned state is becoming the current_state
    current_state = ret_state;
  }

  // this is not normal, it means the finite state
  // machine did not end correctly.
  return -1;
}
