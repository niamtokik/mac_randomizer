/**
 * MAC Address common shared functions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "mac.h"

/**
 * set (or reset) to zero a mac address.
 */
int mac_init(uint8_t *mac_address) {
  // start with a clean mac_address array
  bzero(mac_address, sizeof(uint8_t) * MAC_SIZE);
  return 0;
}
