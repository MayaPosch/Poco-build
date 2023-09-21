include ../../Makefile.$(TARGET)

OUTPUT := libPocoDataSQLite
VERSION := 1.10

INCLUDE := $(STD_INCLUDE) -Iinclude -I$(TOP)/build/$(TARGET)/poco/include -Isrc
CFLAGS := $(STD_CFLAGS) $(INCLUDE)
CXXFLAGS := $(STD_CXXFLAGS) $(INCLUDE)
SHARED_FLAGS := -fPIC -shared -Wl,-soname,$(OUTPUT).so.$(VERSION)

OBJROOT := ../../build/$(TARGET)/poco/obj
INCLOUT := $(TOP)/build/$(TARGET)/poco
SOURCES := $(wildcard src/*.cpp)
HEADERS := $(addprefix $(INCLOUT)/,$(wildcard include/Poco/Data/SQLite/*.h))
LD_FLAGS := $(STD_LDFLAGS)
LIBS := -L$(TOP)/build/$(TARGET)/poco/lib -lPocoFoundation -lPocoData $(STD_LIBS)

# ifeq ($(TARGET), android_aarch64)
C_SOURCES := $(wildcard src/*.c)
C_OBJECTS := $(addprefix $(OBJROOT)/data/sqlite/static/,$(notdir $(C_SOURCES:.c=.o)))
C_SHARED_OBJECTS := $(addprefix $(OBJROOT)/data/sqlite/shared/,$(notdir $(C_SOURCES:.c=.o)))
# endif

OBJECTS := $(addprefix $(OBJROOT)/data/sqlite/static/,$(notdir $(SOURCES:.cpp=.o)))
SHARED_OBJECTS := $(addprefix $(OBJROOT)/data/sqlite/shared/,$(notdir $(SOURCES:.cpp=.o)))

all: makedir objfile $(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).a \
		$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).so.$(VERSION) $(HEADERS)
		
objfile:
	$(file >$(TOP)/build/$(TARGET)/poco/obj/data/sqlite/shared/s_objects.psr, $(SHARED_OBJECTS) $(C_SHARED_OBJECTS))

$(OBJROOT)/data/sqlite/static/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	
$(OBJROOT)/data/sqlite/shared/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(SHARED_FLAGS)

$(OBJROOT)/data/sqlite/static/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJROOT)/data/sqlite/shared/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(SHARED_FLAGS)
	
makedir:
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/obj/data/sqlite/static
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/obj/data/sqlite/shared
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/lib
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/include/Poco/Data/SQLite
	
$(INCLOUT)/%.h: %.h
	$(CP) $< $(INCLOUT)/$<

$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).a: $(OBJECTS) $(C_OBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).so.$(VERSION): $(SHARED_OBJECTS) $(C_SHARED_OBJECTS)
	$(CXX) -o $@ $(LD_FLAGS) $(SHARED_FLAGS) @$(TOP)/build/$(TARGET)/poco/obj/data/sqlite/shared/s_objects.psr $(LIBS)
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
clean:
	$(RM) $(OBJECTS) $(SHARED_OBJECTS) $(C_OBJECTS) $(C_SHARED_OBJECTS)
