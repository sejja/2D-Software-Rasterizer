#include <AEEngine.h>
#include "Engine\Rasterizer\Rasterizer.h"
#include "Levels\GameStates.h"
#include "Levels\Common.h"

int main() {

	// Initialize the alpha engine
	if(!AESysInit("2D Rasterizer") || 
		!Rasterizer::FrameBuffer::Allocate(gAESysWinWidth, gAESysWinHeight))
		return 1;

	AEGameStateMgrAdd(
		GS_SPRITE_SHEET,
		SpriteSheet::Load,
		SpriteSheet::Init,
		SpriteSheet::Update,
		SpriteSheet::Render,
		nullptr,
		SpriteSheet::Unload);

	AEGameStateMgrAdd(
		GS_TEXTURE_TRANSFORMATION,
		TextureTransformation::Load,
		TextureTransformation::Init,
		nullptr,
		TextureTransformation::Render,
		nullptr,
		TextureTransformation::Unload);

	AEGameStateMgrAdd(
		GS_TILEMAP,
		TileMap::Load,
		TileMap::Init,
		TileMap::Update,
		TileMap::Render,
		TileMap::Free,
		TileMap::Unload);

	AEGameStateMgrAdd(
		GS_IMAGE_ERASER,
		ImageEraser::Load,
		ImageEraser::Init,
		ImageEraser::Update,
		ImageEraser::Render,
		ImageEraser::Free,
		ImageEraser::Unload);

	AEGameStateMgrAdd(
		GS_CULLING,
		Culling::Load,
		Culling::Init,
		Culling::Update,
		Culling::Render,
		Culling::Free,
		Culling::Unload);

	AEGameStateMgrAdd(
		GS_POLYGON_EDITOR,
		PolygonEditor::Load,
		PolygonEditor::Init,
		PolygonEditor::Update,
		PolygonEditor::Render,
		PolygonEditor::Free,
		PolygonEditor::Unload);

	AEGameStateMgrAdd(
		GS_LIGHTING,
		Lighting::Load,
		nullptr,
		Lighting::Update,
		Lighting::Render,
		nullptr,
		nullptr);

	// Initialize the game state manager
	AEGameStateMgrInit(GS_SPRITE_SHEET);

	// launch engine
	AESysGameLoop();

	// Terminate Graphics System
	Rasterizer::FrameBuffer::Delete();

	// Terminate AECore
	AESysExit();

	return 0;
}