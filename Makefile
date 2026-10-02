######################################################################
# GNU Makefile
######################################################################
BUILD_DIR ?= ./_build
CC_FLAGS ?= -g -std=c99 -O2 -static -Wall -Werror -Wformat=2 -Wconversion -Wsign-conversion \
						-Wimplicit-fallthrough -Werror=format-security \
						-Werror=implicit -Werror=incompatible-pointer-types -Werror=int-conversion \
						-D_FORTIFY_SOURCE=3 -fstrict-flex-arrays=3  \
						-fstack-protector-strong -fcf-protection=full \
						-fzero-call-used-regs=used-gpr -fno-delete-null-pointer-checks -fno-strict-overflow \
						-fno-strict-aliasing -fexceptions 

.PHONY += all
all: $(BUILD_DIR)/mac test

test: $(BUILD_DIR)/mac_randomizer_test
	$(BUILD_DIR)/mac_randomizer_test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/mac_randomizer.o: $(BUILD_DIR)
	cc $(CC_FLAGS) -c -O -fPIC -o $@ mac_randomizer.c 

$(BUILD_DIR)/mac: $(BUILD_DIR)/mac_randomizer.o
	cc $(CC_FLAGS) -fPIE -pie -o $@ $(BUILD_DIR)/mac_randomizer.o mac.c

$(BUILD_DIR)/mac_randomizer_test: $(BUILD_DIR) $(BUILD_DIR)/mac_randomizer.o
	cc $(CC_FLAGS) -o $@ $(BUILD_DIR)/mac_randomizer.o mac_randomizer_test.c

clean:
	-rm $(BUILD_DIR)/mac_randomizer.o
	-rm $(BUILD_DIR)/mac
	-rm $(BUILD_DIR)/mac_randomizer_test

.PHONY: $(.PHONY)
