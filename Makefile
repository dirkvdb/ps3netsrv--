CXXFLAGS = -O3 -I./utils/inc -D_FILE_OFFSET_BITS=64 -DCONSOLE_SUPPORTS_COLOR -Wall -std=c++11 -Wfatal-errors
CXX = clang++
LD = $(CXX)
LIBS=-lpthread

ifeq ($(CXX),clang++)
	CXXFLAGS := $(CXXFLAGS) -stdlib=libc++
endif

all: ps3netsrv++

ps3netsrv++: ps3netsrv.o fileoperations.o log.o
	$(LD) $(CXXFLAGS) $^ $(LIBS) -o $@

ps3netsrv.o: ps3netsrv.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

fileoperations.o: utils/src/fileoperations.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

log.o: utils/src/log.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

clean:
	rm *.o
