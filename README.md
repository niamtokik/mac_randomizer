# MAC Randomizer

`mac_randomizer` is a small tool used to randomizer MAC addresses. At
this time of writing, this tool is planned to run only on OpenBSD.

## Usage

Print the help message.

```console
$ ./mac_randomizer -h
Usage: ./mac_randomizer -[lumghs] [-o OID] [-c COUNTRY] [-C COMPANY]
  -h: print this message
  -u: unicast mac address
  -m: multicast mac address
  -l: locally assigned mac address
  -g: globally unique mac address
  -s: print the store
  -o OID: use an OID prefix
  -c COUNTRY: use an OID from the store using a country identifier
  -C COMPANY: use an OID from the store using a company identifier
  -i MAC: print information about a mac address
```

Generating a pure random 48bits MAC address

```console
$ ./mac_randomizer
5c:6d:2c:73:2c:43
```

Generating an unicast locally administrated MAC address:

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

Printing information about a mac address:

```console
$ ./mac_randomaizer -i 28:6F:B9:5c:6d:2c
address: 28:6F:B9:5c:6d:2c
oui: 28:6f:b9
unicast: true
multicast: false
locally administered: true
globally unique: false
country: CN
company: Nokia Shanghai Bell Co., Ltd
eli (ieee802c): false
sai (ieee802c): false
aai (ieee802c): false
reserved (ieee802c): false
```

## Build

```console
$ make
```

```console
$ make clean
```

## Ideas

### Multi format MAC address support input

- hexadecimal: 0x5c6d2c5c6d2c
- column: 5c:6d:2c:5c:6d:2c
- dash: 5c-6d-2c-5c-6d-2c
- grouped: 5c6d.2c5c.6d2c
- reversed with a flag(ieee802.3 and token ring):

### Multi format MAC address support output

see previous idea. flags to use:

- `-:` (default, can be ommited)
- `-x`
- `-d`
- `-.`

### MAC Address Random list

Generate 10 random MAC addresses:

```console
$ ./mac_randomizer -R 10
...
```

### SHA256 support

Enhanced privacy using SHA256.

### Argon2 support

Enhanced privacy using Argon2 PBKDF.

```console
$ ./mac_randomizer -a -S my_secret
...
```

```console
$ echo $mac_address | ./mac_randomizer -a -S my_secret
...
```

### OUI and MAC Addresses Information Store

`mac_randomizer` should have all standardized MAC address
information statically compiled in it to allow anyone to
extract those data on demand. It could take a huge amount
of memory, then, compression can be an idea. If compressed,
the data should be loaded in usable memory space only on
demand. Another idea is to use a memory mapping between

## FAQ

### Why another tool for MAC addresses?

I wanted to have a tool to manage random MAC addresses and
privacy-enhanced MAC addresses (using a pseudo-random address based
on HASH or PBKDF). I also wanted a fast solution to collect
information on MAC addresses based on their OID without asking
a remote web site.

### Is it stable?

Not yet.

### Is it secure?

It will be implemented with all OpenBSD safety like `pledge` and `unveil`
to limit security issues. It will also avoid using dynamic memory
allocation if possible.

### Is it portable?

No. At this time, it's only planned to run on OpenBSD.

### What about the license?

3-Clause BSD. Do whatever you want with my code.

### How to compile it?

You need a compiler (`gcc` or `clang`) and `gnumake`.

### How to contribute?

Create an issue or a PR on github.

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
