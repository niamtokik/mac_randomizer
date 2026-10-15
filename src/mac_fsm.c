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
 * mac_fsm: flexible minimalist finite state machine implementation.
 *
 * At this time, the state_t datastructure can be used with only one
 * instance. Indeed, it is possible to apply the transition only when
 * the handler function is changed. A better solution (but adding
 * more LoC) is to create a state_t structure for each state, with
 * their own handlers. In some situation though (e.g. parsing a string),
 * using one state should be enough.
 *
 * An idea would be to create a macro used to initialize/create the
 * data structure, and/or handlers. everytime it is called, the
 * the next/previous state could be generated to find eventual
 * bugs or optimize the fsm.
 *
 * TODO: create a step by step transition.
 *
 * TODO: add in the documentation.
 *
 * TODO: add debug flag and debug functions(s).
 *
 * TODO: add trace flag and tracing function(s).
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "mac.h"

/**
 * Initialize and sanitize the state_t structure.
 *
 * This function is used to reset or initialize the state_t data-structure
 * used to store the main information about the FSM.
 *
 * @param s A pointer to a FSM's state_t data-structure.
 *
 */
int
fsm_state_init(state_t *s) {
  bzero(s, sizeof(state_t));
  return 0;
}

/**
 * Initialize and sanitize the input_t structure.
 *
 * This function is used to reset ro initialize the input_t data-structure
 * used to store the input data.
 *
 * @param s A pointer to a FSM's state_t data-structure.
 *
 */
int
fsm_input_init(input_t *si) {
  bzero(si, sizeof(input_t));
  return 0;
}

// TODO: create a function helper to extract the input part of the
// input_t data-structure.
// void * fsm_input(input_t *si) {
//  return si->input;
// }

/**
 * initialize and sanitize the output_t structure.
 */
int
fsm_output_init(output_t *so) {
  bzero(so, sizeof(output_t));
  return 0;
}

/**
 * helper function to initialize and sanitize input_t, output_t
 * and state_t data structures.
 */
int
fsm_init(input_t *i, output_t *o, state_t *s) {
  if (fsm_state_init(s)<0) return -1;
  if (fsm_input_init(i)<0) return -1;
  if (fsm_output_init(o)<0) return -1;
  return 0;
}

/**
 * set state's status to OK, it means this is the fsm
 * has finished its job and the function will return. If
 * following the convention, output_t should contain the
 * final result.
 */
state_t *
fsm_status_ok(state_t *s) {
  s->status = OK;
  s->message = NULL;
  return s;
}

/**
 * set state's status to CONTINUE, it means the fsm
 * has not finished yet and will continue the execution.
 */
state_t *
fsm_status_continue(state_t *s) {
  s->status = CONTINUE;
  s->message = NULL;
  return s;
}

/**
 * set state's status to ERROR, it means something bad
 * happened during the execution of the code, and the
 * fsm will stop and set the message field with a 
 * specific error message.
 */
state_t *
fsm_status_error(char *msg, state_t *s) {
  s->status = ERROR;
  s->message = msg;
  return s;
}

/**
 * internal function.
 */
state_t *
fsm_status_unknown(state_t *s) {
  s->status = UNKNOWN;
  return s;
}

state_t *
fsm_status_eval_error(char *message, state_t *s) {
  s->status = EVAL_ERROR;
  s->message = message;
  return s;
}

/**
 * set state's handler, this is the function executed
 * by the fsm.
 */
state_t *
fsm_state_handler(state_t *(*handler)(input_t *, output_t *, state_t *), state_t *s) {
  // if (s->handler != handler) set_state_enter(s);
  s->handler = handler;
  return s;
}

/**
 * set state's enter handler, this is the function executed
 * by the fsm when a transition happens.
 */
state_t *
fsm_state_handler_enter(void (*handler_enter)(output_t *, state_t *), state_t *s) {
  s->handler_enter = handler_enter;
  return s;
}

/**
 *  Configure error callback teardown function.
 * 
 */
state_t *
fsm_state_handler_error(fsm_status (*handler_error)(input_t *, output_t *, state_t *), state_t *s) {
  s->handler_error = handler_error;
  return s;
}

/**
 * Configure ok callback teardown function.
 *
 * set state_t data-structure with a teardown function callback executed after the last
 * succesful computation.
 *
 * @param handle_ok a pointer to a callback function.
 * @param s FSM's state data-structure.
 *
 */
state_t *
fsm_state_handler_ok(void (*handle_ok)(input_t *, output_t *, state_t *), state_t *s) {
  s->handler_ok = handle_ok;
  return s;
}

/**
 *
 */
void *
fsm_get_input(input_t *i) {
  return i->input;
}

/**
 *
 */
int
fsm_set_input(void *input, size_t input_length, input_t *i) {
  if (input==NULL) return -1;
  i->input = input;
  i->input_length = input_length;
  return 0;
}

/**
 *
 */
void *
fsm_get_output(output_t *o) {
  return o->output;
}

/**
 *
 */
int
fsm_set_output(void *output, size_t output_length, output_t *o) {
  if (output==NULL) return -1;
  o->output = output;
  o->output_length = output_length;
  return 0;
}

/**
 *
 */
void *
fsm_get_state_data(state_t *s) {
  return s->data;
}

/**
 *
 */
int
fsm_set_state_data(void *data, size_t data_length, state_t *s) {
  if (data==NULL) return -1;
  s->data = data;
  s->data_length = data_length;
  return 0;
}

/**
 * copy data from state_t structure to the new 
 * structure, overwriting the old data if present, using
 * the data_length.
 */
void
fsm_state_data_copy(state_t *src, state_t *dst) {
  if (src->data_length > 0) {
    dst->data_length = src->data_length;
    memcpy(dst->data, src->data, src->data_length);
  }
}

/**
 * A function to do the transition between 2 states by copying
 * the input, out and state data structure. During a transition
 * the data from the old state is copied into the new state,
 * overwriting it if already present.
 */
state_t *
fsm_transition(state_t *src, state_t *dst) {
  fsm_state_data_copy(src, dst);
  dst->previous_state = src;
  dst->status = TRANSITION;
  return dst;
}

/**
 * check if the state is not set to NULL. If something
 * is not correctly configured, it will return -1.
 */
int
fsm_check(input_t *i, output_t *o, state_t *s) {
  if (s == NULL) return -1;
  if (s->handler == NULL) return -1;
  return 0;
}

// function to execute the computation, in charge to manage
// the returned result/status of the fsm.
state_t *
fsm_eval(state_t *(callback)(input_t *, output_t *o, state_t*), input_t *i, output_t *o, state_t *s) {
    // check if the struct to be used is valid.
    if (fsm_check(i, o, s) <0)
      return fsm_status_eval_error("invalid state", s); 

    // if we are still in transition, it means we switched
    // from another state.
    if (s->status == TRANSITION) {
      if (s->handler_enter != NULL)
        (void)s->handler_enter(o, s);
    }

    // if the callback is NULL (is should not be the case), an
    // error must be returned.
    if (callback == NULL)
      fsm_status_eval_error("invalid callback", s);

    // let execute the callback and store its result in a temporary
    // state_t variable.
    fsm_status_unknown(s);
    state_t *ret_state;
    ret_state = callback(i, o, s);

    // the result must be check to see if everything is right.
    if (fsm_check(i, o, ret_state) <0)
      return fsm_status_eval_error("invalid state", ret_state); 

    // an unauthorized transition happened.
    if (ret_state->status != TRANSITION && ret_state != s)
      return fsm_status_eval_error("invalid transition", ret_state); 

    // the user state the finite state machine to error, it
    // means something bad happened, and the fsm must be
    // stopped
    if (ret_state->status == ERROR && ret_state->handler_error != NULL) {
      char * msg = ret_state->message;
      (void)ret_state->handler_error(i, o, ret_state);
      // force the return because the error callback can alter the 
      // status, but because its an error, we must return the value.
      return fsm_status_error(msg, ret_state);
    }

    // if handler_ok function callback is defined, then it is executed
    // before returning everything was good.
    if (ret_state->status == OK && ret_state->handler_ok != NULL) {
      (void)ret_state->handler_ok(i, o, ret_state);
      // force to return an OK status, to stop the execution of the
      // loop in case the handler_ok alter the status.
      return fsm_status_ok(ret_state);;
    }

    // by default we return the returned state, the loop will be
    // in charge to stop the computation or not.
    return ret_state;
}

int
__fsm_loop(input_t *i, output_t *o, state_t *s, __fsm_state *fs) {
  for (;;fs->counter++) {
    // evaluate the computation and check its result, this where
    // the routing happens.
    s = fsm_eval(s->handler, i, o, s);
    switch (s->status) {
      case OK: return 0;
      case CONTINUE: continue;
      case TRANSITION: continue;
      default: return -1;
    }
  }
  return -1;
}

/**
 * fsm main loop, it will give the input, output and state
 * to the handler definined in the state data structure. The
 * status is then checked to define if the computation is done
 * or not.
 */
int
fsm_loop(input_t *i, output_t *o, state_t *s) {
  // create the internal structure for the main loop
  __fsm_state fs;
  bzero(&fs, sizeof(__fsm_state));

  // start the loop
  return __fsm_loop(i, o, s, &fs);
}

/**
 * a wrapper around fsm_loop, currently not being used. in the
 * end it will also be in charge to initialize and configure
 * all data structures used by the fsm. It can be useful when
 * using dynamic memory instead of the stack.
 */
int
fsm_start(state_t *(*init_callback)(input_t *, output_t *, state_t *), input_t *i, output_t *o, state_t *s) {
  state_t *init_state = s;

  // if an init callback function is given, then it must be executed
  if (init_callback != NULL) {
    state_t *init_ret = fsm_eval(init_callback, i, o, s);
    switch (init_ret->status) {
      case OK: return 0;
      case CONTINUE: break;
      default: return -1;
    }
  }

  // TODO: when the callback function is executed, its returned
  // state is not checked in the fsm_loop (first call). It means
  // if something was not correctly initialized, it will be ignored
  // and the main callback function in the fsm will be executed
  // anyway. This is not good, and it must be fixed.
  return fsm_loop(i, o, init_state);
}
