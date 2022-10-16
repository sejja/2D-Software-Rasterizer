#include <AEEngine.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"
using namespace Rasterizer;
#include <iostream> // cout

void StressTestLines()
{
	AESysShowConsole();
	int lineCount = 1000000;
	std::vector<AEVec2> points(lineCount * 2);
	// draw a random combination of lines contained within the viewport
	for (int i = 0; i < lineCount * 2; i += 2)
	{
		// generate the two random points
		AEVec2 p0(AERandFloat(0, (f32)gAESysWinWidth), AERandFloat(0, (f32)gAESysWinHeight));
		AEVec2 p1(AERandFloat(0, (f32)gAESysWinWidth), AERandFloat(0, (f32)gAESysWinHeight));

		points[i] = p0;
		points[i + 1] = p1;
	}

	f64 timeDDA, timeNaive, timeBresenham;

	Rasterizer::Color c;
	// do stress test comparison
	auto s = AEGetTime();
	for (u32 i = 0; i < points.size(); i += 2)
		Rasterizer::DrawLineDDA(points[i], points[i + 1], c);
	timeDDA = AEGetTime() - s;

	s = AEGetTime();
	for (u32 i = 0; i < points.size(); i += 2)
		Rasterizer::DrawLineNaive(points[i], points[i + 1], c);
	timeNaive = AEGetTime() - s;

	s = AEGetTime();
	for (u32 i = 0; i < points.size(); i += 2)
		Rasterizer::DrawLineBresenham(points[i], points[i + 1], c);
	timeBresenham = AEGetTime() - s;

	std::cout << "Naive Time: " << timeNaive << "\n";
	std::cout << "DDA Time: " << timeDDA << "\n";
	std::cout << "Bresenham Time: " << timeBresenham << "\n";
}
void StressTestCircles()
{
	AESysShowConsole();
	int circleCount = 1000000;
	std::vector<std::pair<AEVec2, float>> circles(circleCount);
	// draw a random combination of lines contained within the viewport
	for (int i = 0; i < circleCount; ++i)
	{
		// generate random circle
		AEVec2 p0(AERandFloat(0, (f32)gAESysWinWidth), AERandFloat(0, (f32)gAESysWinHeight));
		circles[i] = {p0, AERandFloat(10, 150)};
	}

	f64 timeAlgebraic, timeParametric, timeParametricInc, timeMidpoint;

	Rasterizer::Color c;
	// do stress test comparison
	auto s = AEGetTime();
	for (u32 i = 0; i < circles.size(); ++i)
		Rasterizer::DrawCircleAlgebraic(circles[i].first, circles[i].second, c);
	timeAlgebraic = AEGetTime() - s;

	s = AEGetTime();
	for (u32 i = 0; i < circles.size(); ++i)
		Rasterizer::DrawCircleParametric(circles[i].first, circles[i].second, c);
	timeParametric = AEGetTime() - s;

	s = AEGetTime();
	for (u32 i = 0; i < circles.size(); ++i)
		Rasterizer::DrawCircleParametricInc(circles[i].first, circles[i].second, c);
	timeParametricInc = AEGetTime() - s;

	s = AEGetTime();
	for (u32 i = 0; i < circles.size(); ++i)
		Rasterizer::DrawCircleMidpoint(circles[i].first, circles[i].second, c);
	timeMidpoint = AEGetTime() - s;

	std::cout << "Circle Algebraic Time: " << timeAlgebraic << "\n";
	std::cout << "Circle Parametric Time: " << timeParametric << "\n";
	std::cout << "Circle Parametric Incremental Time: " << timeParametricInc << "\n";
	std::cout << "Circle Midpoint Time: " << timeMidpoint << "\n";
}
void StressTestEllipses()
{
	AESysShowConsole();
	int ellipseCount = 1000000;
	std::vector<std::pair<AEVec2, AEVec2>> ellipses(ellipseCount);
	// draw a random combination of lines contained within the viewport
	for (int i = 0; i < ellipseCount; ++i)
	{
		// generate random circle
		AEVec2 p0(AERandFloat(0, (f32)gAESysWinWidth), AERandFloat(0, (f32)gAESysWinHeight));
		AEVec2 p1(AERandFloat(20, 50), AERandFloat(20,50));
		ellipses[i] = { p0, p1 };
	}

	f64 timeAlgebraic, timeParametric, timeParametricInc, timeMidpoint;

	Rasterizer::Color c;
	// do stress test comparison
	auto s = AEGetTime();
	for (u32 i = 0; i < ellipses.size(); ++i)
		Rasterizer::DrawEllipseAlgebraic(ellipses[i].first, ellipses[i].second.x, ellipses[i].second.y, c);
	timeAlgebraic = AEGetTime() - s;

	s = AEGetTime();
	for (u32 i = 0; i < ellipses.size(); ++i)
		Rasterizer::DrawEllipseParametric(ellipses[i].first, ellipses[i].second.x, ellipses[i].second.y, c);
	timeParametric = AEGetTime() - s;

	s = AEGetTime();
	for (u32 i = 0; i < ellipses.size(); ++i)
		Rasterizer::DrawEllipseParametricInc(ellipses[i].first, ellipses[i].second.x, ellipses[i].second.y, c);
	timeParametricInc = AEGetTime() - s;

	s = AEGetTime();
	for (u32 i = 0; i < ellipses.size(); ++i)
		Rasterizer::DrawEllipseMidpoint(ellipses[i].first, ellipses[i].second.x, ellipses[i].second.y, c);
	timeMidpoint = AEGetTime() - s;

	std::cout << "Ellipse Algebraic Time: " << timeAlgebraic << "\n";
	std::cout << "Ellipse Parametric Time: " << timeParametric << "\n";
	std::cout << "Ellipse Parametric Incremental Time: " << timeParametricInc << "\n";
	std::cout << "Ellipse Midpoint Time: " << timeMidpoint << "\n";
}
void StressTests_Load()
{
	StressTestLines();
	StressTestCircles();
	StressTestEllipses();
}
void StressTests_Update()
{
	ShowFrameworkMenu();

	FrameBuffer::Present();
}