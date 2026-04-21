# --- Info --- # 
NAME := morgan
VERSION := 1.0

# --- Build settings --- # 
# Compilers
CC_LINUX := gcc
CC_WINDOWS_64 := x86_64-w64-mingw32-gcc

# Paths
SRC_ROOT := src
SRC_GLOBAL := $(SRC_ROOT)

SRC_WINDOWS := $(SRC_ROOT)/windows
SRC_LINUX := $(SRC_ROOT)/linux

# Flags
CFLAGS_GLOBAL := -Wall
CFLAGS_LINUX := -lX11 -lXi -lasound
CFLAGS_WINDOWS := -lwinmm

# --- Runtime variables --- # 

ifeq ($(OS),Windows_NT) # 'OS' is a default env variable on windows
	DETECTED_OS := windows
else
	UNAME := $(shell uname)

	ifeq ($(UNAME),Linux)
		DETECTED_OS := linux
		COMPATIBILITY_PREFIX := "wine "
	else
		$(error Unsupported OS: $(UNAME))
	endif
endif

# --- Targets --- # 

.PHONY: linux-vars windows-vars linux windows test-windows test-linux test build all run clean
.DEFAULT_GOAL := build

build: $(DETECTED_OS)
test: test-$(DETECTED_OS)
linux: build-linux
windows: build-windows
test-all: test-linux test-windows
all: linux windows

linux-vars:
	$(eval CC := $(CC_LINUX))
	$(eval SRC_PLATFORM := $(SRC_LINUX))
	$(eval CFLAGS_PLATFORM := $(CFLAGS_LINUX))

windows-vars:
	$(eval CC := $(CC_WINDOWS_64))
	$(eval SRC_PLATFORM := $(SRC_WINDOWS))
	$(eval CFLAGS_PLATFORM := $(CFLAGS_WINDOWS))

build-%: %-vars
	$(eval SRC_FILES := $(shell find $(SRC_GLOBAL) $(SRC_PLATFORM) -maxdepth 1 -name \*.c -o -name \*.h))
	$(CC) $(SRC_FILES) $(CFLAGS_PLATFORM) $(CFLAGS_GLOBAL) $(CFLAGS_EXTRA) -DVERSION=\"$(VERSION)\" -o $(NAME)

test-linux:
	CFLAGS_EXTRA="-DENABLE_TESTS -DTESTS_ONLY" make linux
	./$(NAME)

test-windows:
	CFLAGS_EXTRA="-DENABLE_TESTS -DTESTS_ONLY" make windows
	$(COMPATIBILITY_PREFIX)./$(NAME).exe

run: build
	./$(NAME)

clean:
	rm -f $(NAME).exe
	rm -f $(NAME)

