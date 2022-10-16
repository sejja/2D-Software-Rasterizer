#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"
using namespace Rasterizer;

namespace GenericVertex {

	static f32 sModel[] =
	{
		0,1, 1,0,0,0.5f,
		0,0, 0,1,0,0.5f,
		1,0, 0,0,1,0.5f,

		0,1, 1,0,0,0.5f,
		1,0, 0,0,1,0.5f,
		1,1, 1,1,0,0.5f,
	};
	static u32 sVtxSize = 6;	// 6 floats for xyrgba
	static u32 sVtxCount = 6;	// 3 vertices
	static u32 sModelSize = sVtxCount * sVtxSize;

	static std::vector<std::pair<std::string, ShaderFunc>> sShaders;
	static u32 sCurrentShader = 0;

	void Init()
	{
		sShaders.resize(3);
		sShaders[0] = { "Color", Shaders::FSFragmentColor };
		sShaders[1] = { "Color Inverse", Shaders::FSFragmentColorInv };
		sShaders[2] = { "Alpha Blend", Shaders::FSAlphaBlend };
	}
	void Menu()
	{
		ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
		ImVec4 red = ImVec4(1, 0, 0, 1);
		for (u32 i = 0; i < sShaders.size(); ++i) {
			ImGui::GetStyle().Colors[ImGuiCol_Text] = sCurrentShader == i ? red : prevColor;
			bool isCurrent = sCurrentShader == i;
			if (ImGui::MenuItem(sShaders[i].first.c_str(), 0, &isCurrent)) sCurrentShader = i;
		}
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
	}
	void Update()
	{

	}
	void Render()
	{
		AEGfxSetClearColor(0xFFFFFFFF);
		AEGfxClearBuffer();
		AEGfxSetBlendMode(AEGfxBlendMode::AE_GFX_BM_NONE);

		// clear to white
		FrameBuffer::Clear(Color(1, 1, 1, 1));

		// allocate memory for transformed model
		f32* vtxCopy = new f32[sModelSize];
		std::memcpy(vtxCopy, sModel, sizeof(sModel));

		// transform model
		auto modelMtx = AEMtx33::Translate(300, 200) * AEMtx33::Scale(200, 200);
		for (u32 i = 0; i < sVtxCount; ++i)
		{
			AEVec2 p = { sModel[i * sVtxSize], sModel[i * sVtxSize + 1] };
			p = modelMtx * p;
			vtxCopy[i * sVtxSize] = p.x;
			vtxCopy[i * sVtxSize + 1] = p.y;
		}

		// draw model
		for(u32 i = 0 ; i < sVtxCount; i+=3)
			DrawTriangleShaded(vtxCopy+(i*sVtxSize), sVtxSize, sShaders[sCurrentShader].second, nullptr);

		// draw again the quad with a small offset if showing alpha blending
		if (sCurrentShader == 2) {
			// transform model
			modelMtx = AEMtx33::Translate(350, 200) * AEMtx33::Scale(200, 200);
			for (u32 i = 0; i < sVtxCount; ++i)
			{
				AEVec2 p = { sModel[i * sVtxSize], sModel[i * sVtxSize + 1] };
				p = modelMtx * p;
				vtxCopy[i * sVtxSize] = p.x;
				vtxCopy[i * sVtxSize + 1] = p.y;
			}

			// draw model
			for (u32 i = 0; i < sVtxCount; i += 3)
				DrawTriangleShaded(vtxCopy + (i * sVtxSize), sVtxSize, sShaders[sCurrentShader].second, nullptr);
		}
		// clean-up
		delete[] vtxCopy;

		// present
		FrameBuffer::Present();
	}
}