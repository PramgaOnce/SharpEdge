//Written By:	Wesley Copeland
//File:			Main.cpp
//Intended use:	Entry Point for the Program 
#include "stdafx.h"
using namespace std;

#include "Renderer\DirectXRenderer.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	srand(unsigned int(time(NULL)));

	DirectXRenderer* p_DRenderer = new DirectXRenderer();

	p_DRenderer->Init(hInstance,(WNDPROC)WndProc);

	MSG msg; ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && p_DRenderer->Update())
	{
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	p_DRenderer->Shutdown();
	delete p_DRenderer;
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
	switch (message)
	{
	case (WM_DESTROY) : { PostQuitMessage(0); }
						break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}