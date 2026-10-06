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
uint8_t mac_unicast_universal_mask[4] = {0x00, 0x04, 0x08, 0x0c};
size_t mac_unicast_universal_mask_l = 4;

uint8_t mac_unicast_local_mask[4] = {0x02, 0x06, 0x0a, 0x0e};
size_t mac_unicast_local_mask_l = 4;

uint8_t mac_multicast_universal_mask[4] = {0x01, 0x05, 0x09, 0x0d};
size_t mac_multicast_universal_mask_l = 4;

uint8_t mac_multicast_local_mask[4] = {0x03, 0x07, 0x0b, 0x0f};
size_t mac_multicast_local_mask_l = 4;

/**
 * set (or reset) to zero a mac address.
 */
int mac_init(uint8_t *mac_address) {
  // start with a clean mac_address array
  bzero(mac_address, sizeof(uint8_t) * MAC_SIZE);
  return 0;
}

/**
 *
 * supported format:
 *   xx:xx:xx:xx:xx:xx
 *   xx-xx-xx-xx-xx-xx
 *   xxxx.xxxx.xxxx
 */
int mac_parse(uint8_t *mac_address, char *string, size_t string_len) {
  int i=0;
  int counter=1;
  int pos=0;
  uint8_t buffer;

  for(i=0; i<string_len || pos<MAC_SIZE || counter<=12; i++) {
    buffer = (uint8_t)(buffer << 4);
    if (counter%2 == 0) {
      mac_address[pos] = buffer;
      pos++;
    }

    printf("state: %d %d %d %x %c\n", i, counter, pos, buffer, string[i]);

    switch (string[i]) {
      case '0': buffer = buffer | 0x0; counter++; continue;
      case '1': buffer = buffer | 0x1; counter++; continue;
      case '2': buffer = buffer | 0x2; counter++; continue;
      case '3': buffer = buffer | 0x3; counter++; continue;
      case '4': buffer = buffer | 0x4; counter++; continue;
      case '5': buffer = buffer | 0x5; counter++; continue;
      case '6': buffer = buffer | 0x6; counter++; continue;
      case '7': buffer = buffer | 0x7; counter++; continue;
      case '8': buffer = buffer | 0x8; counter++; continue;
      case '9': buffer = buffer | 0x9; counter++; continue;
      case 'a': buffer = buffer | 0xa; counter++; continue;
      case 'b': buffer = buffer | 0xb; counter++; continue;
      case 'c': buffer = buffer | 0xc; counter++; continue;
      case 'd': buffer = buffer | 0xd; counter++; continue;
      case 'e': buffer = buffer | 0xe; counter++; continue;
      case 'f': buffer = buffer | 0xf; counter++; continue;
      case 'A': buffer = buffer | 0xa; counter++; continue;
      case 'B': buffer = buffer | 0xb; counter++; continue;
      case 'C': buffer = buffer | 0xc; counter++; continue;
      case 'D': buffer = buffer | 0xd; counter++; continue;
      case 'E': buffer = buffer | 0xe; counter++; continue;
      case 'F': buffer = buffer | 0xf; counter++; continue;
      case ':': continue;
      case '-': continue;
      case '.': continue;
      default: return -1;
    }
  }
  return 0;  
}

/**
* Gnererate a purely random mac address. uses arc4random_buf
* at this time.
*/
int mac_random(uint8_t *mac_address) {
  arc4random_buf(mac_address, MAC_SIZE);
  return 0;
}

/**
 * check if a mac address is a broadcast address
 */
int is_mac_broadcast(uint8_t *mac_address) {
  for (int i=0; i<MAC_SIZE; i++)
    if (mac_address[i] != 0xff)
      return 0;
  return 1;
}

/**
 * Returns a random unicast universally administrated MAC
 * address.
 */
void mac_unicast_universal(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  rand = rand % mac_unicast_universal_mask_l;
  mac_address[0] = (mac_address[0] & 0xf0) | mac_unicast_universal_mask[rand];
}

/**
 * Returns 1 if the input is an unicast universally MAC address.
 */
int is_mac_unicast_universal(uint8_t *mac_address) {
  for (int i=0; i<mac_unicast_universal_mask_l; i++)
    if ((mac_address[0] & 0x0f) == mac_unicast_universal_mask[i])
      return 1;

  return 0;
}

/**
 * Returns a random unicast locally administrated MAC
 * address.
 */
void mac_unicast_local(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  rand = rand % mac_unicast_local_mask_l;
  // TODO: fix when mac_address[0] is set to 0x_0
  // it also impacts other functions.
  mac_address[0] = (mac_address[0] & 0xf0) | mac_unicast_local_mask[rand];
}

/**
 * Returns 1 if the mac address provided is an unicast
 * locally administered address, else returns 0.
 */
int is_mac_unicast_local(uint8_t *mac_address) {
  for (int i=0; i<mac_unicast_local_mask_l; i++)
    if ((mac_address[0] & 0x0f) == mac_unicast_local_mask[i])
      return 1;

  return 0;
}

/**
 * Returns a random multicast universally administrated MAC
 * address.
 */
void mac_multicast_universal(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  rand = rand % mac_multicast_universal_mask_l;
  mac_address[0] = (mac_address[0] & 0xf0) | mac_multicast_universal_mask[rand];
}

int is_mac_multicast_universal(uint8_t *mac_address) {
  for (int i=0; i<mac_multicast_universal_mask_l; i++)
    if ((mac_address[0] & 0x0f) == mac_multicast_universal_mask[i])
      return 1;

  return 0;
}

/**
 * Returns a random multicast locally administrated MAC
 * address.
 */
void mac_multicast_local(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  rand = rand % mac_multicast_local_mask_l;
  mac_address[0] = (mac_address[0] & 0xf0) | mac_multicast_local_mask[rand];
}

int is_mac_multicast_local(uint8_t *mac_address) {
  for (int i=0; i<mac_multicast_local_mask_l; i++)
    if ((mac_address[0] & 0x0f) == mac_multicast_local_mask[i])
      return 1;

  return 0;
}

/**
 *
 */
void mac_extended_local(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  mac_address[0] = (mac_address[0] & 0xf0) | 0xa;
}


int is_mac_extended_local(uint8_t *mac_address) {
  if ((mac_address[0] & 0x0f) == 0xa) 
    return 1;
  return 0;
}

/**
 *
 */
void mac_standard_assigned(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  mac_address[0] = (mac_address[0] & 0xf0) | 0xe;
}

int is_mac_standard_assigned(uint8_t *mac_address) {
  if ((mac_address[0] & 0x0f) == 0xe) 
    return 1;
  return 0;
}

/**
 *
 */
void mac_administratively_assigned(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  mac_address[0] = (mac_address[0] & 0xf0) | 0x2;
}

int is_mac_administratively_assigned(uint8_t *mac_address) {
  if ((mac_address[0] & 0x0f) == 0x2) 
    return 1;
  return 0;
}

/**
 *
 */
void mac_reserved(uint8_t *mac_address) {
  uint8_t rand = 0;
  arc4random_buf(&rand, sizeof(uint8_t));
  mac_address[0] = (mac_address[0] & 0xf0) | 0x6;
}

int is_mac_reserved(uint8_t *mac_address) {
  if ((mac_address[0] & 0x0f) == 0x6) 
    return 1;
  return 0;
}

/**
 *
 */
int mac_info(uint8_t *mac_address, struct mac_info_s *mac_info) {
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
