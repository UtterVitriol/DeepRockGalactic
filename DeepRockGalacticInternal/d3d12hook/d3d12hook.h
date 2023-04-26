#pragma once

#include <windows.h>
#include <psapi.h>
#include <cstdint>

#include "MinHook.h"
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include <dxgi1_4.h>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")



#if defined _M_X64
typedef uint64_t uintx_t;
#elif defined _M_IX86
typedef uint32_t uintx_t;
#endif

struct _FrameContext {
	ID3D12CommandAllocator* CommandAllocator;
	ID3D12Resource* Resource;
	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle;
};

class DirectX12Interface {
public:
	ID3D12Device* Device = nullptr;
	ID3D12DescriptorHeap* DescriptorHeapBackBuffers;
	ID3D12DescriptorHeap* DescriptorHeapImGuiRender;
	ID3D12GraphicsCommandList* CommandList;
	ID3D12CommandQueue* CommandQueue;

	uintx_t BuffersCounts = -1;
	_FrameContext* FrameContext;
};

class Process {
public:
	DWORD ID;			// Process ID
	HANDLE Handle;		// Process Handle
	HWND Hwnd;			// Handle to foreground indow

	//HMODULE Module;	// Not used 
	
	WNDPROC WndProc;	// Stores origional addres for window procedure

	int WindowWidth;	// Width 
	int WindowHeight;	// Height

	LPCSTR Title;		// Foreground window title
	LPCSTR ClassName;	// Foregroudn window parent class name
	LPCSTR Path;		// Dll name
	char tTitle[MAX_PATH];
	char tClass[MAX_PATH];
};


//
// Start: My Hooks
//
typedef HRESULT(APIENTRY* Present12) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(APIENTRY* ExecuteCommandLists)(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);
typedef void(APIENTRY* DrawInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
typedef void(APIENTRY* DrawIndexedInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);

HRESULT APIENTRY hkPresent(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
void hkExecuteCommandLists(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);
void APIENTRY hkDrawInstanced(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
void APIENTRY hkDrawIndexedInstanced(ID3D12GraphicsCommandList* dCommandList, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
//
// End: My Hooks
//




class D3D12Hook
{
public:
	
	bool ShowMenu = false;
	bool ImGui_Initialised = false;
	bool bShutDown = false;


	WNDCLASSEX WindowClass;
	HWND WindowHwnd;

	uintx_t* MethodsTable;

	DirectX12Interface directX12Interface;

	Process process;

	Present12 oPresent = NULL;
	ExecuteCommandLists oExecuteCommandLists = NULL;
	DrawInstanced oDrawInstanced = NULL;
	DrawIndexedInstanced oDrawIndexedInstanced = NULL;

	void (*MyMenu)();
	
	// Main function
	bool d3d12InitHook();
	void d3d12UnHook();
};

LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);





















//=========================================================================================================================//

//#include <fstream>
//using namespace std;
//
//char dlldir[320];
//char* GetDirectoryFile(char* filename)
//{
//	static char path[320];
//	strcpy_s(path, dlldir);
//	strcat_s(path, filename);
//	return path;
//}
//
//void Log(const char* fmt, ...)
//{
//	if (!fmt)	return;
//
//	char		text[4096];
//	va_list		ap;
//	va_start(ap, fmt);
//	vsprintf_s(text, fmt, ap);
//	va_end(ap);
//
//	ofstream logfile(GetDirectoryFile((PCHAR)"log.txt"), ios::app);
//	if (logfile.is_open() && text)	logfile << text << endl;
//	logfile.close();
//}

//=========================================================================================================================//
