BUILDDIR = ./bin
BENCHDIR = ./benchmarks

all:
	mkdir -p $(BUILDDIR)
	gcc -Iinclude src/*.c -o bin/hoard -lm
	
clean:
	rm -r $(BUILDDIR)
so:
	gcc -I include -shared -o /usr/local/lib/lib506hoard.so -fPIC src/hoard.c