//
//  GameStates.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 12/07/2020
//  Original Author - Thomas Komair
//  Copyright � 2020 . All rights reserved.
//

#ifndef _GAME_STATES__H_
#define _GAME_STATES__H_

enum EGameStatesIDs {

	GS_SPRITE_SHEET,
	GS_TEXTURE_TRANSFORMATION,
	GS_TILEMAP,
	GS_IMAGE_ERASER,
	GS_CULLING,
	GS_POLYGON_EDITOR,
	GS_LIGHTING

};

namespace Lighting {

	void Load();
	void Update();
	void Render();

}

namespace SpriteSheet {
	void Load();
	void Unload();
	void Init();
	void Update();
	void Render();

}

namespace TextureTransformation {

	void Load();
	void Init();
	void Render();
	void Unload();
	void Free();

}

namespace TileMap {

	void Load();
	void Unload();
	void Init();
	void Free();
	void Update();
	void Render();

}

namespace ImageEraser {

	void Load();
	void Unload();
	void Init();
	void Free();
	void Update();
	void Render();

}

namespace Culling {

	void Load();
	void Unload();
	void Init();
	void Free();
	void Update();
	void Render();

}

namespace PolygonEditor {

	void Load();
	void Unload();
	void Init();
	void Free();
	void Update();
	void Render();

}

#endif