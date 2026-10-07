/**
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "mac.h"

// a mac token contains the position of the token
// the type of token (digit or separator) and its
// raw value
struct token {
  int position;
  int type;
  char value;
};

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

// parse a string and convert it into mac address.
int mac_parse(uint8_t *mac_address, char *string, size_t string_len) {
  struct token tokens[string_len];
  bzero(tokens, sizeof(struct token)*string_len);

  for (int position=0; position<string_len; position++) {
    int ret = mac_tokenize(tokens, position, string[position]);
    if (ret<0) return -1;
  }

  // int digit_index=0;
  for (int i=0; i<string_len; i++) {
    if (i==0 && tokens[i].type != MAC_TOKEN_SEPARATOR_DIGIT) {
      printf("error: mac address starting with a separator at position %d\n", i);
      return -1;
    }

    if (tokens[i].type == MAC_TOKEN_SEPARATOR_DIGIT) {
      // uint8_t value = char_to_uint8(tokens[i].value);
    }
  }

  return 0;
}
