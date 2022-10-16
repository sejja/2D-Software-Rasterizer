#include <AEEngine.h>		// AEMtx33
#include "..\Rasterizer\Rasterizer.h"
#include "..\Pipeline2D\Pipeline2D.h"

namespace Rasterizer
{
	// ------------------------------------------------------------------------
	// STATIC MEMBER VARIABLES
	AEMtx33		Pipeline2D::mModelWorldMatrix;			// Model-to-world matrix	
	Camera2D	Pipeline2D::mCamera;					// The current camera
	bool		Pipeline2D::mbClippingEnabled = false;	// Enable Clipping

	// ------------------------------------------------------------------------
	
	void Pipeline2D::ModelToWorld(Model & model)
	{
		// Create a transformed Model into view
		model.Transform(mModelWorldMatrix);
	}

	// ------------------------------------------------------------------------
	
	void Pipeline2D::ModelToCam(Model & model, bool shouldclip)
	{
		// Get the model-view and viewport matrices
		AEMtx33 camMtx = mCamera.ComputeMatrix();
		AEMtx33 mvp = camMtx * mModelWorldMatrix;

		// Transform model
		model.Transform(mvp);
		if (shouldclip)
			mCamera.Clip(model);
	}

	// ------------------------------------------------------------------------
	
	void Pipeline2D::ModelToVP(Model & model)
	{
		// Get the model-view and viewport matrices
		AEMtx33 camMtx = mCamera.ComputeMatrix();
		AEMtx33 vpMtx = mCamera.GetViewport().ComputeMatrix();
		AEMtx33 mvp = vpMtx * camMtx * mModelWorldMatrix;

		// Transform model
		model.Transform(mvp);
	}

	// ------------------------------------------------------------------------
	
	void Pipeline2D::WorldToCam(Model & worldmodel)
	{
		// Get the model-view and viewport matrices
		AEMtx33 camMtx = mCamera.ComputeMatrix();

		// Create a transformed Model into view
		worldmodel.Transform(camMtx);
	}

	// ------------------------------------------------------------------------
	
	void Pipeline2D::CamToVP(Model & viewmodel)
	{
		// Get the model-view and viewport matrices
		AEMtx33 vpMtx = mCamera.GetViewport().ComputeMatrix();

		// Create a transformed Model into view
		viewmodel.Transform(vpMtx);
	}

	// ------------------------------------------------------------------------
	
	void Pipeline2D::VPToCam(Model & vpmodel)
	{
		// Get the model-view and viewport matrices
		AEMtx33 invVPMatrix = mCamera.GetViewport().ComputeInvMatrix();
		
		// Create a transformed Model into view
		vpmodel.Transform(invVPMatrix);
	}

	// ------------------------------------------------------------------------
	
	void Pipeline2D::CamToWorld(Model & viewmodel)
	{
		// Get the model-view and viewport matrices
		AEMtx33 invCamMtx = mCamera.ComputeInvMatrix();
		
		// Create a transformed Model into view
		viewmodel.Transform(invCamMtx);
	}
	
	// ------------------------------------------------------------------------
	
	void Pipeline2D::DrawModel(const Model & model, ShaderFunc shader, void * shaderData)
	{
		// input model is empty
		if (model.GetVtxCount() == 0)
			return;

		// copy the model
		Model copyModel(model);

		// Create a transformed Model into view
		ModelToCam(copyModel);

		// Clip only if triangle list
		if (ClippingEnabled() && model.GetPrimitiveType() == Model::ePT_TRIANGLE_LIST)
			mCamera.Clip(copyModel);

		// clipped model is empty
		if (copyModel.GetVtxCount() == 0)
			return;

		// Transform to viewport
		CamToVP(copyModel);

		// Draw the model
		copyModel.Draw(shader, shaderData);
	}

} // namespace Rasterizer