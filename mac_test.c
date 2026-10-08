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
 * MAC Randomizer MinUnit Test Suite.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "mac.h"

// helper function to print mac address if DEBUG flag is set.
void _mac_print(uint8_t *mac_address){
  if (getenv("DEBUG"))
    mac_print(mac_address);
}

/*********************************************************************
 * initialization function test.
 ********************************************************************/
MU_TEST(test_init_mac_address) {
  uint8_t mac_address[MAC_SIZE+1];
  int i=0;

  for (i=0; i<MAC_SIZE+1; i++)
    mac_address[i] = 0xff;
  
  for (i=0; i<MAC_SIZE+1; i++)
    mu_assert(mac_address[i] == 0xff, "set canary issue");

  // cleanup the buffer
  mac_init(mac_address);
  for (i=0; i<MAC_SIZE; i++)
    mu_assert(mac_address[i] == 0x00, "cleanup function issue");

  // buffer overflow, it should not overwrite the
  // canaries.
  mu_assert(mac_address[MAC_SIZE] == 0xff, "buffer overflow, canary overwritten");
}

MU_TEST_SUITE(test_init_suite) {
  MU_RUN_TEST(test_init_mac_address);
}

/*********************************************************************
 * randomly generated MAC addresses test.
 ********************************************************************/
MU_TEST(test_random_mac_address_generator) {
  uint8_t mac_address[MAC_SIZE];
	mac_random(mac_address);
	for (int i=0; i<MAC_SIZE; i++) {
		mu_assert(mac_address[i] != 0, "the probably to be 0 is low");
	}
}

MU_TEST_SUITE(test_randomizer_suite) {
  MU_RUN_TEST(test_random_mac_address_generator);
}

/*********************************************************************
 * Unicast/Universal MAC address test.
 ********************************************************************/
MU_TEST(test_unicast_universal_mac_address_generator) {
  uint8_t random_mac_address[MAC_SIZE];
  mac_init(random_mac_address);
  mac_random(random_mac_address);
  mac_unicast_universal(random_mac_address);
  _mac_print(random_mac_address);
  mu_assert(is_mac_unicast_universal(random_mac_address) == 1, "must be an unicast universal mac address");
}

MU_TEST(test_unicast_universal_mac_address_identifier) {
  uint8_t valid_uni_uni0[MAC_SIZE] = {0x70, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_universal(valid_uni_uni0) == 1, "0x00 must be an unicast universal address");

  uint8_t valid_uni_uni1[MAC_SIZE] = {0xe4, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_universal(valid_uni_uni1) == 1, "0x04 must be an unicast universal address");

  uint8_t valid_uni_uni2[MAC_SIZE] = {0xf8, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_universal(valid_uni_uni2) == 1, "0x08 must be an unicast universal address");

  uint8_t valid_uni_uni3[MAC_SIZE] = {0xbc, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_universal(valid_uni_uni3) == 1, "0x0c must be an unicast universal address");

  uint8_t invalid_uni_uni0[MAC_SIZE] = {0xaf, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_universal(invalid_uni_uni0) == 0, "0x0f is not an unicast universal address");
}

MU_TEST_SUITE(test_unicast_universal_mac_address_suite) {
  MU_RUN_TEST(test_unicast_universal_mac_address_generator);
  MU_RUN_TEST(test_unicast_universal_mac_address_identifier);
}

/*********************************************************************
 * Unicast/Local MAC address test.
 ********************************************************************/
MU_TEST(test_unicast_local_mac_address_generator) {
  uint8_t random_mac_address[MAC_SIZE];
  mac_init(random_mac_address);
  mac_random(random_mac_address);
  mac_unicast_local(random_mac_address);
  _mac_print(random_mac_address);
  mu_assert(is_mac_unicast_local(random_mac_address) == 1, "must be an unicast local mac address");
}

MU_TEST(test_unicast_local_mac_address_identifier) {
  uint8_t valid_uni_loc0[MAC_SIZE] = {0x32, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_local(valid_uni_loc0) == 1, "0x02 must be an unicast local address");

  uint8_t valid_uni_loc1[MAC_SIZE] = {0x26, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_local(valid_uni_loc1) == 1, "0x06 must be an unicast local address");

  uint8_t valid_uni_loc2[MAC_SIZE] = {0x7a, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_local(valid_uni_loc2) == 1, "0x0a must be an unicast local address");

  uint8_t valid_uni_loc3[MAC_SIZE] = {0xbe, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_local(valid_uni_loc3) == 1, "0x0e must be an unicast local address");

  uint8_t invalid_uni_loc0[MAC_SIZE] = {0xa0, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_local(invalid_uni_loc0) == 0, "0x00 should not be an unicast local address");
}

MU_TEST_SUITE(test_unicast_local_mac_address_suite) {
  MU_RUN_TEST(test_unicast_local_mac_address_generator);
  MU_RUN_TEST(test_unicast_local_mac_address_identifier);
}

/*********************************************************************
 * Multicast/Universal MAC address test.
 ********************************************************************/
MU_TEST(test_multicast_universal_mac_address_generator) {
  uint8_t random_mac_address[MAC_SIZE];
  mac_init(random_mac_address);
  mac_random(random_mac_address);
  mac_multicast_universal(random_mac_address);
  _mac_print(random_mac_address);
  mu_assert(is_mac_multicast_universal(random_mac_address) == 1, "must be a multicast universal mac address");
}

MU_TEST(test_multicast_universal_mac_address) {
  uint8_t valid_multiuni0[MAC_SIZE] = {0x21, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_multicast_universal(valid_multiuni0) == 1, "0x01 must be a multicast universal address");

  uint8_t valid_multiuni1[MAC_SIZE] = {0x15, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_multicast_universal(valid_multiuni1) == 1, "0x05 must be a multicast universal address");

  uint8_t valid_multiuni2[MAC_SIZE] = {0xe9, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_multicast_universal(valid_multiuni2) == 1, "0x09 must be a multicast universal address");

  uint8_t valid_multiuni3[MAC_SIZE] = {0xad, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_multicast_universal(valid_multiuni3) == 1, "0x0d must be a multicast universal address");

  uint8_t invalid_multiuni0[MAC_SIZE] = {0x00, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_multicast_universal(invalid_multiuni0) == 0, "0x00 is not a multicast universal address");
}

MU_TEST_SUITE(test_multicast_universal_mac_address_suite) {
  MU_RUN_TEST(test_multicast_universal_mac_address_generator);
  MU_RUN_TEST(test_multicast_universal_mac_address);  
}

/*********************************************************************
 * Multicast/Local MAC address test.
 ********************************************************************/
MU_TEST(test_multicast_local_mac_address_generator) {
  uint8_t random_mac_address[MAC_SIZE];
  mac_init(random_mac_address);
  mac_random(random_mac_address);
  mac_multicast_local(random_mac_address);
  _mac_print(random_mac_address);
  mu_assert(is_mac_multicast_local(random_mac_address) == 1, "must be a multicast local mac address");
}

MU_TEST(test_multicast_local_mac_address) {
  uint8_t valid_multiuni0[MAC_SIZE] = {0x23, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_multicast_local(valid_multiuni0) == 1, "0x01 must be a multicast local address");

  uint8_t valid_multiuni1[MAC_SIZE] = {0x17, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_multicast_local(valid_multiuni1) == 1, "0x05 must be a multicast local address");

  uint8_t valid_multiuni2[MAC_SIZE] = {0xeb, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_multicast_local(valid_multiuni2) == 1, "0x09 must be a multicast local address");

  uint8_t valid_multiuni3[MAC_SIZE] = {0xaf, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_multicast_local(valid_multiuni3) == 1, "0x0d must be a multicast local address");

  uint8_t invalid_multiuni0[MAC_SIZE] = {0x00, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_multicast_local(invalid_multiuni0) == 0, "0x00 must be a multicast local address");
}

MU_TEST_SUITE(test_multicast_local_mac_address_suite) {
  MU_RUN_TEST(test_multicast_local_mac_address_generator);
  MU_RUN_TEST(test_multicast_local_mac_address);  
}

/*********************************************************************
 * ieee 802c 
 ********************************************************************/
MU_TEST(test_mac_address_ieee_802c) {
  uint8_t mac_address[MAC_SIZE];
  mac_init(mac_address);
  mac_extended_local(mac_address);
  mu_assert(is_mac_extended_local(mac_address) == 1, "mac extended local");

  mac_init(mac_address);
  mac_standard_assigned(mac_address);
  mu_assert(is_mac_standard_assigned(mac_address) == 1, "mac standard assigned");

  mac_init(mac_address);
  mac_administratively_assigned(mac_address);
  mu_assert(is_mac_administratively_assigned(mac_address) == 1, "mac administratively assigned");

  mac_init(mac_address);
  mac_reserved(mac_address);
  mu_assert(is_mac_reserved(mac_address) == 1, "mac reserved");
}

MU_TEST_SUITE(test_mac_address_ieee_802c_suite) {
  MU_RUN_TEST(test_mac_address_ieee_802c);
}

/*
 *
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
 * mac information
 ********************************************************************/
MU_TEST(test_mac_address_info) {
  uint8_t random_mac_address[MAC_SIZE];
  mac_init(random_mac_address);
  mac_random(random_mac_address);
}

MU_TEST_SUITE(test_mac_address_info_suite) {
  MU_RUN_TEST(test_mac_address_info);
}

/*********************************************************************
 * Main test suite.
 ********************************************************************/
int main() {
  MU_RUN_SUITE(test_init_suite);
  MU_RUN_SUITE(test_randomizer_suite);

  // unicast suite
  MU_RUN_SUITE(test_unicast_universal_mac_address_suite);
  MU_RUN_SUITE(test_unicast_local_mac_address_suite);

  // multicast suite
  MU_RUN_SUITE(test_multicast_universal_mac_address_suite);
  MU_RUN_SUITE(test_multicast_local_mac_address_suite);

  // ieee802c extension
  MU_RUN_SUITE(test_mac_address_ieee_802c_suite);

  // parser
  MU_RUN_SUITE(test_mac_parser_suite);

  // info
  MU_RUN_SUITE(test_mac_address_info_suite);

  MU_REPORT();
  return MU_EXIT_CODE;
}
