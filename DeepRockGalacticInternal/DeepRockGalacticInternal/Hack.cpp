#include "Hack.h"

namespace Hack {
	// Patch memory with bytes.
	void Patch(BYTE* dst, BYTE* src, unsigned int size)
	{

		DWORD oldprotect;

		// Change proctection of memory and store old protections in oldprotect.
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

		// Write bytes to memory.
		memcpy(dst, src, size);

		// Restore old protections with oldprotect.
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	}

	// Same as PatchEx, except the just writes  NOPs.
	void Nop(BYTE* dst, unsigned int size)
	{

		DWORD oldprotect;

		// Change proctection of memory and store old protections in oldprotect.
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

		// Write bytes to memory.
		memset(dst, 0x90, size);

		// Restore old protections with oldprotect.
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	}

	bool Detour(void* toHook, const void* myFunc, const uint64_t length)
	{
		if (toHook && myFunc)
		{
			// Check min size
			if (length < INST_CALL_SIZE)
			{

				return false;
			}

			// Change permissions
			DWORD curProtection;
			VirtualProtect(toHook, length, PAGE_EXECUTE_READWRITE, &curProtection);

			// NOP instructions so we can replace with jmp or call?
			if (length > INST_CALL_SIZE)
				memset(toHook, INST_NOP, length);

			*(uint64_t*)toHook = INST_CALL;
			*(uint64_t*)((uint64_t)toHook + 8) = (uint64_t)myFunc;

			DWORD temp;
			VirtualProtect(toHook, length, curProtection, &temp);

			return true;
		}
		else
		{
			return false;
		}
	}

	// Find Dynamic Memory Allocation Address (multi-level pointer).
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
	{

		uintptr_t addr = ptr;

		/* Read memory at address (addr) and store value in addr.
		*  Move offset (offsets[i]).
		*  Repeat for all offsets.
		*/

		for (unsigned int i = 0; i < offsets.size(); ++i) {
			addr = *(uintptr_t*)addr;
			addr += offsets[i];
		}

		return addr;
	}
}