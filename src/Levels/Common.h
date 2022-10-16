//
//  Common.h
//   2D Rasterizer
//
//  Created by Diego Revilla on 12/07/2020
//  Original Author - Thomas Komair
//  Copyright � 2020 . All rights reserved.
//

#ifndef _COMMON__H_
#define _COMMON__H_

// forward declare the Rasterizer class
namespace Rasterizer {

	class Model;
	class Camera2D;
	class Viewport;
	struct VertexFormat;

}

void ShowFrameworkMenu();
void CamUpdate(Rasterizer::Camera2D& cam);

#endif