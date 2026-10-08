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

// debugging flag to print information during fsm execution
#define FSM_DEBUG 0x01
#define FSM_TRACE 0x02

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

// exported functions.
int fsm_state_init(struct state *);
int fsm_start(char *, size_t, struct state *, struct state_data *);
