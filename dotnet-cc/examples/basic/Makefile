OBJS_ZKVM = main.o startup.o mod0.o addon.o mathfuncs.o wasi.o stubs.o custom_imports.o memops.o

run: basic.bin
	ziskemu -e ./basic.bin -i riscv.bmp

clean:
	rm -f *.o basic.bin mod0.c mod0.h unbundled-module0-patched.wat

.PHONY: clean

include ../../zkvm/make.inc
