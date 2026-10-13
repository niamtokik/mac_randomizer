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
 * TODO: add in the documentation.
 *
 * TODO: add handler_ok to be called when everything has been
 *       correctly done. It can be used to ensure a file descriptor
 *       must be closed or memory freed at the end of the fsm.
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
int fsm_state_init(state_t *s) {
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
int fsm_input_init(input_t *si) {
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
int fsm_output_init(output_t *so) {
  bzero(so, sizeof(output_t));
  return 0;
}

/**
 * helper function to initialize and sanitize input_t, output_t
 * and state_t data structures.
 */
int fsm_init(input_t *i, output_t *o, state_t *s) {
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
// TODO: rename fsm_state_ok to fsm_status_ok
state_t *fsm_state_ok(state_t *s) {
  s->status = OK;
  s->message = NULL;
  return s;
}

/**
 * set state's status to CONTINUE, it means the fsm
 * has not finished yet and will continue the execution.
 */
// TODO: rename fsm_state_continue to fsm_status_continue
state_t *fsm_state_continue(state_t *s) {
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
// TODO: rename fsm_state_error to fsm_status_error
state_t *fsm_state_error(char *msg, state_t *s) {
  s->status = ERROR;
  s->message = msg;
  return s;
}

/**
 * internal function.
 */
// TODO: rename fsm_state_unknown to fsm_status_unknown
state_t *fsm_state_unknown(state_t *s) {
  s->status = UNKNOWN;
  return s;
}

/**
 * set state's handler, this is the function executed
 * by the fsm.
 */
state_t *fsm_state_handler(state_t *(*handler)(input_t *, output_t *, state_t *), state_t *s) {
  // if (s->handler != handler) set_state_enter(s);
  s->handler = handler;
  return s;
}

/**
 * set state's enter handler, this is the function executed
 * by the fsm when a transition happens.
 */
state_t *fsm_state_handler_enter(void (*handler_enter)(output_t *, state_t *), state_t *s) {
  s->handler_enter = handler_enter;
  return s;
}

/**
 *  Configure error callback teardown function.
 * 
 */
state_t *fsm_state_handler_error(enum fsm_status (*handler_error)(input_t *, output_t *, state_t *), state_t *s) {
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
state_t *fsm_state_handler_ok(void (*handle_ok)(input_t *, output_t *, state_t *), state_t *s) {
  s->handler_ok = handle_ok;
  return s;
}

// TODO:
// state_t *fsm_state_handler_ok(void (*handler_enter)(input_t *, output_t *, state_t *), state_t *s) {
//  s->handler_ok = handler_ok;
//  return s;
// }

/**
 * check if the state is not set to NULL. If something
 * is not correctly configured, it will return -1.
 */
int fsm_check(input_t *i, output_t *o, state_t *s) {
  if (s == NULL) return -1;
  if (s->handler == NULL) return -1;
  return 0;
}

// function to execute the computation, in charge to manage
// the returned result/status of the fsm.
enum fsm_status fsm_eval(state_t *(callback)(input_t *, output_t *o, state_t*), input_t *i, output_t *o, state_t *s) {
    // check if the struct to be used is valid.
    if (fsm_check(i, o, s) <0) return EVAL_ERROR; 

    // execute the handler present in the state_t data structure,
    // it should return a valid struct.
    fsm_state_unknown(s);
    state_t *ret_state;
    if (callback) ret_state = callback(i, o, s);
    if (fsm_check(i, o, s) <0) return EVAL_ERROR; 

    // if the returned struct is different than the one we
    // currently use, it means this is a state transition. In this
    // case, if the handler_enter callback function is defined,
    // the status of the structure must be set to ENTER and then
    // the handler_enter callback must be called. The ENTER status
    // MUST NOT alter the state_t data structure.
    if (ret_state != s && ret_state->handler_enter != NULL)
      (void)ret_state->handler_enter(o, s);

    // when a full transition happens, the previous state address
    // is stored there.
    if (ret_state != s)
      ret_state->previous_state = s;

    // if the state returned is unknown, it means the user never
    // set a valid state after the computation, it should be
    // treated as an error (unkonwn state).
    if (ret_state->status == UNKNOWN)
      return UNKNOWN;

    // the user state the finite state machine to error, it
    // means something bad happened, and the fsm must be
    // stopped
    if (ret_state->status == ERROR && ret_state->handler_error != NULL)
      return ret_state->handler_error(i, o, s);

    // if no error callback functions is defined, simply return the error
    if (ret_state->status == ERROR)
      return ERROR;

    // if handler_ok function callback is defined, then it is executed
    // before returning everything was good.
    if (ret_state->status == OK && ret_state->handler_ok != NULL)
      ret_state->handler_ok(i, o, s);

    // the user set the finite state machine to ok, it
    // means the state machine finished the computation,
    // and the final data should be present in the
    // output.
    if (ret_state->status == OK)
      return OK;

    // the user set the finite state machine to continue,
    // it means the computation is not done yet, and
    // the callback function will be called again.
    if (ret_state->status == CONTINUE) {
      s = ret_state;
      return CONTINUE;
    }

    // if the loop stop for no reason, return unknow status
    // it should never happen.
    return UNKNOWN;
}

/**
 * fsm main loop, it will give the input, output and state
 * to the handler definined in the state data structure. The
 * status is then checked to define if the computation is done
 * or not.
 */
int fsm_loop(input_t *i, output_t *o, state_t *s) {
  for (;;) {

    // evaluate the computation and check its result, this where
    // the routing happens.
    //
    // TODO: DRY. This should be executed in another function to be reused
    // in other part of the application.
    switch (fsm_eval(s->handler, i, o, s)) {
      case EVAL_ERROR: return -1;
      case ERROR: return -1;
      case OK: return 0;
      case CONTINUE: continue;
      default: return -1;
    }
  }
  return -1;
}

/**
 * a wrapper around fsm_loop, currently not being used. in the
 * end it will also be in charge to initialize and configure
 * all data structures used by the fsm. It can be useful when
 * using dynamic memory instead of the stack.
 */
int fsm_start(state_t *(*init_callback)(input_t *, output_t *, state_t *), input_t *i, output_t *o, state_t *s) {
  state_t *init_state = s;

  // if an init callback function is given, then it must be executed
  if (init_callback != NULL) {
    // TODO: DRY.
    switch (fsm_eval(init_callback, i, o, s)) {
      case EVAL_ERROR: return -1;
      case ERROR: return -1;
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
