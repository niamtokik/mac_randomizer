/**
 * MAC Randomizer headers.
 */

#define MAC_SIZE 6

//
int mac_random(uint8_t *);

//
int mac_unicast_universal(uint8_t *);
int is_mac_unicast_universal(uint8_t *);

//
int mac_unicast_local(uint8_t *);
int is_mac_unicast_local(uint8_t *);

//
int mac_multicast_universal(uint8_t *);
int mac_multicast_local(uint8_t *);

// tooling and helper functions
void mac_print(uint8_t *);
int mac_info(uint8_t *);
