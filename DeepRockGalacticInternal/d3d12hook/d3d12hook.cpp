/////////////////////
// D3D12 HOOK ImGui//
/////////////////////

#include "d3d12hook.h"

#include <cstdio>
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define DERR(s) printf("[-]: %s:%d:%s(): %s", __FILENAME__, __LINE__, __func__, s)
#define DMSG(s) printf("[+]: %s:%d:%s(): %s", __FILENAME__, __LINE__, __func__, s)

extern D3D12Hook MyHook;

// WORD WINAPI MainThread(LPVOID lpParameter) 
void D3D12Hook::d3d12InitHook()
{

	// Get process window infomation
	bool WindowFocus = false;

	while (WindowFocus == false) {

		DWORD ForegroundWindowProcessID;

		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);

		if (GetCurrentProcessId() == ForegroundWindowProcessID) {

			MyHook.process.ID = GetCurrentProcessId();
			MyHook.process.Handle = GetCurrentProcess();
			MyHook.process.Hwnd = GetForegroundWindow();

			RECT TempRect;
			GetWindowRect(MyHook.process.Hwnd, &TempRect);
			MyHook.process.WindowWidth = TempRect.right - TempRect.left;
			MyHook.process.WindowHeight = TempRect.bottom - TempRect.top;

			GetWindowTextA(MyHook.process.Hwnd, MyHook.process.tTitle, sizeof(MyHook.process.tTitle));

			GetClassNameA(MyHook.process.Hwnd, MyHook.process.tClass, sizeof(MyHook.process.tClass));

			char TempPath[MAX_PATH];
			GetModuleFileNameExA(MyHook.process.Handle, NULL, TempPath, sizeof(TempPath));
			MyHook.process.Path = TempPath;

			WindowFocus = true;
		}
	}

	bool InitHook = false;
	while (InitHook == false) {
		if (GetVirtualFunctions() == true) {
			CreateHook(54, (void**)&MyHook.oExecuteCommandLists, hkExecuteCommandLists);
			CreateHook(140, (void**)&MyHook.oPresent, hkPresent);
			/*CreateHook(84, (void**)&MyHook.oDrawInstanced, hkDrawInstanced);
			CreateHook(85, (void**)&MyHook.oDrawIndexedInstanced, hkDrawIndexedInstanced);*/
			InitHook = true;
		}
	}
	//return 0;
}

void D3D12Hook::d3d12UnHook()
{
	// Reset window procedure
	bShutDown = true;
	DisableAll();
	(WNDPROC)SetWindowLongPtr(MyHook.process.Hwnd, GWLP_WNDPROC, (__int3264)(LONG_PTR)MyHook.process.WndProc);
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


bool D3D12Hook::CreateHook(uint16_t Index, void** Original, void* Function)
{
	//assert(_index >= 0 && _original != NULL && _function != NULL);
	void* target = (void*)MethodsTable[Index];
	if (MH_CreateHook(target, Function, Original) != MH_OK || MH_EnableHook(target) != MH_OK) {
		return false;
	}
	return true;
}

LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (MyHook.ShowMenu) {
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		return true;
	}
	return CallWindowProc(MyHook.process.WndProc, hwnd, uMsg, wParam, lParam);
}

/// <summary>
/// User to Get virtual functions for:
///		ID3D12Device
///		ID3D12CommandQueue
///		ID3D12CommandAllocator
///		ID3D12GraphicsCommandList
///		IDXGISwapChain
/// </summary>
/// <returns>
/// true if success
/// false if failure
/// </returns>
bool D3D12Hook::GetVirtualFunctions()
{
	if (CreateDummyWindow() == false)
	{
		return false;
	}

	HMODULE D3D12Module = GetModuleHandle(L"d3d12.dll");
	HMODULE DXGIModule = GetModuleHandle(L"dxgi.dll");
	if (D3D12Module == NULL || DXGIModule == NULL)
	{
		DeleteDummyWindow();
		return false;
	}

	void* CreateDXGIFactory = GetProcAddress(DXGIModule, "CreateDXGIFactory");
	if (CreateDXGIFactory == NULL)
	{
		DeleteDummyWindow();
		return false;
	}

	IDXGIFactory* Factory = NULL;
	if (((long(__stdcall*)(const IID&, void**))(CreateDXGIFactory))(__uuidof(IDXGIFactory), (void**)&Factory) < 0)
	{
		DeleteDummyWindow();
		return false;
	}

	IDXGIAdapter* Adapter = NULL;
	if (Factory->EnumAdapters(0, &Adapter) == DXGI_ERROR_NOT_FOUND)
	{
		DeleteDummyWindow();
		return false;
	}

	void* D3D12CreateDevice = GetProcAddress(D3D12Module, "D3D12CreateDevice");
	if (D3D12CreateDevice == NULL)
	{
		DeleteDummyWindow();
		return false;
	}

	ID3D12Device* Device = NULL;
	if (((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(Adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&Device) < 0)
	{
		DeleteDummyWindow();
		return false;
	}

	D3D12_COMMAND_QUEUE_DESC QueueDesc = { };
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	QueueDesc.Priority = 0;
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	QueueDesc.NodeMask = 0;

	ID3D12CommandQueue* CommandQueue = NULL;
	if (Device->CreateCommandQueue(&QueueDesc, __uuidof(ID3D12CommandQueue), (void**)&CommandQueue) < 0)
	{
		DeleteDummyWindow();
		return false;
	}

	ID3D12CommandAllocator* CommandAllocator = NULL;
	if (Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&CommandAllocator) < 0)
	{
		DeleteDummyWindow();
		return false;
	}

	ID3D12GraphicsCommandList* CommandList = NULL;
	if (Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&CommandList) < 0)
	{
		DeleteDummyWindow();
		return false;
	}

	DXGI_RATIONAL RefreshRate = {};
	RefreshRate.Numerator = 60;
	RefreshRate.Denominator = 1;

	DXGI_MODE_DESC BufferDesc = {};
	BufferDesc.Width = 100;
	BufferDesc.Height = 100;
	BufferDesc.RefreshRate = RefreshRate;
	BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SAMPLE_DESC SampleDesc = {};
	SampleDesc.Count = 1;
	SampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
	SwapChainDesc.BufferDesc = BufferDesc;
	SwapChainDesc.SampleDesc = SampleDesc;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 2;
	SwapChainDesc.OutputWindow = WindowHwnd;
	SwapChainDesc.Windowed = 1;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGISwapChain* SwapChain = NULL;
	if (Factory->CreateSwapChain(CommandQueue, &SwapChainDesc, &SwapChain) < 0)
	{
		DeleteDummyWindow();
		return false;
	}


	MyHook.MethodsTable = NULL;
	MyHook.MethodsTable = (uintx_t*)::calloc(150, sizeof(uintx_t));
	if (NULL != MyHook.MethodsTable) {
		memcpy(MyHook.MethodsTable, *(uintx_t**)Device, 44 * sizeof(uintx_t));
		memcpy(MyHook.MethodsTable + 44, *(uintx_t**)CommandQueue, 19 * sizeof(uintx_t));
		memcpy(MyHook.MethodsTable + 44 + 19, *(uintx_t**)CommandAllocator, 9 * sizeof(uintx_t));
		memcpy(MyHook.MethodsTable + 44 + 19 + 9, *(uintx_t**)CommandList, 60 * sizeof(uintx_t));
		memcpy(MyHook.MethodsTable + 44 + 19 + 9 + 60, *(uintx_t**)SwapChain, 18 * sizeof(uintx_t));
	}

	MH_Initialize();
	Device->Release();
	Device = NULL;
	CommandQueue->Release();
	CommandQueue = NULL;
	CommandAllocator->Release();
	CommandAllocator = NULL;
	CommandList->Release();
	CommandList = NULL;
	SwapChain->Release();
	SwapChain = NULL;
	DeleteDummyWindow();
	return true;
}

/// <summary>
/// Creates Temporary Window used to get virtual table functions
/// </summary>
/// <returns>
/// true if success
/// false if failure
/// </returns>
bool D3D12Hook::CreateDummyWindow()
{
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = DefWindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandle(NULL);
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = L"MJ";
	WindowClass.hIconSm = NULL;
	RegisterClassEx(&WindowClass);
	WindowHwnd = CreateWindow(WindowClass.lpszClassName, L"DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL);
	if (WindowHwnd == NULL) {
		return false;
	}
	return true;
}

/// <summary>
/// Deletes Temporary Window used to get virtual table functions
/// </summary>
/// <returns></returns>
bool D3D12Hook::DeleteDummyWindow()
{
	DestroyWindow(WindowHwnd);
	UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
	if (WindowHwnd != NULL) {
		return false;
	}
	return true;
}

/// <summary>
/// Disables Specific hook
/// </summary>
/// <param name="Index"></param>
void D3D12Hook::DisableHook(uint16_t Index)
{
	assert(Index >= 0);
	MH_DisableHook((void*)MethodsTable[Index]);
}

/// <summary>
/// Disables all hooks
/// </summary>
void D3D12Hook::DisableAll()
{
	MH_DisableHook(MH_ALL_HOOKS);
	free(MethodsTable);
	MethodsTable = NULL;
}


/// <summary>
///	Sets My Command queue to process command queue
/// </summary>
void hkExecuteCommandLists(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists)
{
	if (!MyHook.directX12Interface.CommandQueue)
		MyHook.directX12Interface.CommandQueue = queue;

	MyHook.oExecuteCommandLists(queue, NumCommandLists, ppCommandLists);
}

/// <summary>
/// Main loop for imgui
/// </summary>
HRESULT APIENTRY hkPresent(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!MyHook.ImGui_Initialised) {
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void**)&MyHook.directX12Interface.Device))) {

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); 

			//// Lol what?????
			//(void)io;

			// This might not be required.
			ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;

			//  Enable keyboard controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;


			// 
			DXGI_SWAP_CHAIN_DESC Desc;
			pSwapChain->GetDesc(&Desc);
			Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			Desc.OutputWindow = MyHook.process.Hwnd;
			Desc.Windowed = ((GetWindowLongPtr(MyHook.process.Hwnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

			MyHook.directX12Interface.BuffersCounts = Desc.BufferCount;
			MyHook.directX12Interface.FrameContext = new _FrameContext[MyHook.directX12Interface.BuffersCounts];

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = {};
			DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			DescriptorImGuiRender.NumDescriptors = MyHook.directX12Interface.BuffersCounts;
			DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			if (MyHook.directX12Interface.Device->CreateDescriptorHeap(&DescriptorImGuiRender, IID_PPV_ARGS(&MyHook.directX12Interface.DescriptorHeapImGuiRender)) != S_OK)
				return MyHook.oPresent(pSwapChain, SyncInterval, Flags);

			ID3D12CommandAllocator* Allocator;
			if (MyHook.directX12Interface.Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)) != S_OK)
				return MyHook.oPresent(pSwapChain, SyncInterval, Flags);

			for (size_t i = 0; i < MyHook.directX12Interface.BuffersCounts; i++) {
				MyHook.directX12Interface.FrameContext[i].CommandAllocator = Allocator;
			}

			if (MyHook.directX12Interface.Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&MyHook.directX12Interface.CommandList)) != S_OK ||
				MyHook.directX12Interface.CommandList->Close() != S_OK)
				return MyHook.oPresent(pSwapChain, SyncInterval, Flags);

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers = {};
			DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			DescriptorBackBuffers.NumDescriptors = MyHook.directX12Interface.BuffersCounts;
			DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DescriptorBackBuffers.NodeMask = 1;

			if (MyHook.directX12Interface.Device->CreateDescriptorHeap(&DescriptorBackBuffers, IID_PPV_ARGS(&MyHook.directX12Interface.DescriptorHeapBackBuffers)) != S_OK)
				return MyHook.oPresent(pSwapChain, SyncInterval, Flags);

			const auto RTVDescriptorSize = MyHook.directX12Interface.Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = MyHook.directX12Interface.DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();

			for (size_t i = 0; i < MyHook.directX12Interface.BuffersCounts; i++) {
				ID3D12Resource* pBackBuffer = nullptr;
				MyHook.directX12Interface.FrameContext[i].DescriptorHandle = RTVHandle;
				pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
				MyHook.directX12Interface.Device->CreateRenderTargetView(pBackBuffer, nullptr, RTVHandle);
				MyHook.directX12Interface.FrameContext[i].Resource = pBackBuffer;
				RTVHandle.ptr += RTVDescriptorSize;
			}

			ImGui_ImplWin32_Init(MyHook.process.Hwnd);
			ImGui_ImplDX12_Init(MyHook.directX12Interface.Device, MyHook.directX12Interface.BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM, MyHook.directX12Interface.DescriptorHeapImGuiRender, MyHook.directX12Interface.DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(), MyHook.directX12Interface.DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
			ImGui_ImplDX12_CreateDeviceObjects();
			ImGui::GetIO().ImeWindowHandle = MyHook.process.Hwnd;

			// Must reset (Probably)
			MyHook.process.WndProc = (WNDPROC)SetWindowLongPtr(MyHook.process.Hwnd, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc);
		}
		MyHook.ImGui_Initialised = true;
	}

	if (MyHook.directX12Interface.CommandQueue == nullptr)
	{
		return MyHook.oPresent(pSwapChain, SyncInterval, Flags);
	}

	if (!MyHook.bShutDown)
	{


		if (GetAsyncKeyState(VK_INSERT) & 1) MyHook.ShowMenu = !MyHook.ShowMenu;
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::GetIO().MouseDrawCursor = MyHook.ShowMenu;
		if (MyHook.ShowMenu == true) {
			ImGui::ShowDemoWindow();
		}
		ImGui::EndFrame();

		_FrameContext& CurrentFrameContext = MyHook.directX12Interface.FrameContext[pSwapChain->GetCurrentBackBufferIndex()];
		CurrentFrameContext.CommandAllocator->Reset();

		D3D12_RESOURCE_BARRIER Barrier = {};
		Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		Barrier.Transition.pResource = CurrentFrameContext.Resource;
		Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		MyHook.directX12Interface.CommandList->Reset(CurrentFrameContext.CommandAllocator, nullptr);
		MyHook.directX12Interface.CommandList->ResourceBarrier(1, &Barrier);
		MyHook.directX12Interface.CommandList->OMSetRenderTargets(1, &CurrentFrameContext.DescriptorHandle, FALSE, nullptr);
		MyHook.directX12Interface.CommandList->SetDescriptorHeaps(1, &MyHook.directX12Interface.DescriptorHeapImGuiRender);

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), MyHook.directX12Interface.CommandList);
		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		MyHook.directX12Interface.CommandList->ResourceBarrier(1, &Barrier);
		MyHook.directX12Interface.CommandList->Close();
		MyHook.directX12Interface.CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&MyHook.directX12Interface.CommandList));
	}
	return MyHook.oPresent(pSwapChain, SyncInterval, Flags);
}


//
// Start My Hooks
//

/// <summary>
/// Doesn't fucking do anything
/// </summary>
void APIENTRY hkDrawInstanced(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation) {

	return MyHook.oDrawInstanced(dCommandList, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

/// <summary>
/// Also doesn't fucking do anything
/// </summary>
void APIENTRY hkDrawIndexedInstanced(ID3D12GraphicsCommandList* dCommandList, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation) {

	/*
	//cyberpunk 2077 no pants hack (low settings)
	if (nopants_enabled)
		if (IndexCountPerInstance == 10068 || //bargirl pants near
			IndexCountPerInstance == 3576) //med range
			return; //delete texture

	if (GetAsyncKeyState(VK_F12) & 1) //toggle key
		nopants_enabled = !nopants_enabled;


	//logger, hold down B key until a texture disappears, press END to log values of those textures
	if (GetAsyncKeyState('V') & 1) //-
		countnum--;
	if (GetAsyncKeyState('B') & 1) //+
		countnum++;
	if (GetAsyncKeyState(VK_MENU) && GetAsyncKeyState('9') & 1) //reset, set to -1
		countnum = -1;

	if (countnum == IndexCountPerInstance / 100)
		if (GetAsyncKeyState(VK_END) & 1) //log
			Log("IndexCountPerInstance == %d && InstanceCount == %d",
				IndexCountPerInstance, InstanceCount);

	if (countnum == IndexCountPerInstance / 100)
		return;
	*/

	return MyHook.oDrawIndexedInstanced(dCommandList, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}
//
// End My Hooks
//



//=========================================================================================================================//

//D3D12 Methods Table:
//[0]   QueryInterface
//[1]   AddRef
//[2]   Release
//[3]   GetPrivateData
//[4]   SetPrivateData
//[5]   SetPrivateDataInterface
//[6]   SetName
//[7]   GetNodeCount
//[8]   CreateCommandQueue
//[9]   CreateCommandAllocator
//[10]  CreateGraphicsPipelineState
//[11]  CreateComputePipelineState
//[12]  CreateCommandList
//[13]  CheckFeatureSupport
//[14]  CreateDescriptorHeap
//[15]  GetDescriptorHandleIncrementSize
//[16]  CreateRootSignature
//[17]  CreateConstantBufferView
//[18]  CreateShaderResourceView
//[19]  CreateUnorderedAccessView
//[20]  CreateRenderTargetView
//[21]  CreateDepthStencilView
//[22]  CreateSampler
//[23]  CopyDescriptors
//[24]  CopyDescriptorsSimple
//[25]  GetResourceAllocationInfo
//[26]  GetCustomHeapProperties
//[27]  CreateCommittedResource
//[28]  CreateHeap
//[29]  CreatePlacedResource
//[30]  CreateReservedResource
//[31]  CreateSharedHandle
//[32]  OpenSharedHandle
//[33]  OpenSharedHandleByName
//[34]  MakeResident
//[35]  Evict
//[36]  CreateFence
//[37]  GetDeviceRemovedReason
//[38]  GetCopyableFootprints
//[39]  CreateQueryHeap
//[40]  SetStablePowerState
//[41]  CreateCommandSignature
//[42]  GetResourceTiling
//[43]  GetAdapterLuid
//[44]  QueryInterface
//[45]  AddRef
//[46]  Release
//[47]  GetPrivateData
//[48]  SetPrivateData
//[49]  SetPrivateDataInterface
//[50]  SetName
//[51]  GetDevice
//[52]  UpdateTileMappings
//[53]  CopyTileMappings
//[54]  ExecuteCommandLists
//[55]  SetMarker
//[56]  BeginEvent
//[57]  EndEvent
//[58]  Signal
//[59]  Wait
//[60]  GetTimestampFrequency
//[61]  GetClockCalibration
//[62]  GetDesc
//[63]  QueryInterface
//[64]  AddRef
//[65]  Release
//[66]  GetPrivateData
//[67]  SetPrivateData
//[68]  SetPrivateDataInterface
//[69]  SetName
//[70]  GetDevice
//[71]  Reset
//[72]  QueryInterface
//[73]  AddRef
//[74]  Release
//[75]  GetPrivateData
//[76]  SetPrivateData
//[77]  SetPrivateDataInterface
//[78]  SetName
//[79]  GetDevice
//[80]  GetType
//[81]  Close
//[82]  Reset
//[83]  ClearState
//[84]  DrawInstanced
//[85]  DrawIndexedInstanced
//[86]  Dispatch
//[87]  CopyBufferRegion
//[88]  CopyTextureRegion
//[89]  CopyResource
//[90]  CopyTiles
//[91]  ResolveSubresource
//[92]  IASetPrimitiveTopology
//[93]  RSSetViewports
//[94]  RSSetScissorRects
//[95]  OMSetBlendFactor
//[96]  OMSetStencilRef
//[97]  SetPipelineState
//[98]  ResourceBarrier
//[99]  ExecuteBundle
//[100] SetDescriptorHeaps
//[101] SetComputeRootSignature
//[102] SetGraphicsRootSignature
//[103] SetComputeRootDescriptorTable
//[104] SetGraphicsRootDescriptorTable
//[105] SetComputeRoot32BitConstant
//[106] SetGraphicsRoot32BitConstant
//[107] SetComputeRoot32BitConstants
//[108] SetGraphicsRoot32BitConstants
//[109] SetComputeRootConstantBufferView
//[110] SetGraphicsRootConstantBufferView
//[111] SetComputeRootShaderResourceView
//[112] SetGraphicsRootShaderResourceView
//[113] SetComputeRootUnorderedAccessView
//[114] SetGraphicsRootUnorderedAccessView
//[115] IASetIndexBuffer
//[116] IASetVertexBuffers
//[117] SOSetTargets
//[118] OMSetRenderTargets
//[119] ClearDepthStencilView
//[120] ClearRenderTargetView
//[121] ClearUnorderedAccessViewUint
//[122] ClearUnorderedAccessViewFloat
//[123] DiscardResource
//[124] BeginQuery
//[125] EndQuery
//[126] ResolveQueryData
//[127] SetPredication
//[128] SetMarker
//[129] BeginEvent
//[130] EndEvent
//[131] ExecuteIndirect
//[132] QueryInterface
//[133] AddRef
//[134] Release
//[135] SetPrivateData
//[136] SetPrivateDataInterface
//[137] GetPrivateData
//[138] GetParent
//[139] GetDevice
//[140] Present
//[141] GetBuffer
//[142] SetFullscreenState
//[143] GetFullscreenState
//[144] GetDesc
//[145] ResizeBuffers
//[146] ResizeTarget
//[147] GetContainingOutput
//[148] GetFrameStatistics
//[149] GetLastPresentCount