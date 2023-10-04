#include "GameRenderer.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "SDLHandler.h"
#include "UIRenderer.h"
#include "Lighting.h"
#include "Particles.h"



RenderSetupComponent renderStates[8];
int currentSize = 0;
int backBufferRenderSlot;

struct Vertex {
	float pos[4];
	float normal[4];
	float uv[2];
};

struct ScreenVertex {
	float pos[3];
};

bool Setup3dGraphics()
{
	if (SetupDirectX(sdl.window) < 0)
	{
		return false;
	};
	return true;
}

int SetupUIRenderState()
{
	renderStates[currentSize].renderTargetView = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, (RESOURCE_FLAGS)((int)BIND_RENDER_TARGET | (int)BIND_SHADER_RESOURCE), CPU_FLAGS::NONE, sdl.WIDTH, sdl.HEIGHT);
	ID3D11Texture2D* texture;
	GetTextureByType(texture, TEXTURE_HOLDER_TYPE::RENDER_TARGET_VIEW, renderStates[currentSize].renderTargetView);
	//RENDER TARGET

	//UI SURFACE
	if (FAILED(texture->QueryInterface<IDXGISurface>(&UISurface)))
	{
		printf("Failed to create UI Surface\n");
		return false;
	}

	//DEPTH STENCIL
	renderStates[currentSize].depthStencilView = CreateDepthStencil(sdl.WIDTH, sdl.HEIGHT);

	//SHADER RESOURCE VIEW
	renderStates[currentSize].shaderResourceView = CreateShaderResourceViewTexture(renderStates[currentSize].renderTargetView, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, RESOURCE_FLAGS::BIND_RENDER_TARGET, 0);

	texture->Release();

	//-----------------------------------------------------------------//

	//Vertex screenQuad[] =
	//{
	//   { {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },  // TOP RIGHT
	//   { {1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }, // BOTTOM RIGHT
	//   { {-1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} }, // TOP LEFT
	//   { {-1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} } // BOTTOM LEFT
	//};
	ScreenVertex screenQuad[] =
	{
	   { {1.0f, 1.0f, 0.0f}},  // TOP RIGHT
	   { {1.0f, -1.0f, 0.0f}}, // BOTTOM RIGHT
	   { {-1.0f, 1.0f, 0.0f}}, // TOP LEFT
	   { {-1.0f, -1.0f, 0.0f}} // BOTTOM LEFT
	};
	renderStates[currentSize].vertexBuffer = CreateVertexBuffer(screenQuad, sizeof(ScreenVertex), ARRAYSIZE(screenQuad), USAGE_IMMUTABLE);
	uint32_t idxs[] = { 0, 1, 2, 2, 1, 3 };
	renderStates[currentSize].indexBuffer = CreateIndexBuffer(idxs, sizeof(uint32_t), ARRAYSIZE(idxs));

	renderStates[currentSize].vertexShaders[0] = LoadVertexShader("UIVS.cso", LAYOUT_DESC::SCREEN);
	renderStates[currentSize].pixelShaders[0] = LoadPixelShader("UIPS.cso");

	return currentSize++;
}

int SetupGameRenderer()
{
	//int hr = SetupDirectX(sdl.window);
	renderStates[currentSize].rasterizerState = CreateRasterizerState(true, true);
	//bool s = SetRasterizerState(renderStates[currentSize].rasterizerState);

	renderStates[currentSize].pixelShaders[0] = LoadPixelShader("TestPS.cso");
	renderStates[currentSize].pixelShaders[1] = LoadPixelShader("TestPS.cso");
	//s = SetPixelShader(renderStates[currentSize].pixelShader);
	renderStates[currentSize].vertexShaders[0] = LoadVertexShader("TestVS.cso");
	renderStates[currentSize].vertexShaders[1] = LoadVertexShader("TestSkelVS.cso");
	//s = SetVertexShader(renderStates[currentSize].vertexShader);

	Vertex triangle[3] = {
		0.9f, -0.9f, 0.5f, 1.f,		/**/ 0, 0, -1.f, 0, /**/ 1, 0,
		-0.9f, -0.9f, 0.5f, 1.f,		/**/ 0, 0, -1.f, 0, /**/ 0, 0,
		0, 0.9f, 0.5f, 1.f,			/**/ 0, 0, -1.f, 0, /**/ 0.5, 1 };
	renderStates[currentSize].vertexBuffer = CreateVertexBuffer(triangle, sizeof(Vertex), 3, USAGE_IMMUTABLE);
	uint32_t idxs[3] = { 0, 1, 2 };
	renderStates[currentSize].indexBuffer = CreateIndexBuffer(idxs, sizeof(uint32_t), 3);

	//s = SetVertexBuffer(renderStates[currentSize].vertexBuffer);
	//s = SetIndexBuffer(renderStates[currentSize].indexBuffer);

	d3d11Data->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a sampler state
	//renderStates[currentSize].samplerState = CreateSamplerState();
	// Set the sampler state
	d3d11Data->deviceContext->PSSetSamplers(0, 1, &smpHolder->smp_arr[renderStates[currentSize].samplerState]);

	// Create a viewport
	renderStates[currentSize].viewPort = CreateViewport(sdl.WIDTH, sdl.HEIGHT);
	// Set the viewport
	bool s = SetViewport(renderStates[currentSize].viewPort);
	// Create a render target view
	renderStates[currentSize].renderTargetView = CreateBackBuffer();
	// Create a depth stencil view
	renderStates[currentSize].depthStencilView = CreateDepthStencil(sdl.WIDTH, sdl.HEIGHT);
	// Set a render target view and depth stencil view
	s = SetRenderTargetViewAndDepthStencil(renderStates[currentSize].renderTargetView, renderStates[currentSize].depthStencilView);
	//Create and set a simple sampler
	SMP_IDX sampler = CreateSamplerState();
	SetSamplerState(sampler);
	return currentSize++;
}

void PrepareBackBuffer(const bool use_skeleton)
{
	SetRasterizerState(renderStates[backBufferRenderSlot].rasterizerState);
	SetPixelShader(renderStates[backBufferRenderSlot].pixelShaders[use_skeleton]);
	SetVertexShader(renderStates[backBufferRenderSlot].vertexShaders[use_skeleton]);
}

void ClearBackBuffer()
{
	Clear(backBufferRenderSlot);
}

void Clear(const int& s)
{
	ClearRenderTargetView(renderStates[s].renderTargetView);
	ClearDepthStencilView(renderStates[s].depthStencilView);

	// temporary needed for ui rendering, only set once otherwise
	SetRenderTargetViewAndDepthStencil(renderStates[s].renderTargetView, renderStates[s].depthStencilView);
}

void RenderIndexed(const size_t& count)
{
	//ID3D11Buffer* lightBuf = nullptr;
	//bool test = CreateLightingConstantBuffer( lightBuf);
	//LightingStruct values;
	//values.ambientColor = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.f);
	//values.diffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f);  // White light
	//values.specularColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.f);  // White specular light
	//values.intensity = 32.f;
	//UpdateLightingBuffer(lightBuf, values);
	//d3d11Data->deviceContext->PSSetConstantBuffers(0, 1, &lightBuf);

	d3d11Data->deviceContext->DrawIndexed((UINT)count, 0, 0);
}

void ClearParticles()
{
	ClearRenderTargetView(Particles::m_renderTargetView);
	ClearDepthStencilView(Particles::m_depthStencilView);

	SetRenderTargetViewAndDepthStencil(Particles::m_renderTargetView, Particles::m_depthStencilView);
}

void Present()
{
	//Check if frame capped
	d3d11Data->swapChain->Present(0, 0);
}

void Dispatch(UINT threadX, UINT threadY, UINT threadZ)
{
	d3d11Data->deviceContext->Dispatch(threadX, threadY, threadZ);
}