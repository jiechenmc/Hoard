BUILDDIR = ./bin

all:
	mkdir -p $(BUILDDIR)
	gcc -Iinclude src/*.c -o bin/hoard
clean:
	rm -r $(BUILDDIR)