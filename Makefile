
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
CFLAGS_LINUX := -lX11 -lXi -lasound
CFLAGS_WINDOWS := -lwinmm

# --- Current config --- #
# Uncomment one to compile for the platform. 
# TODO: Make it work by for example `make linux` and `make windows`
# - Windows -
CC := $(CC_WINDOWS_64)
SRC_PLATFORM := $(SRC_WINDOWS)
CFLAGS_PLATFORM := $(CFLAGS_WINDOWS)

# - Linux -
#CC := $(CC_LINUX)
#SRC_PLATFORM := $(SRC_LINUX)
#CFLAGS_PLATFORM := $(CFLAGS_LINUX)

# --- Processed --- #
SRC_FILES := $(shell find $(SRC_GLOBAL) $(SRC_PLATFORM) -maxdepth 1 -name \*.c -o -name \*.h)

.PHONY: build clean

build:
	$(CC) $(SRC_FILES) $(CFLAGS_PLATFORM) -o morgan

clean:
	rm -f morgan.exe
	rm -f morgan
