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
 * TODO: create macros to help creating the FSM.
 *
 */

// supported FSM state's statuses.
enum fsm_status {
  ERROR,
  OK,
  CONTINUE,
  EVAL_ERROR,
  UNKNOWN,
};

// TODO:
// Using data instead of enum for the status can probably
// be better. using uint32_t for example, it could then
// be possible to let develoeprs to define their own
// status dynamically.
// #define FSM_STATUS_CONTINUE 0x000_0000
// #define FSM_STATUS_OK 0x0000_0001
// #define FSM_STATUS_ERROR -0x0000_1000
// #define FSM_STATUS_ERROR_EVAL 0x0000_2000
// #define FSM_STATUS_UNKNOWN 0x0000_3000

// FSM input data-structure, usually where the
// inputs/events are stored. This is a flexible structure,
// it can also be a file descriptor or any other 
// reference.
typedef struct __state_input {
  void *input;
  size_t input_length;
} input_t;

// FSM output structure, usually where the final
// data is stored, but like the input data structure, this
// is flexible, and it can also be used to store 
// file descriptors.
typedef struct __state_output {
  void *output;
  size_t output_length;
} output_t;

// FSM state data-structure.
typedef struct __state {
  // a name for the state, not mandatory, can be used for debugging
  // purpose or tracing.
  char *name;

  // fsm status, defining what to do at the end of every computations.
  enum fsm_status status;

  // fsm message, used when something goes wrong during computation.
  char *message;

  // main handler, called evenry time a new input is taken.
  struct __state *(*handler)(input_t *, output_t *, struct __state *);

  // callback function called when a state transition happens
  void (*handler_enter)(output_t *, struct __state *);

  // callback function called when the fsm return an error
  enum fsm_status (*handler_error)(input_t *, output_t *, struct __state *);

  // previous state called, set by the fsm after a transition
  struct __state *previous_state;

  // internal data defined by the user if needed
  void *data;
  size_t data_length;
} state_t;

// initializers
int fsm_input_init(input_t *);
int fsm_output_init(output_t *);
int fsm_state_init(state_t *);
int fsm_init(input_t *, output_t *, state_t *);

// setters
// // setter for input_t data-structure
// int fsm_input(void *, size_t, input_t);
//
// // setter for the output_t data-structure
// int fsm_output(void *, size_t, output_t);
//
// // setter for the data part of the fsm
// int fsm_state_data(void *, size_t , struct_t *);
//
// // setters for the handlers
// int fsm_state_handler_enter();
// int fsm_state_handler_error();
state_t *fsm_state_handler(state_t *(*handler)(input_t *, output_t *, state_t *), state_t *);

// state management
state_t *fsm_state_ok(state_t *);
state_t *fsm_state_continue(state_t *);
state_t *fsm_state_error(char *, state_t *);
state_t *fsm_state_unknown(state_t *);

// fsm main functions
int fsm_init(input_t *, output_t *, state_t *);
int fsm_check(input_t *, output_t *, state_t *);
int fsm_start(state_t *(*init_callback)(input_t *, output_t *, state_t *), input_t *, output_t *, state_t *);
int fsm_loop(input_t *, output_t *, state_t *);
