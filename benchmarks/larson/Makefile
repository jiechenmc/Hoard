include ../Makefile.inc

all: larson larson-506-hoard larson-hoard larson-jemalloc larson-mimalloc

TARGET = larson larson-506-hoard larson-hoard larson-jemalloc larson-mimalloc

larson: larson.cpp
	$(CXX) $(CXXFLAGS) larson.cpp -o larson -lpthread

larson-506-hoard: larson.cpp
	$(CXX) $(CXXFLAGS) larson.cpp -L/usr/local/lib/ -l506hoard -o larson-506hoard -lpthread

larson-hoard: larson.cpp
	$(CXX) $(CXXFLAGS) larson.cpp -L/usr/local/lib/ -lhoard -o larson-hoard -lpthread

larson-jemalloc: larson.cpp
	$(CXX) $(CXXFLAGS) larson.cpp -L/usr/local/lib/ -ljemalloc -o larson-jemalloc -lpthread

larson-mimalloc: larson.cpp
	$(CXX) $(CXXFLAGS) larson.cpp -L/usr/local/lib -lmimalloc -o larson-mimalloc -lpthread

clean:
	rm -f $(TARGET)