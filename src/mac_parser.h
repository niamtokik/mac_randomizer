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
 * MAC Tokenizer and Parser headers.
 *
 */

// TODO: move that in parser headers
// valid token separators: "-", ":", and "."
#define MAC_TOKEN_DIGIT 0x01
#define MAC_TOKEN_COLUMN 0x02
#define MAC_TOKEN_DASH 0x03
#define MAC_TOKEN_DOT 0x04

// a mac token contains the position of the token
// the type of token (digit or separator) and its
// raw value
struct token {
  int position;
  int type;
  char value;
  void *data;
  size_t data_length; 
};

// tokenizer functions
void mac_token(struct token*, int, int, char);
int mac_tokenize(struct token*, int, char);
void mac_token_print(struct token *);

// parser functions
int mac_parse(uint8_t *, char *, size_t);

// helper
uint8_t char_to_uint8(char);
int char_to_uint8n(char, uint8_t *);
int nibble_to_uint8(uint8_t, uint8_t *);
int uint8_to_nibble(uint8_t, uint8_t *);
int is_char_digit(char);
int is_char_separator(char);
