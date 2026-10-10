
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
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "mac.h"

// TODO: a safer approach is to create a struct
// containing the length of the valid tokens
// stored. every new valid token increases the length
// value
//
// struct tokens {
//  int length;
//  struct token *tokens;
// }

// helper function to update a token structure.
void mac_token(struct token *token, int position, int type, char value) {
  token->position = position;
  token->type = type;
  token->value = value;
}

// tokenize a string, convert a valid string into a list of
// valid tokens. The parser will be in charge to convert the
// tokens and returns an error if the mac address format is
// not correct.
// TODO: improve the safety of this function by enforcing
// a maximum tokens structure length.
int mac_tokenize(struct token *tokens, int position, char c) {
  struct token token;
  bzero(&token, sizeof(struct token));

  // valid token: digit
  if (c>='0' && c<='9') {
    mac_token(&token, position, MAC_TOKEN_SEPARATOR_DIGIT, c);
    goto valid_token;
  }
  
  // valid token: hex digit
  if (c>='a' && c<='f') {
    mac_token(&token, position, MAC_TOKEN_SEPARATOR_DIGIT, c);
    goto valid_token;
  }
  
  // valid token: hex digit
  if (c>= 'A' && c <= 'F') {
    mac_token(&token, position, MAC_TOKEN_SEPARATOR_DIGIT, c);
    goto valid_token;
  }

  // valid token: separator
  if (c==':') {
    mac_token(&token, position, MAC_TOKEN_SEPARATOR_COLUMN, c);
    goto valid_token;
  }

  // valid token: separator
  if (c=='-') {
    mac_token(&token, position, MAC_TOKEN_SEPARATOR_DASH, c);
    goto valid_token;
  }

  // valid token: separator
  if (c=='.') {
    mac_token(&token, position, MAC_TOKEN_SEPARATOR_DOT, c);
    goto valid_token;
  }

  // invalid_token
  // TODO: improve tokenizer error handling.
  return -1;

  // valid_token: the local token is valid then it is copied
  // into the array of tokens.
  valid_token:
    memcpy(&tokens[position], &token, sizeof(struct token));
    return 0;
}

// helper to print the content of a token.
void mac_token_print(struct token *token) {
  printf(
    "token(position:%d, type:%d, value:%c);\n",
    token->position, token->type, token->value
  );
}

uint8_t char_to_uint8(char c) {
  switch(c) {
    case '0': return 0x0;
    case '1': return 0x1;
    case '2': return 0x2;
    case '3': return 0x3;
    case '4': return 0x4;
    case '5': return 0x5;
    case '6': return 0x6;
    case '7': return 0x7;
    case '8': return 0x8;
    case '9': return 0x9;
    case 'a': return 0xa;
    case 'b': return 0xb;
    case 'c': return 0xc;
    case 'd': return 0xd;
    case 'e': return 0xe;
    case 'f': return 0xf;
    case 'A': return 0xa;
    case 'B': return 0xb;
    case 'C': return 0xc;
    case 'D': return 0xd;
    case 'E': return 0xe;
    case 'F': return 0xf;
    default: return 255;
  }
}

int char_to_uint8n(char c, uint8_t *dst) {
  switch(c) {
    case '0': *dst = 0; break;
    case '1': *dst = 0x1; break;
    case '2': *dst = 0x2; break;
    case '3': *dst = 0x3; break;
    case '4': *dst = 0x4; break;
    case '5': *dst = 0x5; break;
    case '6': *dst = 0x6; break;
    case '7': *dst = 0x7; break;
    case '8': *dst = 0x8; break;
    case '9': *dst = 0x9; break;
    case 'a': *dst = 0xa; break;
    case 'b': *dst = 0xb; break;
    case 'c': *dst = 0xc; break;
    case 'd': *dst = 0xd; break;
    case 'e': *dst = 0xe; break;
    case 'f': *dst = 0xf; break;
    case 'A': *dst = 0xa; break;
    case 'B': *dst = 0xb; break;
    case 'C': *dst = 0xc; break;
    case 'D': *dst = 0xd; break;
    case 'E': *dst = 0xe; break;
    case 'F': *dst = 0xf; break;
    default: return -1;
  }
  return 1;
}

// A function to shift to the left an uint8_t, and
// replace the right part with a nibble (4bits). 
int nibble_to_uint8(uint8_t nibble, uint8_t *dst) {
  if (nibble>0xf) return -1;
  if (nibble<0x0) return -1;
  *dst <<= 4;
  *dst |= nibble;
  return 0;
}

// this is the main parser, at this time, this is a naive
// implementation parsing only the digit and ignoring the
// separators.
// TODO: add support for the separator
// TODO: add a way to identify dynamically which kind of
//       format is being used in the input
int mac_parser(uint8_t *mac_address, struct token *tokens, size_t length) {
  int i;
  int counter = 0;
  int address_index = 0;
  uint8_t buffer = 0;

  for (i=0; i<length; i++) {
    (void)buffer;

    if (i==0 && tokens[i].type != MAC_TOKEN_SEPARATOR_DIGIT) {
      printf("error: mac address starting with a separator at position %d\n", i);
      return -1;
    }

    if (tokens[i].type == MAC_TOKEN_SEPARATOR_DIGIT) {
      buffer <<= 4;
      buffer |= char_to_uint8(tokens[i].value);
      if (counter%2) {
        mac_address[address_index] = buffer;
        buffer = 0;
        address_index++;
      }
      counter++;
    }
  }
  return 0;
}

// parse a string and convert it into mac address.
int mac_parse(uint8_t *mac_address, char *string, size_t string_len) {
  bzero(mac_address, sizeof(uint8_t)*MAC_SIZE);

  struct token tokens[string_len];
  bzero(tokens, sizeof(struct token)*string_len);

  for (int position=0; position<string_len; position++) {
    int ret = mac_tokenize(tokens, position, string[position]);
    if (ret<0) return -1;
  }

  if (mac_parser(mac_address, tokens, string_len)<0)
    return -1;

  return 0;
}

int is_char_digit(char c) {
  if (c>='0' && c<='9') return 1;
  if (c>='a' && c<='f') return 1;
  if (c>= 'A' && c <= 'F') return 1;
  return 0;
}

int is_char_separator(char c) {
  char sep[3] = {':', '-', '.'};
  for (int i=0; i<3; i++)
    if (c == sep[i]) return 1;
  return 0;
}
