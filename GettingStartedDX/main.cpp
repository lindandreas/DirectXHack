#pragma once
#include <iostream>

#define D3D_DEBUG_INFO


#include "DeviceResources.h"
#include "Renderer.h"
#include "Utility.h"
#include "Loader.h"

constexpr auto WINDOWCLASSNAME = L"WindowClassName";

HINSTANCE g_hInstance;


static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main(int argc, char** argv)
{
	HWND hWnd;

	if (g_hInstance == NULL)
		g_hInstance = (HINSTANCE)GetModuleHandle(NULL);

	// Register the windows class
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = g_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WINDOWCLASSNAME;

	if (!RegisterClassEx(&wndClass))
	{
		DWORD dwError = GetLastError();
		if (dwError != ERROR_CLASS_ALREADY_EXISTS)
		{
			HRESULT hr = HRESULT_FROM_WIN32(dwError);
			Utility::PrintHRESULT(hr);
			return hr;
		}
	}


	// Setup Window Rect
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;
	int width = 800;
	int height = 600;

	RECT rect;
	SetRect(&rect, 0, 0, width, height);
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window where we will display out result!
	hWnd = CreateWindow(
		WINDOWCLASSNAME,
		L"Getting Started",
		WS_OVERLAPPEDWINDOW,
		x,
		y,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		g_hInstance,
		NULL
	);

	// Check if window handle ok
	if (hWnd == NULL)
	{
		HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
		Utility::PrintHRESULT(hr);
		return hr;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	auto deviceResources = std::make_unique<DeviceResources>(hWnd);
	auto sceneResources = std::make_unique<SceneResources>();
	auto loader = std::make_unique<Loader>();

	ID3D11Device& device = *deviceResources->GetDevice().Get();

	sceneResources->mesh = Utility::CreateSquare(device);

	// Load Bitmap that can hopefully be used to create a Texture2D
	// I have not tested this
	std::unique_ptr<Bitmap> bitmap = loader->LoadTexture(
		L"../GettingStartedDX/resources/textures/UV_Grid_Sm.jpg"
	);

	// Load basic shader (both VS and PS are loaded here)
	sceneResources->shader = Shader::CreateShader(
		*deviceResources->GetDevice().Get(), 
		L"../GettingStartedDX/resources/shaders/basic.hlsl"
	);


	auto renderer = std::make_unique<Renderer>(*sceneResources, *deviceResources, width, height, 60);

	
	// Start RenderLoop.
	// 
	MSG msg { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Whatever needs to be done before drawing can be done here.
			// Example: Update time, game logic, update camera and so on.

			// Draw a frame!
			renderer->Draw();
		}
	}

	std::cout << "Done" << std::endl;
	return 0;
}

// Render Callback that handles messaging. 
// Input, resizing and such are sent as messages and can be handeled here.
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		UnregisterClass(
			WINDOWCLASSNAME,
			g_hInstance
		);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}