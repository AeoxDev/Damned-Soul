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
	renderStates[currentSize].shaderResourceView = CreateShaderResourceViewTexture(renderStates[currentSize].renderTargetView, RESOURCE_FLAGS::BIND_RENDER_TARGET);

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

	renderStates[currentSize].vertexShader = LoadVertexShader("UIVS.cso", LAYOUT_DESC::SCREEN);
	renderStates[currentSize].pixelShader = LoadPixelShader("UIPS.cso");

	return currentSize++;
}

int SetupGameRenderer()
{
	//int hr = SetupDirectX(sdl.window);
	renderStates[currentSize].rasterizerState = CreateRasterizerState(true, true);
	//bool s = SetRasterizerState(renderStates[currentSize].rasterizerState);

	renderStates[currentSize].pixelShader = LoadPixelShader("TestPS.cso");
	//s = SetPixelShader(renderStates[currentSize].pixelShader);
	renderStates[currentSize].vertexShader = LoadVertexShader("TestVS.cso");
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
	renderStates[currentSize].samplerState = CreateSamplerState();
	// Set the sampler state
	SetSamplerState(renderStates[currentSize].samplerState);
	//d3d11Data->deviceContext->PSSetSamplers(0, 1, &smpHolder->smp_map[renderStates[currentSize].samplerState]);

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

int SetupParticles(/*Particle* particles,*/)
{
	//particleRenderState.rasterizerState = CreateRasterizerState(false, true);
	//particleRenderState.pixelShader = LoadPixelShader("ParticlePS.cso");
	//particleRenderState.vertexShader = LoadVertexShader("ParticleVS.cso");
	//particleRenderState.geometryShader = LoadGeometryShader("ParticleGS.cso");
	//particleRenderState.computeShader = LoadComputeShader("ParticleCS.cso");
	//particleRenderState.vertexBuffer = CreateVertexBuffer();

	//particleRenderState.readSRV = ;
	//particleRenderState.writeSRV = ;
	//particleRenderState.readUAV = ;
	//particleRenderState.writeUAV = ;
	//particleRenderState.constantBuffer = ;


	renderStates[currentSize].constantBuffer = CreateConstantBuffer(Particles::GetData(), sizeof(ParticleMetadataBuffer)); // THIS IS FOR THE COMPUTE SHADER
	renderStates[currentSize].rasterizerState = CreateRasterizerState(false, true);
	renderStates[currentSize].vertexBuffer = CreateVertexBuffer(sizeof(Particle), MAX_PARTICLES, USAGE_DEFAULT);
	renderStates[currentSize].vertexShader = LoadVertexShader("ParticleVS.cso", PARTICLE);
	renderStates[currentSize].pixelShader = LoadPixelShader("ParticlePS.cso");
	renderStates[currentSize].geometryShader = LoadGeometryShader("ParticleGS.cso");

	RESOURCE_FLAGS resourceFlags = static_cast<RESOURCE_FLAGS>(BIND_SHADER_RESOURCE | BIND_RENDER_TARGET);
	renderStates[currentSize].renderTargetView = CreateRenderTargetView(USAGE_DEFAULT, resourceFlags, (CPU_FLAGS)0, sdl.WIDTH, sdl.HEIGHT);
	renderStates[currentSize].shaderResourceView = CreateShaderResourceViewTexture(renderStates[currentSize].renderTargetView, BIND_RENDER_TARGET);
	renderStates[currentSize].depthStencilView = CreateDepthStencil(sdl.WIDTH, sdl.HEIGHT);

	return currentSize++;
}

void PrepareDefaultPipeline()
{
	SetTopology(TRIANGLELIST);

	SetVertexShader(renderStates[backBufferRenderSlot].vertexShader);
	SetPixelShader(renderStates[backBufferRenderSlot].pixelShader);

	SetRasterizerState(renderStates[backBufferRenderSlot].rasterizerState);
}

void PrepareBackBuffer()
{
	SetTopology(TRIANGLELIST);

	SetRasterizerState(renderStates[backBufferRenderSlot].rasterizerState);
	SetPixelShader(renderStates[backBufferRenderSlot].pixelShader);
	SetVertexShader(renderStates[backBufferRenderSlot].vertexShader);
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

void RenderOffset(const size_t& count, const size_t& offset)
{
	d3d11Data->deviceContext->Draw(count, offset);
}

void ClearParticles()
{
	ClearRenderTargetView(Particles::m_renderTargetView);
	ClearDepthStencilView(Particles::m_depthStencilView);

	SetRenderTargetViewAndDepthStencil(Particles::m_renderTargetView, Particles::m_depthStencilView);
}

void Present()
{
	d3d11Data->swapChain->Present(0, 0);
}

void Dispatch(UINT threadX, UINT threadY, UINT threadZ)
{
	d3d11Data->deviceContext->Dispatch(threadX, threadY, threadZ);
}