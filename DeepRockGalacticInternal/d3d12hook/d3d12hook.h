#pragma once

#include <windows.h>
#include <psapi.h>
#include <cstdint>
#include <dxgi1_4.h>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include "Minhook\include\MinHook.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"

#if defined _M_X64
typedef uint64_t uintx_t;
#elif defined _M_IX86
typedef uint32_t uintx_t;
#endif


//
// Begin: Globals
//
int countnum = -1;
bool nopants_enabled = true;

bool ShowMenu = false;
bool ImGui_Initialised = false;

WNDCLASSEX WindowClass;
HWND WindowHwnd;

static uintx_t* MethodsTable = NULL;

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
}DirectX12Interface;

class Process {
public:
	DWORD ID;
	HANDLE Handle;
	HWND Hwnd;
	HMODULE Module;
	WNDPROC WndProc;
	int WindowWidth;
	int WindowHeight;
	LPCSTR Title;
	LPCSTR ClassName;
	LPCSTR Path;
}Process;
//
// End: Globals
//

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace DirectX12
{
	bool Init();
}

//
// Start: Call These
//
void d3d12InitHook();
void DisableAll();
// End: Call These

bool InitWindow();
bool DeleteWindow();

bool CreateHook(uint16_t Index, void** Original, void* Function);
void DisableHook(uint16_t Index);


//
// Start: My Hooks
//
typedef HRESULT(APIENTRY* Present12) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
Present12 oPresent = NULL;

typedef void(APIENTRY* ExecuteCommandLists)(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);
ExecuteCommandLists oExecuteCommandLists = NULL;


typedef void(APIENTRY* DrawInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
DrawInstanced oDrawInstanced = NULL;

typedef void(APIENTRY* DrawIndexedInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
DrawIndexedInstanced oDrawIndexedInstanced = NULL;

HRESULT APIENTRY hkPresent(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
void hkExecuteCommandLists(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);
void APIENTRY hkDrawInstanced(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
void APIENTRY hkDrawIndexedInstanced(ID3D12GraphicsCommandList* dCommandList, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
//
// End: My Hooks
//





















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
