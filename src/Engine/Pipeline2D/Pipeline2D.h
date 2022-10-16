#ifndef _PIPELINE_2D_H_
#define _PIPELINE_2D_H_

#include "Model.h"
#include "Viewport.h"
#include "Camera2D.h"

namespace Rasterizer
{
	class Pipeline2D
	{
	public:

		// PROVIDED
		// processes a model
		static void DrawModel(const Model & model, ShaderFunc shader, void * shaderData);

		// PROVIDED: Common model transformations. Note: These functions will modify the model 
		// that is provided.
		static void ModelToWorld(Model & model);
		static void ModelToCam(Model & model, bool shouldclip = false);
		static void ModelToVP(Model & model);
		static void WorldToCam(Model & model);
		static void CamToVP(Model & viewmodel);
		static void VPToCam(Model & vpmodel);
		static void CamToWorld( Model & model);

		#pragma region // GETTERS & SETTERS (PROVIDED)

		// Set the model to world matrix
		static void		SetModelToWorld(const AEMtx33 & worldMatrix)		{ mModelWorldMatrix = worldMatrix; }
		static void		SetCamera(const Camera2D & camera)					{ mCamera = camera; }
		static AEMtx33	GetModelToWorld()									{ return mModelWorldMatrix; }
		static Camera2D	GetCamera()											{ return mCamera; }

		static void EnableClipping(bool enabled)							{ mbClippingEnabled = enabled; }
		static bool ClippingEnabled()										{ return mbClippingEnabled; }

		#pragma endregion

	private:
		static AEMtx33	mModelWorldMatrix;	// Model-to-world matrix	
		static Camera2D mCamera;			// The current camera
		static bool		mbClippingEnabled;		// Enable Clipping
	};
}

#endif