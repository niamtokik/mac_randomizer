/**
 * MAC Randomizer headers.
 */

#define MAC_SIZE 6

//
int mac_init(uint8_t *);
int mac_random(uint8_t *);

int mac_parse(uint8_t *, char *, size_t);

//
int is_mac_broadcast(uint8_t *);

//
void mac_unicast_universal(uint8_t *);
int is_mac_unicast_universal(uint8_t *);

//
void mac_unicast_local(uint8_t *);
int is_mac_unicast_local(uint8_t *);

//
void mac_multicast_universal(uint8_t *);
int is_mac_multicast_universal(uint8_t *);

//
void mac_multicast_local(uint8_t *);
int is_mac_multicast_local(uint8_t *);

//
void mac_extended_local(uint8_t *);
int is_mac_extended_local(uint8_t *);

//
void mac_standard_assigned(uint8_t *);
int is_mac_standard_assigned(uint8_t *);

//
void mac_administratively_assigned(uint8_t *);
int is_mac_administratively_assigned(uint8_t *);

//
void mac_reserved(uint8_t *);
int is_mac_reserved(uint8_t *);

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
