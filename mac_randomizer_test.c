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
// MU_TEST(test_unicast_universal_mac_address) {}
// MU_TEST(test_unicast_local_mac_address) {}


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
}

int main() {
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return MU_EXIT_CODE;
}
