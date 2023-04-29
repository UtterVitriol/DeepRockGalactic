/////////////////////
// D3D12 HOOK ImGui//
/////////////////////

#include "d3d12hook.h"
#include "kiero.h"

extern D3D12Hook MyHook;

bool D3D12Hook::d3d12InitHook()
{
	bool HasWindowHandle = false;
	while (HasWindowHandle == false) {

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

			HasWindowHandle = true;
		}
	}

	if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success)
	{
		kiero::bind(54, (void**)&MyHook.oExecuteCommandLists, hkExecuteCommandLists);
		kiero::bind(140, (void**)&MyHook.oPresent, hkPresent);
		return true;
	}

	return false;
}

void D3D12Hook::d3d12UnHook()
{
	if (!MyHook.ImGui_Initialised)
	{
		return;
	}
	// Reset window procedure
	bShutDown = true;

	directX12Interface.Device->Release();
	directX12Interface.DescriptorHeapBackBuffers->Release();
	directX12Interface.DescriptorHeapImGuiRender->Release();
	directX12Interface.CommandList->Release();
	directX12Interface.CommandQueue->Release();
	//DisableAll();
	kiero::shutdown();
	(WNDPROC)SetWindowLongPtr(MyHook.process.Hwnd, GWLP_WNDPROC, (__int3264)(LONG_PTR)MyHook.process.WndProc);
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
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
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsDark();
			io.Fonts->AddFontDefault();
			io.FontGlobalScale = 1.5f;
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
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			MyHook.ShowMenu = !MyHook.ShowMenu;
		}

		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::GetIO().MouseDrawCursor = MyHook.ShowMenu;

		if (MyHook.ShowMenu == true) {
			//ImGui::ShowDemoWindow();
			MyHook.MyMenu();
		}
		ImGui::EndFrame();

		ImGui::Render();

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