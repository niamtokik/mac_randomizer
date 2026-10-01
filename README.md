# MAC Randomizer

`mac_randomizer` is a small tool used to randomizer MAC addresses.

## Usage

Generating a pure random 48bits MAC address

```console
$ ./mac_randomizer
5c:6d:2c:73:2c:43
```

Generating an unicast locally adminstrated MAC address:

```console
$ ./mac_randomizer -l -u
...
```

Generating a multicast globally unique MAC address:

```console
$ ./mac_randomizer -m -g
...
```

Generating a random MAC address based on an OUI:

```console
$ ./mac_randomizer -o 0x286FB9
28:6F:B9:aa:bb:cc

$ ./mac_randomizer -o 28:6F:B9
28:6F:B9:aa:bb:cc
```

Generating a random MAC address based using registered OUI country pattern.

```console
$ ./mac_randomizer -c "CN"
88:B3:62:aa:bb:cc
```

Generating a random MAC address based using registered OUI company pattern:

```console
$ ./mac_randomizer -C "Nokia Shanghai Bell Co., Ltd."
88:B3:62:aa:bb:cc
```

Printing the OUI store (CSV-like output):

```console
$ ./mac_randomzier -s
oui;country;company
28:6F:B9;CN;Nokia Shanghai Bell Co., Ltd.
...
```

## Build

```console
$ make
```

```console
$ make clean
```

## OUI Store

## BUGS AND CAVEATS

# References and Resources

 * https://en.wikipedia.org/wiki/MAC_address
 * https://en.wikipedia.org/wiki/MAC_address_anonymization
 * https://standards-oui.ieee.org/oui/oui.txt
 * https://standards-oui.ieee.org/oui28/mam.txt
 * https://standards-oui.ieee.org/oui36/oui36.txt
 * https://www.iana.org/assignments/ethernet-numbers
 * https://www.scitepress.org/Link.aspx?doi=10.5220/0009825105720579
 * https://www.scitepress.org/Papers/2020/98251/98251.pdf
 * https://github.com/winlibs/argon2
