//
//  TileMap.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 13/07/2020
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

namespace TileMap {

	static Model QuadModel = Model(Model::ePT_TRIANGLE_LIST);
	static TextureSampler Sampler;
	static void* sShaderData = &Sampler;
	static ShaderFunc sShaderFn = Shaders::FSTextureMap;
	static AEMtx33 transform = AETransform2D({ 400,300 }, { 50, 50 }, 0).GetMatrix();
	static std::vector<AEVec2> Positions;
	static std::vector<Model*> Models;
	static Model* Models2 = nullptr;
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

		//If we can open the file
		if (GetOpenFileNameA(&ofn)) {

			delete Sampler.GetTexture();
			Sampler.SetTexture(LoadTextureFromFile(filename));

		}
	}

	// ------------------------------------------------------------------------
	/*! Load
	*/ // --------------------------------------------------------------------
	void Load() {

		Sampler.SetTexture(LoadTextureFromFile("data/Tile.jpg"));
		Sampler.SetBorderMode(TextureSampler::EBorderMode::eBorderColor);
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

	static unsigned short selection = 0;

	// ------------------------------------------------------------------------
	/*! Update
	*/ // --------------------------------------------------------------------
	void Update() {

		CamUpdate(cam);
		AEVec2 Pos = cam.ComputeInvMatrix() * (gAEMousePosition + AEVec2(400, 300));

		//Switch by the Optimization preset
		switch (selection) {

		//case Optimization 3
		case 2:
		{

			//If we pressed the Left Mouse Button
			if (AEInputMouseTriggered(AE_MOUSE_LEFT)) {

				//if we have a valid pointer to the model
				if (Models2) {

					std::vector<AEVec2> positions;
					size_t vtxcount = Models2->GetVtxCount();
					const size_t idxcount = Models2->GetIdxCount();

					//Push Back all previous positions
					for (size_t i = 0; i < vtxcount; i++)
						positions.push_back(Models2->GetVtxPos(i));

					delete Models2;
					Models2 = new Model(Model::ePT_TRIANGLE_LIST);

					VertexFormat format;

					format.resize(2);
					format[0] = { 0, 2, 4, AU_POSITION };
					format[1] = { 2, 2, 4, AU_TEX_COORD };

					// allocate memory
					Models2->Alloc(vtxcount + 4, idxcount + 6, format);

					//Set the vertex positions
					for (size_t i = 0; i < vtxcount; i++) 
						Models2->SetVtxPos(i, positions[i]);

					Models2->SetVtxPos(vtxcount, Pos + AEVec2(-25, 25));
					Models2->SetVtxPos(vtxcount + 1, Pos + AEVec2(-25, -25));
					Models2->SetVtxPos(vtxcount + 2, Pos + AEVec2(25, -25));
					Models2->SetVtxPos(vtxcount + 3, Pos + AEVec2(25, 25));
					vtxcount = vtxcount + 4;

					//Set every UV
					for (size_t i = 0; i < vtxcount; i += 4) {

						Models2->SetVtxUV(i, { 0,1 });
						Models2->SetVtxUV(i + 1, { 0,0 });
						Models2->SetVtxUV(i + 2, { 1,0 });
						Models2->SetVtxUV(i + 3, { 1,1 });

					}

					//Set every index
					for (unsigned short i = 0, j = 0; i < vtxcount; i += 4) {

						Models2->SetIdx(j++, i + 0);
						Models2->SetIdx(j++, i + 1);
						Models2->SetIdx(j++, i + 2);
						Models2->SetIdx(j++, i + 0);
						Models2->SetIdx(j++, i + 2);
						Models2->SetIdx(j++, i + 3);

					}

					Positions.push_back(Pos);

					return;
				}

				Models2 = new Model(QuadModel);
				Models2->Transform(AETransform2D(Pos, { 50, 50 }, 0).GetMatrix());
				Positions.push_back(Pos);

			}
			break;

		}

		//case optimization 2
		case 1:
		{
			
			//If we pressed the Left mouse button
			if (AEInputMouseTriggered(AE_MOUSE_LEFT)) {

				Model* cpy = new Model(QuadModel);
				cpy->Transform(AETransform2D(Pos, { 50, 50 }, 0).GetMatrix());
				Models.push_back(cpy);
				Positions.push_back(gAEMousePosition + AEVec2(400, 300));

			}
		}

			break;

		//case no optimization
		default:
			//If we pressed the left mouse button
			if (AEInputMouseTriggered(AE_MOUSE_LEFT))
				Positions.push_back(Pos);

		}
	}

	// ------------------------------------------------------------------------
	/*! Save Tile Map
	*
	*   Save the Tile map onto a file
	*/ // --------------------------------------------------------------------
	void SaveTileMap() {

		OPENFILENAME ofn = { 0 };
		char filename[MAX_PATH] = { 0 };

		ofn.lStructSize = sizeof(ofn);
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST;
		ofn.hInstance = GetModuleHandle(0);
		ofn.hwndOwner = NULL;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFile = filename;
		ofn.nFilterIndex = 1;
		ofn.lpstrFilter = "Tilemap\0*.tm1\0";

		//If we can save the file as desired
		if (GetSaveFileName(&ofn)) {

			std::string buffer(filename);
			std::ofstream output(buffer + ".tm");

			//For each position
			for (AEVec2 x : Positions) {

				output << x.x;
				output << ' ';
				output << x.y;
				output << ' ';

			}

			output.close();

		}
	}

	// ------------------------------------------------------------------------
	/*! Load Tile Map
	*
	*   Load a tilemap from a file
	*/ // --------------------------------------------------------------------
	void LoadTileMap() {

		OPENFILENAME ofn = { 0 };
		char filename[MAX_PATH] = { 0 };

		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFilter = "Tilemap\0*.tm\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrInitialDir = "data/";
		ofn.lpstrTitle = "Select a tilemap";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		//If we could succesfully open a file
		if (GetOpenFileNameA(&ofn)) {

			filename[strlen(filename)] = '\0';
			std::ifstream file(filename);
			float x;
			float y;
			Model* cpy;

			Positions.clear();

			//Switch by the selection
			switch (selection) {

			//case optimization 3
			case 2:
				while (!file.eof()) {
					file >> x, file >> y;
					Positions.push_back(AEVec2(x, y));
				}
				{
					size_t vtxcount = Positions.size() * 4;
					size_t idxcount = static_cast<size_t>(vtxcount * 1.5f);

					//If we had a previous model
					if(Models2)
						delete Models2;

					Models2 = new Model(Model::ePT_TRIANGLE_LIST);

					VertexFormat format;

					format.resize(2);
					format[0] = { 0, 2, 4, AU_POSITION };
					format[1] = { 2, 2, 4, AU_TEX_COORD };

					// allocate memory
					Models2->Alloc(vtxcount, idxcount, format);

					//Set the positions of each vertex
					for (size_t i = 0, j = 0; i < vtxcount; i += 4) {

						Models2->SetVtxPos(i, Positions[j] + AEVec2(-25, 25));
						Models2->SetVtxPos(i + 1, Positions[j] + AEVec2(-25, -25));
						Models2->SetVtxPos(i + 2, Positions[j] + AEVec2(25, -25));
						Models2->SetVtxPos(i + 3, Positions[j++] + AEVec2(25, 25));

					}

					//Set the uvs of every vertex
					for (size_t i = 0; i < vtxcount + 4; i += 4) {

						Models2->SetVtxUV(i, { 0,1 });
						Models2->SetVtxUV(i + 1, { 0,0 });
						Models2->SetVtxUV(i + 2, { 1,0 });
						Models2->SetVtxUV(i + 3, { 1,1 });

					}

					//Set every index
					for (unsigned short i = 0, j = 0; i < vtxcount; i += 4) {

						Models2->SetIdx(j++, i + 0);
						Models2->SetIdx(j++, i + 1);
						Models2->SetIdx(j++, i + 2);
						Models2->SetIdx(j++, i + 0);
						Models2->SetIdx(j++, i + 2);
						Models2->SetIdx(j++, i + 3);

					}
				}
				break;

			//case optimization 2
			case 1:
				
				//while we have file to read
				while (!file.eof()) {

					file >> x, file >> y;
					Positions.push_back(AEVec2(x, y));
					cpy = new Model(QuadModel);
					cpy->Transform(AETransform2D(Positions.back(), { 50, 50 }, 0).GetMatrix());
					Models.push_back(cpy);

				}

				break;

			//case no optimization
			default:
				while (!file.eof()) {

					file >> x, file >> y;
					Positions.push_back(AEVec2(x, y));

				}

				break;
			}

		}
	}

	static std::string PresetName = "Level 1";

	// ------------------------------------------------------------------------
	/*! Render
	*/ // --------------------------------------------------------------------
	void Render() {

		//If we display the Demo GUI
		if (ImGui::Begin("Demo Options")) {

			//Switch by optimization level
			switch (selection)
			{

			//case optimization level 3
			case 2:
				ImGui::Text("Object Count %i", Models2 ? 1 : 0);
				break;

			//case optimization level 2
			case 1:
				ImGui::Text("Object Count %i", Models.size());
				break;

			//case no optimization
			default:
				ImGui::Text("Object Count %i", Positions.size());
				break;
			}

			//If we want to save the map
			if (ImGui::Button("Save"))
				SaveTileMap();

			ImGui::SameLine();

			//If we want to load the map
			if (ImGui::Button("Load"))
				LoadTileMap();

			ImGui::Separator();

			ImGui::Text("Optimization Presets");
			
			//in the Optimization presets combo
			if(ImGui::BeginCombo("", PresetName.c_str())) {

				//Diplay the First optimization Level
				if (ImGui::MenuItem("Level 1")) {

					PresetName = "Level 1";
					selection = 0;
					Positions.clear();
					Models.clear();

					//If there is any instance of unique Models
					if(Models2)
						delete Models2;

					Models2 = nullptr;

				}

				//Diplay the Second optimization Level
				if (ImGui::MenuItem("Level 2")) {

					PresetName = "Level 2";
					selection = 1;
					Positions.clear();
					Models.clear();

					//If there is any instance of unique Models
					if (Models2)
						delete Models2;

					Models2 = nullptr;

				}

				//Diplay the Third optimization Level
				if (ImGui::MenuItem("Level 3")) {

					PresetName = "Level 3";
					selection = 2;
					Positions.clear();
					Models.clear();

					//If there is any instance of unique Models
					if (Models2)
						delete Models2;

					Models2 = nullptr;

				}

				ImGui::EndCombo();

			}

			ImGui::TextColored({1, 0, 0, 1}, "Changing Presets will \nerease current tilmeap");
			ImGui::Separator();

			//If we want to change the tile sprites
			if (ImGui::Button("Change Tile Sprite"))
				NewTexture();

			ImGui::End();

		}

		Rasterizer::FrameBuffer::Clear(Color(1, 1, 1, 1));
		ShowFrameworkMenu();

		const AEMtx33 CamM = cam.ComputeMatrix();

		//Render tilemap by optimization level
		switch (selection) {

		//case 3th level optimization
		case 2:
		{

			//If there is a unique model instance
			if (Models2) {

				Model cpy;
				cpy = *Models2;
				cpy.Transform(CamM);
				cpy.Draw(sShaderFn, &Sampler);

			}
		}

			break;

		//case 2nd level optimization
		case 1:
		{

			//For each model
			for (Model* x : Models) {
				
				Model cpy;
				cpy = *x;
				cpy.Transform(CamM);
				cpy.Draw(sShaderFn, &Sampler);

			}

		}
			break;

		//case no optimization
		default:
		{

			//For each Position
			for (AEVec2 x : Positions) {

				Model cpy = QuadModel;
				cpy.Transform(AETransform2D(x, { 50, 50 }, 0).GetMatrix());
				cpy.Transform(CamM);
				cpy.Draw(sShaderFn, &Sampler);

			}
		}
		}

		Rasterizer::FrameBuffer::Present();
	}

	// ------------------------------------------------------------------------
	/*! Free
	*/ // --------------------------------------------------------------------
	void Free() {

		//Free every model
		for(Model* x : Models)
			delete x;

		//Delete any unqiue model instances
		if (Models2)
			delete Models2;

		Models2 = nullptr;

	}

	// ------------------------------------------------------------------------
	/*! Unload
	*/ // --------------------------------------------------------------------
	void Unload() {

		delete Sampler.GetTexture();

	}
}