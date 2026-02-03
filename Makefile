
# --- Sources --- # 
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

ifeq ($(OS),Windows_NT) # 'OS' is a default env variable on windows
	DETECTED_OS := windows
else
	UNAME := $(shell uname)

	ifeq ($(UNAME),Linux)
		DETECTED_OS := linux
	else
		$(error Unsupported OS: $(UNAME))
	endif
endif

.PHONY: linux-vars windows-vars linux windows build all run clean
.DEFAULT_GOAL := build

build: $(DETECTED_OS)
linux: build-linux
windows: build-windows
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
	$(CC) $(SRC_FILES) $(CFLAGS_PLATFORM) -o morgan

run: build
	./morgan

clean:
	rm -f morgan.exe
	rm -f morgan
