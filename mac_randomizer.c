/**
 *
 * https://en.wikipedia.org/wiki/MAC_address
 * https://en.wikipedia.org/wiki/MAC_address_anonymization
 * https://standards-oui.ieee.org/oui/oui.txt
 * https://standards-oui.ieee.org/oui28/mam.txt
 * https://standards-oui.ieee.org/oui36/oui36.txt
 * https://www.iana.org/assignments/ethernet-numbers
 * https://www.scitepress.org/Link.aspx?doi=10.5220/0009825105720579
 * https://www.scitepress.org/Papers/2020/98251/98251.pdf
 * https://github.com/winlibs/argon2
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#define MAC_SIZE 6

uint8_t mac_address[MAC_SIZE];

struct mac_info {
  uint8_t oui[3];
  uint8_t multicast;
  uint8_t local;
  uint8_t nic[3];
};

int init() {
  bzero(mac_address, sizeof(uint8_t) * MAC_SIZE);
  return 0;
}

int mac_random(uint8_t *mac_address) {
  arc4random_buf(mac_address, MAC_SIZE);
  return 0;
}

int mac_unicast(uint8_t *mac_address) {
  return 0;
}

int mac_multicast(uint8_t *mac_address) {
  return 0;
}

int mac_extended_local(uint8_t *mac_address) {
  return 0;
}

int mac_standard_assigned(uint8_t *mac_address) {
  return 0;
}

int mac_administratively_assigned(uint8_t *mac_address) {
  return 0;
}

int mac_reserved(uint8_t *mac_address) {
  return 0;
}

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

void mac_print(uint8_t *mac_address) {
  for (int i=0; i<MAC_SIZE; i++) {
    if (i==MAC_SIZE-1)
      printf("%02x", mac_address[i]);
    else
      printf("%02x:", mac_address[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  init();
  mac_random(mac_address);
  mac_print(mac_address);
  mac_info(mac_address);
  return 0;
}
