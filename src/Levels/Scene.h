#pragma once

/// PROVIDED
struct Scene
{
	// ------------------------------------------------------------------------
	// SCENE CAMERAS
	std::vector<Rasterizer::Camera2D> mCameras; 

	// ------------------------------------------------------------------------
	// OBJECT DEFINITION

	// TRANSFORM COMPONENT
	std::vector<AEVec2> mPositions;
	std::vector<AEVec2> mScales;
	std::vector<f32> mRotates;
	std::vector<AEMtx33> mWorldMtx;

	// RENDERABLE COMPONENT
	std::vector<Rasterizer::Model*> mModels;	// Model instances (one per object)
	std::vector<Rasterizer::ShaderFunc> mShaders;
	std::vector<void *>	mShaderDatas;

	// METHODS
	void RemoveObject(u32 i);
	void Draw();
	void Update();
	void Clear();
};

void InitSceneRandom(Scene & gScene, u32 GOCount, f32 gScaleMultiplier, const std::vector<Rasterizer::Model*> & GOModels);