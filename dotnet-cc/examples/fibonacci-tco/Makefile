OBJS_ZKVM = main.o startup.o mod0.o addon.o mathfuncs.o wasi.o stubs.o custom_imports.o memops.o

run: fibonacci-tco.bin
	ziskemu -e ./fibonacci-tco.bin -x --max-steps 40000000000

debug: fibonacci-tco.bin
	ziskemu -e ./fibonacci-tco.bin -X -S -D --max-steps 40000000000

clean:
	rm -f *.o fibonacci-tco.bin mod0.c mod0.h unbundled-module0-patched.wat

.PHONY: clean

include ../../zkvm/make.inc
