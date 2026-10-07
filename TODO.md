- Build:
  - [x] enable hardening clang flags
  - [x] add minunit support
  - [ ] add static analysis tool
  - [ ] add memory analysis tool (e.g. valgrind)

- Portability:
  - [ ] OpenBSD
    - [x] arc4random
    - [ ] pledge
    - [ ] unveil
  - [ ] FreeBSD
    - [ ] capsicum
  - [ ] NetBSD
  - [ ] DragonFlyBSD
  - [ ] Linux
  - [ ] low-level code (for embedded device and kernelland)

- Identification:
  - [ ] EUI-48 addresses:
    - [x] identify unicast universally administrated mac address
    - [x] identify unicast locally administrated mac address
    - [x] identify multicast universally administrated mac address
    - [x] identify multicast locally administrated mac address
    - [x] identify IEEE 802c extended local address
    - [x] identify IEEE 802c standard assigned address
    - [x] identify IEEE 802c adminstratively assigned address
    - [x] identify IEEE 802c reserved address
  - [ ] EUI-64 addresses:
    - add support
  - [ ] create data-structure mac address information

- Parser
  - [ ] parse EUI-48 addresses
  - [ ] parse EUI-64 addresses
  - [ ] parse addresses with dash separator
  - [ ] parse addresses with column separator
  - [ ] parse addresses with dot separator

- Generator
  - [x] generate random EUI-48 address
  - [ ] generate random EUI-64 address

- Converter
  - [ ] convert EUI-48 to EUI-64
  - [ ] convert EUI-64 to EUI-48

- Authentication/Security
  - [ ] implement pseudo-random mac address generator
    - [ ] add sha256 support
    - [ ] add hmac-sha256 support
    - [ ] add argon2 support
