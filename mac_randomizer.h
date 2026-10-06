/**
 * MAC Randomizer headers.
 */

#define MAC_SIZE 6

//
int mac_random(uint8_t *);
int mac_init(uint8_t *);

//
int mac_unicast_universal(uint8_t *);
int is_mac_unicast_universal(uint8_t *);

//
int mac_unicast_local(uint8_t *);
int is_mac_unicast_local(uint8_t *);

//
int mac_multicast_universal(uint8_t *);
int is_mac_multicast_universal(uint8_t *);

//
int mac_multicast_local(uint8_t *);
int is_mac_multicast_local(uint8_t *);

// tooling and helper functions
#define UNICAST_UNIVERSAL 0x00
#define UNICAST_LOCAL 0x01
#define MULTICAST_UNIVERSAL 0x02
#define MULTICAST_LOCAL 0x03

struct mac_info_s {
  uint8_t type;
};

void mac_print(uint8_t *);
int mac_info(uint8_t *, struct mac_info_s *);
