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

OBJECT_TARGETS = mac_common mac_randomizer mac_parser mac_identifier mac_fsm

# template to generate C objects.
define object_builder
OBJECTS += $$(BUILD_DIR)/$(1).o
$$(BUILD_DIR)/$(1).o: $$(BUILD_DIR)
	cc $$(CC_FLAGS) -c -O -fPIC -o $$(@) $(1).c 
endef

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
$(foreach object,$(OBJECT_TARGETS), \
	$(eval $(call object_builder,$(object))))

######################################################################
# main mac address randomizer cli application.
######################################################################
$(BUILD_DIR)/mac: $(OBJECTS)
	cc $(CC_FLAGS) -fPIE -pie -o $@ $(OBJECTS) mac.c

######################################################################
# test unit mac address randomizer application.
######################################################################
$(BUILD_DIR)/mac_test: $(OBJECTS)
	cc $(CC_FLAGS) -o $@ $(OBJECTS) mac_test.c

######################################################################
# main targets.
######################################################################
.PHONY += all
all: $(BUILD_DIR)/mac

.PHONY += auto
auto: clean all test

.PHONY += test
test: $(BUILD_DIR)/mac_test
	$(BUILD_DIR)/mac_test

.PHONY += clean
clean:
	-rm $(OBJECTS)
	-rm $(BUILD_DIR)/mac
	-rm $(BUILD_DIR)/mac_test

.PHONY: $(.PHONY)
