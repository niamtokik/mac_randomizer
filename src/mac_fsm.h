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

// supported FSM state.
enum fsm_status {
  ERROR,
  OK,
  CONTINUE,
};

// FSM input structure.
typedef struct __state_input {
  void *input;
  size_t input_length;
} input_t;

// FSM output structure.
typedef struct __state_output {
  void *output;
  size_t output_length;
} output_t;

// FSM state.
typedef struct __state {
  enum fsm_status status;
  char *message;
  struct __state *(*handler)(input_t *, output_t *, struct __state *);
  // struct __state *(*handler_enter)(output_t *, struct __state *);
  // struct __state *(*handler_previous)(input_t *, output_t *, struct __state *);
} state_t;

// initializers
int fsm_input_init(input_t *);
int fsm_output_init(output_t *);
int fsm_state_init(state_t *);
int fsm_init(input_t *, output_t *, state_t *);

// state management
void fsm_state_ok(state_t *);
void fsm_state_continue(state_t *);
void fsm_state_error(char *, state_t *);
void fsm_state_handler(struct __state *(*handler)(input_t *, output_t *, struct __state *), state_t *);

// fsm main functions
int fsm_init(input_t *, output_t *, state_t *);
int fsm_check(input_t *, output_t *, state_t *);
int fsm_loop(input_t *, output_t *, state_t *);
int fsm_start(input_t *, output_t *, state_t *);
