#ifndef DirectXRender_H
#define DirectXRender_H

//Written By:	Wesley Copeland
//File:			DirectXRenderer.h
//Intended use:	Layout structure of the directX rendering 
#include <d3d11.h>

#pragma comment(lib,"d3d11.lib")





#define WINDOW_BUFFER_WIDTH 800
#define WINDOW_BUFFER_HEIGHT 500

class DirectXRenderer
{
public:
	DirectXRenderer();
	~DirectXRenderer();
	

	//Do all of the Initialization Required for DirectX
	bool Init(HINSTANCE _HINSTANCE, WNDPROC _WNDPROC);

	//Clear all of the buffers
	bool Clear(void);

	//Cleanup All of the memory used in the rendering process
	//should be called before the termination of the program
	bool Shutdown(void);

	//Render the Window and all objects
	bool Update(void);

	//Read Shader Byte Code (This is used to set the shaders up)
	bool LoadShaderByteCode(const char* _csoPath, void ** _returnByteCode, unsigned int & byted_Code_Size);

private:
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	HRESULT							result;

	ID3D11Device * m_pDevice;
	ID3D11DeviceContext* m_pContext;
	ID3D11RenderTargetView* m_pRTV;
	IDXGISwapChain* m_pSwapChain;
	D3D11_VIEWPORT m_ViewPort;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	ID3D11VertexShader* m_pBasicVertShader;
	ID3D11PixelShader* m_pBasicPixelShader;
	ID3D11Resource * m_pBackbuffer;
	XMFLOAT4X4		m_ViewMatrix;
	XMFLOAT4X4		m_ProjMatrix;

	bool SetUpSwapchain(DXGI_SWAP_CHAIN_DESC &_swapChainDesc);
	bool SetUpBackBuffer(void);
	bool SetUpViewPort(void);

};

#endif
