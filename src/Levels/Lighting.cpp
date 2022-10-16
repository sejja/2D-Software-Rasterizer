//
//  Lighting.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 19/07/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "..\Engine\Pipeline2D\Pipeline2D.h"
#include "Common.h"
#include <ctime>
#include <list>

//Using the Rasterizer to simplify work
using namespace Rasterizer;

namespace Lighting {

	static std::vector<AELineSegment2D_> Segments;
	static std::vector<AEVec2> RayCasts;
	static bool poly = false;
	static AEVec2 MousePos;

	// ------------------------------------------------------------------------
	/*! Raycast
	*
	*   Casts A Ray into an entire Scene
	*/ // --------------------------------------------------------------------
	void Raycast(const AEVec2& x) {

		AEVec2 MousePos = (gAEMousePosition + AEVec2(400, 300));
		AELineSegment2D_ LS;
		AEVec2 c = x;
		AEVec2 coll;
		float v = 1;
		float temp[2];

		AEBuildLineSegment2D(&LS, &MousePos, (AEVec2*)&x);

		//For each segment
		for (AELineSegment2D_ s : Segments) {

			//If the ray is intersecting with the segment
			if (AEStaticLineToStaticLine(&LS, &s, &coll, temp, temp + 1))
				
				//If it intersects closer to the ray origin
				if (*temp < v) {

					c = coll;
					v = *temp;

				}
		}
		
		DrawLine(MousePos, c, { 0, 1, 0, 1 });
		RayCasts.push_back(c);
	
	}

	// ------------------------------------------------------------------------
	/*! CreateAABB
	*
	*   Creates an AABB of segments at a position given a radius
	*/ // --------------------------------------------------------------------
	void CreateAABB(const AEVec2& center, const float radius) {

		AELineSegment2D Temp;
		AEVec2 tempp1(center.x - radius, center.y - radius);
		AEVec2 tempp2(center.x - radius, center.y + radius);

		AEBuildLineSegment2D(&Temp, &tempp1, &tempp2);
		Segments.push_back(Temp);
		tempp1 = AEVec2(center.x - radius, center.y + radius);
		tempp2 = AEVec2(center.x + radius, center.y + radius);
		AEBuildLineSegment2D(&Temp, &tempp1, &tempp2);
		Segments.push_back(Temp);
		tempp1 = AEVec2(center.x + radius, center.y + radius);
		tempp2 = AEVec2(center.x + radius, center.y - radius);
		AEBuildLineSegment2D(&Temp, &tempp1, &tempp2);
		Segments.push_back(Temp);
		tempp1 = AEVec2(center.x + radius, center.y - radius);
		tempp2 = AEVec2(center.x - radius, center.y - radius);
		AEBuildLineSegment2D(&Temp, &tempp1, &tempp2);
		Segments.push_back(Temp);
		
	}

	// ------------------------------------------------------------------------
	/*! Load
	*/ // --------------------------------------------------------------------
	void Load() {

		Segments.clear();
		Rasterizer::FrameBuffer::Clear(255, 255, 255, 255);
		SetDrawLineMethod(EDrawLineMethod::eDL_BRESENHAM);
		AELineSegment2D Temp;
		AEVec2 tempp1(0, 0);
		AEVec2 tempp2(0, 600);

		AEBuildLineSegment2D(&Temp, &tempp1, &tempp2);
		Segments.push_back(Temp);
		tempp1 = AEVec2(0, 600);
		tempp2 = AEVec2(800, 600);
		AEBuildLineSegment2D(&Temp, &tempp1, &tempp2);
		Segments.push_back(Temp);
		tempp1 = AEVec2(800, 600);
		tempp2 = AEVec2(800, 0);
		AEBuildLineSegment2D(&Temp, &tempp1, &tempp2);
		Segments.push_back(Temp);
		tempp1 = AEVec2(800, 0);
		tempp2 = AEVec2(0, 0);
		AEBuildLineSegment2D(&Temp, &tempp1, &tempp2);
		Segments.push_back(Temp);
		srand(static_cast<unsigned>(time(NULL)));

		//Create 45 AABBs
		for (size_t i = 0; i < 45; i++)
			CreateAABB(AEVec2(static_cast<float>(rand() % 801), static_cast<float>(rand() % 576)), static_cast<float>(rand() % (100 - 20 + 1) - 20));

	}

	// ------------------------------------------------------------------------
	/*! Update
	*/ // --------------------------------------------------------------------
	void Update() {

		//If we press CTRL
		if(AEInputKeyTriggered(AE_KEY_CTRL))
			
			//Activate polygon rendering
			poly = !poly;

	}

	// ------------------------------------------------------------------------
	/*! Sort By Angle
	*
	*   Sorts two vectors by the angle formed to the origin
	*/ // --------------------------------------------------------------------
	bool SortByAngle(const AEVec2& a, const AEVec2& b) {

		return atan2f(a.y - MousePos.y, a.x - MousePos.x) < atan2f(b.y - MousePos.y, b.x - MousePos.x);

	}

	// ------------------------------------------------------------------------
	/*! Render
	*/ // --------------------------------------------------------------------
	void Render() {

		Rasterizer::FrameBuffer::Clear(Color(0, 0, 0, 1));
		ShowFrameworkMenu();
		AEGfxPrint(0, 20, 0xFFFFFFFF, "Press CRTL to toogle Polygon Lighting");
		MousePos = (gAEMousePosition + AEVec2(400, 300));
		RayCasts.push_back(MousePos);

		//For each AABB
		for (size_t i = 4; i < Segments.size(); i++) {

			Raycast(Segments[i].mP0);
			DrawLine(Segments[i].mP0, Segments[i].mP1, { 1, 1, 1, 1 });

		}

		std::sort(RayCasts.begin(), RayCasts.end(), SortByAngle);

		//IF we have poly rendering turned on
		if (poly) {

			VertexDefs::VtxPosColor a, b, c;
			a.mColor = { .8f, .8f, .8f, .8f };
			b.mColor = { .8f, .8f, .8f, .8f };
			c.mColor = { .8f, .8f, .8f, .8f };

			//For each RayCast result, draw a Polygon
			for (size_t i = 1; i < RayCasts.size() - 2; i++) {

				a.mPosition = RayCasts[0];
				b.mPosition = RayCasts[i];
				c.mPosition = RayCasts[i + 1];
				DrawTrianglePlaneNormal(a, b, c);

			}

			a.mPosition = RayCasts[0];
			b.mPosition = RayCasts[RayCasts.size() - 1];
			c.mPosition = RayCasts[1];
			DrawTrianglePlaneNormal(a, b, c);

		}

		//Draw a red circle at the mous position
		if (gAEMouseInWindow)
			FillCircle(MousePos, 10, { 1, 0, 0, 1 });

		RayCasts.clear();
		Rasterizer::FrameBuffer::Present();
	}
}