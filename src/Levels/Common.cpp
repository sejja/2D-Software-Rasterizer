//
//  Common.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 12/07/2020
//  Original Author - Thomas Komair
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Pipeline2D\Pipeline2D.h"
#include "Common.h"
#include "GameStates.h"

// ------------------------------------------------------------------------
/*! Show Framework Menu
*
*   Shows the Menu to change from level to level
*/ // --------------------------------------------------------------------
void ShowFrameworkMenu() {

	//IF we can succesfully create a main menu bar
	if (ImGui::BeginMainMenuBar()) {

		ImGui::Text("2D Rasterizer");
		ImGui::Separator();

		//If we can succesfully create a Demos Menu
		if (ImGui::BeginMenu("Demos")) {

			//Create Sprite Sheet Button
			if (ImGui::MenuItem("Sprite Sheet"))
				gAEGameStateNext = GS_SPRITE_SHEET;

			//Create Texture Transformation Button
			else if (ImGui::MenuItem("Texture Transformation"))
				gAEGameStateNext = GS_TEXTURE_TRANSFORMATION;

			//Create Tile Map Button
			else if (ImGui::MenuItem("Tile Map"))
				gAEGameStateNext = GS_TILEMAP;

			//Create Polygon Editor Button
			else if (ImGui::MenuItem("Polygon Editor"))
				gAEGameStateNext = GS_POLYGON_EDITOR;

			//Create Image Ereaser Button
			else if (ImGui::MenuItem("Image Eraser"))
				gAEGameStateNext = GS_IMAGE_ERASER;

			//Create Culling Button
			else if (ImGui::MenuItem("Culling"))
				gAEGameStateNext = GS_CULLING;

			//Create Lighting Button
			 else if (ImGui::MenuItem("Lighting"))
				gAEGameStateNext = GS_LIGHTING;

			ImGui::EndMenu();

		}

		ImGui::EndMainMenuBar();

	}
}

// ------------------------------------------------------------------------
/*! Cam Update
*
*   Updates the Level Camera
*/ // --------------------------------------------------------------------
void CamUpdate(Rasterizer::Camera2D& cam) {

	//If pressed A
	if (AEInputKeyPressed('A'))
		cam.MoveBy(AEVec2(-1.f, 0.f));

	//If prressed D
	if (AEInputKeyPressed('D'))
		cam.MoveBy(AEVec2(1.f, 0.f));

	//If pressed S
	if (AEInputKeyPressed('S'))
		cam.MoveBy(AEVec2(0.f, -1.f));

	//If pressed W
	if (AEInputKeyPressed('W'))
		cam.MoveBy(AEVec2(0.f, 1.f));

	//If pressed Q
	if (AEInputKeyPressed('Q'))
		cam.RotateBy(AEDegToRad(1.f));

	//If pressed E
	if (AEInputKeyPressed('E'))
		cam.RotateBy(AEDegToRad(-1.f));

	//If pressed Z
	if (AEInputKeyPressed('Z'))
		cam.SetViewRect(cam.GetViewRect() * 1.1f);

	//If pressed X
	if (AEInputKeyPressed('X'))
		cam.SetViewRect(cam.GetViewRect() * .99f);
 
}