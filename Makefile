######################################################################
# GNU Makefile
######################################################################
BUILD_DIR ?= ./_build

.PHONY += all
all: $(BUILD_DIR)/mac_randomizer

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/mac_randomizer: $(BUILD_DIR)
	cc -o $@ mac_randomizer.c

clean:
	rm $(BUILD_DIR)/mac_randomizer

.PHONY: $(.PHONY)
