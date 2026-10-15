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
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mac_fsm.h"

struct input {
  int argc;
  char **argv;
};

state_t *parser_callback(input_t *i, output_t *o, state_t *s) {
  struct input *input = i->input;
  int *counter = s->data;
  if (*counter < input->argc) {
    size_t len = strnlen(input->argv[*counter], 256);
    printf("%d (%zu): %s\n", *counter, len, input->argv[*counter]);
    *counter = *counter+1;
    return fsm_status_continue(s);
  }
  return fsm_status_ok(s);
}

state_t parser_state =  {
  .name = "parser",
  .handler = parser_callback,
  .handler_enter = NULL,
  .handler_error = NULL,
  .handler_ok = NULL,
  .data = NULL,
  .data_length = 0,
};

int main(int argc, char **argv) {
  struct input input;
  input.argc = argc;
  input.argv = argv;

  input_t i;
  fsm_input_init(&i);
  i.input = &input;
  i.input_length = sizeof(struct input);

  int counter = 0;
  parser_state.data = &counter;
  parser_state.data_length = sizeof(counter);

  fsm_loop(&i, NULL, &parser_state);
  return 0;
}
