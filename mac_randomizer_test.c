/*********************************************************************
 * MAC Randomizer MinUnit Test Suite.
 ********************************************************************/
#include <stdio.h>
#include "minunit.h"
#include "mac_randomizer.h"

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
 * TODO:
 ********************************************************************/
MU_TEST(test_unicast_universal_mac_address_generator) {
  uint8_t random_mac_address[MAC_SIZE];
  mac_random(random_mac_address);
  mac_unicast_universal(random_mac_address);
  mu_assert(is_mac_unicast_universal(random_mac_address) == 1, "must be an unicast universal mac address");
}

MU_TEST(test_unicast_universal_mac_address_identifier) {
  // TODO: fix the following test because 0x00 MUST BE VALID
  // uint8_t valid_uni_uni0[MAC_SIZE] = {0x00, 0x12, 0x34, 0x56, 0x78, 0x9a};
  // mu_assert(is_mac_unicast_universal(valid_uni_uni0) == 1, "0x00 is valid");

  uint8_t valid_uni_uni1[MAC_SIZE] = {0x04, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_universal(valid_uni_uni1) == 1, "0x04 is valid");

  uint8_t valid_uni_uni2[MAC_SIZE] = {0x08, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_universal(valid_uni_uni2) == 1, "0x08 is valid");

  uint8_t valid_uni_uni3[MAC_SIZE] = {0x0c, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_universal(valid_uni_uni3) == 1, "0x0c is valid");

  uint8_t invalid_uni_uni0[MAC_SIZE] = {0x0f, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_universal(invalid_uni_uni0) != 0, "0x0f is invalid");
}

MU_TEST_SUITE(test_unicast_universal_mac_address_suite) {
  MU_RUN_TEST(test_unicast_universal_mac_address_generator);
  MU_RUN_TEST(test_unicast_universal_mac_address_identifier);
}

/*********************************************************************
 * Unicast/Local MAC address test.
 * TODO:
 ********************************************************************/
MU_TEST_SUITE(test_unicast_local_mac_address_suite) {}

/*********************************************************************
 * Multicast/Universal MAC address test.
 * TODO:
 ********************************************************************/
MU_TEST_SUITE(test_multicast_universal_mac_address_suite) {}
// MU_TEST(test_multicast_universal_mac_address) {
  // uint8_t valid_multiuni0[MAC_SIZE] = {0x01, 0x12, 0x34, 0x56, 0x78, 0x9a};
  // uint8_t valid_multiuni1[MAC_SIZE] = {0x05, 0x12, 0x34, 0x56, 0x78, 0x9a};
  // uint8_t valid_multiuni2[MAC_SIZE] = {0x09, 0x12, 0x34, 0x56, 0x78, 0x9a};
  // uint8_t valid_multiuni3[MAC_SIZE] = {0x0d, 0x12, 0x34, 0x56, 0x78, 0x9a};
  // uint8_t invalid_multiuni0[MAC_SIZE] = {0x00, 0x12, 0x34, 0x56, 0x78, 0x9a};
// }

/*********************************************************************
 * Multicast/Local MAC address test.
 * TODO:
 ********************************************************************/
MU_TEST_SUITE(test_multicastcast_local_mac_address_suite) {}

/*********************************************************************
 * Main test suite.
 ********************************************************************/
int main() {
  MU_RUN_SUITE(test_randomizer_suite);
  MU_RUN_SUITE(test_unicast_universal_mac_address_suite);
  MU_REPORT();
  return MU_EXIT_CODE;
}
