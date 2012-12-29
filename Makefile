CXXFLAGS = -O3 -I./utils/inc -D_FILE_OFFSET_BITS=64 -Wall -std=c++11 -stdlib=libc++ -stdlib=libc++ -Wfatal-errors

CXX = clang++
LD = clang++
LIBS=-lpthread

all: ps3netsrv++

ps3netsrv++: ps3netsrv.o utils.o
	$(LD) $(CXXFLAGS) $^ $(LIBS) -o $@

ps3netsrv.o: ps3netsrv.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

utils.o: utils/src/fileoperations.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

clean:
	rm *.o