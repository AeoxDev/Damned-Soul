#include "GameRenderer.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "SDLHandler.h"
#include "UIRenderer.h"

struct RenderSetupComponent
{
	RS_IDX	rasterizerState;
	PS_IDX	pixelShader;
	VS_IDX	vertexShader;
	CS_IDX	computeShader;
	VB_IDX	vertexBuffer;
	IB_IDX	indexBuffer;
	VP_IDX	viewPort;
	RTV_IDX renderTargetView;
	DSV_IDX depthStencilView;
	SRV_IDX shaderResourceView;
	UAV_IDX unorderedAccessView;
	SMP_IDX samplerState;
};

RenderSetupComponent renderStates[8];
int currentSize = 0;

int SetupGameRenderer()
{
	int hr = SetupDirectX(sdl.window);
	renderStates[0].rasterizerState = CreateRasterizerState(true, true);
	bool s = SetRasterizerState(renderStates[0].rasterizerState);

	renderStates[0].pixelShader = LoadPixelShader("TestPS.cso");
	s = SetPixelShader(renderStates[0].pixelShader);
	renderStates[0].vertexShader = LoadVertexShader("TestVS.cso");
	s = SetVertexShader(renderStates[0].vertexShader);

	struct Vertex {
		float pos[4];
		float normal[4];
		float uv[2];
	};

	Vertex triangle[3] = {
		0.9f, -0.9f, 0.5f, 1.f,		/**/ 0, 0, -1.f, 0, /**/ 1, 0,
		-0.9f, -0.9f, 0.5f, 1.f,		/**/ 0, 0, -1.f, 0, /**/ 0, 0,
		0, 0.9f, 0.5f, 1.f,			/**/ 0, 0, -1.f, 0, /**/ 0.5, 1 };
	renderStates[0].vertexBuffer = CreateVertexBuffer(triangle, sizeof(Vertex), 3, USAGE_IMMUTABLE);
	uint32_t idxs[3] = { 0, 1, 2 };
	renderStates[0].indexBuffer = CreateIndexBuffer(idxs, sizeof(uint32_t), 3);

	s = SetVertexBuffer(renderStates[0].vertexBuffer);
	s = SetIndexBuffer(renderStates[0].indexBuffer);

	d3d11Data->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a sampler state
	//renderStates[0].samplerState = CreateSamplerState();
	// Set the sampler state
	d3d11Data->deviceContext->PSSetSamplers(0, 1, &smpHolder->smp_arr[renderStates[0].samplerState]);

	// Create a viewport
	renderStates[0].viewPort = CreateViewport(sdl.WIDTH, sdl.HEIGHT);
	// Set the viewport
	s = SetViewport(renderStates[0].viewPort);
	// Create a render target view
	renderStates[0].renderTargetView = CreateRenderTargetView();
	// Create a depth stencil view
	renderStates[0].depthStencilView = CreateDepthStencil(sdl.WIDTH, sdl.HEIGHT);
	// Set a render target view and depth stencil view
	s = SetRenderTargetViewAndDepthStencil(renderStates[0].renderTargetView, renderStates[0].depthStencilView);
	return currentSize++;
}

void Clear(const int& s)
{

	ClearRenderTargetView(renderStates[s].renderTargetView);
	ClearDepthStencilView(renderStates[s].depthStencilView);

	// temporary needed for ui rendering, only set once otherwise
	SetRenderTargetViewAndDepthStencil(renderStates[0].renderTargetView, renderStates[0].depthStencilView);

}

void Render(const size_t& count)
{
	d3d11Data->deviceContext->DrawIndexed((UINT)count, 0, 0);
}

void Present()
{
	d3d11Data->swapChain->Present(0, 0);

}

void DispatchParticles(UINT threadX, UINT threadY, UINT threadZ)
{
	d3d11Data->deviceContext->Dispatch(threadX, threadY, threadZ);
}

void SetSwappedParticles()
{
	//Get particle functions in here some how
}
