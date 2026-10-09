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
 * TODO: add handler_init to be called when the fsm_start function
 *       is used. it should be used to initialize input, output and
 *       eventually state.
 *
 * TODO: add handler_error to be called in case of error, default to
 *       NULL to disable the feature. can be used to close a file
 *       descriptor or free memory.
 *
 * TODO: add handler_ok to be called when everything has been
 *       correctly done. It can be used to ensure a file descriptor
 *       must be closed or memory freed at the end of the fsm.
 *
 * TODO: add handler_enter to be called when a transition happens
 *       (when the pointer to the handler change). it should not
 *       have access to the input, only to the output.
 *
 * TODO: add handler_previous to be set with the previous function
 *       called.
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
 * initialize and sanitize the state_t structure.
 */
int fsm_state_init(state_t *s) {
  bzero(s, sizeof(state_t));
  return 0;
}

/**
 * initialize and sanitize the input_t structure.
 */
int fsm_input_init(input_t *si) {
  bzero(si, sizeof(input_t));
  return 0;
}

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
void fsm_state_ok(state_t *s) {
  s->status = OK;
  s->message = NULL;
}

/**
 * set state's status to CONTINUE, it means the fsm
 * has not finished yet and will continue the execution.
 */
void fsm_state_continue(state_t *s) {
  s->status = CONTINUE;
  s->message = NULL;
}

/**
 * set state's status to ERROR, it means something bad
 * happened during the execution of the code, and the
 * fsm will stop and set the message field with a 
 * specific error message.
 */
void fsm_state_error(char *msg, state_t *s) {
  s->status = ERROR;
  s->message = msg;
}

/**
 * set state's handler, this is the function executed
 * by the fsm.
 */
void fsm_state_handler(struct __state *(*handler)(input_t *, output_t *, struct __state *), state_t *s) {
  // if (s->handler != handler) set_state_enter(s);
  s->handler = handler;
}

/**
 * check if the state is not set to NULL. If something
 * is not correctly configured, it will return -1.
 */
int fsm_check(input_t *i, output_t *o, state_t *s) {
  if (s == NULL) return -1;
  if (s->handler == NULL) return -1;
  return 0;
}

/**
 * fsm main loop, it will give the input, output and state
 * to the handler definined in the state data structure. The
 * status is then checked to define if the computation is done
 * or not.
 */
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

/**
 * a wrapper around fsm_loop, currently not being used. in the
 * end it will also be in charge to initialize and configure
 * all data structures used by the fsm.
 */
int fsm_start(input_t *i, output_t *o, state_t *s) {
  return fsm_loop(i, o, s);
}
