include ../Makefile.$(TARGET)

OUTPUT := libPocoNet
VERSION := 1.10

INCLUDE := $(STD_INCLUDE) -Iinclude -I$(TOP)/build/$(TARGET)/poco/include
CFLAGS := $(STD_CFLAGS) $(INCLUDE)
CXXFLAGS := $(STD_CXXFLAGS) $(INCLUDE)
SHARED_FLAGS := -fPIC -shared -Wl,-soname,$(OUTPUT).so.$(VERSION)

OBJROOT := ../build/$(TARGET)/poco/obj
INCLOUT := $(TOP)/build/$(TARGET)/poco
SOURCES := $(wildcard src/*.cpp)
HEADERS := $(addprefix $(INCLOUT)/,$(wildcard include/Poco/Net/*.h))
LD_FLAGS := $(STD_LDFLAGS)
LIBS := -L$(TOP)/build/$(TARGET)/poco/lib -lPocoFoundation $(STD_LIBS) 

ifeq ($(TARGET), android_aarch64)
#C_SOURCES := $(wildcard src/android/*.c)
C_OBJECTS := $(addprefix $(OBJROOT)/net/static/android/,$(notdir $(C_SOURCES:.c=.o)))
C_SHARED_OBJECTS := $(addprefix $(OBJROOT)/net/shared/android/,$(notdir $(C_SOURCES:.c=.o)))
endif

OBJECTS := $(addprefix $(OBJROOT)/net/static/,$(notdir $(SOURCES:.cpp=.o)))
SHARED_OBJECTS := $(addprefix $(OBJROOT)/net/shared/,$(notdir $(SOURCES:.cpp=.o)))

all: makedir objfile $(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).a \
		$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).so.$(VERSION) $(HEADERS)
		
objfile:
	$(file >$(TOP)/build/$(TARGET)/poco/obj/net/shared/s_objects.psr, $(SHARED_OBJECTS))

$(OBJROOT)/net/static/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	
$(OBJROOT)/net/shared/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(SHARED_FLAGS)

$(OBJROOT)/net/static/android/%.o: src/android/%.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJROOT)/net/shared/android/%.o: src/android/%.c
	$(CCC) -c -o $@ $< $(CFLAGS) $(SHARED_FLAGS)
	
makedir:
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/obj/net/static/android
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/obj/net/shared/android
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/lib
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/include/Poco/Net
	
$(INCLOUT)/%.h: %.h
	$(CP) $< $(INCLOUT)/$<

$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).a: $(OBJECTS) $(C_OBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).so.$(VERSION): $(SHARED_OBJECTS) $(C_SHARED_OBJECTS)
	$(CXX) -o $@ $(LD_FLAGS) $(SHARED_FLAGS) @$(TOP)/build/$(TARGET)/poco/obj/net/shared/s_objects.psr $(LIBS)
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
clean:
	$(RM) $(OBJECTS) $(SHARED_OBJECTS) $(C_OBJECTS) $(C_SHARED_OBJECTS)
