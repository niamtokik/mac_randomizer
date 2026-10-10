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
#include "minunit.h"
#include "mac.h"
#include "mac_test_helper.h"

/*********************************************************************
 * helper test suite.
 ********************************************************************/
MU_TEST(test_nibble_to_uint8) {
  uint8_t nibble = 0x0;
  uint8_t dst = 0x0;
  int ret = 0;
  
  ret = nibble_to_uint8(nibble, &dst);
  mu_assert(ret == 0, "return issue");
  mu_assert(dst == 0, "dst issue");

  nibble = 0x0f;
  dst = 0x00;
  ret = nibble_to_uint8(nibble, &dst);
  _debug("%x", dst);
  mu_assert(ret == 0, "return issue");
  mu_assert(dst == 0x0f, "dst issue");

  nibble = 0x01;
  dst = 0x12;
  ret = nibble_to_uint8(nibble, &dst);
  mu_assert(ret == 0, "return issue");
  mu_assert(dst == 0x21, "dst issue");
}

MU_TEST_SUITE(test_nibble_to_uint8_suite) {
  MU_RUN_TEST(test_nibble_to_uint8);
}

/*
 * This test is a draft for now.
 */
MU_TEST(test_mac_parser) {
  int ret = 0;
  char *string = "ff:ff:ff:ff:ff:ff";
  uint8_t mac_address[MAC_SIZE];

  mac_init(mac_address);
  ret = mac_parse(mac_address, string, 17);
  _mac_print(mac_address);
  mu_assert(ret == 0, "broadcast address parsing issue");
  for (int i=0; i<MAC_SIZE; i++) mu_assert(mac_address[i] == 0xff, "broadcast address");

  ret = 0;
  char *string2 = "00:00:00:00:00:00";
  mac_init(mac_address);
  ret = mac_parse(mac_address, string2, 17);
  _mac_print(mac_address);
  mu_assert(ret == 0, "null address parsing issue");
  for (int i=0; i<MAC_SIZE; i++) mu_assert(mac_address[i] == 0x00, "null address");

  // "random" valid lower case mac address
  ret = 0;
  char *string3 = "12:34:56:78:9a:bc";
  mac_init(mac_address);
  ret = mac_parse(mac_address, string3, 17);
  _mac_print(mac_address);
  mu_assert(ret == 0, "random valid lowercase address parsing issue");

  // "random" valid uppercase mac address
  ret = 0;
  char *string4 = "12:34:56:78:9A:BC";
  mac_init(mac_address);
  ret = mac_parse(mac_address, string4, 17);
  _mac_print(mac_address);
  mu_assert(ret == 0, "random valid uppercase address parsing issue");

  // "random" valid mixed mac address
  ret = 0;
  char *string5 = "12:34:56:78:9a:Bc";
  mac_init(mac_address);
  ret = mac_parse(mac_address, string5, 17);
  _mac_print(mac_address);
  mu_assert(ret == 0, "random valid mixed address parsing issue");

  // "random" valid mac address
  // ret = 0;
  // char *string7 = "1234.5678.9aBc";
  // mac_init(mac_address);
  // ret = mac_parse(mac_address, string7, 17);
  // mac_print(mac_address);
  // mu_assert(ret == 0, "random address parsing issue");

  // starting with invalid char
  // a mac address can't start a space
  // ret = -1;
  // char *invalid_string1 = " ff:ff:ff:ff:ff:ff";

  // starting with invalid char
  // a mac address can't start with a column
  // ret = -1;
  // char *invalid_string1 = ":ff:ff:ff:ff:ff:ff";

  // starting with invalid char
  // a mac address can't start with a dash
  // ret = -1;
  // char *invalid_string1 = "-ff-ff-ff-ff-ff-ff";
}

MU_TEST_SUITE(test_mac_parser_suite) {
  MU_RUN_TEST(test_mac_parser);
}

/*********************************************************************
 * Main test suite.
 ********************************************************************/
int main() {
  MU_RUN_SUITE(test_nibble_to_uint8_suite);
  MU_RUN_SUITE(test_mac_parser_suite);
  MU_REPORT();
  return MU_EXIT_CODE;
}
