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
 * MAC Randomizer headers.
 *
 */
#define MAC_SIZE 6
// TODO: implement MAC EUI48/64:
// #define MAC_EUI_48_SIZE 6
// #define MAC_EUI_64_SIZE 8
#define MAC_TOKEN_SEPARATOR_DIGIT 0x01
#define MAC_TOKEN_SEPARATOR_COLUMN 0x02
#define MAC_TOKEN_SEPARATOR_DASH 0x03
#define MAC_TOKEN_SEPARATOR_DOT 0x04

// cleanup and initialize a mac address buffer
int mac_init(uint8_t *);

// generate random mac address
int mac_random(uint8_t *);

//
int is_mac_broadcast(uint8_t *);

//
void mac_unicast_universal(uint8_t *);
int is_mac_unicast_universal(uint8_t *);

//
void mac_unicast_local(uint8_t *);
int is_mac_unicast_local(uint8_t *);

//
void mac_multicast_universal(uint8_t *);
int is_mac_multicast_universal(uint8_t *);

//
void mac_multicast_local(uint8_t *);
int is_mac_multicast_local(uint8_t *);

//
void mac_extended_local(uint8_t *);
int is_mac_extended_local(uint8_t *);

//
void mac_standard_assigned(uint8_t *);
int is_mac_standard_assigned(uint8_t *);

//
void mac_administratively_assigned(uint8_t *);
int is_mac_administratively_assigned(uint8_t *);

//
void mac_reserved(uint8_t *);
int is_mac_reserved(uint8_t *);

// parser from mac_parser.c
int mac_parse(uint8_t *, char *, size_t);

// tooling and helper functions
#define UNICAST_UNIVERSAL 0x00
#define UNICAST_LOCAL 0x01
#define MULTICAST_UNIVERSAL 0x02
#define MULTICAST_LOCAL 0x03

struct mac_info_s {
  uint8_t type;
};

void mac_print(uint8_t *);
int mac_info(uint8_t *, struct mac_info_s *);

/*
 * mac fsm section
 */
#include "mac_fsm.h"
