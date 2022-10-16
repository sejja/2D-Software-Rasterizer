#include <AEEngine.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"
#include <imgui/imgui.h>
using namespace Rasterizer;


void SimpleCircles_Init()
{
}
void SimpleCircles_Update()
{
	static f32 minA = 0.0f, maxA = TWO_PI, outerRad = 100, innerRad = 50;
	static bool bPieEnabled = true, bRingEnabled = true;
	FrameBuffer::Clear(Color().FromU32(AE_COLORS_WHITE));
	AEVec2 winCenter = AEVec2((f32)gAESysWinWidth, (f32)gAESysWinHeight) * 0.5f;
	//DrawCircle(winCenter + AEVec2(-100, 0), 100, Color());
	//FillCircle(winCenter + AEVec2(100,0), 100, Color().FromU32(AE_COLORS_BLUSH));

	if(bRingEnabled)
		FillRing(winCenter, outerRad, innerRad, Color().FromU32(AE_COLORS_ALMOND));

	if(bPieEnabled)
		FillPie(winCenter, 100, minA, maxA, Color().FromU32(AE_COLORS_ALMOND));

	//DrawEllipse(winCenter + AEVec2(-200, 0), 50, 200, Color());
	//FillEllipse(winCenter + AEVec2(-320, 0), 50, 200, Color().FromU32(AE_COLORS_ALMOND));
	//DrawEllipse(winCenter + AEVec2(100, 100), 200, 50, Color());
	//FillEllipse(winCenter + AEVec2(100, -100), 200, 50, Color().FromU32(AE_COLORS_APPLE_GREEN));
	//DrawEllipse(winCenter + AEVec2(100, 0), 50, 50, Color());

	ImGui::Begin("Challenges Options");
	
	ImGui::PushID(0);
	ImGui::Checkbox("", &bPieEnabled); ImGui::SameLine();
	ImGui::PopID();
	if (ImGui::CollapsingHeader("Fill Pie")) {
		minA = AERadToDeg(minA);
		ImGui::DragFloat("min angle", &minA, 0.5f);
		minA = AEDegToRad(minA);

		maxA = AERadToDeg(maxA);
		ImGui::DragFloat("max angle", &maxA, 0.5f);
		maxA = AEDegToRad(maxA);
	}
	ImGui::PushID(1);
	ImGui::Checkbox("", &bRingEnabled); ImGui::SameLine();
	ImGui::PopID();
	if (ImGui::CollapsingHeader("Fill Ring")) {
		ImGui::DragFloat("inner radius", &innerRad, 1.0f, 0.000001f, 5000.f);
		ImGui::DragFloat("outer radius", &outerRad, 1.0f, 0.000001f, 5000.f);
	}

	ImGui::End();

	ShowFrameworkMenu();
}
void SimpleCircles_Render()
{
	FrameBuffer::Present();
}