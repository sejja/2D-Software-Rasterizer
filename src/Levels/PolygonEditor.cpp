//
//  PolygonEditor.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 16/07/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "..\Engine\Pipeline2D\Pipeline2D.h"
#include "Common.h"
#include <list>

//Using the Rasterizer to simplify work
using namespace Rasterizer;

namespace PolygonEditor {

	static TextureSampler Sampler;
	static void* sShaderData = &Sampler;
	static ShaderFunc sShaderFn = Shaders::FSTextureMap;
	static std::list<AEVec2> Positions;
	static std::list<std::pair<Model*, std::string>> Models;
	static bool CreatingPolygon = false;
	static std::pair<Model*, std::string>* BB = nullptr;

	// ------------------------------------------------------------------------
	/*! Is Model Concave
	*
	*   Is a model Concave?
	*/ // --------------------------------------------------------------------
	bool IsModelConcave(Model& model) {

		//If the model is less than 3 units 
		if(model.GetVtxCount() <= 3)
			return false;

		//Go through every vertex
		for (size_t i = 0; i < model.GetVtxCount(); i++) {

			unsigned i1, i2;

			//If we are int the last pair of vertex
			if (i <= model.GetVtxCount() - 2) {

				i1 = i + 1;
				i2 = (i + 2) == model.GetVtxCount() ? 0 : i + 2;

			//else
			} else {

				i1 = 0;
				i2 = 1;

			}

			//Get the Vertex Position
			const AEVec2& p = model.GetVtxPos(i1);

			//If we have more than 90 degrees
			if((model.GetVtxPos(i2) - p).CrossMag(model.GetVtxPos(i) - p) < 0.f)
				return true;

		}

		return false;

	}

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

		if (GetOpenFileNameA(&ofn)) {

			delete Sampler.GetTexture();
			Sampler.SetTexture(LoadTextureFromFile(filename));

		}
	}

	// ------------------------------------------------------------------------
	/*! Load
	*/ // --------------------------------------------------------------------
	void Load() {

		Rasterizer::SetBlendFunction(Rasterizer::eBF_ADD);
		Rasterizer::SetBlendParameters(Rasterizer::eBP_ONE, Rasterizer::eBP_ZERO);
		Sampler.SetTexture(LoadTextureFromFile("data/Adachi.png"));
		Sampler.SetBorderMode(TextureSampler::EBorderMode::eBorderColor);
		Rasterizer::FrameBuffer::Clear(255, 255, 255, 255);
		SetDrawLineMethod(EDrawLineMethod::eDL_BRESENHAM);

	}

	// ------------------------------------------------------------------------
	/*! Init
	*/ // --------------------------------------------------------------------
	void Init() {

		sShaderData = &Sampler;
		Positions.clear();

	}

	struct AABBlisting {

		float downkey = FLT_MAX;
		float upkey = 0;
		float leftkey = FLT_MAX;
		float rightkey = 0;
		void RegisterVertex(const AEVec2& a);

	};

	// ------------------------------------------------------------------------
	/*! Register Vertex
	*
	*   Updates the AABB boundaries given the new vertex in model
	*/ // --------------------------------------------------------------------
	void AABBlisting::RegisterVertex(const AEVec2& a) {

		//If we have a new low value
		if(a.y < downkey)
			downkey = a.y;

		//If we have a new up value
		if(a.y > upkey)
			upkey = a.y;

		//If we have a new left value
		if(a.x < leftkey)
			leftkey = a.x;

		//If we have a new right value
		if(a.x > rightkey)
			rightkey = a.x;

	}

	// ------------------------------------------------------------------------
	/*! Update
	*/ // --------------------------------------------------------------------
	void Update() {

		const AEVec2 Pos = gAEMousePosition + AEVec2(400, 300);

		//IF we press the left button and we are creating a polygon
		if (AEInputMouseTriggered(AE_MOUSE_LEFT) && CreatingPolygon)
			Positions.push_back(Pos);

			//If we press the right button
		if (AEInputMouseTriggered(AE_MOUSE_RIGHT) && CreatingPolygon) {

			Model* cpy = new Model(Model::ePT_TRIANGLE_LIST);
			AABBlisting listing;
			VertexFormat format;

			format.resize(2);
			format[0] = { 0, 2, 4, AU_POSITION };
			format[1] = { 2, 2, 4, AU_TEX_COORD };

			// allocate memory
			cpy->Alloc(Positions.size(), Positions.size(), format);

			//Create each Vertex within the model
			for (size_t i = 0; i < Positions.size(); i++) {

				auto it = Positions.begin();
				std::advance(it, i);

				cpy->SetVtxPos(i, *it);
				memcpy(cpy->GetVtxPtr(i), &(*it), sizeof(AEVec2));
				cpy->SetIdx(i, static_cast<unsigned short>(i));
				listing.RegisterVertex(*it);
				

			}

			//Compute the UVs
			for (size_t i = 0; i < Positions.size(); i++)
				cpy->SetVtxUV(i, { (cpy->GetVtxPos(i).x - listing.leftkey) / (listing.rightkey - listing.leftkey),
					(cpy->GetVtxPos(i).y - listing.downkey) / (listing.upkey - listing.downkey) });

			Models.push_back(std::make_pair(cpy, "No Name"));
			Positions.clear();
			CreatingPolygon = false;

		}
	}

	using Att_Tranf = std::vector<float>;

	// ------------------------------------------------------------------------
	/*! Simple Triangulation
	*
	*   Triangulates a list of Vertices
	*/ // --------------------------------------------------------------------
	std::vector<Att_Tranf> SimpleTriangulation(const std::vector<Att_Tranf>& in) {

		//Create an output list
		std::vector<Att_Tranf> out_list_;

		//Retrieve the traversal anchor (which is the size of the passed list
		//	minus one)
		size_t anchor_ = in.size() - 1;

		out_list_.reserve(in.size() * 3);

		//Traversal
		for (size_t i = 1; i < anchor_; i++) {

			//Push back the first Vertex
			out_list_.push_back(in[0]);

			//Push back the iteration-matched Vertex
			out_list_.push_back(in[i]);

			//Push back the next vertex
			out_list_.push_back(in[i + 1]);

		}

		//Return the output list
		return out_list_;

	}

#pragma warning(suppress : 4996)

	// ------------------------------------------------------------------------
	/*! Render
	*/ // --------------------------------------------------------------------
	void Render() {

		Rasterizer::FrameBuffer::Clear(Color(1, 1, 1, 1));
		ShowFrameworkMenu();

		//Bring the Menu options GUI
		if (ImGui::Begin("Demo Options")) {

			//If we are creating a polygon
			if (CreatingPolygon)
				ImGui::TextColored({1, 0 ,0 , 1}, "CREATING POLYGON");

			//If we can display the menu
			if (ImGui::BeginChild("Polygons")) {

				ImGui::Text("Polygon List");
				ImGui::Separator();

				int k = 0;

				//Display the list of polygons
				for (auto i = Models.begin(); i != Models.end(); i++) {

					ImGui::PushID(std::_Integral_to_string<char>(k++).c_str());

					//Select the choosed polygon
					if (ImGui::Button((*i).second.c_str()))
						BB = &(*i);

				}

				//if we have selected a polygon
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

				//If we have selected a polygon
				if (BB) {

					//If we want to edit it
					if (ImGui::BeginMenu("Edit")) {

						//IF we want to delete it
						if (ImGui::MenuItem("Delete")) {

							Models.remove(*BB);
							BB = nullptr;

						}

						ImGui::End();

					}
				}

				//If we have selected a polygon
				if (BB) {

					//For eacg vertex in the polygon
					for (size_t i = 0; i < BB->first->GetVtxCount(); i++) {

						char BUFF[MAX_PATH];
						AEVec2 Temp_ = BB->first->GetVtxPos(i);

						snprintf(BUFF, MAX_PATH, "Vertex nº: %u", i);

						//If we change the position of a vertex
						if (ImGui::InputFloat2(BUFF, Temp_.v, 2,
							ImGuiInputTextFlags_EnterReturnsTrue))
							BB->first->SetVtxPos(i, Temp_);

					}
				}

				ImGui::Separator();

				//If you want to create a polygon
				if (ImGui::Button("New Polygon"))
					CreatingPolygon = true;

				ImGui::EndChild();

				//If you want to change the sprite
				if (ImGui::Button("New Image"))
					NewTexture();
			}

			ImGui::End();

		}

		//If we have less than 2 Positions in the New polygon
		if (Positions.size() >= 2) {

			auto pos = Positions.size();

			//FOr each position
			for (size_t i = 0; i < Positions.size(); i++) {

				//If we are not in the last position
				if (i != Positions.size() - 1) {
					
					auto it = Positions.begin();
					std::advance(it, i);

					auto it2 = Positions.begin();
					std::advance(it2, i + 1);

					DrawLine(*it, *(it2), {0, 0, 0, 1});

				//else
				} else {

					auto it = Positions.begin();
					std::advance(it, i);

					DrawLine(*it, *Positions.begin(), { 0, 0, 0, 1 });

				}
			}
		} 

		//For each model that we have
		for (std::pair<Model*, std::string> x : Models) {

			Model cpy;
			cpy = *(x.first);
			std::vector<Att_Tranf> a;

			//If the model is not concave
			if (!IsModelConcave(cpy)) {

				a.resize(x.first->GetVtxCount());

				//Copy the Vertex Positions inside the array
				for (size_t i = 0; i < x.first->GetVtxCount(); i++) {

					a[i].resize(4);
					memcpy(a[i].data(), cpy.GetVtxPtr(i), sizeof(AEVec2) * 2);

				}

				a = SimpleTriangulation(a);
				cpy.Alloc(a.size(), a.size(), cpy.GetFormat());

				//Remake the new Model
				for (size_t i = 0; i < cpy.GetVtxCount(); i++) {

					cpy.SetVtxPos(i, *reinterpret_cast<AEVec2*>(a[i].data()));
					cpy.SetVtxUV(i, *reinterpret_cast<AEVec2*>(a[i].data() + 2));
					cpy.SetIdx(i, static_cast<unsigned short>(i));

				}

				cpy.Draw(sShaderFn, &Sampler);


			} else {

				size_t j = 1;
				Model temp;

				a.resize(4);
				a[0].resize(4);
				a[1].resize(4);
				a[2].resize(4);
				a[3].resize(4);

				//For each triangle in the polygon
				for(; j < cpy.GetIdxCount() - 1; j += 2) {
				
					memcpy(a[0].data(), cpy.GetVtxPtr(0), sizeof(AEVec2) * 2);
					memcpy(a[1].data(), cpy.GetVtxPtr(j), sizeof(AEVec2) * 2);
					memcpy(a[2].data(), cpy.GetVtxPtr(j + 1), sizeof(AEVec2) * 2);
					memcpy(a[3].data(), cpy.GetVtxPtr(j + 2), sizeof(AEVec2) * 2);
					a = SimpleTriangulation(a);
					temp.Alloc(a.size(), a.size(), cpy.GetFormat());

					//Set every vertex to draw (separately)
					for (size_t i = 0; i < temp.GetVtxCount(); i++) {

						temp.SetVtxPos(i, *reinterpret_cast<AEVec2*>(a[i].data()));
						temp.SetVtxUV(i, *reinterpret_cast<AEVec2*>(a[i].data() + 2));
						temp.SetIdx(i, static_cast<unsigned short>(i));

					}

					temp.Draw(sShaderFn, &Sampler);

				}

				memcpy(a[0].data(), cpy.GetVtxPtr(0), sizeof(AEVec2) * 2);
				memcpy(a[1].data(), cpy.GetVtxPtr(cpy.GetIdxCount() - 3), sizeof(AEVec2) * 2);
				memcpy(a[2].data(), cpy.GetVtxPtr(cpy.GetIdxCount() - 2), sizeof(AEVec2) * 2);
				memcpy(a[3].data(), cpy.GetVtxPtr(cpy.GetIdxCount() - 1), sizeof(AEVec2) * 2);
				a = SimpleTriangulation(a);
				temp.Alloc(a.size(), a.size(), cpy.GetFormat());

				//Set every vertex to draw (separately)
				for (size_t i = 0; i < temp.GetVtxCount(); i++) {

					temp.SetVtxPos(i, *reinterpret_cast<AEVec2*>(a[i].data()));
					temp.SetVtxUV(i, *reinterpret_cast<AEVec2*>(a[i].data() + 2));
					temp.SetIdx(i, static_cast<unsigned short>(i));

				}

				temp.Draw(sShaderFn, &Sampler);

			}
		}

		Rasterizer::FrameBuffer::Present();
	}

	// ------------------------------------------------------------------------
	/*! Free
	*/ // --------------------------------------------------------------------
	void Free() {

		//Delete every model
		for (std::pair<Model*, std::string> x : Models)
			delete x.first;

	}

	// ------------------------------------------------------------------------
	/*! Unload
	*/ // --------------------------------------------------------------------
	void Unload() {

		delete Sampler.GetTexture();

	}
}