/**
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "mac_randomizer.h"

/* MAC mask address */
uint8_t mac_unicast_universal_mask[4] = {0xf0, 0xf4, 0xf8, 0xfc};
size_t mac_unicast_universal_mask_l = 4;

uint8_t mac_unicast_local_mask[4] = {0xf2, 0xf6, 0xfa, 0xfe};
size_t mac_unicast_local_mask_l  = 4;

uint8_t mac_multicast_universal_mask[4] = {0xf1, 0xf5, 0xf9, 0xfd};
size_t mac_multicast_universal_mask_l = 4;

uint8_t mac_multicast_local_mask[4] = {0xf3, 0xf7, 0xfb, 0xff};
size_t mac_multicast_local_mask_l = 4;

/**
 *
 */
int mac_init(uint8_t *mac_address) {
  // start with a clean mac_address array
  bzero(mac_address, sizeof(uint8_t) * MAC_SIZE);
  return 0;
}

/**
* Gnererate a purely random mac address
*/
int mac_random(uint8_t *mac_address) {
  arc4random_buf(mac_address, MAC_SIZE);
  return 0;
}

/**
 * Returns a random unicast universally administrated MAC
 * address.
 */
int mac_unicast_universal(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  rand = rand % mac_unicast_universal_mask_l;
  mac_address[0] = mac_address[0] & mac_unicast_universal_mask[rand];
  return 0;
}

/**
 * Returns 1 if the input is an unicast universally MAC address.
 */
int is_mac_unicast_universal(uint8_t *mac_address) {
  // special case when the first bytes are set to 0x00.
  if ((mac_address[0] & ~0xf0) == 0)
    return 1;

  for (int i=0; i<mac_unicast_universal_mask_l; i++)
    if (mac_address[0] & ~mac_unicast_universal_mask[i])
      return 1;

  return 0;
}

/**
 * Returns a random unicast locally administrated MAC
 * address.
 */
int mac_unicast_local(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  rand = rand % mac_unicast_local_mask_l;
  mac_address[0] = mac_address[0] & mac_unicast_local_mask[rand];
  return 0;
}

/**
 *
 */
int is_mac_unicast_local(uint8_t *mac_address) {
  for (int i=0; i<mac_unicast_local_mask_l; i++)
    if (mac_address[0] & ~mac_unicast_local_mask[i])
      return 1;
  return 0;
}

/**
 * Returns a random multicast universally administrated MAC
 * address.
 */
int mac_multicast_universal(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  rand = rand % mac_multicast_universal_mask_l;
  mac_address[0] = mac_address[0] & mac_multicast_universal_mask[rand];
  return 0;
}

/**
 * Returns a random multicast locally administrated MAC
 * address.
 */
int mac_multicast_local(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  rand = rand % mac_multicast_local_mask_l;
  mac_address[0] = mac_address[0] & mac_multicast_local_mask[rand];
  return 0;
}

/**
 *
 */
int mac_extended_local(uint8_t *mac_address) {
  return 0;
}

/**
 *
 */
int mac_standard_assigned(uint8_t *mac_address) {
  return 0;
}

/**
 *
 */
int mac_administratively_assigned(uint8_t *mac_address) {
  return 0;
}

/**
 *
 */
int mac_reserved(uint8_t *mac_address) {
  return 0;
}

/**
 *
 */
int mac_info(uint8_t *mac_address) {
  uint8_t oui[3];
  bzero(oui, sizeof(uint8_t)*3);
  uint32_t buf = 0;
  uint32_t *ptr = (uint32_t *)oui;
  memcpy(oui, mac_address, sizeof(uint8_t)*3);
  memcpy(&buf, mac_address, sizeof(uint8_t)*3);
  // using diectly the array
  printf("oui: %02x:%02x:%02x\n", oui[0], oui[1], oui[2]);

  // copying the value
  printf("oui: %06x\n", htonl(buf) >> 8);

  // using a 32bits pointer
  printf("oui: %06x\n", htonl((*ptr)) >> 8);
  return 0;
}

/**
 *
 */
void mac_print(uint8_t *mac_address) {
  for (int i=0; i<MAC_SIZE; i++) {
    if (i==MAC_SIZE-1)
      printf("%02x", mac_address[i]);
    else
      printf("%02x:", mac_address[i]);
  }
  printf("\n");
}
