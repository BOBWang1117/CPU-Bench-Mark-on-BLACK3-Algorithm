CC := gcc
CFLAGS := -O3 -Wall
LDFLAGS := -lpthread -lstdc++
BLAKE3_PATH := ./blake3


ASM_TARGETS :=
ASM_TARGETS += $(BLAKE3_PATH)/blake3_sse2_x86-64_windows_gnu.S
ASM_TARGETS += $(BLAKE3_PATH)/blake3_sse41_x86-64_windows_gnu.S
ASM_TARGETS += $(BLAKE3_PATH)/blake3_avx2_x86-64_windows_gnu.S
ASM_TARGETS += $(BLAKE3_PATH)/blake3_avx512_x86-64_windows_gnu.S


BLAKE3_FILES :=
BLAKE3_FILES += $(BLAKE3_PATH)/blake3.c
BLAKE3_FILES += $(BLAKE3_PATH)/blake3_dispatch.c
BLAKE3_FILES += $(BLAKE3_PATH)/blake3_portable.c
BLAKE3_FILES += $(BLAKE3_PATH)/blake3.h
BLAKE3_FILES += $(BLAKE3_PATH)/blake3_impl.h


GEN_CPP_FILES :=
GEN_CPP_FILES += hashgen.cpp
GEN_CPP_FILES += my_command.cpp
GEN_CPP_FILES += my_hash_generation.cpp
GEN_CPP_FILES += my_hash_sort.cpp


GEN_HPP_FILES :=
GEN_HPP_FILES += my_command.hpp
GEN_HPP_FILES += my_hash_generation.hpp
GEN_HPP_FILES += my_hash_sort.hpp

VERIFY_CPP_FILES :=
VERIFY_CPP_FILES += hashverify.cpp
VERIFY_CPP_FILES += my_verify_command.cpp
VERIFY_CPP_FILES += my_hash_verify.cpp

VERIFY_HPP_FILES := 
VERIFY_HPP_FILES += my_verify_command.hpp
VERIFY_HPP_FILES += my_hash_verify.hpp

all: hashgen hashverify

hashgen: $(GEN_CPP_FILES) $(GEN_HPP_FILES)
	$(CC) $(CFLAGS) -o hashgen $(GEN_CPP_FILES) $(GEN_HPP_FILES) $(BLAKE3_FILES) $(ASM_TARGETS) $(LDFLAGS)

hashverify: $(VERIFY_CPP_FILES) $(VERIFY_HPP_FILES)
	$(CC) $(CFLAGS) -o hashverify $(VERIFY_CPP_FILES) $(VERIFY_HPP_FILES) $(LDFLAGS)

clean:
	rm -rf *.o