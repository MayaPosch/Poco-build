include ../Makefile.$(TARGET)

OUTPUT := libPocoNet
VERSION := 1.10

INCLUDE := $(STD_INCLUDE) -Iinclude -I$(TOP)/build/$(TARGET)/poco/include
CFLAGS := $(STD_CFLAGS) $(INCLUDE)
CXXFLAGS := $(STD_CXXFLAGS) $(INCLUDE)
SHARED_FLAGS := -fPIC -shared -Wl,-soname,$(OUTPUT).so.$(VERSION)

OBJROOT := $(TOP)/build/$(TARGET)/poco/obj
INCLOUT := $(TOP)/build/$(TARGET)/poco
SOURCES := $(wildcard src/*.cpp)
HEADERS := $(addprefix $(INCLOUT)/,$(wildcard include/Poco/Net/*.h))
LIBS := -L$(TOP)/build/$(TARGET)/poco/lib -lPocoFoundation $(STD_LIBS) 

OBJECTS := $(addprefix $(OBJROOT)/net/static/,$(notdir $(SOURCES:.cpp=.o)))
SHARED_OBJECTS := $(addprefix $(OBJROOT)/net/shared/,$(notdir $(SOURCES:.cpp=.o)))

all: makedir $(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).a \
		$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).so.$(VERSION) $(HEADERS)

$(OBJROOT)/net/static/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	
$(OBJROOT)/net/shared/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(SHARED_FLAGS)
	
makedir:
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/obj/net/static
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/obj/net/shared
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/lib
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/include/Poco/Net
	
$(INCLOUT)/%.h: %.h
	$(CP) $< $(INCLOUT)/$<

$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).a: $(OBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).so.$(VERSION): $(SHARED_OBJECTS)
	$(CXX) -o $@ $(CXXFLAGS) $(SHARED_FLAGS) $(SHARED_OBJECTS) $(LIBS)
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
clean:
	$(RM) $(OBJECTS) $(SHARED_OBJECTS)
