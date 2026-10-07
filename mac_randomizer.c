/**
 * MAC Address random generators.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "mac.h"

/**
* Gnererate a purely random mac address. uses arc4random_buf
* at this time.
*/
int mac_random(uint8_t *mac_address) {
  arc4random_buf(mac_address, MAC_SIZE);
  return 0;
}

