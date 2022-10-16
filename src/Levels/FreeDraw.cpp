// ----------------------------------------------------------------------------
// File: FreeDraw.cpp
// Project: CS200 Assignment 1
// Purpose: 
// Author: Thomas Komair
// Copyright DigiPen Institute of Technology, 2015. All rights reserved
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// INCLUDES
#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"

// ----------------------------------------------------------------------------
// GLOBALS AND DEFINITIONS
std::vector<std::pair<AEVec2, f32>> gCircleArray;
std::vector<std::pair<AEVec2, AEVec2>> gEllipseArray;
std::vector<std::pair<AEVec2, AEVec2>> gLineArray;
std::vector<Rasterizer::Vertex> gTriangleArray;
u32	gCurrentLinePoint = 0;
enum EFreeDrawPrimitive { eFD_LINES, eFD_CIRCLES, eFD_ELLIPSES, eFD_TRIANGLES, eFD_COUNT};
EFreeDrawPrimitive gCurrentPrimitive = eFD_TRIANGLES;
const char* FreeDrawPrimitiveStr[] = { "Lines", "Cirlces", "Ellipses", "Triangles" };
// ----------------------------------------------------------------------------
// FORWARD DECLARATIONS
void FreeDrawMenu()
{
	
	// determine if this method is the current one
	bool isCurrent[] = {
	gCurrentPrimitive == eFD_LINES,
	gCurrentPrimitive == eFD_CIRCLES,
	gCurrentPrimitive == eFD_ELLIPSES,
	gCurrentPrimitive == eFD_TRIANGLES
	};
	// change color if it's the selected game state
	ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
	ImVec4 red = ImVec4(1, 0, 0, 1);

	ImVec4 colors[] = {
		isCurrent[0] ? red : prevColor,
		isCurrent[1] ? red : prevColor,
		isCurrent[2] ? red : prevColor,
		isCurrent[3] ? red : prevColor
	};

	for (int i = 0; i < eFD_COUNT; ++i) {
		ImGui::GetStyle().Colors[ImGuiCol_Text] = colors[i];
		if (ImGui::MenuItem(FreeDrawPrimitiveStr[i], 0, &isCurrent[i])) gCurrentPrimitive = (EFreeDrawPrimitive)i;
	}

	// reset style color
	ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;

	int step = Rasterizer::GetCircleParametricPrecision();
	ImGui::SetNextItemWidth(50);
	if (ImGui::DragInt("Parametric Precision", &step, 1.0f, 1, 250))
		Rasterizer::SetCircleParametricPrecision(step);

}

// ----------------------------------------------------------------------------
// GAMESTATEFUNCTIONS

void FreeDraw_Init()
{
	gLineArray.clear();
	gCircleArray.clear();
	gEllipseArray.clear();
	gTriangleArray.clear();
	gCurrentLinePoint = 0;
}
void FreeDraw_Update()
{
	bool isGuiActive = true;

	// clear all primitives
	if (AEInputKeyTriggered('C'))
	{
		gLineArray.clear();
		gCircleArray.clear();
		gEllipseArray.clear();
		gTriangleArray.clear();
	}

	// current the mouse position to frame buffer space (origin at botleft)
	AEVec2 mouseVP = gAEMousePosition + AEVec2((f32)gAESysWinWidth / 2.0f, (f32)gAESysWinHeight / 2.0f);
 
	// create primitives with the mouse, unless the gui is active.
	if (!isGuiActive) {

		// new line
		if (AEInputMouseTriggered(AE_MOUSE_LEFT))
		{
			if (gCurrentLinePoint != 0 && gCurrentPrimitive == eFD_TRIANGLES)
			{
				// the last point is being added and we want to connect the other two triangles
				if (gCurrentLinePoint == 2 )
				{
					if (AEInputKeyPressed(AE_KEY_CTRL)) 
					{
						auto v0 = gTriangleArray[gTriangleArray.size() - 3];
						auto v1 = gTriangleArray[gTriangleArray.size() - 1];
						gTriangleArray.push_back(v0);
						gTriangleArray.push_back(v1);
						gTriangleArray.push_back({ mouseVP, Rasterizer::Color(AERandFloat(),AERandFloat(),AERandFloat(),1) });
					}
					else// reset
						gCurrentLinePoint = 0;
				}
				else
					gCurrentLinePoint++;
			}
			else if (gCurrentLinePoint == 0 || AEInputKeyPressed(AE_KEY_CTRL)) 
			{
				switch (gCurrentPrimitive) {
				case eFD_LINES: gLineArray.push_back({ mouseVP, mouseVP }); break;
				case eFD_CIRCLES: gCircleArray.push_back({ mouseVP, 0.0f }); break;
				case eFD_ELLIPSES: gEllipseArray.push_back({ mouseVP, mouseVP }); break;
				case eFD_TRIANGLES: 
					gTriangleArray.push_back({ mouseVP, Rasterizer::Color(1,0,0,1) });
					gTriangleArray.push_back({ mouseVP, Rasterizer::Color(0,1,0,1) });
					gTriangleArray.push_back({ mouseVP, Rasterizer::Color(0,0,1,1) });
					break;
				}

				gCurrentLinePoint++;
			}
			else gCurrentLinePoint = 0;
		}

		// follow mouse
		if (gCurrentLinePoint)
		{
			switch (gCurrentPrimitive) {
			case eFD_LINES:			gLineArray.back().second = mouseVP; break;
			case eFD_CIRCLES:		gCircleArray.back().second = mouseVP.Distance(gCircleArray.back().first); break;
			case eFD_ELLIPSES: {
				auto d = gEllipseArray.back().first - mouseVP;
				gEllipseArray.back().second = { fabsf(d.x), fabs(d.y) }; break;
			}
			case eFD_TRIANGLES: {
				if (gCurrentLinePoint == 1)// move 2 points
				{
					gTriangleArray[gTriangleArray.size() - 1].mPosition = mouseVP;
					gTriangleArray[gTriangleArray.size() - 2].mPosition = mouseVP;
				}
				else
				{
					gTriangleArray[gTriangleArray.size() - 1].mPosition = mouseVP;
				}

				break;
			}
			}
		}
	}
}
void FreeDraw_Render()
{
	Rasterizer::FrameBuffer::Clear(Rasterizer::Color(1.0f, 1.0f, 1.0f, 1.0f));

	// render all the lines
	for (auto& line : gLineArray) 
		Rasterizer::DrawLine(line.first, line.second, Rasterizer::Color());

	// render all the circles
	for(auto& circle: gCircleArray)
		Rasterizer::DrawCircle(circle.first, circle.second, Rasterizer::Color());

	// render all the ellipses
	for (auto& ellipse : gEllipseArray)
		Rasterizer::DrawEllipse(ellipse.first, ellipse.second.x, ellipse.second.y, Rasterizer::Color());

	// render all triangles
	for (u32 i = 0; i < gTriangleArray.size(); i += 3) {
		Rasterizer::DrawTriangle(gTriangleArray[i], gTriangleArray[i + 1], gTriangleArray[i + 2]);
		Rasterizer::DrawLine(gTriangleArray[i].mPosition, gTriangleArray[i + 1].mPosition, Rasterizer::Color());
		Rasterizer::DrawLine(gTriangleArray[i + 1].mPosition, gTriangleArray[i + 2].mPosition, Rasterizer::Color());
		Rasterizer::DrawLine(gTriangleArray[i].mPosition, gTriangleArray[i + 2].mPosition, Rasterizer::Color());

	}

	Rasterizer::FrameBuffer::Present();
}


// ----------------------------------------------------------------------------
