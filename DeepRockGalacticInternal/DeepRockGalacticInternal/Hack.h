#pragma once

#include <Windows.h>
#include <vector>
#include <TlHelp32.h>

#include "Player.h"

constexpr auto INST_NOP = 0x90;
constexpr auto INST_CALL = 0x08EB0000000215FF; // Note: This is flipped around to how it appears in memory because x86 processors are little endian
constexpr auto INST_CALL_SIZE = 16; // size of call + size of function address ?


namespace Hack{
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void Nop(BYTE* dst, unsigned int size);

	bool Detour(void* toHook, const void* myFunc, const uint64_t length);

	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
};

