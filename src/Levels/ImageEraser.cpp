//
//  ImageEreaser.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 17/07/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "..\Engine\Pipeline2D\Pipeline2D.h"
#include "Common.h"
#include <fstream>

//Using the Rasterizer to simplify work
using namespace Rasterizer;

namespace ImageEraser {

	struct Object {

		Model* model;
		Texture* tex;
		bool Rotating = false;

	};

	static Model QuadModel = Model(Model::ePT_TRIANGLE_LIST);
	static TextureSampler Sampler;
	static void* sShaderData = &Sampler;
	static ShaderFunc sShaderFn = Shaders::FSTextureMap;
	static std::vector<AEVec2> Positions;
	static std::list<std::pair<Object, std::string>> Models;
	static std::pair<Object, std::string>* BB = nullptr;
	static std::vector<Model*> Eraser;
	static Texture* TexE = nullptr;
	static Color c = {1, 1, 1, 1};
	static float brushsize = 50.f;

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

		//If we have choosen a file
		if (GetOpenFileNameA(&ofn))
			Sampler.SetTexture(LoadTextureFromFile(filename));

	}

	// ------------------------------------------------------------------------
	/*! Load
	*/ // --------------------------------------------------------------------
	void Load() {

		Sampler.SetTexture(LoadTextureFromFile("data/Gull.jpg"));
		Sampler.SetBorderMode(TextureSampler::EBorderMode::eBorderColor);
		Sampler.SetBorderColor({ 1, 0, 0, 1 });
		TexE = LoadTextureFromFile("data/Circle.png");
		Rasterizer::FrameBuffer::Clear(255, 255, 255, 255);

	}

	// ------------------------------------------------------------------------
	/*! Init
	*/ // --------------------------------------------------------------------
	void Init() {

		VertexFormat format;

		format.resize(2);
		format[0] = { 0, 2, 4, AU_POSITION };
		format[1] = { 2, 2, 4, AU_TEX_COORD };

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

	}

	// ------------------------------------------------------------------------
	/*! Update
	*/ // --------------------------------------------------------------------
	void Update() {

		 //If you want to place a new Ereaser Position
		 if (AEInputMousePressed(AE_MOUSE_LEFT) && AEInputKeyPressed(VK_CONTROL)) {

			Model* cpy = new Model(QuadModel);
			AEVec2 Pos = gAEMousePosition + AEVec2(400, 300);
			cpy->Transform(AETransform2D(Pos, { brushsize, brushsize }, 0).GetMatrix());
			Eraser.push_back(cpy);

		 //If you want to place a new image
		 } else if (AEInputMouseTriggered(AE_MOUSE_LEFT) && AEInputKeyPressed(AE_KEY_SHIFT)) {

			Model* cpy = new Model(QuadModel);
			AEVec2 Pos = gAEMousePosition + AEVec2(400, 300);
			Object a;

			a.tex = Sampler.GetTexture();
			a.model = cpy;
			Models.push_back(std::make_pair(a, std::string("No Name")));
			Positions.push_back(gAEMousePosition + AEVec2(400, 300));

		}
	}

	static float Rotation = 0;

	// ------------------------------------------------------------------------
	/*! Render
	*/ // --------------------------------------------------------------------
	void Render() {

		Rotation += AEDegToRad(1.f);

		//Start the Demo
		if (ImGui::Begin("Demo Options")) {

			ImGui::Text("Object List");
			ImGui::Separator();

			int j = 0;

			//For each image 
			for (auto i = Models.begin(); i != Models.end(); i++) {

				ImGui::PushID(std::to_string(j++).c_str());

				//IF you select this image
				if (ImGui::Button((*i).second.c_str())) {

					BB = &(*i);

				}
			}

			//If we have an image selected
			if (BB) {

				char buffer[256];

				sprintf_s(buffer, BB->second.c_str());
				ImGui::Text("Name: ");

				//If we have set a new name
				if (ImGui::InputText("## Name ", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue)) {

					BB->second = std::string(buffer);
					BB = nullptr;

				}
			}

			//If we have an image selected
			if (BB) {

				//We can edit it
				if (ImGui::BeginMenu("Edit")) {

					ImGui::Checkbox("Rotate", &BB->first.Rotating);
					ImGui::End();

				}
			}

			ImGui::Text("Background Color");
			ImGui::ColorPicker4("Pick Color", reinterpret_cast<float*>(&c));
			ImGui::Separator();
			ImGui::InputFloat("Brush Scale", &brushsize);
			ImGui::Separator();

			//If tou want to change it's sprite
			if (ImGui::Button("Change Sprite"))
				NewTexture();

			//If you want to reset the scene
			if (ImGui::Button("Reset Scene")) {

				Eraser.clear();
				Models.clear();
				Positions.clear();

			}

			ImGui::End();

		}

		Rasterizer::FrameBuffer::Clear(c);
		ShowFrameworkMenu();
		Rasterizer::SetBlendFunction(Rasterizer::eBF_ADD);
		Rasterizer::SetBlendParameters(Rasterizer::eBP_ONE, Rasterizer::eBP_ZERO);
		Rasterizer::TextureSampler S = Sampler;

		//For each Model
		for (size_t i = 0; i < Positions.size(); i++) {

			auto it = Positions.begin();
			auto it2 = Models.begin();
			Model cpy;

			std::advance(it, i);
			std::advance(it2, i);
			cpy = *(*it2).first.model;
			S.SetTexture((*it2).first.tex);
			cpy.Transform(AETransform2D(*it, { 50, 50 }, 
				(*it2).first.Rotating ? Rotation : 0).GetMatrix());
			cpy.Draw(sShaderFn, &S);

		}

		Rasterizer::SetBlendFunction(Rasterizer::eBF_ADD);
		Rasterizer::SetBlendParameters(Rasterizer::eBP_ZERO, Rasterizer::eBP_INV_SRC_ALPHA);
		S.SetTexture(TexE);

		//For each eraser position
		for (Model* x : Eraser)
			x->Draw(sShaderFn, &S);

		Rasterizer::FrameBuffer::Present();

	}

	// ------------------------------------------------------------------------
	/*! Free
	*/ // --------------------------------------------------------------------
	void Free() {

		//Free every object
		for (std::pair<Object, std::string> x : Models)
			delete x.first.model;

	}

	// ------------------------------------------------------------------------
	/*! Unload
	*/ // --------------------------------------------------------------------
	void Unload() {

		delete Sampler.GetTexture();

	}
}