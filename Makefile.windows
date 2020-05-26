TARGET_OS = windows
TARGET_ARCH = x86

export CC = gcc
export CXX = g++
export CPP = cpp
export AR = ar
export LD = g++
export STRIP = strip
export OBJCOPY = objcopy

PLATFORM_FLAGS = -D_WIN32 -DPOCO_NO_FPENVIRONMENT -DPOCO_THREAD_STACK_SIZE -DWINVER=0x501
STD_FLAGS = $(PLATFORM_FLAGS) -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -fdata-sections -DPOCO_OS_FAMILY_WINDOWS -U__STRICT_ANSI__ -DDLLAPI=
STD_CFLAGS = $(STD_FLAGS)
STD_CXXFLAGS = -std=c++11 $(STD_FLAGS)
STD_LDFLAGS = -L.
STD_INCLUDE = -I.
STD_LIBDIRS = $(STD_LDFLAGS)
STD_LIBS = -lssl -lcrypto -lws2_32 -lIphlpapi
