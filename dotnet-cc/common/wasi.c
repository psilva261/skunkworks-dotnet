#include "w2c2_base.h"
#include "mod0.h"

#include <stdio.h>

wasmMemory*
wasiMemory(
	void* instance
);

void wasisnapshotpreview1Instantiate(void* instance, void* resolve(const char* module, const char* name)) {
	printf("wasisnapshotpreview1Instantiate\n");
}

void wasisnapshotpreview1FreeInstance(void* instance) {
	printf("wasisnapshotpreview1FreeInstance\n");
}

void wasisnapshotpreview1_proc_exit(void* i, U32 l0) {
	printf("wasisnapshotpreview1_proc_exit\n");
}

U32 wasisnapshotpreview1_fd_prestat_get(void* i, U32 l0, U32 l1) {
	printf("wasisnapshotpreview1_fd_prestat_get\n");
	return 0;
}

U32 wasisnapshotpreview1_fd_prestat_dir_name(void* i, U32 l0, U32 l1, U32 l2) {
	printf("wasisnapshotpreview1_fd_prestat_dir_name\n");
	return 0;
}

U32 wasisnapshotpreview1_fd_fdstat_get(void* i, U32 l0, U32 l1) {
	printf("wasisnapshotpreview1_fd_fdstat_get\n");
	return 0;
}

U32 wasisnapshotpreview1_fd_filestat_get(void*, U32, U32) {
	printf("wasisnapshotpreview1_fd_filestat_get\n");
	return 0;
}

U32 wasisnapshotpreview1_fd_pread(void*, U32, U32, U32, U64, U32) {
	printf("wasisnapshotpreview1_fd_pread\n");
	return 0;
}

U32 wasisnapshotpreview1_fd_read(void*, U32, U32, U32, U32) {
	printf("wasisnapshotpreview1_fd_read\n");
	return 0;
}

U32 wasisnapshotpreview1_path_filestat_get(void* i, U32 l0, U32 l1, U32 l2, U32 l3, U32 l4) {
	printf("wasisnapshotpreview1_path_filestat_get\n");
	return 0;
}

U32 wasisnapshotpreview1_path_open(void*, U32, U32, U32, U32, U32, U64, U64, U32, U32) {
	printf("wasisnapshotpreview1_path_open\n");
	return 0;
}

U32 wasisnapshotpreview1_path_readlink(void*, U32, U32, U32, U32, U32, U32) {
	printf("wasisnapshotpreview1_path_readlink\n");
	return 0;
}

U32 wasisnapshotpreview1_environ_sizes_get(void* i, U32 l0, U32 l1) {
	printf("wasisnapshotpreview1_environ_sizes_get\n");
	return 0;
}

U32 wasisnapshotpreview1_poll_oneoff(void* i, U32 l0, U32 l1, U32 l2, U32 l3) {
	printf("wasisnapshotpreview1_poll_oneoff\n");
	return 0;
}

U32 wasisnapshotpreview1_adapter_close_badfd(void* i, U32 l0) {
	printf("wasisnapshotpreview1_adapter_close_badfd\n");
	return 0;
}

U32 wasisnapshotpreview1_fd_close(void* i, U32 l0) {
	printf("wasisnapshotpreview1_fd_close\n");
	return 0;
}

U32 wasisnapshotpreview1_environ_get(void* i, U32 l0, U32 l1) {
	printf("wasisnapshotpreview1_environ_get\n");
	return 0;
}

U32 wasisnapshotpreview1_clock_time_get(void* i, U32 l0, U64 l1, U32 l2) {
	printf("wasisnapshotpreview1_clock_time_get\n");
	return 0;
}

U32 wasisnapshotpreview1_fd_advise(void*, U32, U64, U64, U32) {
	printf("wasisnapshotpreview1_fd_advise\n");
	return 0;
}

U32 wasisnapshotpreview1_fd_readdir(void*, U32, U32, U32, U64, U32) {
	printf("wasisnapshotpreview1_fd_seek\n");
	return 0;
}

U32 wasisnapshotpreview1_fd_seek(void* i, U32 l0, U64 l1, U32 l2, U32 l3) {
	printf("wasisnapshotpreview1_fd_seek\n");
	return 0;
}

U32 wasisnapshotpreview1_sched_yield(void* i) {
	printf("wasisnapshotpreview1_sched_yield\n");
	return 0;
}

struct iovec {
	void* iov_base;
	size_t iov_len;
};

#define WASI_ERRNO_NOMEM 48

// Size of a single ciovec item
static const size_t ciovecSize = 8;

// Number of iovecs to be pre-allocated. Choose a number high enough to
// let programs work that use file and socket I/O outside of printing to the
// console. But small enough to raise an error since this stub implementation
// would likely have to be extended in that case. (On Linux IOV_MAX is 1024)
//
// Pre-allocation is done to allow Console.WriteLine to work even when malloc
// fails.
#define IOVECS_SIZE 10

/* use part of wasi.c from w2c2 here but avoid full implementation */
U32 wasisnapshotpreview1_fd_write(void* i, U32 wasiFD, U32 ciovecsPointer, U32 ciovecsCount, U32 resultPointer) {
	wasmMemory* memory = wasiMemory(i);
	struct iovec iovecs[IOVECS_SIZE];
	I64 total = 0;
	/* printf("wasisnapshotpreview1_fd_write\n"); */

	if (ciovecsCount > IOVECS_SIZE) {
		printf("fd_write: unexpected iovecs\n");
		return WASI_ERRNO_NOMEM;
	}

	/* Convert WASI ciovecs to native iovecs */
	{
		U32 ciovecIndex = 0;
		for (; ciovecIndex < ciovecsCount && ciovecIndex < IOVECS_SIZE; ciovecIndex++) {
			U64 ciovecPointer = ciovecsPointer + ciovecIndex * ciovecSize;
			U32 bufferPointer = i32_load(memory, ciovecPointer);
			U32 length = i32_load(memory, ciovecPointer + 4);

			/* printf("length = %d\n", length); */

			iovecs[ciovecIndex].iov_base = memory->data + bufferPointer;
			iovecs[ciovecIndex].iov_len = length;
			total += length;

			for (int i = 0; i < iovecs[ciovecIndex].iov_len; i++) {
				putchar(((char*)iovecs[ciovecIndex].iov_base)[i]);
			}
		}
	}

	/* Store the amount of written bytes at the result pointer */
	i32_store(memory, resultPointer, total);
	return 0; // success
}

U32 wasisnapshotpreview1_path_unlink_file(void* i, U32 l0, U32 l1, U32 l2) {
	printf("wasisnapshotpreview1_path_unlink_file\n");
	return 0;
}


