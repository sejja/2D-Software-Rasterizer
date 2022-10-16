#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"
using namespace Rasterizer;

// -----------------------------------------------------------------------
// DEMO GLOBALS

namespace SimpleTriangles
{
	u32		gMode = 0;
	u32		gDemo = 0;
	bool	gDebug = true;

	// Renders a simple quad in the middle of the screen
	void RenderDemo1();
	// Renders a simple quad (rotated by 45 def) in the middle of the screen
	void RenderDemo2();
	// Render all possible cases (1 through 6)
	void RenderDemo3();
	// Render Simple Triangle Lerped
	void RenderDemo4();
	// Render Simple Quad
	void RenderDemo5();
	// Render simple quad rotate by 45 deg
	void RenderDemo6();
	// Render all cases
	void RenderDemo7();

	void(*AllDemos[])() = {
		RenderDemo1,
		RenderDemo2,
		RenderDemo3,
		RenderDemo4,
		RenderDemo5,
		RenderDemo6,
		RenderDemo7
	};

	const char* gDemoTitles[] = {
		"Red-Blue Quad",
		"Red-Blue Quad (45 deg)",
		"All Cases",
		"Simple Triangle Lerped",
		"Colored Quad",
		"Colored Quad (45 deg)",
		"All Cases (colored)"
	};
	u32 gDemoCount = sizeof(AllDemos) / sizeof(void*);

	// Menu
	void OptionsMenu();

}using namespace SimpleTriangles;

// -----------------------------------------------------------------------
// GAMESTATE FUNCTIONS

void SimpleTriangles_Init()
{
	gMode = 0;
	gDemo = 3;
	gDebug = true;
}
void SimpleTriangles_Update()
{
	


	if (AEInputKeyTriggered('M'))
	{
		gMode = ++gMode % 2;
	}
	if (AEInputKeyTriggered('N'))
	{
		gDemo = ++gDemo % gDemoCount;
	}

	if (AEInputKeyTriggered('D'))
		gDebug = !gDebug;
}
void SimpleTriangles_Render()
{
	FrameBuffer::Clear(Color().FromU32(AE_COLORS_WHITE));

	
	AllDemos[gDemo]();

	FrameBuffer::Present();
}

// -----------------------------------------------------------------------
// DEMO FUNCTIONS

namespace SimpleTriangles
{
	// Renders a simple quad in the middle of the screen
	void RenderDemo1()
	{
		AEVec2 p0(300, 400);
		AEVec2 p1(300, 200);
		AEVec2 p2(500, 200);
		AEVec2 p3(500, 400);

		// Draw Outline
		if (gDebug)
		{
			Rasterizer::DrawLine(AEVec2(p0.x, p0.y), AEVec2(p1.x, p1.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p1.x, p1.y), AEVec2(p2.x, p2.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p2.x, p2.y), AEVec2(p3.x, p3.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p0.x, p0.y), AEVec2(p3.x, p3.y), Rasterizer::Color());
		}
		// use naive
		if (gMode == 0)
		{
			Rasterizer::FillTriangleNaive(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			Rasterizer::FillTriangleNaive(p0, p2, p3, Rasterizer::Color().FromU32(0xFF0000FF));
		}
		// use topleft
		else
		{
			Rasterizer::FillTriangleTopLeft(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			Rasterizer::FillTriangleTopLeft(p0, p2, p3, Rasterizer::Color().FromU32(0xFF0000FF));
		}
	}
	// Renders a simple quad (rotated by 45 def) in the middle of the screen
	void RenderDemo2()
	{
		AEVec2 p0(-100, 100);
		AEVec2 p1(-100, -100);
		AEVec2 p2(100, -100);
		AEVec2 p3(100, 100);

		AEMtx33 rot;
		AEMtx33RotDeg(&rot, 45.0f);

		// apply rotation to all points
		AEMtx33MultVec(&p0, &rot, &p0);
		AEMtx33MultVec(&p1, &rot, &p1);
		AEMtx33MultVec(&p2, &rot, &p2);
		AEMtx33MultVec(&p3, &rot, &p3);

		// translate to middle
		p0 += AEVec2(400, 300);
		p1 += AEVec2(400, 300);
		p2 += AEVec2(400, 300);
		p3 += AEVec2(400, 300);

		// Draw Outline
		if (gDebug)
		{
			Rasterizer::DrawLine(AEVec2(p0.x, p0.y), AEVec2(p1.x, p1.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p1.x, p1.y), AEVec2(p2.x, p2.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p2.x, p2.y), AEVec2(p3.x, p3.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p0.x, p0.y), AEVec2(p3.x, p3.y), Rasterizer::Color());
		}
		// use naive
		if (gMode == 0)
		{
			Rasterizer::FillTriangleNaive(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			Rasterizer::FillTriangleNaive(p0, p2, p3, Rasterizer::Color().FromU32(0xFF0000FF));
		}
		// use topleft
		else
		{
			Rasterizer::FillTriangleTopLeft(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			Rasterizer::FillTriangleTopLeft(p0, p2, p3, Rasterizer::Color().FromU32(0xFF0000FF));
		}
	}
	// Render all possible cases (1 through 6)
	void RenderDemo3()
	{
		AEVec2 top(75.0f, 500.0f);
		AEVec2 midLeft(0.0f, 425.0f);
		AEVec2 midRight(150.0f, 425.0f);
		AEVec2 bot(75.0f, 350.0f);

		const int TOP = 0;
		const int MID_L = 1;
		const int MID_R = 2;
		const int BOT = 3;

		AEVec2 vtx[4] = { top, midLeft, midRight, bot };
		int triangles[6 * 3] =
		{
			BOT, TOP, MID_L,	// CASE 1
			BOT, MID_R, TOP,	// CASE 2
			MID_L, BOT, TOP,	// CASE 4
			MID_R, TOP, BOT,	// CASE 3
			TOP, MID_L, BOT,	// CASE 5
			TOP, BOT, MID_R		// CASE 6
		};

		AEVec2 offset(100, 0);
		for (int i = 0; i < 18; i += 3)
		{
			AEVec2 p0 = vtx[triangles[i]];
			AEVec2 p1 = vtx[triangles[i + 1]];
			AEVec2 p2 = vtx[triangles[i + 2]];

			p0 += offset;
			p1 += offset;
			p2 += offset;

			// Draw Outline
			if (gDebug)
			{
				Rasterizer::DrawLine(AEVec2(p0.x, p0.y), AEVec2(p1.x, p1.y), Rasterizer::Color());
				Rasterizer::DrawLine(AEVec2(p1.x, p1.y), AEVec2(p2.x, p2.y), Rasterizer::Color());
				Rasterizer::DrawLine(AEVec2(p2.x, p2.y), AEVec2(p0.x, p0.y), Rasterizer::Color());
			}
			// use naive
			if (gMode == 0)
			{
				Rasterizer::FillTriangleNaive(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			}
			// use topleft
			else
			{
				Rasterizer::FillTriangleTopLeft(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			}

			offset.x += 25.0f;
			if (i % 6) // additional offset when done a pair
				offset.x += 150.0f;
		}
	}
	// Render a simple triangle
	void RenderDemo4()
	{
		Vertex v0 = { {300, 400}, {1,0,0,1} };
		Vertex v1 = { {200, 200}, {0,1,0,1} };
		Vertex v2 = { {400, 100}, {0,0,1,1} };
		Rasterizer::DrawTriangle(v0, v1, v2);
		if (gDebug)
		{
			Rasterizer::DrawLine(v0.mPosition, v1.mPosition, Rasterizer::Color());
			Rasterizer::DrawLine(v0.mPosition, v2.mPosition, Rasterizer::Color());
			Rasterizer::DrawLine(v1.mPosition, v2.mPosition, Rasterizer::Color());
		}
	}
	// Render simple quad rotate by 45 deg
	void RenderDemo5()
	{

		Vertex v0 = { {300, 400}, {1,0,0,1} };
		Vertex v1 = { {300, 200}, {0,1,0,1} };
		Vertex v2 = { {500, 200}, {0,0,1,1} };
		Vertex v3 = { {500, 400}, {0,1,1,1} };

		// Draw Outline
		if (gDebug)
		{
			Rasterizer::DrawLine(v0.mPosition, v1.mPosition, Rasterizer::Color());
			Rasterizer::DrawLine(v1.mPosition, v2.mPosition, Rasterizer::Color());
			Rasterizer::DrawLine(v2.mPosition, v3.mPosition, Rasterizer::Color());
			Rasterizer::DrawLine(v0.mPosition, v3.mPosition, Rasterizer::Color());
		}
		Rasterizer::DrawTriangle(v0, v1, v2);
		Rasterizer::DrawTriangle(v0, v2, v3);
	}
	// Render Simple Quad
	void RenderDemo6()
	{
		Vertex v0 = { {-100, 100 }, {1,0,0,1} };
		Vertex v1 = { {-100, -100 }, {0,1,0,1} };
		Vertex v2 = { {100, -100 }, {0,0,1,1} };
		Vertex v3 = { {100, 100 }, {0,1,1,1}};


		AEMtx33 rot;
		AEMtx33RotDeg(&rot, 45.0f);

		// apply rotation to all points
		AEMtx33MultVec(&v0.mPosition, &rot, &v0.mPosition);
		AEMtx33MultVec(&v1.mPosition, &rot, &v1.mPosition);
		AEMtx33MultVec(&v2.mPosition, &rot, &v2.mPosition);
		AEMtx33MultVec(&v3.mPosition, &rot, &v3.mPosition);

		// translate to middle
		v0.mPosition += AEVec2(400, 300);
		v1.mPosition += AEVec2(400, 300);
		v2.mPosition += AEVec2(400, 300);
		v3.mPosition += AEVec2(400, 300);

		// Draw Outline
		if (gDebug)
		{
			Rasterizer::DrawLine(v0.mPosition, v1.mPosition, Rasterizer::Color());
			Rasterizer::DrawLine(v1.mPosition, v2.mPosition, Rasterizer::Color());
			Rasterizer::DrawLine(v2.mPosition, v3.mPosition, Rasterizer::Color());
			Rasterizer::DrawLine(v0.mPosition, v3.mPosition, Rasterizer::Color());
		}
		Rasterizer::DrawTriangle(v0, v1, v2);
		Rasterizer::DrawTriangle(v0, v2, v3);
	}
	// Render all cases
	void RenderDemo7()
	{
		Vertex top		= {{75.0f, 500.0f}	, {1,0,0,1}};
		Vertex midLeft	= {{0.0f, 425.0f}	, {0,1,0,1}};
		Vertex midRight	= {{150.0f, 425.0f}	, {0,1,0,1}};
		Vertex bot		= {{75.0f, 350.0f}	, {0,0,1,1}};

		const int TOP = 0;
		const int MID_L = 1;
		const int MID_R = 2;
		const int BOT = 3;

		Vertex vtx[4] = { top, midLeft, midRight, bot };
		int triangles[6 * 3] =
		{
			BOT, TOP, MID_L,	// CASE 1
			BOT, MID_R, TOP,	// CASE 2
			MID_L, BOT, TOP,	// CASE 4
			MID_R, TOP, BOT,	// CASE 3
			TOP, MID_L, BOT,	// CASE 5
			TOP, BOT, MID_R		// CASE 6
		};

		AEVec2 offset(100, 0);
		for (int i = 0; i < 18; i += 3)
		{
			Vertex p0 = vtx[triangles[i]];
			Vertex p1 = vtx[triangles[i + 1]];
			Vertex p2 = vtx[triangles[i + 2]];

			p0.mPosition += offset;
			p1.mPosition += offset;
			p2.mPosition += offset;

			// Draw Outline
			if (gDebug)
			{
				Rasterizer::DrawLine(p0.mPosition, p1.mPosition, Rasterizer::Color());
				Rasterizer::DrawLine(p0.mPosition, p2.mPosition, Rasterizer::Color());
				Rasterizer::DrawLine(p1.mPosition, p2.mPosition, Rasterizer::Color());
			}

			Rasterizer::DrawTriangle(p0, p1, p2);

			offset.x += 25.0f;
			if (i % 6) // additional offset when done a pair
				offset.x += 150.0f;
		}
	}

	void OptionsMenu()
	{
		ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
		ImVec4 red = ImVec4(1, 0, 0, 1);
		if (ImGui::BeginMenu("Fill Algorithm"))
		{
			bool isCurrent = gMode == 0;
			ImGui::GetStyle().Colors[ImGuiCol_Text] = gMode == 0 ? red : prevColor;
			if (ImGui::MenuItem("Naive", "M", &isCurrent)) gMode = 0;

			isCurrent = gMode == 1;
			ImGui::GetStyle().Colors[ImGuiCol_Text] = gMode == 1 ? red : prevColor;
			if (ImGui::MenuItem("Top-Left", "M", &isCurrent)) gMode = 1;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Demo"))
		{
			for (u32 i = 0; i < gDemoCount; ++i) {
				bool isCurrent = gDemo == 0;
				ImGui::GetStyle().Colors[ImGuiCol_Text] = gDemo == i ? red : prevColor;
				if (ImGui::MenuItem(gDemoTitles[i], "N", &isCurrent)) gDemo = i;
			}

			ImGui::EndMenu();
		}

		// debug
		ImGui::MenuItem("Draw Triangle Outline", "D", &gDebug);

		// reset color
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
	}
}