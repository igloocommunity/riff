# Makefile - CDAL

CXX=g++
INCLUDE=-I../os_wrappers -fPIC -I/usr/include/libusb-1.0
CFLAGS=-g $(INCLUDE)
CXXFLAGS=$(CFLAGS)
LIBS= -shared -lstdc++ -lusb-1.0
LD=$(CXX) $(CXXFLAGS) 

ifndef TARGET
TARGET=libcdal.so
endif

.PHONY: all
all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $^

SOURCE_FILES= \
	./AsyncCommunication.cpp \
	./CDAL.cpp \
	./CommDevice.cpp \
	./CommDeviceManager.cpp \
	./CommException.cpp \
	./LibusbDevice.cpp \
	../os_wrappers/CriticalSection.cpp \
	../os_wrappers/Event.cpp \
	../os_wrappers/Thread.cpp \

HEADER_FILES= \
	./AsyncCommunication.h \
	./CDAL.h \
	./CommDevice.h \
	./CommDeviceManager.h \
	./CommException.h \
	./LibusbDevice.h \
	./Debug.h \
	../os_wrappers/CriticalSection.h \
	../os_wrappers/Event.h \
	../os_wrappers/Thread.h \
	../os_wrappers/Utilities.h

OBJS= \
	./AsyncCommunication.o \
	./CDAL.o \
	./CommDevice.o \
	./CommDeviceManager.o \
	./CommException.o \
	./LibusbDevice.o \
	../os_wrappers/CriticalSection.o \
	../os_wrappers/Event.o \
	../os_wrappers/Thread.o \


SRCS=$(SOURCE_FILES) $(HEADER_FILES) 

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	-rm -f -v $(OBJS) $(TARGET) CDAL.dep *.orig

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > CDAL.dep

-include CDAL.dep

