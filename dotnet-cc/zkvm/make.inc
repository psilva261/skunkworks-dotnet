PREFIX ?= /opt/riscv-newlib/bin/riscv64-unknown-elf-
W2C2_ROOT=/opt/w2c2/w2c2
ZISKEMU ?= ziskemu

CC = $(PREFIX)gcc
AS = $(PREFIX)as
LD = $(PREFIX)ld

CFLAGS_EXTRA ?= -O0
CFLAGS = $(CFLAGS_EXTRA) -Wall -march=rv64ima -mabi=lp64 -mcmodel=medany -specs=nosys.specs -include custom_shims.h -I../../common -I../../zkvm -I./include -I. -I$(W2C2_ROOT) -D__bool_true_false_are_defined -nostdlib
LDFLAGS_ZKVM = -march=rv64ima -mabi=lp64 -T ../../zkvm/riscv64ima_zisk_zkvm_elf_linker_script.ld -nostartfiles -nostdlib -lgcc

%.bin: $(OBJS_ZKVM) ../../zkvm/riscv64ima_zisk_zkvm_elf_linker_script.ld
	$(CC) $(CFLAGS) $(LDFLAGS_EXTRA) -o $@ $(OBJS_ZKVM) $(LDFLAGS_ZKVM)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

mod0.c: Example.cs
	../../docker_compile.sh

mod0.o: mod0.c
	$(CC) $(CFLAGS) -Wno-all -c mod0.c -o mod0.o

startup.o: ../../zkvm/startup.S
	$(CC) $(CFLAGS) -c $< -o $@

memops.o: ../../zkvm/memops.S
	$(CC) $(CFLAGS) -c $< -o $@

addon.o: ../../zkvm/addon.c
	$(CC) $(CFLAGS) -c $< -o $@

custom_imports.o: ../../zkvm/custom_imports.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o: ../../zkvm/main.c mod0.c
	$(CC) $(CFLAGS) -c $< -o $@

mathfuncs.o: ../../zkvm/mathfuncs.c
	$(CC) $(CFLAGS) -c $< -o $@

syscalls.o: ../../zkvm/syscalls.c
	$(CC) $(CFLAGS) -c $< -o $@

stubs.o: ../../common/stubs.c
	$(CC) $(CFLAGS) -c $< -o $@

wasi.o: ../../common/wasi.c
	$(CC) $(CFLAGS) -c $< -o $@
