OBJS_ZKVM = main.o startup.o mod0.o addon.o mathfuncs.o wasi.o stubs.o custom_imports.o memops.o

run: empty.bin
	$(ZISKEMU) -e ./empty.bin -x

clean:
	rm -f *.o empty.bin mod0.c mod0.h unbundled-module0-patched.wat

.PHONY: clean

include ../../zkvm/make.inc
