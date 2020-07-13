GTEST = /usr/src/gtest
FIRMWARE_SRC = ../..

INCDIRS = . \
	$(GTEST) \
	$(FIRMWARE_SRC) \
	$(FIRMWARE_SRC)/inc

CC = g++
CFLAGS = -g -std=c++11 -Wall -Wextra -lpthread -pthread

CFLAGS += $(patsubst %,-I%,$(INCDIRS))

.PHONY: all
all: test

.PHONY: clean
clean:
	rm -f *.o test

%.o : $(GTEST)/src/%.cc
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

CCIDHID_usb_desc.o: $(FIRMWARE_SRC)/ccid/CCIDHID_USB/CCIDHID_usb_desc.c
	$(CC) $(CFLAGS) -c $< -o $@

USB_DESCRIPTORS_TEST = usb_descriptors.o CCIDHID_usb_desc.o

test: gtest-all.o gtest_main.o $(USB_DESCRIPTORS_TEST)
	$(CC) $(CFLAGS) $^ -o $@