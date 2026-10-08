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
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "mac.h"

// exported state
typedef struct state {
  // a status is returned with its returned and checked
  // by the fsm. If the the status is different than 0,
  // then this is an error.
  int status;

  // the state also embed its own data. a data_length
  // field has been created to help storing more information
  // about the eventual length of the data.
  void *data;
  size_t data_length;

  // the handler_enter function is executed when
  // a transition occurs, S0 -> S1.enter() -> S1
  struct state *(*handler_enter)(char *, struct state*);

  // then handler_input function is executed when the
  // state is set. S0.input(
  struct state *(*handler_input)(char *, struct state*);
} state;

struct __fsm_state {
  int counter;
};

int fsm_init(struct __fsm_state *);
int fsm_start(struct state *, char *, size_t, void *);

// init the finite state machine environment
int fsm_init(struct __fsm_state *fsm_state) {
  return -1;
}

// start the finite state machine
int fsm_start(struct state *init, char *input, size_t length, void *data_init) {
  return -1;
}
