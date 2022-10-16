// ----------------------------------------------------------------------------
// File: Level 3.cpp
// Project: CS200 Assignment 4
// Purpose: 
// Author: Thomas Komair
// Copyright DigiPen Institute of Technology, 2015. All rights reserved
// ----------------------------------------------------------------------------

#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"
using namespace Rasterizer;


namespace SimpleBlending
{
	// Defaults
	const u32					gTextureCount = 5;
	const u32					gQuadCount = 2;
	const u32					vertexSize = sizeof(Rasterizer::VertexDefs::VtxPosTexCol) / sizeof(float);

	Rasterizer::Texture* gpTexture[gTextureCount] = { NULL };
	f32* gQuadModels[gQuadCount] = { NULL };
	Rasterizer::TextureSampler	gTextureSampler;

	// Blending parameters	
	Rasterizer::EBlendFunction	gCurrentBlendFunction = Rasterizer::eBF_ADD;
	Rasterizer::EBlendParam		gCurrentSourceBlendParam = Rasterizer::eBP_ONE;
	Rasterizer::EBlendParam		gCurrentDestBlendParam = Rasterizer::eBP_ZERO;

	u32							gDemo = 0; // 0 -> alpha blending, 1 -> additive blending, 2 -> modulation blending. 
	void RenderDemo0();
	void RenderDemo1();
	void RenderDemo2();

	// ----------------------------------------------------------------------------
	// Helper functions
	void Menu()
	{
		bool isSelected; 

		isSelected = gDemo == 0;
		if (ImGui::MenuItem("Alpha Blending Demo", "N", &isSelected)) gDemo = 0;

		isSelected = gDemo == 1;
		if (ImGui::MenuItem("Additive Blending Demo", "N", &isSelected)) gDemo = 1;

		isSelected = gDemo == 2;
		if (ImGui::MenuItem("Modulation Blending Demo", "N", &isSelected)) gDemo = 2;
	}

	// ----------------------------------------------------------------------------
	// Game State
	void Load()
	{
		// Load textures from file
		gpTexture[0] = Rasterizer::LoadTextureFromFile("data\\twisted_idle_full.png");
		gpTexture[1] = Rasterizer::LoadTextureFromFile("data\\twisted_idle_transluscent.png");
		gpTexture[2] = Rasterizer::LoadTextureFromFile("data\\green_alpha_10.png");
		gpTexture[3] = Rasterizer::LoadTextureFromFile("data\\twisted_idle_full_mask.png");

		// allocate render texture manually to be the size of the first texture ("twisted_idle_full.png");
		gpTexture[4] = new Rasterizer::Texture();
		gpTexture[4]->mWidth = gpTexture[0]->mWidth;
		gpTexture[4]->mHeight = gpTexture[0]->mHeight;
		u32 texDataSize = gpTexture[4]->mWidth * gpTexture[4]->mHeight * 4;
		gpTexture[4]->mPixels = new u8[texDataSize];// width * height * 4 components per pixel.
		// clear pixels
		std::memset(gpTexture[4]->mPixels, 0, sizeof(u8) * texDataSize);

		// create a quad with simple pos Color
		CreatePosColTexQuad(gQuadModels[0], 0.50f, AEVec2(0, 0));
		CreatePosColTexQuad(gQuadModels[1], 0.50f, AEVec2(0, 0));
	}
	void Init()
	{
		gDemo = 0;
		gTextureSampler.SetBorderMode(Rasterizer::TextureSampler::eClamp);
	}
	void Update()
	{
		if (AEInputKeyTriggered('N'))
		{
			// cycle through demos
			gDemo = ++gDemo > 2 ? 0 : gDemo;
		}

		

	}
	void Render()
	{
		switch (gDemo)
		{
		case 0: RenderDemo0(); break;
		case 1: RenderDemo1(); break;
		case 2: RenderDemo2(); break;
		}

		// NOTE: Enable Alpha blending in AEngine. This is so that 
		// when we present our own frame buffer. It takes into account
		// the alpha color as well when drawing it.
		AEGfxSetClearColor(0);
		AEGfxClearBuffer();
		AEGfxSetBlendMode(AEGfxBlendMode::AE_GFX_BM_BLEND);

		// Send to display
		Rasterizer::FrameBuffer::Present();

	}
	void Unload()
	{
		for (u32 i = 0; i < gQuadCount; ++i) {
			delete[] gQuadModels[i]; gQuadModels[i] = 0;
		}
		for (u32 i = 0; i < gTextureCount; ++i) {
			delete gpTexture[i]; gpTexture[i] = 0;
		}
	}

	// ----------------------------------------------------------------------------
	void RenderDemo0()
	{
		// Set Blend Mode
		gCurrentBlendFunction = Rasterizer::eBF_ADD;
		gCurrentSourceBlendParam = Rasterizer::eBP_SRC_ALPHA;
		gCurrentDestBlendParam = Rasterizer::eBP_INV_SRC_ALPHA;
		Rasterizer::SetBlendFunction(gCurrentBlendFunction);
		Rasterizer::SetBlendParameters(gCurrentSourceBlendParam, gCurrentDestBlendParam);

		// clear frame buffer to white
		Rasterizer::FrameBuffer::Clear(Rasterizer::Color(1, 1, 1, 1));

		// cast the quads to vertex pos cols for easier processing
		Rasterizer::VertexDefs::VtxPosTexCol* original_quad = (Rasterizer::VertexDefs::VtxPosTexCol*)  gQuadModels[0];
		Rasterizer::VertexDefs::VtxPosTexCol* transformed_quad = (Rasterizer::VertexDefs::VtxPosTexCol*)  gQuadModels[1];

		// transform quad to have the object at the center
		AEVec2 texSize = { (f32)gpTexture[0]->mWidth, (f32)gpTexture[0]->mHeight };
		AETransform2D tr = { { 300, 300 }, texSize * 2.f, 0.0f };
		AEMtx33 trMat = tr.GetMatrix();
		for (int v = 0; v < 6; ++v)
			transformed_quad[v].mPosition = trMat * original_quad[v].mPosition;

		// Set the texture sampler texture
		gTextureSampler.SetTexture(gpTexture[0]);
		// transform the model and store in the second model
		DrawModel(gQuadModels[1], 6, vertexSize, Rasterizer::Shaders::FSTextureMap, &gTextureSampler);

		// draw with the other texture
		gTextureSampler.SetTexture(gpTexture[1]);
		// start at 500
		tr.mPosition.x = 500;
		for (int i = 0; i < 6; ++i)
		{
			// move by 10 pixels to the right. 
			tr.mPosition.x += 30;

			// get matrix and transform quad.
			trMat = tr.GetMatrix();
			for (int v = 0; v < 6; ++v)
				transformed_quad[v].mPosition = trMat * original_quad[v].mPosition;

			// transform the model and store in the second model
			DrawModel(gQuadModels[1], 6, vertexSize, Rasterizer::Shaders::FSTextureMap, &gTextureSampler);
		}

	}
	void RenderDemo1() {

		// Set Blend Mode
		gCurrentBlendFunction = Rasterizer::eBF_ADD;
		gCurrentSourceBlendParam = Rasterizer::eBP_ONE;
		gCurrentDestBlendParam = Rasterizer::eBP_ONE;
		Rasterizer::SetBlendFunction(gCurrentBlendFunction);
		Rasterizer::SetBlendParameters(gCurrentSourceBlendParam, gCurrentDestBlendParam);

		// clear frame buffer to black
		Rasterizer::FrameBuffer::Clear(Rasterizer::Color(0, 0, 0, 0));

		// set texture in sampler
		gTextureSampler.SetTexture(gpTexture[2]);

		// transform quad to have the object at the center
		//AEVec2 texSize = { (f32)gpTexture[2]->mWidth, (f32)gpTexture[2]->mHeight };
		AEVec2 texSize = { 800 / 8, 600 };
		AETransform2D tr = { { texSize.x * 0.5f, 300 }, texSize, 0.0f };
		AEMtx33 trMat = tr.GetMatrix();

		// cast the quads to vertex pos cols for easier processing
		Rasterizer::VertexDefs::VtxPosTexCol* original_quad = (Rasterizer::VertexDefs::VtxPosTexCol*)  gQuadModels[0];
		Rasterizer::VertexDefs::VtxPosTexCol* transformed_quad = (Rasterizer::VertexDefs::VtxPosTexCol*)  gQuadModels[1];

		// Draw an band with the quad, with high density of objects at the beginning, and low density towards the end
		for (int i = 1; i <= 8; ++i)
		{
			// get matrix and transform quad.
			trMat = tr.GetMatrix();
			for (int v = 0; v < 6; ++v)
				transformed_quad[v].mPosition = trMat * original_quad[v].mPosition;

			int end = 16 / i;
			for (int obj = 0; obj < end; ++obj)
			{
				// transform the model and store in the second model
				DrawModel(gQuadModels[1], 6, vertexSize, Rasterizer::Shaders::FSTextureMap, &gTextureSampler);
			}

			// move diagonally up-right
			tr.mPosition.x += texSize.x * 1.01f;
		}

	}
	void RenderDemo2() {
		RenderDemo1();

		// cast the quads to vertex pos cols for easier processing
		Rasterizer::VertexDefs::VtxPosTexCol* original_quad = (Rasterizer::VertexDefs::VtxPosTexCol*)  gQuadModels[0];
		Rasterizer::VertexDefs::VtxPosTexCol* transformed_quad = (Rasterizer::VertexDefs::VtxPosTexCol*)  gQuadModels[1];

		// transform quad to matcht the render texture size
		AEVec2 texSize = { (f32)gpTexture[4]->mWidth, (f32)gpTexture[4]->mHeight };
		AETransform2D tr = { { texSize.x / 2.0f, texSize.y / 2.0f }, texSize, 0.0f };
		AEMtx33 trMat = tr.GetMatrix();
		for (int v = 0; v < 6; ++v)
			transformed_quad[v].mPosition = trMat * original_quad[v].mPosition;

		// set render texture 
		Rasterizer::FrameBuffer::SetRenderTarget(gpTexture[4]);

		// Pass 1: render base texture with replace blending mode
		{
			// Set blend mode
			Rasterizer::SetBlendFunction(Rasterizer::eBF_ADD);
			Rasterizer::SetBlendParameters(Rasterizer::eBP_ONE, Rasterizer::eBP_ZERO);
			gTextureSampler.SetTexture(gpTexture[0]);
			DrawModel(gQuadModels[1], 6, vertexSize, Rasterizer::Shaders::FSTextureMap, &gTextureSampler);
		}

		// Pass 2: render mask texture with modulation blending
		{
			// Set blend mode
			Rasterizer::SetBlendFunction(Rasterizer::eBF_ADD);
			Rasterizer::SetBlendParameters(Rasterizer::eBP_DST_COLOR, Rasterizer::eBP_ZERO);
			gTextureSampler.SetTexture(gpTexture[3]);
			DrawModel(gQuadModels[1], 6, vertexSize, Rasterizer::Shaders::FSTextureMap, &gTextureSampler);
		}

		// Pass 3: Render resulting texture in the center of the screen
		// Use alpha blending
		{
			// reset render target to frame buffer
			Rasterizer::FrameBuffer::SetRenderTarget(NULL);

			// update quad transform
			tr.mPosition = AEVec2(200, 300);
			tr.mScale *= 2.0f;
			AEMtx33 trMat = tr.GetMatrix();
			for (int v = 0; v < 6; ++v)
				transformed_quad[v].mPosition = trMat * original_quad[v].mPosition;

			// set blend mode
			Rasterizer::SetBlendFunction(Rasterizer::eBF_ADD);
			Rasterizer::SetBlendParameters(Rasterizer::eBP_SRC_ALPHA, Rasterizer::eBP_INV_SRC_ALPHA);

			// set sample ltexture to the previous render target (result of base + mask)
			gTextureSampler.SetTexture(gpTexture[4]);

			// render
			DrawModel(gQuadModels[1], 6, vertexSize, Rasterizer::Shaders::FSTextureMap, &gTextureSampler);
		}
	}

}//namespace SimpleBlending