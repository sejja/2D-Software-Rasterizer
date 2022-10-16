//
//  TextureTransformation.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 12/07/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "..\Engine\Pipeline2D\Pipeline2D.h"
#include "Common.h"

//Using the Rasterizer to simplify work
using namespace Rasterizer;

namespace TextureTransformation {

	static Model QuadModel = Model(Model::ePT_TRIANGLE_LIST);
	static TextureSampler Sampler;
	static void* sShaderData = &Sampler;
	static ShaderFunc sShaderFn = Shaders::FSTextureMap;
	static Shaders::TexTransformShaderData sTexTransformData;
	static int Horizontal = 1;
	static int Vertical = 1;
	static AEMtx33 TextureTransform;
	static AEVec2 Scale = {4, 4};
	static float topcrop = 1.f;
	static float bottomcrop = 0.f;
	static float rightcrop = 1.f;
	static float leftcrop = 0.f;
	static AEVec2 firstpos;

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

		//If we could open the file
		if (GetOpenFileNameA(&ofn)) {

			delete Sampler.GetTexture();
			Sampler.SetTexture(LoadTextureFromFile(filename));
			Sampler.SetBorderMode(TextureSampler::EBorderMode::eRepeat);

		}
	}

	// ------------------------------------------------------------------------
	/*! Load
	*/ // --------------------------------------------------------------------
	void Load() {

		Rasterizer::SetBlendFunction(Rasterizer::eBF_ADD);
		Rasterizer::SetBlendParameters(Rasterizer::eBP_ONE, Rasterizer::eBP_ZERO);
		Sampler.SetTexture(LoadTextureFromFile("data/Koipond.png"));
		Sampler.SetBorderMode(TextureSampler::EBorderMode::eRepeat);
		sTexTransformData.pSampler = &Sampler;

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
	/*! Compute Transformation of Crop
	*
	*   Computes the Texture Transformation for cropping
	*/ // --------------------------------------------------------------------
	void ComputeTransformCrop() {

		const AEMtx33 a = AEMtx33::Translate(leftcrop, bottomcrop) * 
			AEMtx33::Scale(static_cast<float>(rightcrop - leftcrop), static_cast<float>(topcrop - bottomcrop));

		QuadModel.SetVtxUV(0, a * AEVec2(0, 1));
		QuadModel.SetVtxUV(1, a * AEVec2());
		QuadModel.SetVtxUV(2, a * AEVec2(1, 0));
		QuadModel.SetVtxUV(3, a * AEVec2(1, 1));

	}

	static std::string Combotype = "Tiling";
	static std::string Tiletype = "User Specified";
	static unsigned char selecttile = 0;
	static unsigned char select = 0;
	static bool firstplaced = false;

	// ------------------------------------------------------------------------
	/*! Render
	*/ // --------------------------------------------------------------------
	void Render() {

		AEMtx33 a;

		//If we can create a Demo UI
		if (ImGui::Begin("Demo Options")) {

			//If the Presets Combo is expanded
			if (ImGui::BeginCombo("Presets", Combotype.c_str())) {

				//If we are tilling
				if (ImGui::MenuItem("Tiling")) {

					select = 0;
					Combotype = "Tiling";

				}

				//If we are cropping
				if (ImGui::MenuItem("Cropping")) {

					select = 1;
					Combotype = "Cropping";

				}

				ImGui::EndCombo();

			}

			ImGui::Separator();

			//Switch by the mode selection
			switch (select) {

			case 1: 
			{

				//If we press Left Mouse
				if (AEInputMouseTriggered(AE_MOUSE_LEFT)) {

					const AEVec2 hw(gAEMousePosition.x + ((100 * Scale.x) / 2), gAEMousePosition.y + ((100 * Scale.y) / 2));

					//If we are inside the Image View 
					if ((hw.x > 0 && hw.y > 0) && (hw.x < (100 * Scale.x) && hw.y < (100 * Scale.y))) {

						//If we placed the first Pibot
						if (!firstplaced) {

							firstpos = hw;
							firstplaced = true;
							leftcrop = 0;
							bottomcrop = 0;
							rightcrop = 1;
							topcrop = 1;
							ComputeTransformCrop();

							//else
						} else {

							firstplaced = false;
							leftcrop = firstpos.x / (100 * Scale.x);
							bottomcrop = firstpos.y / (100 * Scale.y);
							rightcrop = hw.x / (100 * Scale.x);
							topcrop = hw.y / (100 * Scale.y);
							ComputeTransformCrop();

						}
					}
				}

				//If we changed one of the values
				if (ImGui::InputFloat("Left Crop", &leftcrop, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue) ||
					ImGui::InputFloat("Right Crop", &rightcrop, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue) ||
					ImGui::InputFloat("Top Crop", &topcrop, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue) ||
					ImGui::InputFloat("Bottom Crop", &bottomcrop, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) {

					//If the left value is too small
					if (leftcrop <= -EPSILON)
						leftcrop = 1;

					//If the right value is too small
					if (rightcrop <= EPSILON)
						rightcrop = 1;

					//If the bottom value is too small
					if (bottomcrop <= -EPSILON)
						bottomcrop = 1;

					//If the top value is too small
					if (topcrop <= EPSILON)
						topcrop = 1;

					ComputeTransformCrop();

				}

				break;
			}

			default:

				//Create a Combo of tiling options
				if (ImGui::BeginCombo("Tiling Options", Tiletype.c_str())) {

					//If this is a user defined preset
					if (ImGui::MenuItem("User Defined")) {

						Tiletype = "User Defined";
						selecttile = 0;
						a = AEMtx33::Scale(static_cast<float>(Horizontal), static_cast<float>(Vertical));
						QuadModel.SetVtxUV(0, a * AEVec2(0, 1));
						QuadModel.SetVtxUV(1, a * AEVec2());
						QuadModel.SetVtxUV(2, a * AEVec2(1, 0));
						QuadModel.SetVtxUV(3, a * AEVec2(1, 1));

					}

					//If this is a size proportional preset
					if (ImGui::MenuItem("Size-Proporcional")) {

						Tiletype = "Size-Proporcional";
						selecttile = 1;
						a = AEMtx33::Scale(Scale.x / static_cast<float>(Horizontal), Scale.y / static_cast<float>(Vertical));
						QuadModel.SetVtxUV(0, a * AEVec2(0, 1));
						QuadModel.SetVtxUV(1, a * AEVec2());
						QuadModel.SetVtxUV(2, a * AEVec2(1, 0));
						QuadModel.SetVtxUV(3, a * AEVec2(1, 1));

					}

					//If this is a texture proporcional preset
					if (ImGui::MenuItem("Texture-Proporcional")) {

						Tiletype = "Texture-Proporcional";
						selecttile = 2;
						a = AEMtx33::Scale(100.f * Scale.x / static_cast<float>(Sampler.GetTexture()->mWidth), 
							100.f * Scale.y / static_cast<float>(Sampler.GetTexture()->mHeight));
						QuadModel.SetVtxUV(0, a * AEVec2(0, 1));
						QuadModel.SetVtxUV(1, a * AEVec2());
						QuadModel.SetVtxUV(2, a * AEVec2(1, 0));
						QuadModel.SetVtxUV(3, a * AEVec2(1, 1));

					}

					ImGui::EndCombo();

				}

				ImGui::Separator();

				//If we input horizontal/vertical tiles
				if (ImGui::InputInt("Horizontal Tiles", &Horizontal) || ImGui::InputInt("Vertical Tiles", &Vertical)) {

					//If have less than 1 horizontal tile
					if (Horizontal <= 0)
						Horizontal = 1;

					//If have less than 1 vertical tile
					if (Vertical <= 0)
						Vertical = 1;

					//switch by the selected tile
					switch (selecttile) {

					//case: texture based tiling
					case 1:
					{

						a = AEMtx33::Scale(Scale.x / static_cast<float>(Horizontal), Scale.y / static_cast<float>(Vertical));
						QuadModel.SetVtxUV(0, a * AEVec2(0, 1));
						QuadModel.SetVtxUV(1, a * AEVec2());
						QuadModel.SetVtxUV(2, a * AEVec2(1, 0));
						QuadModel.SetVtxUV(3, a * AEVec2(1, 1));

					}

					break;

					//case: Size based tiling
					case 2:
					{

						a = AEMtx33::Scale(100.f * Scale.x / static_cast<float>(Sampler.GetTexture()->mWidth), 100.f * Scale.y / static_cast<float>(Sampler.GetTexture()->mHeight));
						QuadModel.SetVtxUV(0, a * AEVec2(0, 1));
						QuadModel.SetVtxUV(1, a * AEVec2());
						QuadModel.SetVtxUV(2, a * AEVec2(1, 0));
						QuadModel.SetVtxUV(3, a * AEVec2(1, 1));

					}

					break;

					//case: user defined tiling
					default:
					{

						a = AEMtx33::Scale(static_cast<float>(Horizontal), static_cast<float>(Vertical));
						QuadModel.SetVtxUV(0, a * AEVec2(0, 1));
						QuadModel.SetVtxUV(1, a * AEVec2());
						QuadModel.SetVtxUV(2, a * AEVec2(1, 0));
						QuadModel.SetVtxUV(3, a * AEVec2(1, 1));

					}
					}
				}

				//If we input the X/Y size
				if (ImGui::InputFloat("Size X", &Scale.x) || ImGui::InputFloat("Size Y", &Scale.y)) {

					//If the X coordinate is too small
					if (Scale.x <= EPSILON)
						Scale.x = 1;

					//If the Y coordinate is too small
					if (Scale.y <= EPSILON)
						Scale.y = 1;

				}
			}

			ImGui::Separator();

			//If we change the sprite
			if (ImGui::Button("Change Sprite"))
				NewTexture();

			ImGui::End();

		}

		ShowFrameworkMenu();

		// here we bypass the scene default render to show our own results
		Rasterizer::FrameBuffer::Clear({1, 1, 1, 1});

		Model cpy = QuadModel;
		cpy.Transform(AETransform2D({ 400,300 }, Scale * 100.f, 0).GetMatrix());
		cpy.Draw(sShaderFn, &Sampler);

		//If we have placed our first pibot
		if (firstplaced) {
			
			AEVec2 StartPos = firstpos + AEVec2(200, 100);
			AEVec2 CurrentPos = gAEMousePosition + AEVec2(400, 300);

			DrawLine(CurrentPos, AEVec2(CurrentPos.x, StartPos.y), {1, 0, 0, 1});
			DrawLine(AEVec2(CurrentPos.x, StartPos.y), StartPos, { 1, 0, 0, 1 });
			DrawLine(StartPos, AEVec2(StartPos.x, CurrentPos.y), { 1, 0, 0, 1 });
			DrawLine(AEVec2(StartPos.x, CurrentPos.y), CurrentPos, { 1, 0, 0, 1 });

		}

		Rasterizer::FrameBuffer::Present();

	}

	// ------------------------------------------------------------------------
	/*! Unload
	*/ // --------------------------------------------------------------------
	void Unload() {

		delete Sampler.GetTexture();

	}
}