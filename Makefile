ifndef TARGET
$(error TARGET parameter not provided.)
endif

export TOP := $(CURDIR)
export TARGET

# Determine whether host OS is Linux or Windows
# Linux returns 'Linux', Windows doesn't have uname, OS X returns 'Darwin'.
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
export HOST = linux
else
export HOST = win32
export FILE_EXT = .exe
endif

# Set proper commandline commands for the detected OS
ifeq ($(HOST), linux)
export MKDIR 	= mkdir -p
export RM		= rm -rf
export CP		= cp -RL
#export LN		= ln -sf
else
# Assuming MSYS shell
export MKDIR 	= mkdir -p
export RM		= rm -rf
export CP		= cp -RL
#export LN		= mklink /d .
endif

include Makefile.$(TARGET)

export TARGET_OS
export TARGET_ARCH
export TOOLCHAIN_NAME

all: foundation crypto util net netssl json data

json:
	$(MAKE) -C ./JSON

foundation:
	$(MAKE) -C ./Foundation
	
crypto:
	$(MAKE) -C ./Crypto

net:
	$(MAKE) -C ./Net
	
netssl: crypto
	$(MAKE) -C ./NetSSL_OpenSSL
	
util:
	$(MAKE) -C ./Util
	
data:
	$(MAKE) -C ./Data

clean: clean-json clean-foundation clean-net clean-netssl clean-util clean-data clean-crypto

clean-foundation:
	$(MAKE) -C ./Foundation clean
	
clean-crypto:
	$(MAKE) -C ./Crypto clean
	
clean-net:
	$(MAKE) -C ./Net clean
	
clean-netssl:
	$(MAKE) -C ./NetSSL_OpenSSL clean
	
clean-util:
	$(MAKE) -C ./Util clean
	
clean-json:
	$(MAKE) -C ./JSON clean
	
clean-data:
	$(MAKE) -C ./Data clean

.PHONY: all clean foundation crypto net netssl util json data clean-net clean-foundation clean-json clean-util
