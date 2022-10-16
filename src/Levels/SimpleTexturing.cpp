#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"
#include <sstream>
using namespace Rasterizer;

namespace SimpleTexturing {

	// vertex data
	VertexDefs::VtxPosTexCol sModel[6] =
	{
		{ {0,1}, {0,1},	{1,0,0,1}},
		{ {0,0}, {0,0}, {0,1,0,1}},
		{ {1,0}, {1,0}, {0,0,1,1}},

		{ {0,1}, {0,1}, {1,0,0,1}},
		{ {1,0}, {1,0}, {0,0,1,1}},
		{ {1,1}, {1,1}, {1,1,0,1}}
	};
	VertexDefs::VtxPosTexCol sModelUV[6] =
	{
		{ {0,1}, {0,2},	{1,0,0,1}},
		{ {0,0}, {0,0}, {0,1,0,1}},
		{ {1,0}, {2,0}, {0,0,1,1}},

		{ {0,1}, {0,2}, {1,0,0,1}},
		{ {1,0}, {2,0}, {0,0,1,1}},
		{ {1,1}, {2,2}, {1,1,0,1}}
	};
	VertexDefs::VtxPosTexCol sModelUVNeg[6] =
	{
		{ {0,1}, {-2,2},	{1,0,0,1}},
		{ {0,0}, {-2,-2}, {0,1,0,1}},
		{ {1,0}, {2,-2}, {0,0,1,1}},

		{ {0,1}, {-2,2}, {1,0,0,1}},
		{ {1,0}, {2,-2}, {0,0,1,1}},
		{ {1,1}, {2,2}, {1,1,0,1}}
	};
	static u32 sVtxSize = sizeof(VertexDefs::VtxPosTexCol)/sizeof(float);
	static u32 sVtxCount = 6;	// 2 triangles
	static u32 sModelSize = sVtxCount * sVtxSize;

	static std::vector<std::pair<std::string, VertexDefs::VtxPosTexCol*>> sModels;
	static u32 sCurrentModel = 0;

	// shaders
	static std::vector<std::pair<std::string, ShaderFunc>> sShaders;
	static u32 sCurrentShader = 0;

	// textures and sampler object
	static TextureSampler sTexSampler;
	static std::vector< std::pair<std::string, Texture*>> sTextures;
	static u32 sCurrentTexture = 0;

	static const char * sBorderModeStr[] = {
		"Border Color", "Repeat", "Clamp", "Mirror Horizontal", "Mirror Vertical", "Mirror Both"};
	static u32 sCurrentBorderMode = 0;

	static Shaders::TexTransformShaderData sTexTransformData;
	static AEVec2 sTexTranslate, sTexScale;


	void Init()
	{
		// Initialize textures
		sTextures.resize(3);
		sTextures[0] = { "brick",  LoadTextureFromFile("data/brick.png") };
		sTextures[1] = { "run", LoadTextureFromFile("data/run.png") };
		sTextures[2] = { "face", LoadTextureFromFile("data/face.png") };
		sTexSampler.SetTexture(sTextures[0].second);

		// Initialize shaders
		sShaders.resize(4);
		sShaders[0] = { "Texture Map", Shaders::FSTextureMap};
		sShaders[1] = { "Texture Decal", Shaders::FSTextureDecal};
		sShaders[2] = { "Texture Modulation", Shaders::FSTextureVSColorMod };
		sShaders[3] = { "Texture Transform", Shaders::FSTextureTransform };

		// Initialize Models
		sModels.resize(3);
		sModels[0] = { "UV in [0,1]", sModel };
		sModels[1] = { "UV in [0,2]", sModelUV };
		sModels[2] = { "UV in [-2,2]", sModelUVNeg };

		// initialize transform data
		sTexTransformData.pSampler = &sTexSampler;
		sTexTransformData.texTransform = AEMtx33::Identity();
		sTexTranslate = { 0,0 };
		sTexScale = { 1,1 };
	}
	void Menu()
	{
		ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
		ImVec4 red = ImVec4(1, 0, 0, 1);

		if (ImGui::BeginMenu("Shaders")) {
			for (u32 i = 0; i < sShaders.size(); ++i) {
				ImGui::GetStyle().Colors[ImGuiCol_Text] = sCurrentShader == i ? red : prevColor;
				bool isCurrent = sCurrentShader == i;
				if (ImGui::MenuItem(sShaders[i].first.c_str(), "V", &isCurrent)) sCurrentShader = i;
			}
			ImGui::EndMenu();
			ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
		}

		if (ImGui::BeginMenu("Textures")) {
			for (u32 i = 0; i < sTextures.size(); ++i)
			{
				ImGui::GetStyle().Colors[ImGuiCol_Text] = sCurrentTexture == i ? red : prevColor;
				bool isCurrent = sCurrentTexture == i;
				if (ImGui::MenuItem(sTextures[i].first.c_str(), "N", &isCurrent)) sCurrentTexture = i;
			}
			ImGui::EndMenu();
			ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
		}
		if (ImGui::BeginMenu("Models")) {
			for (u32 i = 0; i < sModels.size(); ++i)
			{
				ImGui::GetStyle().Colors[ImGuiCol_Text] = sCurrentModel == i ? red : prevColor;
				bool isCurrent = sCurrentModel == i;
				if (ImGui::MenuItem(sModels[i].first.c_str(), "M", &isCurrent)) sCurrentModel = i;
			}
			ImGui::EndMenu();
			ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
		}
		if (ImGui::BeginMenu("Sampler")) {
			if (ImGui::BeginCombo("Border Modes", sBorderModeStr[sCurrentBorderMode])) 
			{
				for (u32 i = 0; i < TextureSampler::eCOUNT; ++i) {
					ImGui::GetStyle().Colors[ImGuiCol_Text] = sCurrentBorderMode == i ? red : prevColor;
					bool isCurrent = sCurrentBorderMode == i;
					if (ImGui::MenuItem(sBorderModeStr[i], "B", &isCurrent)) {
						sCurrentBorderMode = i;
					}
				}
				ImGui::EndCombo();
				ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
			}
			Color bc = sTexSampler.GetBorderColor();
			if (ImGui::ColorEdit4("Border Color", bc.v))
				sTexSampler.SetBorderColor(bc);
			ImGui::EndMenu();
		}
	}
	void Update()
	{
		if (AEInputKeyTriggered('V'))
			sCurrentShader = ++sCurrentShader % sShaders.size();
		if (AEInputKeyTriggered('N'))
			sCurrentTexture = ++sCurrentTexture % sTextures.size();
		if (AEInputKeyTriggered('M'))
			sCurrentModel = ++sCurrentModel % sModels.size();
		if (AEInputKeyTriggered('B'))
			sCurrentBorderMode = ++sCurrentBorderMode % TextureSampler::eCOUNT;

		

		if (sCurrentShader == 3 && ImGui::Begin("Texture Transform")) {
			ImGui::Text("Texture Transform"); ImGui::Separator();
			bool changed = false;
			ImGui::PushID(123);
			ImGui::Text("Scale: "); ImGui::Indent();
			changed |= ImGui::DragFloat2("", sTexScale.v, 0.1f);
			ImGui::Unindent();
			ImGui::PopID();

			ImGui::PushID(124);
			ImGui::Text("Translate: "); ImGui::Indent();
			changed |= ImGui::DragFloat2("", sTexTranslate.v, 0.1f);
			ImGui::Unindent();
			ImGui::PopID();

			// update matrix if necessary
			if (changed)sTexTransformData.texTransform = AEMtx33::Translate(sTexTranslate.x, sTexTranslate.y) *
				AEMtx33::Scale(sTexScale.x, sTexScale.y);
			ImGui::End();
		}
	}
	void Render()
	{
		AEGfxSetClearColor(0xFFFFFFFF);
		AEGfxClearBuffer();
		AEGfxSetBlendMode(AEGfxBlendMode::AE_GFX_BM_NONE);

		// allocate memory for transformed model
		VertexDefs::VtxPosTexCol* vtxCopy = new VertexDefs::VtxPosTexCol[sVtxCount];
		std::memcpy(vtxCopy, sModels[sCurrentModel].second, sModelSize*sizeof(float));

		// transform model
		auto modelMtx = AEMtx33::Translate(300, 200) * AEMtx33::Scale(200, 200);
		for (u32 i = 0; i < sVtxCount; ++i)
		{
			AEVec2 p = sModels[sCurrentModel].second[i].mPosition;
			p = modelMtx * p;
			vtxCopy[i].mPosition = p;
		}

		// clear to white
		FrameBuffer::Clear(Color(1, 1, 1, 1));

		// configure sampler
		sTexSampler.SetTexture(sTextures[sCurrentTexture].second);
		sTexSampler.SetBorderMode((TextureSampler::EBorderMode)sCurrentBorderMode);

		// draw model
		for (u32 i = 0; i < sVtxCount; i += 3) {
			if(sCurrentShader == 3)
				DrawTriangleShaded((f32*)(vtxCopy + i), sVtxSize, sShaders[sCurrentShader].second, &sTexTransformData);
			else
				DrawTriangleShaded((f32*)(vtxCopy + i), sVtxSize, sShaders[sCurrentShader].second, &sTexSampler);
		}

		delete[] vtxCopy;

		// present
		FrameBuffer::Present();

		std::stringstream ss;
		ss << "Model = " << sModels[sCurrentModel].first << "\n";
		ss << "Shader = " << sShaders[sCurrentShader].first << "\n";
		ss << "Border Mode = " << sBorderModeStr[sCurrentBorderMode] << "\n";
		ss << "Texture = " << sTextures[sCurrentTexture].first << "\n";
		AEGfxPrint(5, 25, 0xFF000000, ss.str().c_str());
	}
	void Free()
	{
		while(sTextures.size())
		{
			delete sTextures.back().second;
			sTextures.pop_back();
		}
	}
}