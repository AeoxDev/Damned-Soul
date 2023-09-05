#include "D3D11Graphics.h"
#include <iostream>
#include <fstream>
#include <string>

#define HEIGHT 720
#define WIDTH 1280

//Externs:
ID3D11DeviceContext* deviceContext;
//Privates:
ID3D11Device* device;
IDXGISwapChain* swapChain;

ID3D11PixelShader* tps; // Test Pixel Shader
ID3D11VertexShader* tvs; // Test Vertex Shader
ID3D11InputLayout* til; // Test Input Layout
ID3D11Texture2D* tbb; // Test Back Buffer
ID3D11Texture2D* tdst; // Test Depth Stencil Texture
ID3D11DepthStencilView* tdsv; // Test Depth Stencil View
ID3D11RenderTargetView* trtv; // Test Render Target View

bool CreateDeviceAndSwapChain(HWND& window, UINT width, UINT height)
{
	UINT flags = 0x0;
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);

	if (FAILED(hr))
	{
		std::cout << "Failed to create device and swap chain!" << std::endl;
		return false;
	}

	return true;
}


bool LoadPixelShader(ID3D11PixelShader* pixelShader)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open("bin\\TestPS.cso", std::ios::binary | std::ios::ate);
	if (false == reader.is_open())
	{
		std::cerr << "Could not open PS test file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = device->CreatePixelShader(shaderData.c_str(), shaderData.length(), NULL, &pixelShader);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create PS test shader!" << std::endl;
		return false;
	}

	reader.close();
	return true;
}


bool LoadVertexShader(ID3D11VertexShader* vertexShader, std::string& byteCode)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open("bin\\TestVS.cso", std::ios::binary | std::ios::ate);
	if (false == reader.is_open())
	{
		std::cerr << "Could not open VS test file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = device->CreateVertexShader(shaderData.c_str(), shaderData.length(), NULL, &vertexShader);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create VS test shader!" << std::endl;
		return false;
	}

	byteCode = shaderData;
	reader.close();
	return true;
}


bool CreateInputLayout(ID3D11InputLayout* inputLayout, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, 3, vShaderByteCode.c_str(), vShaderByteCode.length(), &inputLayout);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Input Layout!" << std::endl;
		return false;
	}

	return true;
}


void SetViewport(D3D11_VIEWPORT& viewport, UINT width, UINT height)
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}


bool CreateRenderTargetView(ID3D11Texture2D*& backBuffer, ID3D11RenderTargetView*& rtv)
{
	// get the address of the back buffer
	/*ID3D11Texture2D* */backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "Failed to get back buffer!" << std::endl;
		return false;
	}

	// use the back buffer address to create the render target
	// null as description to base it on the backbuffers values
	HRESULT hr = device->CreateRenderTargetView(backBuffer, NULL, &rtv);
	backBuffer->Release();

	if (FAILED(hr))
	{
		std::cerr << "Failed to create Render Target View!" << std::endl;
		return false;
	}
	return true;

}


bool CreateDepthStencil(UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &dsTexture);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Depth Stencil Texture!" << std::endl;
		return false;
	}

	hr = device->CreateDepthStencilView(dsTexture, 0, &dsView);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Depth Stencil View!" << std::endl;
		return false;
	}

	return true;
}


#define FAIL_MSG { std::cerr << "Failed to set up D3D11!" << std::endl; return -1; }

int SetupDirectX(HWND& w)
{
	if (false == CreateDeviceAndSwapChain(w, WIDTH, HEIGHT))
		FAIL_MSG

	if (false == LoadPixelShader(tps))
		FAIL_MSG
	deviceContext->PSSetShader(tps, nullptr, 0);

	std::string byteCode;
	if (false == LoadVertexShader(tvs, byteCode))
		FAIL_MSG
	deviceContext->VSSetShader(tvs, nullptr, 0);

	CreateInputLayout(til, byteCode);
	deviceContext->IASetInputLayout(til);

	D3D11_VIEWPORT vp;
	SetViewport(vp, WIDTH, HEIGHT);
	deviceContext->RSSetViewports(1, &vp);

	if (false == CreateDepthStencil(WIDTH, HEIGHT, tdst, tdsv))
		FAIL_MSG

	if (false == CreateRenderTargetView(tbb, trtv))
		FAIL_MSG
	deviceContext->OMSetRenderTargets(1, &trtv, tdsv);

	float color[4] = { 1.0f, 0.0f, 0.8235f, 1.0f };
	deviceContext->ClearRenderTargetView(trtv, color);
	deviceContext->Draw(0, 0);
	swapChain->Present(0, 0);

	return 0;
}
