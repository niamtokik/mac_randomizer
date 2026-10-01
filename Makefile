######################################################################
# GNU Makefile
######################################################################
all: mac_randomizer

mac_randomizer:
	cc -o $@ mac_randomizer.c

clean:
	rm mac_randomizer
