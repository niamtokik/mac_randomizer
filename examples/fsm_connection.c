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
 * A connection manager using an FSM.
 *
 */
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdlib.h>
#include "mac_fsm.h"

// input
input_t input;
struct input_s {
  char *target;
  int port;
};

// output
output_t output;

struct data_s {
  int fd;
};

// states
state_t connection;
state_t connected;
state_t disconnected;
state_t closed;

state_t *init(input_t *i, output_t *o, state_t *s) {
  return s;
}

void error_callback(input_t *i, output_t *o, state_t *s) {
  struct data_s *data = s->data;
  close(data->fd);
  free(data);
}

void enter(output_t *o, state_t *s) {
  printf("enter: name=%s\n", s->name);
}

state_t *connection_callback(input_t *i, output_t *o, state_t *s) {
  printf("test: %zu %d\n", s->data_length, ((struct data_s *)s->data)->fd);
  return fsm_transition(s, &connected);
}

state_t *connected_callback(input_t *i, output_t *o, state_t *s) {
  return fsm_transition(s, &disconnected);
}

state_t *disconnected_callback(input_t *i, output_t *o, state_t *s) {
  return fsm_transition(s, &closed);
}

state_t *closed_callback(input_t *i, output_t *o, state_t *s) {
  return fsm_status_ok(s);
}

int main(int argc, char *argv[]) {
  fsm_input_init(&input);
  fsm_output_init(&output);
  fsm_state_init(&connection);
  fsm_state_init(&connected);
  fsm_state_init(&disconnected);
  fsm_state_init(&closed);

  struct input_s i;
  bzero(&i, sizeof(struct input_s));
  input.input_length = sizeof(struct input_s);
  input.input = &i;

  struct data_s *d = malloc(sizeof(struct data_s));
  bzero(&d, sizeof(struct data_s));
  connection.data_length = sizeof(struct data_s);
  connection.data = &d;

  connection.handler = connection_callback;
  connection.handler_enter = enter;

  connected.handler = connected_callback;
  connected.handler_enter = enter;

  disconnected.handler = disconnected_callback;
  disconnected.handler_enter = enter;

  closed.handler = closed_callback;
  closed.handler_enter = enter;

  fsm_loop(&input, &output, &connection);

  return 0;
}
