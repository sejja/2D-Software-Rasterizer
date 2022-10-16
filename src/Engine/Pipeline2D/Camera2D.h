#ifndef _CAMERA2D_H_
#define _CAMERA2D_H_

namespace Rasterizer
{
	class Camera2D
	{
	private:
		AEVec2	 mPosition;		// Camera position
		float	 mOrientation;	// Camera orientation ( in radians)
		AEVec2	 mViewRect;		// View window rectangle (x = width, y = height)
		Viewport mViewport;		// Viewport
		bool	 mClearViewport;// Should we clear the camera
	public:

		// PROVIDED
		// ------------------------------------------------------------------------
		/// \fn		Camera2D (default constructor)
		/// \brief	Constructs a camera at (0,0), (1,0), (0,1), size of the screen
		Camera2D()
			: mPosition(0.0f, 0.0f)
			, mOrientation(0.0f)
			, mViewRect((f32)gAESysWinWidth, (f32)gAESysWinHeight)
			, mClearViewport(true)
		{}

		// PROVIDED
		// ------------------------------------------------------------------------
		/// \fn		Camera2D (custom constructor)
		/// \brief	Constructs a camera with the speficied data
		/// \param	position	Camera position
		/// \param	u			Camera right axis
		/// \param	v			Camera up axis
		/// \param	viewRect	Camera view rectangle dimensions (x = width, y = height)
		Camera2D(const AEVec2 & position, float orientation, const AEVec2 & viewRect)
			: mPosition(position)
			, mOrientation(orientation)
			, mViewRect(viewRect)
			, mClearViewport(true)
		{}


		// TODO
		// ------------------------------------------------------------------------
		/// \fn		MoveBy
		/// \brief	translates the camera by the specified translation
		/// \param	translation the amount the camera needs to move. 
		void	MoveBy(const AEVec2 & translation);

		// TODO
		// ------------------------------------------------------------------------
		/// \fn		ComputeMatrix
		/// \brief	Computes the world to camera matrix using the orthogonal method 
		///			seen in class. Additionally, the matrix needs to transform 
		///			the world points in NDC space.
		AEMtx33 ComputeMatrix();

		// TODO
		// ------------------------------------------------------------------------
		/// \fn		ComputeInvMatrix
		/// \brief	Computes the camera to world matrix. You cannot use AEMtx33Inverse
		///			to compute the matrix
		AEMtx33 ComputeInvMatrix();

		// TODO
		// ------------------------------------------------------------------------
		/// \fn		Clip
		/// \brief	Clips the given model against the sides of the camera
		/// \param	model Model given in NDC space. 
		void Clip(Model & model);
		
		// ------------------------------------------------------------------------
		#pragma region // GETTERS & SETTERS (PROVIDED)

		// Set Camera position
		AEVec2	GetPosition()const						{		return mPosition;			}
		void	SetPosition(const AEVec2 & newPos)		{		mPosition;					}

		// Camera Orientation
		float	GetRotation()const						{ return mOrientation;				}	
		void	SetRotation(float angleRads)			{ mOrientation = angleRads;			}
		void	RotateBy(float angleRads)				{ mOrientation += angleRads;		}
		float	GetRotationDegrees()const				{ return AERadToDeg(mOrientation);	}
		void	SetRotationDegrees(float angleDegrees)	{ mOrientation = AEDegToRad(angleDegrees); }
		void	RotateByDegrees(float angleDegrees)		{ mOrientation += AEDegToRad(angleDegrees); }

		// Get/Set View Window Rect
		void SetViewRect(const AEVec2 & dimensions)		{		mViewRect = dimensions;		}
		AEVec2 GetViewRect()const						{		return mViewRect;			}

		// Get/Set width only
		void SetViewRectWidth(float width)				{		mViewRect.x = width;		}
		f32	 GetViewRectWidth()const					{		return mViewRect.x;			}

		// Get/Set height only
		void SetViewRectHeight(float height)			{		mViewRect.y = height;		}
		f32  GetViewRectHeight()const					{		return mViewRect.y;			}
		
		// Viewport getters/setters
		void SetViewport(const Viewport & viewport)		{		mViewport = viewport;		}
		Viewport& GetViewport()							{		return mViewport;			}

		bool GetClearViewportEnabled()const				{		return mClearViewport;		}
		void SetClearViewportEnabled(bool enabled)		{		mClearViewport = enabled;	}
		#pragma endregion
	};
}

#endif