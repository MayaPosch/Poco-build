include ../Makefile.$(TARGET)

OUTPUT := libPocoJSON
VERSION := 1.10

INCLUDE = $(STD_INCLUDE) -Iinclude -I$(TOP)/build/$(TARGET)/poco/include
CFLAGS = $(STD_CFLAGS) $(INCLUDE)
SHARED_FLAGS := -fPIC -shared -Wl,-soname,$(OUTPUT).so.$(VERSION)

OBJROOT = $(TOP)/build/$(TARGET)/poco/obj
INCLOUT = $(TOP)/build/$(TARGET)/poco

CSOURCES := $(wildcard src/*.c)
SOURCES := $(wildcard src/*.cpp)
HEADERS := $(addprefix $(INCLOUT)/,$(wildcard include/Poco/JSON/*.h))
LIBS := -L$(TOP)/build/$(TARGET)/poco/lib -lPocoFoundation $(STD_LIBS) 

COBJECTS := $(addprefix $(OBJROOT)/json/static/,$(notdir $(CSOURCES:.c=.o)))
SHARED_COBJECTS := $(addprefix $(OBJROOT)/json/shared/,$(notdir $(CSOURCES:.c=.o)))
OBJECTS := $(addprefix $(OBJROOT)/json/static/,$(notdir $(SOURCES:.cpp=.o)))
SHARED_OBJECTS := $(addprefix $(OBJROOT)/json/shared/,$(notdir $(SOURCES:.cpp=.o)))

all: makedir $(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).a \
		$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).so.$(VERSION) $(HEADERS)

$(OBJROOT)/json/static/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

$(OBJROOT)/json/static/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJROOT)/json/shared/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(SHARED_FLAGS)
	
$(OBJROOT)/json/shared/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(SHARED_FLAGS)
	
makedir:
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/obj/json/static
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/obj/json/shared
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/lib
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/include/Poco/JSON
	
$(INCLOUT)/%.h: %.h
	$(CP) $< $(INCLOUT)/$<

$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).a: $(OBJECTS) $(COBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).so.$(VERSION): $(SHARED_OBJECTS) $(SHARED_COBJECTS)
	$(CXX) -o $@ $(CFLAGS) $(SHARED_FLAGS) $(SHARED_OBJECTS) $(SHARED_COBJECTS) $(LIBS)
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
clean:
	$(RM) $(OBJECTS)
