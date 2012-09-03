# Makefile - riff

PREFIX := /usr
CXX=g++
CFLAGS=-I../os_wrappers -I../CDAL -D_FILE_OFFSET_BITS=64 -DRIFF_CONFIG_PREFIX=\"$(PREFIX)\" -I/usr/include/loadercomm
CXXFLAGS=$(CFLAGS)
LD=$(CXX) $(CXXFLAGS)
LDFLAGS=-L../CDAL
LIBS+=-llcdriver -lcdal -lstdc++ -lc -lpthread

ifndef TARGET
TARGET=riff
endif

.PHONY: all
all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

SOURCE_FILES= \
	./Config.cpp \
	./DumpArea.cpp \
	./DUT.cpp \
	./DUTManager.cpp \
	./EraseArea.cpp \
	./InitializeDut.cpp \
	./Logger.cpp \
	./main.cpp \
	./ProcessRawImage.cpp \
	./SequenceFactory.cpp \
	./Shutdown.cpp \
	../os_wrappers/CriticalSection.cpp \
	../os_wrappers/Event.cpp \
	../os_wrappers/Thread.cpp

HEADER_FILES= \
	./Command.h \
	./Config.h \
	./constants.h \
	./DumpArea.h \
	./DUT.h \
	./DUTManager.h \
	./EraseArea.h \
	./InitializeDut.h \
	./Logger.h \
	./main.h \
	./ProcessRawImage.h \
	./SequenceFactory.h \
	./Shutdown.h \
	../os_wrappers/CriticalSection.h \
	../os_wrappers/Event.h \
	../os_wrappers/Thread.h \
	../os_wrappers/Utilities.h \

OBJS= \
	./Config.o \
	./DumpArea.o \
	./DUT.o \
	./DutManager.o \
	./EraseArea.o \
	./InitializeDut.o \
	./Logger.o \
	./main.o \
	./ProcessRawImage.o \
	./SequenceFactory.o \
	./Shutdown.o \
	../os_wrappers/CriticalSection.o \
	../os_wrappers/Event.o \
	../os_wrappers/Thread.o \

SRCS=$(SOURCE_FILES) $(HEADER_FILES)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	-rm -f -v $(OBJS) $(TARGET) riff.dep *.orig

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > riff.dep

-include riff.dep

