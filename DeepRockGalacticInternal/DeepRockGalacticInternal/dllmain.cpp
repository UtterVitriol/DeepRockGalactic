#include <Windows.h>
#include <iostream>
#include <cstdio>

#include "Player.h"
#include "Hack.h"
#include "Menus.h"

#include "d3d12hook.h"

typedef void (*tShoot)(Weapon*);

tShoot Shoot = nullptr; // Modulebase + 1514AA0

uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);

D3D12Hook MyHook;
MyPlayer g_player;

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define DERR(s) g_player.log.AddLog("[-]: %s:%d:%s(): %s\n", __FILENAME__, __LINE__, __func__, s)
#define DMSG(s) g_player.log.AddLog("[+]: %s:%d:%s(): %s\n", __FILENAME__, __LINE__, __func__, s)


DWORD WINAPI HackThread(HMODULE hModule) {

	g_player.Start();

	MyHook.MyMenu = MyMenu;

	HWND hWnd = NULL;
	

	if (!MyHook.d3d12InitHook())
	{
		goto END;
	}
	
	// This can get deleted
	hWnd = FindWindowW(L"Unreal Window", L"Deep Rock Galactic");

	if (NULL == hWnd)   
	{
		DERR("No Window");
	}
	else
	{
		DMSG("YAY WINDOW");
	}


	DMSG(MyHook.process.tTitle);
	DMSG(MyHook.process.tClass);

	DERR("Hello Fucker");

	while (1) {

		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}
		Sleep(5);
	}


	

	MyHook.d3d12UnHook();

END:
	

	FreeLibraryAndExitThread(hModule, 0);
}



BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	DisableThreadLibraryCalls(hModule);

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
