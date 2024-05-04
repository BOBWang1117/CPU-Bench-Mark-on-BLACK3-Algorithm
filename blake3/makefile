CC := gcc
CFLAGS := -O3 -Wall


ASM_TARGETS :=
ifeq ($(OS),Windows_NT)
	ASM_TARGETS += blake3_sse2_x86-64_windows_gnu.S
	ASM_TARGETS += blake3_sse41_x86-64_windows_gnu.S
	ASM_TARGETS += blake3_avx2_x86-64_windows_gnu.S
	ASM_TARGETS += blake3_avx512_x86-64_windows_gnu.S
else
	ASM_TARGETS += blake3_sse2_x86-64_unix.S
	ASM_TARGETS += blake3_sse41_x86-64_unix.S
	ASM_TARGETS += blake3_avx2_x86-64_unix.S
	ASM_TARGETS += blake3_avx512_x86-64_unix.S
endif


BLAKE3_FILES :=
BLAKE3_FILES += blake3.c
BLAKE3_FILES += blake3_dispatch.c
BLAKE3_FILES += blake3_portable.c
BLAKE3_FILES += blake3.h
BLAKE3_FILES += blake3_impl.h


GEN_C_FILES :=
GEN_C_FILES += example.c


all:
	$(CC) $(CFLAGS) -o example $(GEN_C_FILES) $(BLAKE3_FILES) $(ASM_TARGETS)