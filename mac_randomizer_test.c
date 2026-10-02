/**
 * MAC Randomizer MinUnit Test Suite.
 */
#include <stdio.h>
#include "minunit.h"
#include "mac_randomizer.h"

uint8_t mac_address[6];

/**
 * randomly generated MAC addresses test.
 */
MU_TEST(generate_random_mac_address) {
	mac_random(mac_address);
	for (int i=0; i<6; i++) {
		mu_assert(mac_address[i] != 0, "the probably to be 0 is low");
	}
}

/**
 * Unicast MAC address test.
 */
MU_TEST(test_unicast_universal_mac_address) {
  uint8_t random_mac_address[6];
  mac_random(random_mac_address);
  mac_unicast_universal(random_mac_address);
  mu_assert(is_mac_unicast_local(random_mac_address) == 1, "must be an unicast universal mac address");
}

MU_TEST(test_unicast_local_mac_address) {
  // TODO: fix this test
  // uint8_t valid_unilocal0[MAC_SIZE] = {0x00, 0x12, 0x34, 0x56, 0x78, 0x9a};
  // mu_assert(is_mac_unicast_local(valid_unilocal0) == 1, "0x00 is valid");

  uint8_t valid_unilocal1[MAC_SIZE] = {0x04, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_local(valid_unilocal1) == 1, "0x04 is valid");

  uint8_t valid_unilocal2[MAC_SIZE] = {0x08, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_local(valid_unilocal2) == 1, "0x08 is valid");

  uint8_t valid_unilocal3[MAC_SIZE] = {0x0c, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_local(valid_unilocal3) == 1, "0x0c is valid");

  uint8_t invalid_unilocal0[MAC_SIZE] = {0x00, 0x12, 0x34, 0x56, 0x78, 0x9a};
  mu_assert(is_mac_unicast_local(invalid_unilocal0) == 0, "0x0f is invalid");
}

/**
 * Multicast MAC address test.
 */
// MU_TEST(test_multicast_universal_mac_address) {}
// MU_TEST(test_multicast_local_mac_address) {}

/**
 * Main test suite.
 */
MU_TEST_SUITE(test_suite) {
  MU_RUN_TEST(generate_random_mac_address);
  MU_RUN_TEST(test_unicast_local_mac_address);
}

int main() {
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return MU_EXIT_CODE;
}
