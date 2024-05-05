include ../Makefile.inc

all: larson larson-506-hoard larson-hoard larson-jemalloc

TARGET = larson larson-hoard

larson: larson.cpp
	$(CXX) $(CXXFLAGS) larson.cpp -o larson -lpthread

larson-506-hoard: larson.cpp
	$(CXX) $(CXXFLAGS) larson.cpp -L /usr/local/lib/506-hoard.so -o larson-506-hoard -lpthread

larson-hoard: larson.cpp
	$(CXX) $(CXXFLAGS) larson.cpp -L /usr/local/lib/libhoard.so -o larson-hoard -lpthread

larson-jemalloc: larson.cpp
	$(CXX) $(CXXFLAGS) larson.cpp -L /usr/local/lib/libjemalloc.so -o larson-jemalloc -lpthread

larson-mimalloc: larson.cpp
	$(CXX) $(CXXFLAGS) larson.cpp -L /usr/local/lib/libmimalloc.so -o larson-mimalloc -lpthread

clean:
	rm -f $(TARGET)