//Written By:	Wesley Copeland
//File:			DirectXRenderer.cpp
//Intended use:	Write the logic for the rendering process, running and cleanup
#include "stdafx.h"
#include "DirectXRenderer.h"



DirectXRenderer::DirectXRenderer()
{

}

DirectXRenderer::~DirectXRenderer()
{

}

//Do all of the Initialization Required for DirectX
bool DirectXRenderer::Init(HINSTANCE _HINSTANCE, WNDPROC _WNDPROC)
{
	application = _HINSTANCE;
	appWndProc = _WNDPROC;

	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"SharpEdge";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	
	RegisterClassEx(&wndClass);

	RECT window_size = { 0, 0, WINDOW_BUFFER_WIDTH, WINDOW_BUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(L"SharpEdge", L"V 1.0", WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, this);

	ShowWindow(window, SW_SHOW);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	
	SetUpSwapchain(swapChainDesc);

	unsigned int flag;
#if _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif
#if !_DEBUG
	flag = D3D11_CREATE_DEVICE_SINGLETHREADED;
#endif

	D3D_FEATURE_LEVEL featurelevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D_FEATURE_LEVEL F;
	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, flag,
		featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain,
		&m_pDevice, &F, &m_pContext);

	if (result != S_OK)
		return false;

	SetUpBackBuffer();
	SetUpViewPort();

	std::string shaderpath = "../";
#ifdef _WIN64
	shaderpath += "x64/";
#endif
#ifdef _DEBUG
	shaderpath += "Debug/";
#else
	shaderpath += "Release/";
#endif

	void * bc = 0;
	unsigned int bcSize = 0;

	if(LoadShaderByteCode((shaderpath + "AlwaysLitVertex.cso").c_str(), &bc, bcSize))
	m_pDevice->CreateVertexShader(bc, bcSize, 0, &m_pBasicVertShader);
	
	if(LoadShaderByteCode((shaderpath + "AlwaysLitPixel.cso").c_str(), &bc, bcSize))
	m_pDevice->CreatePixelShader(bc, bcSize, 0, &m_pBasicPixelShader);

	m_pContext->VSSetShader(m_pBasicVertShader, 0, 0);
	m_pContext->PSSetShader(m_pBasicPixelShader, 0, 0);

	return true;
}

//Clear all of the buffers
bool DirectXRenderer::Clear(void)
{
	return true;
}

//Cleanup All of the memory used in the rendering process
//should be called before the termination of the program
bool DirectXRenderer::Shutdown(void)
{
	BOOL isfullscreen = 0;
	m_pSwapChain->GetFullscreenState(&isfullscreen, nullptr);
	if (isfullscreen)
		m_pSwapChain->SetFullscreenState(false, nullptr);


	UnregisterClass(L"SharpEdge",application);
	return true;
}

bool DirectXRenderer::Update(void)
{
	m_pContext->OMSetRenderTargets(1, &m_pRTV,0);
	m_pContext->RSSetViewports(1, &m_ViewPort);
	m_pContext->ClearRenderTargetView(m_pRTV, DirectX::Colors::Orange);

	m_pSwapChain->Present(0, 0);
	return true;
}

bool DirectXRenderer::SetUpSwapchain(DXGI_SWAP_CHAIN_DESC &_swapChainDesc)
{
	
	
	ZeroMemory(&_swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	_swapChainDesc.BufferCount = 1;
	_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	_swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	_swapChainDesc.OutputWindow = window;
	_swapChainDesc.BufferDesc.Width = WINDOW_BUFFER_WIDTH;
	_swapChainDesc.BufferDesc.Height = WINDOW_BUFFER_HEIGHT;
	_swapChainDesc.Windowed = true;
	_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	_swapChainDesc.SampleDesc.Count = 1;
	_swapChainDesc.SampleDesc.Quality = 0;


	return true;
}

bool DirectXRenderer::SetUpBackBuffer(void)
{
	m_pSwapChain->GetBuffer(0, __uuidof(m_pBackbuffer), reinterpret_cast<void**>(&m_pBackbuffer));
	m_pDevice->CreateRenderTargetView(m_pBackbuffer, NULL, &m_pRTV);
	m_pBackbuffer->Release();
	m_pBackbuffer = nullptr;
	return true;
}

bool DirectXRenderer::SetUpViewPort(void)
{
	m_ViewPort.TopLeftX = 0.0f;
	m_ViewPort.TopLeftY = 0.0f;
	m_ViewPort.Height = (float)WINDOW_BUFFER_HEIGHT;
	m_ViewPort.Width = (float)WINDOW_BUFFER_WIDTH;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	return true;
}

bool DirectXRenderer::LoadShaderByteCode(const char* _csoPath, void ** _returnByteCode, unsigned int & byte_Code_Size)
{
  std::ifstream shader;
  shader.open(_csoPath, std::ios_base::binary);

  if (!shader.is_open())
	  return false;
  
  shader.seekg(0, std::ios_base::end);
  byte_Code_Size = (unsigned int)shader.tellg();
	
  if (shader.eof())
	  return false;

  *_returnByteCode = (void*)new char[byte_Code_Size];
  shader.seekg(0, std::ios_base::beg);
  shader.read((char*)*_returnByteCode, byte_Code_Size);
  shader.close();
  return true;
}