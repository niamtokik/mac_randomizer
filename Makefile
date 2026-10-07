######################################################################
# MAC Address Randomizer GNU Makefile
######################################################################
BUILD_DIR ?= ./_build
CC_FLAGS ?= -g -std=c99 -O2 -static -Wall -Werror -Wformat=2 -Wconversion -Wsign-conversion \
						-Wimplicit-fallthrough -Werror=format-security \
						-Werror=implicit -Werror=incompatible-pointer-types -Werror=int-conversion \
						-D_FORTIFY_SOURCE=3 -fstrict-flex-arrays=3  \
						-fstack-protector-strong -fcf-protection=full \
						-fzero-call-used-regs=used-gpr -fno-delete-null-pointer-checks -fno-strict-overflow \
						-fno-strict-aliasing -fexceptions 

OBJECTS = $(BUILD_DIR)/mac_common.o \
					$(BUILD_DIR)/mac_randomizer.o \
					$(BUILD_DIR)/mac_parser.o \
					$(BUILD_DIR)/mac_identifier.o

######################################################################
# help target.
######################################################################
.PHONY += help
help:
	@echo "Usage: make [all|test|auto|clean]"

######################################################################
# main build directory target.
######################################################################
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

######################################################################
# objects targets.
######################################################################
$(BUILD_DIR)/mac_common.o: $(BUILD_DIR)
	cc $(CC_FLAGS) -c -O -fPIC -o $@ mac_common.c 

$(BUILD_DIR)/mac_randomizer.o: $(BUILD_DIR)
	cc $(CC_FLAGS) -c -O -fPIC -o $@ mac_randomizer.c 

$(BUILD_DIR)/mac_parser.o: $(BUILD_DIR)
	cc $(CC_FLAGS) -c -O -fPIC -o $@ mac_parser.c 

$(BUILD_DIR)/mac_identifier.o: $(BUILD_DIR)
	cc $(CC_FLAGS) -c -O -fPIC -o $@ mac_identifier.c 

######################################################################
# main mac address randomizer cli application.
######################################################################
$(BUILD_DIR)/mac: $(OBJECTS)
	cc $(CC_FLAGS) -fPIE -pie -o $@ $(OBJECTS) mac.c

######################################################################
# test unit mac address randomizer application.
######################################################################
$(BUILD_DIR)/mac_randomizer_test: $(OBJECTS)
	cc $(CC_FLAGS) -o $@ $(OBJECTS) mac_randomizer_test.c

######################################################################
# main targets.
######################################################################
.PHONY += all
all: $(BUILD_DIR)/mac

.PHONY += auto
auto: clean all test

.PHONY += test
test: $(BUILD_DIR)/mac_randomizer_test
	$(BUILD_DIR)/mac_randomizer_test

.PHONY += clean
clean:
	-rm $(OBJECTS)
	-rm $(BUILD_DIR)/mac
	-rm $(BUILD_DIR)/mac_randomizer_test

.PHONY: $(.PHONY)
