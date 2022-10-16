//
//  SpriteSheet.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 11/07/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "..\Engine\Pipeline2D\Pipeline2D.h"
#include "Common.h"

//Using the Rasterizer to simplify work
using namespace Rasterizer;

namespace SpriteSheet {
	
	static Model QuadModel = Model(Model::ePT_TRIANGLE_LIST);
	static TextureSampler Sampler;
	static float AnimationSpeed = 20.f;
	static bool Playing = true;
	static void* sShaderData = &Sampler;
	static ShaderFunc sShaderFn = Shaders::FSTextureTransform;
	static Shaders::TexTransformShaderData sTexTransformData;
	static float AnimTimer = 0.f;
	static size_t AnimCurrentFrame = 0;
	static size_t FrameCount = 9*9;
	static int Columns = 9;
	static int Rows = 9;
	static bool Loop = true;
	static AEMtx33 TextureTransform;

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
	/*! Compute Texture Transform
	*
	*   Computes the Transform of a Texture given the rows and columns
	*/ // --------------------------------------------------------------------
	void ComputeTextureTransform() {

		const float _scax = 1.f / Columns;
		const float _scay = 1.f / Rows;

		TextureTransform = AEMtx33::Translate(
			_scax * (static_cast<byte>(AnimCurrentFrame) % Columns), 
			_scay * (static_cast<byte>(AnimCurrentFrame) / Columns)) 
			* AEMtx33::Scale(_scax, _scay);

	}

	// ------------------------------------------------------------------------
	/*! Reset Animation
	*
	*   Resets the Animation
	*/ // --------------------------------------------------------------------
	void ResetAnimation() {

		AnimCurrentFrame = 0;

	}

	// ------------------------------------------------------------------------
	/*! Update Animation
	*
	*   Updates the animation values
	*/ // --------------------------------------------------------------------
	void UpdateAnimation() {

		AnimTimer += static_cast<float>(gAEFrameTime) * AnimationSpeed;

		//If there is an Animation Data or SpriteSheet and we are playinh
		if (Playing) {

			//If the timer has surpassed the expected ammount of time for this frame
			if (AnimTimer > 100) {

				//If the Current frame is not the last one
				if (AnimCurrentFrame != FrameCount - 1) {

					AnimTimer -= 100.f;
					AnimCurrentFrame++;

				} else {

					//if we are playing a loop
					if (Loop) {

						AnimCurrentFrame = 0;
						AnimTimer = 0;
						ResetAnimation();

					}
				}
			} else
				AnimTimer += AnimationSpeed;

		}

		ComputeTextureTransform();

	}

	// ------------------------------------------------------------------------
	/*! Load
	*/ // --------------------------------------------------------------------
	void Load() {

		Sampler.SetTexture(LoadTextureFromFile("data/WaterSpriteSheet.png"));
		sTexTransformData.pSampler = &Sampler;
		Rasterizer::FrameBuffer::Clear(255, 255, 255, 255);

	}

	// ------------------------------------------------------------------------
	/*! Init
	*/ // --------------------------------------------------------------------
	void Init() {

		VertexFormat format;
		const AEMtx33 transform = AETransform2D({ 400,300 }, { 200,200 }, 0).GetMatrix();

		Rasterizer::SetBlendFunction(Rasterizer::eBF_ADD);
		Rasterizer::SetBlendParameters(Rasterizer::eBP_ONE, Rasterizer::eBP_ZERO);
		format.resize(2);
		format[0] = { 0, 2, 4, AU_POSITION };
		format[1] = { 2, 2, 4, AU_TEX_COORD };

		// allocate memory
		QuadModel.Alloc(4, 6, format);
		QuadModel.SetVtxPos(0, transform * AEVec2(-0.5f,0.5f ));
		QuadModel.SetVtxPos(1, transform * AEVec2( -0.5f,-0.5f ));
		QuadModel.SetVtxPos(2, transform * AEVec2( 0.5f,-0.5f ));
		QuadModel.SetVtxPos(3, transform * AEVec2( 0.5f,0.5f ));
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
		ComputeTextureTransform();

	}

	// ------------------------------------------------------------------------
	/*! Update
	*/ // --------------------------------------------------------------------
	void Update() {

		UpdateAnimation();
		sTexTransformData.texTransform = TextureTransform;

	}

	// ------------------------------------------------------------------------
	/*! Render
	*/ // --------------------------------------------------------------------
	void Render() {

		//Demo Options
		if(ImGui::Begin("Demo Options")) {

			//Play / Pause Button
			if (ImGui::Button(Playing ? "Pause" :  "Play"))
				Playing = !Playing;

			ImGui::SameLine();

			//Reestart Button
			if (ImGui::Button("Restart"))
				ResetAnimation();

			//Loop Button
			if (ImGui::Checkbox("Loop", &Loop))
				ResetAnimation();

			//Set the Columns and Rows
			if (ImGui::InputInt("Columns", &Columns) || ImGui::InputInt("Rows", &Rows)) {

				//If the user inputed less tha 1 Column
				if (Columns <= 0)
					Columns = 1;

				//If the user inputed less than 1 Row
				if (Rows <= 0)
					Rows = 1;

				FrameCount = Columns * Rows;
				ResetAnimation();

			}

			ImGui::SliderFloat("Animation Speed", &AnimationSpeed, 0, 100);
			ImGui::Separator();

			//Change Animation Sprite
			if (ImGui::Button("Change Animation Sprite"))
				NewTexture();

			ImGui::End();

		} 

		ShowFrameworkMenu();
		QuadModel.Draw(sShaderFn, &sTexTransformData);
		Rasterizer::FrameBuffer::Present();

	}

	// ------------------------------------------------------------------------
	/*! Unload
	*/ // --------------------------------------------------------------------
	void Unload() {

		delete Sampler.GetTexture();

	}
}