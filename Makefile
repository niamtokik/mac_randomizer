######################################################################
# GNU Makefile
######################################################################
BUILD_DIR ?= ./_build

.PHONY += all
all: $(BUILD_DIR)/mac test

test: $(BUILD_DIR)/mac_randomizer_test
	$(BUILD_DIR)/mac_randomizer_test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/mac_randomizer.o: $(BUILD_DIR)
	cc -g -c -O -fPIC -o $@ mac_randomizer.c 

$(BUILD_DIR)/mac: $(BUILD_DIR)/mac_randomizer.o
	cc -static -o $@ $(BUILD_DIR)/mac_randomizer.o mac.c

$(BUILD_DIR)/mac_randomizer_test: $(BUILD_DIR) $(BUILD_DIR)/mac_randomizer.o
	cc -static -o $@ $(BUILD_DIR)/mac_randomizer.o mac_randomizer_test.c

clean:
	rm $(BUILD_DIR)/mac_randomizer

.PHONY: $(.PHONY)
