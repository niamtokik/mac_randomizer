/**
 * MAC Randomizer Command Line Interface.
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
#include "mac_randomizer.h"
uint8_t mac_address[MAC_SIZE];

/* MAC address information data structure */
struct mac_info {
  uint8_t oui[3];
  uint8_t multicast;
  uint8_t local;
  uint8_t nic[3];
};

struct command_opts_s {
  // program name
  char name[128];

  // -c: country flag
  char country[2];

  // -C: company flag
  char company[256];

  // -h: help flag
  uint8_t help;

  // -u: unicast flag
  uint8_t unicast;

  // -m: multicast flag
  uint8_t multicast;

  // -l: locally assigned flag
  uint8_t locally_assigned;

  // -g: globally unique flag
  uint8_t globally_unique;

  // -s: store export flag
  uint8_t store_export;

  // -i: info flag
  uint8_t info;

} command_opts;

int init() {
  // start with a clean command_opts structure
  bzero(&command_opts, sizeof(command_opts));
  return 0;
}

int parse_args(int argc, char *argv[]) {
  (void)strncpy(command_opts.name, argv[0], 128);
  uint8_t is_arg = 0;
  char *params = NULL;
  for (int i=1; i<argc; i++) {
    for (int j=0; j<strnlen(argv[i], 256); j++) {
      if (j==0) {
        is_arg=0;
        params = NULL;
      }
      switch (argv[i][j]) {
        case '-': 
          if (j==0) is_arg=1;
          continue;
        case 'h': 
          if (j>0 && is_arg==1) command_opts.help=1;
          continue;
        case 'u': 
          if (j>0 && is_arg==1) command_opts.unicast=1;
          continue;
        case 'm': 
          if (j>0 && is_arg==1) command_opts.multicast=1;
          continue;
        case 'l':
          if (j>0 && is_arg==1) command_opts.locally_assigned=1;
          continue;
        case 'g':
          if (j>0 && is_arg==1) command_opts.globally_unique=1;
          continue;
        case 'c': 
          if (j>0 && is_arg==1) {
            params = command_opts.country;
          };
          continue;
        case 'C':
          if (j>0 && is_arg==1) {
          params = command_opts.company;
          }
          continue;
      }
    }
  }
  return 0; 
}

void show_opts() {
  printf("name: %s\n", command_opts.name);
  printf("country: %s\n", command_opts.country);
  printf("company: %s\n", command_opts.company);
  printf("help: %x\n", command_opts.help);
  printf("unicast: %x\n", command_opts.unicast);
  printf("multicast: %x\n", command_opts.multicast);
  printf("locally_assigned: %x\n", command_opts.locally_assigned);
  printf("globally_unique: %x\n", command_opts.globally_unique);
  printf("store_export: %x\n", command_opts.store_export);
  printf("info: %x\n", command_opts.info);
}


int usage() {
  printf("Usage: %s [...]\n", command_opts.name);
  return 0;
}

int main(int argc, char *argv[]) {
  init();
  parse_args(argc, argv);

  show_opts();

  if (command_opts.help) {
    usage();
    return 1;
  }

  mac_random(mac_address);
  mac_unicast_universal(mac_address);
  mac_print(mac_address);
  printf("debug: %x\n", is_mac_unicast_local(mac_address));
  printf("debug: %x\n", is_mac_unicast_universal(mac_address));
  mac_info(mac_address);
  return 0;
}
