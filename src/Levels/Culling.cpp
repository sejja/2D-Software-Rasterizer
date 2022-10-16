//
//  Culling.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 18/07/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "..\Engine\Pipeline2D\Pipeline2D.h"
#include "Common.h"
#include <fstream>
#include <iostream>
#include <ctime>

//Using the Rasterizer to simplify work
using namespace Rasterizer;

namespace Culling {

	static Model QuadModel = Model(Model::ePT_TRIANGLE_LIST);
	static TextureSampler Sampler;
	static void* sShaderData = &Sampler;
	static ShaderFunc sShaderFn = Shaders::FSTextureMap;
	static std::vector<std::pair<Model*, AETransform2D>> Models;
	static Camera2D cam;

	// ------------------------------------------------------------------------
	/*! New Texture
	*
	*   Changes to the desired texture
	*/ // --------------------------------------------------------------------
	void NewTexture() {

		char filename[MAX_PATH];

		OPENFILENAME ofn;
		memset(&filename, 0, sizeof(filename));
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = ghAESysWin;
		ofn.lpstrFilter = "PNG files\0*png\0";
		ofn.lpstrFile = filename;
		ofn.lpstrInitialDir = "data/";
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Choose An Image";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		//If we want to open a new file
		if (GetOpenFileNameA(&ofn)) {

			delete Sampler.GetTexture();
			Sampler.SetTexture(LoadTextureFromFile(filename));

		}
	}

	// ------------------------------------------------------------------------
	/*! Load
	*/ // --------------------------------------------------------------------
	void Load() {

		Sampler.SetTexture(LoadTextureFromFile("data/Horizon.jpg"));
		Sampler.SetBorderMode(TextureSampler::EBorderMode::eBorderColor);
		Sampler.SetBorderColor({ 1, 0, 0, 1 });
		Rasterizer::FrameBuffer::Clear(255, 255, 255, 255);
		cam.SetViewRect(AEVec2(1, 1));
		Rasterizer::SetBlendFunction(Rasterizer::eBF_ADD);
		Rasterizer::SetBlendParameters(Rasterizer::eBP_ONE, Rasterizer::eBP_ZERO);

	}

	// ------------------------------------------------------------------------
	/*! Init
	*/ // --------------------------------------------------------------------
	void Init() {

		VertexFormat format;

		format.resize(3);
		format[0] = { 0, 2, sizeof(VertexDefs::VtxPosTexCol) / sizeof(float), AU_POSITION };
		format[1] = { 2, 2, sizeof(VertexDefs::VtxPosTexCol) / sizeof(float), AU_TEX_COORD };
		format[2] = { 4, 4, sizeof(VertexDefs::VtxPosTexCol) / sizeof(float), AU_COLOR };

		// allocate memory
		QuadModel.Alloc(4, 6, format);
		QuadModel.SetVtxPos(0, { -0.5f,0.5f });
		QuadModel.SetVtxPos(1, { -0.5f,-0.5f });
		QuadModel.SetVtxPos(2, { 0.5f,-0.5f });
		QuadModel.SetVtxPos(3, { 0.5f,0.5f });
		QuadModel.SetVtxUV(0, { 0,1 });
		QuadModel.SetVtxUV(1, { 0,0 });
		QuadModel.SetVtxUV(2, { 1,0 });
		QuadModel.SetVtxUV(3, { 1,1 });
		QuadModel.SetIdx(0, 0);
		QuadModel.SetIdx(1, 1);
		QuadModel.SetIdx(2, 2);
		QuadModel.SetIdx(3, 0);
		QuadModel.SetIdx(4, 2);
		QuadModel.SetIdx(5, 3);
		sShaderData = &Sampler;

		srand(static_cast<unsigned>(time(NULL)));

		//Place 200 Random objects
		for (size_t i = 0; i < 200; i++) {

			Model* cpy = new Model(QuadModel);
			AEVec2 Pos(static_cast<float>(rand() % (750 + 250 + 1) - 250), 
				static_cast<float>(rand() % (750 + 250 + 1) - 250));
			Pos = cam.ComputeInvMatrix() * Pos;
			AETransform2D tr(Pos, { 50, 50 }, 0);
			cpy->Transform(tr.GetMatrix());
			Models.push_back(std::make_pair(cpy, tr));

		}

	}

	static bool culling = false;
	static bool forwardCulling = false;

	// ------------------------------------------------------------------------
	/*! Update
	*/ // --------------------------------------------------------------------
	void Update() {

		CamUpdate(cam);

		//If you have presed the Left Mouse button, you create a new object
		if (AEInputMouseTriggered(AE_MOUSE_LEFT)) {

			Model* cpy = new Model(QuadModel);
			AEVec2 Pos = gAEMousePosition + AEVec2(400, 300);
			Pos = cam.ComputeInvMatrix() * Pos;
			AETransform2D tr(Pos, { 50, 50 }, 0);
			cpy->Transform(tr.GetMatrix());
			Models.push_back(std::make_pair(cpy, tr));

		}
	}

	static std::string PresetName = "Level 1";

	// ------------------------------------------------------------------------
	/*! Render
	*/ // --------------------------------------------------------------------
	void Render() {

		//If we can show the GUI
		if (ImGui::Begin("Demo Options")) {

			ImGui::Checkbox("Culling", &culling);

			//If we are culling
			if (culling)
				ImGui::Checkbox("Forward Culling", &forwardCulling);

			ImGui::Separator();

			//If you want to change the sprite
			if (ImGui::Button("Change Sprite"))
				NewTexture();

			Rasterizer::FrameBuffer::Clear(Color(1, 1, 1, 1));
			ShowFrameworkMenu();

			size_t i = 0;

			//For each model to draw
			for (std::pair<Model*, AETransform2D> x : Models) {

				//If you are culling and forward culling and the object is outside the viewport
				if (culling && forwardCulling && (pow(FrameBuffer::GetWidth(), 2) + pow(FrameBuffer::GetHeight(), 2)) < (pow(cam.GetPosition().x + 400, 2)
					+ pow(cam.GetPosition().y + 300, 2)))
					continue;

				//If you are culling and the object is outside the camera
				if (culling && !AEOrientedRectToOrientedRect(&x.second, &AETransform2D(cam.GetPosition() + AEVec2(400, 300)
					, AEVec2(static_cast<float>(FrameBuffer::GetWidth()), 
					static_cast<float>(FrameBuffer::GetHeight())), cam.GetRotation())))
					continue;

				Model cpy;
				cpy = *x.first;
				cpy.Transform(cam.ComputeMatrix());
				cpy.Draw(sShaderFn, &Sampler);
				i++;

			}

			ImGui::Text("Rendered Objects: %i", i);
			ImGui::End();

		}

		Rasterizer::FrameBuffer::Present();

	}

	// ------------------------------------------------------------------------
	/*! Free
	*/ // --------------------------------------------------------------------
	void Free() {

		//Free every object
		for (std::pair<Model*, AETransform2D> x : Models)
			delete x.first;

	}

	// ------------------------------------------------------------------------
	/*! Unload
	*/ // --------------------------------------------------------------------
	void Unload() {

		delete Sampler.GetTexture();

	}
}