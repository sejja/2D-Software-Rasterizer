#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"
#include <string>
#include <sstream>
namespace PlaneNormalDemo
{

	template <typename T>
	std::string to_string_with_precision(const T a_value, const int n = 2)
	{
		std::ostringstream out;
		out.precision(n);
		out << std::fixed << a_value;
		return out.str();
	}
	void Init()
	{
	}
	void Update()
	{
		ShowFrameworkMenu();
	}
	void Render()
	{
		u32 checker[2] = { AE_COLORS_GRAY, AE_COLORS_ANTI_FLASH_WHITE };
		//Rasterizer::FrameBuffer::ClearCheckerboard(checker, 16);
		Rasterizer::FrameBuffer::Clear(Rasterizer::Color().FromU32(checker[0]));
		Rasterizer::Vertex tri[] = {
			{{0,1},{1.f,0.f,0.f,1.f}},
			{{0,0},{0.f,1.f,0.f,1.f}},
			{{1,0},{0.f,0.f,1.f,1.f}}
		}, tri_cpy[3];

		struct TriView {
			std::string name;
			Rasterizer::Color col[3];
		};
		TriView views[] = {
			{
				"Red Channel",
				{
					{tri[0].mColor.r, 0,0,1.0f},
					{tri[1].mColor.r, 0,0,1.0f},
					{tri[2].mColor.r, 0,0,1.0f}
				}
			},
			{
				"Green Channel",
				{
					{0,tri[0].mColor.g, 0,1.0f},
					{0,tri[1].mColor.g, 0,1.0f},
					{0,tri[2].mColor.g, 0,1.0f}
				}
			},
			{
				"Blue Channel",
				{
					{0,0,tri[0].mColor.b,1.0f},
					{0,0,tri[1].mColor.b,1.0f},
					{0,0,tri[2].mColor.b,1.0f}
				}
			},
			{
				"Alpha Channel",
				{
					{tri[0].mColor.a, tri[0].mColor.a, tri[0].mColor.a, 1.0f},
					{tri[1].mColor.a, tri[1].mColor.a, tri[1].mColor.a, 1.0f},
					{tri[2].mColor.a, tri[2].mColor.a, tri[2].mColor.a, 1.0f}
				}
			}
		};

		struct plane 
		{
			plane(AEVec3 tri[3])
			{
				N = (tri[1] - tri[0]).Cross(tri[2] - tri[0]).Normalize();
				D = N * tri[0];
			}
			f32 findZFromPoint(AEVec2 P)
			{
				return -(N.x * P.x + N.y * P.y + D) / N.z;
			}
			AEVec3 N;
			f32 D;
		};
		
		auto viewCount = sizeof(views) / sizeof(TriView);
		f32* attVal = new f32[viewCount];
		Rasterizer::Color pixColor;
		auto cols = min(4, viewCount);
		auto rows = viewCount / cols + 1;
		// transform
		static AEVec2 pixPos = AEVec2(0, 0), pixPosCpy;
		auto start = AEVec2(20, 400);
		auto offset = AEVec2(150, -150);
		auto pos = start;
		for (u32 row = 0, i = 0; row < rows && i < viewCount; ++row) {
			for (u32 col = 0; col < cols && i < viewCount; ++col, ++i){
				auto oX = col * offset.x;
				auto oY = row * offset.y;
				pos = start + AEVec2(oX,oY);
				auto world = AEMtx33::Translate(pos.x, pos.y) * AEMtx33::Scale(125, 125);
				for (u32 v = 0; v < 3; ++v) {
					tri_cpy[v].mPosition = world * tri[v].mPosition;
					tri_cpy[v].mColor = views[i].col[v];
					pixPosCpy = world * pixPos;
				}
				std::string name = views[i].name + " "; 

				AEVec3 planeVts[3] = {
					{tri[0].mPosition.x, tri[0].mPosition.y, tri[0].mColor.v[i]},
					{tri[1].mPosition.x, tri[1].mPosition.y, tri[1].mColor.v[i]},
					{tri[2].mPosition.x, tri[2].mPosition.y, tri[2].mColor.v[i]}
				};
				plane pl(planeVts);
				pixColor.v[i] = attVal[i] = pl.findZFromPoint(pixPos);

				name += to_string_with_precision(attVal[i]);
				AEGfxPrint((u32)pos.x, gAESysWinHeight - (u32)pos.y, AE_COLORS_WHITE, name.c_str());
				Rasterizer::DrawTriangle(tri_cpy[0], tri_cpy[1], tri_cpy[2]);
				Rasterizer::FillCircle(pixPosCpy, 3, Rasterizer::Color().FromU32(AE_COLORS_YELLOW_GREEN));
			}
		}
		pos = AEVec2(20, 100);
		auto world = AEMtx33::Translate(pos.x, pos.y) * AEMtx33::Scale(250, 250);
		for (u32 v = 0; v < 3; ++v) {
			tri_cpy[v].mPosition = world * tri[v].mPosition;
			tri_cpy[v].mColor = tri[v].mColor;
			pixPosCpy = world * pixPos;
		}
		Rasterizer::DrawTriangle(tri_cpy[0], tri_cpy[1], tri_cpy[2]);
		Rasterizer::FillCircle(pixPosCpy, 3, Rasterizer::Color().FromU32(AE_COLORS_YELLOW_GREEN));
		AEGfxPrint((u32)pos.x, gAESysWinHeight - (u32)pos.y, AE_COLORS_WHITE, "Final");

		
		// 
		AELineSegment2D tri_edges[3];
		AEBuildLineSegment2D(tri_edges+0, &tri_cpy[0].mPosition, &tri_cpy[1].mPosition);
		AEBuildLineSegment2D(tri_edges+1, &tri_cpy[1].mPosition, &tri_cpy[2].mPosition);
		AEBuildLineSegment2D(tri_edges+2, &tri_cpy[2].mPosition, &tri_cpy[0].mPosition);

		// current the mouse position to frame buffer space (origin at botleft)
		AEVec2 mouseVP = gAEMousePosition + AEVec2((f32)gAESysWinWidth / 2.0f, (f32)gAESysWinHeight / 2.0f);
		bool pointInTriangle =
			AEStaticPointToStaticLineSegment(&mouseVP, tri_edges + 0) < 0 &&
			AEStaticPointToStaticLineSegment(&mouseVP, tri_edges + 1) < 0 &&
			AEStaticPointToStaticLineSegment(&mouseVP, tri_edges + 2) < 0;

		if (pointInTriangle && AEInputMousePressed(0)) {
			pixPos = (mouseVP - pos)/250.0f;
		}
		
		// draw the pixel color as a rectangle
		Rasterizer::FillCircle({ 400, 300 }, 30, pixColor);

		// clean up
		delete[] attVal;

		Rasterizer::FrameBuffer::Present();
	}
}