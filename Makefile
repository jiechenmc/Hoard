BUILDDIR = ./bin
BENCHDIR = ./benchmarks

all:
	mkdir -p $(BUILDDIR)
	gcc -Iinclude src/*.c -o bin/hoard -lm
clean:
	rm -r $(BUILDDIR)

so:
	gcc -I include -shared -o hoard.so -fPIC src/hoard.c