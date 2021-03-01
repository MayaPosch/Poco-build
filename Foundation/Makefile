include ../Makefile.$(TARGET)

OUTPUT = libPocoFoundation
VERSION := 1.10

INCLUDE = $(STD_INCLUDE) -Iinclude -I$(TOP)/build/$(TARGET)/poco/include
CFLAGS = $(STD_CFLAGS) $(INCLUDE)
CXXFLAGS := $(STD_CXXFLAGS) $(INCLUDE)
SHARED_FLAGS := -fPIC -shared -Wl,-soname,$(OUTPUT).so.$(VERSION)

OBJROOT := ../build/$(TARGET)/poco/obj
INCLOUT := $(TOP)/build/$(TARGET)/poco
#SOURCES := $(wildcard src/*.cpp)
HEADERS := $(addprefix $(INCLOUT)/,$(wildcard include/Poco/*.h)) $(addprefix $(INCLOUT)/,$(wildcard include/Poco/Dynamic/*.h))
LD_FLAGS := $(STD_LDFLAGS)
LIBS := $(STD_LIBS) -lz

# Filter out files (OpenVMS-specific)
#SOURCES := $(filter-out src/OpcomChannel.cpp, $(SOURCES))
#HEADERS := $(filter-out include/Poco/OpcomChannel.h, $(SOURCES))

CSOURCES := $(wildcard src/*.c)
SOURCES := AbstractObserver.cpp \
ActiveDispatcher.cpp \
ArchiveStrategy.cpp \
Ascii.cpp \
ASCIIEncoding.cpp \
AsyncChannel.cpp \
AtomicCounter.cpp \
Base32Decoder.cpp \
Base32Encoder.cpp \
Base64Decoder.cpp \
Base64Encoder.cpp \
BinaryReader.cpp \
BinaryWriter.cpp \
Bugcheck.cpp \
ByteOrder.cpp \
Channel.cpp \
Checksum.cpp \
Clock.cpp \
Condition.cpp \
Configurable.cpp \
ConsoleChannel.cpp \
CountingStream.cpp \
DateTime.cpp \
DateTimeFormat.cpp \
DateTimeFormatter.cpp \
DateTimeParser.cpp \
Debugger.cpp \
DeflatingStream.cpp \
DigestEngine.cpp \
DigestStream.cpp \
DirectoryIterator.cpp \
DirectoryIteratorStrategy.cpp \
DirectoryWatcher.cpp \
Environment.cpp \
Error.cpp \
ErrorHandler.cpp \
Event.cpp \
EventArgs.cpp \
EventChannel.cpp \
Exception.cpp \
FIFOBufferStream.cpp \
File.cpp \
FileChannel.cpp \
FileStream.cpp \
FileStreamFactory.cpp \
Format.cpp \
Formatter.cpp \
FormattingChannel.cpp \
FPEnvironment.cpp \
Glob.cpp \
Hash.cpp \
HashStatistic.cpp \
HexBinaryDecoder.cpp \
HexBinaryEncoder.cpp \
InflatingStream.cpp \
JSONString.cpp \
Latin1Encoding.cpp \
Latin2Encoding.cpp \
Latin9Encoding.cpp \
LineEndingConverter.cpp \
LocalDateTime.cpp \
LogFile.cpp \
Logger.cpp \
LoggingFactory.cpp \
LoggingRegistry.cpp \
LogStream.cpp \
Manifest.cpp \
MD4Engine.cpp \
MD5Engine.cpp \
MemoryPool.cpp \
MemoryStream.cpp \
Message.cpp \
Mutex.cpp \
NamedEvent.cpp \
NamedMutex.cpp \
NestedDiagnosticContext.cpp \
Notification.cpp \
NotificationCenter.cpp \
NotificationQueue.cpp \
NullChannel.cpp \
NullStream.cpp \
NumberFormatter.cpp \
NumberParser.cpp \
NumericString.cpp \
Path.cpp \
PatternFormatter.cpp \
Pipe.cpp \
PipeImpl.cpp \
PipeStream.cpp \
PriorityNotificationQueue.cpp \
Process.cpp \
PurgeStrategy.cpp \
Random.cpp \
RandomStream.cpp \
RefCountedObject.cpp \
RegularExpression.cpp \
RotateStrategy.cpp \
Runnable.cpp \
RWLock.cpp \
Semaphore.cpp \
SHA1Engine.cpp \
SharedLibrary.cpp \
SharedMemory.cpp \
SignalHandler.cpp \
SimpleFileChannel.cpp \
SortedDirectoryIterator.cpp \
SplitterChannel.cpp \
Stopwatch.cpp \
StreamChannel.cpp \
StreamConverter.cpp \
StreamCopier.cpp \
StreamTokenizer.cpp \
String.cpp \
StringTokenizer.cpp \
SynchronizedObject.cpp \
Task.cpp \
TaskManager.cpp \
TaskNotification.cpp \
TeeStream.cpp \
TemporaryFile.cpp \
TextBufferIterator.cpp \
TextConverter.cpp \
TextEncoding.cpp \
TextIterator.cpp \
Thread.cpp \
ThreadLocal.cpp \
ThreadPool.cpp \
ThreadTarget.cpp \
TimedNotificationQueue.cpp \
Timer.cpp \
Timespan.cpp \
Timestamp.cpp \
Timezone.cpp \
Token.cpp \
Unicode.cpp \
UnicodeConverter.cpp \
URI.cpp \
URIStreamFactory.cpp \
URIStreamOpener.cpp \
UTF16Encoding.cpp \
UTF32Encoding.cpp \
UTF8Encoding.cpp \
UTF8String.cpp \
UUID.cpp \
UUIDGenerator.cpp \
Var.cpp \
VarHolder.cpp \
VarIterator.cpp \
Void.cpp \
Windows1250Encoding.cpp \
Windows1251Encoding.cpp \
Windows1252Encoding.cpp

ifneq ($(TARGET_OS), windows)
SOURCES += SyslogChannel.cpp
else
SOURCES += WindowsConsoleChannel.cpp \
EventLogChannel.cpp
endif

COBJECTS := $(addprefix $(OBJROOT)/foundation/static/,$(notdir $(CSOURCES:.c=.o)))
SHARED_COBJECTS := $(addprefix $(OBJROOT)/foundation/shared/,$(notdir $(CSOURCES:.c=.o)))
OBJECTS := $(addprefix $(OBJROOT)/foundation/static/,$(notdir $(SOURCES:.cpp=.o)))
SHARED_OBJECTS := $(addprefix $(OBJROOT)/foundation/shared/,$(notdir $(SOURCES:.cpp=.o)))

#$(info Headers are [${HEADERS}])

all: makedir objfile $(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).a \
		$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).so.$(VERSION) $(HEADERS)
		
objfile:
	$(file >$(TOP)/build/$(TARGET)/poco/obj/foundation/shared/s_objects.psr, $(SHARED_OBJECTS) $(SHARED_COBJECTS))

$(OBJROOT)/foundation/static/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJROOT)/foundation/static/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJROOT)/foundation/shared/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(SHARED_FLAGS)
	
$(OBJROOT)/foundation/shared/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(SHARED_FLAGS)
	
makedir:
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/obj/foundation/static
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/obj/foundation/shared
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/lib
	$(MKDIR) $(TOP)/build/$(TARGET)/poco/include/Poco/Dynamic
	
$(INCLOUT)/%.h: %.h
	$(CP) $< $(INCLOUT)/$<

$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).a: $(OBJECTS) $(COBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
$(TOP)/build/$(TARGET)/poco/lib/$(OUTPUT).so.$(VERSION): $(SHARED_OBJECTS) $(SHARED_COBJECTS)
	$(CXX) -o $@ $(SHARED_FLAGS) $(LD_FLAGS) @$(TOP)/build/$(TARGET)/poco/obj/foundation/shared/s_objects.psr $(LIBS)
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
clean:
	$(RM) $(OBJECTS)
