#include <stdlib.h>
#include <stdio.h>

#include "zkvm.h"

#include "w2c2_base.h"
#include "wasi.h"

#include "mod0.h"

#define uint32_t unsigned int
#define size_t unsigned int

wasmMemory*
wasiMemory(
	void* instance
) {
	return unbundledmodule0_memory((unbundledmodule0Instance*)instance);
}

int outputCount = 0;

// https://github.com/eth-act/skunkworks-tama/blob/main/tamaboards/zkvm/board.go
void printk(uint32_t val) {
	// TODO: This is a stub. Just write to the output address
	// Write directly to OUTPUT_ADDR
	// Format: [count:u32][data:bytes]
	// First update the count at OUTPUT_ADDR
	outputCount+=1;
	uint32_t *ptr_count = (uint32_t *)OUTPUT_ADDR;
	*ptr_count = outputCount;

	// Write the byte at OUTPUT_ADDR + 4 + (outputCount-1)
	uint32_t *ptr_val = (uint32_t *)(OUTPUT_ADDR+4+4*(outputCount-1));
	*ptr_val = val;
}

U64 len_input_buf() {
	uint32_t *ptr_val = (uint32_t *)(INPUT_ADDR+4*2);
	return *ptr_val;
}

char read_value(U64* i, U64 a, U64 b) {
	char *ptr_val = (char *)(INPUT_ADDR+4*4+b);
	return *ptr_val;
}

// https://github.com/eth-act/skunkworks-tama/blob/main/tamaboards/zkvm/shutdown.s
void shutdown() {
	__asm__("li a7, 93");
	__asm__("ecall");
}

extern char** environ;

int main(void) {
	int test_argc = 0;
	char **test_argv = NULL;

	// Initialize WASI
	if (!wasiInit(test_argc, test_argv, environ)) {
		puts("failed to init WASI\n");
		return 0;
	}

	unbundledmodule0Instance instance0;

	unbundledmodule0Instantiate(&instance0, NULL);

	unbundledmodule0_Example_Run(&instance0);

	unbundledmodule0FreeInstance(&instance0);

	shutdown();
}
