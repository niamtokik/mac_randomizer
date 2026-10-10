######################################################################
# MAC Address Randomizer GNU Makefile
######################################################################
# default build dir
BUILD_DIR ?= ./_build

# default C compiler flags
CC_FLAGS ?= -std=c99 -O2 -static -Wall -Werror -Wformat=2 \
						-Wconversion -Wsign-conversion -Wimplicit-fallthrough \
						-Werror=format-security -Werror=implicit \
						-Werror=incompatible-pointer-types \
						-Werror=int-conversion -D_FORTIFY_SOURCE=3 \
						-fstrict-flex-arrays=3  -fstack-protector-strong \
						-fcf-protection=full -fzero-call-used-regs=used-gpr \
						-fno-delete-null-pointer-checks -fno-strict-overflow \
						-fno-strict-aliasing -fexceptions -fPIE

# C compiler debug flags to help debugging with gdb
CC_FLAGS_DEBUG ?= $(CC_FLAGS) -g3 -ggdb -gdwarf

# static analysis tool
CLANG_CHECK ?= /usr/local/bin/clang-check-21

# default source directory
SRC_DIR = ./src

# targets to build before compiling the final applications
OBJECT_TARGETS = mac_common \
								 mac_randomizer \
								 mac_parser \
								 mac_identifier \
								 mac_fsm \
								 mac_converter \
								 mac_test_helper

# targets to build the unit tests
TEST_TARGETS = mac_test \
							 mac_fsm_test \
							 mac_parser_test

# template to generate C objects
define object_builder
OBJECTS += $$(BUILD_DIR)/$(1).o
$$(BUILD_DIR)/$(1).o: $$(BUILD_DIR)
	cc $$(CC_FLAGS) -c -O -fPIC $$(SRC_DIR)/$(1).c -o $$(@)

OBJECTS_DEBUG += $$(BUILD_DIR)/$(1)_debug.o
$$(BUILD_DIR)/$(1)_debug.o: $$(BUILD_DIR)
	cc $$(CC_FLAGS_DEBUG) -c -O -fPIC $$(SRC_DIR)/$(1).c -o $$(@)
endef

# template to generate test files
define test_builder
TESTS += $$(BUILD_DIR)/$(1)
$$(BUILD_DIR)/$(1): $$(OBJECTS)
	cc $$(CC_FLAGS) $$(OBJECTS) $$(SRC_DIR)/$(1).c -o $$(@)

TESTS_DEBUG += $$(BUILD_DIR)/$(1)_debug
$$(BUILD_DIR)/$(1)_debug: $$(OBJECTS_DEBUG)
	cc $$(CC_FLAGS_DEBUG) $$(OBJECTS_DEBUG) $$(SRC_DIR)/$(1).c -o $$(@)
endef

######################################################################
# help target.
######################################################################
.PHONY += help
help:
	@echo "Usage: make [all|test|debug|auto|clean]"

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
# test unit mac address randomizer application.
######################################################################
$(foreach test,$(TEST_TARGETS), \
	$(eval $(call test_builder,$(test))))

######################################################################
# main mac address randomizer cli application.
######################################################################
$(BUILD_DIR)/mac: $(OBJECTS)
	cc $(CC_FLAGS) -o $@ $(OBJECTS) $(SRC_DIR)/mac.c

$(BUILD_DIR)/mac_debug: $(OBJECTS_DEBUG)
	cc $(CC_FLAGS_DEBUG) -g -o $@ $(OBJECTS_DEBUG) $(SRC_DIR)/mac.c

######################################################################
# main targets.
######################################################################
.PHONY += all
all: $(BUILD_DIR)/mac

.PHONY += auto
auto: clean all test

.PHONY += static-analysis
static-analysis:
	$(CLANG_CHECK) $(SRC_DIR)/*.c

.PHONY += test
test: $(TESTS)
	@for t in $(TESTS); do \
		echo "[TEST] $${t}"; \
		./$${t}; done

.PHONY += debug
debug: $(BUILD_DIR)/mac_debug

.PHONY += test-debug
test-debug: $(TESTS_DEBUG)
	@for t in $(TESTS_DEBUG); do \
		echo "[TEST] $${t}"; \
		./$${t}; done

.PHONY += clean-debug
clean-debug:
	-rm $(TESTS_DEBUG)

.PHONY += clean
clean: clean-debug
	-rm $(OBJECTS)
	-rm $(BUILD_DIR)/mac
	-rm $(TESTS)

.PHONY: $(.PHONY)
