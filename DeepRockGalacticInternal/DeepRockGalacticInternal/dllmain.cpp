// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <TlHelp32.h>
#include <time.h>
#include <sstream>
#include <string>

#include "Display.h"
#include "Player.h"
#include "Hack.h"


typedef void (*tShoot)(Weapon*);

tShoot Shoot = nullptr; // Modulebase + 1514AA0

uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);


DWORD WINAPI HackThread(HMODULE hModule) {

	AllocConsole();
	FILE* fp = NULL;

	freopen_s(&fp, "CONOUT$", "w", stdout);


	MyPlayer Player;

	Player.Start();

	while (!GetAsyncKeyState(VK_END) & 1) {
				
		Player.UpdateValues();

		Sleep(5);
	}

	if (fp)
	{
		fclose(fp);
	}

	FreeConsole();

	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
