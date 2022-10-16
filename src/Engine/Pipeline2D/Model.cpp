//
//  DrawTriangle.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 04/07/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include "../Rasterizer/DrawTriangle.h"
#include "../Rasterizer/Color.h"
#include "src\Engine\Pipeline2D\Model.h"

#define INVALID_ACCESS	-1

namespace Rasterizer {

	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*   Constructs a Model Class with the Primitive type information
	*/ // --------------------------------------------------------------------
	Model::Model(EPrimitiveType primType) :
		mVtxData(nullptr), mIdxData(nullptr), mVtxCount(0), mPrimType(primType) {

	}

	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*   Deep Copies a model class
	*/ // --------------------------------------------------------------------
	Model::Model(const Model& rhs) {

		*this = rhs;

	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Frees the Model Data
	*/ // --------------------------------------------------------------------
	Model::~Model() {

		//if we have Vertex Data
		if(mVtxData)

			//Delete the Vertex Data
			delete [] mVtxData;
		
		//If we have Index Data
		if(mIdxData)

			//Delete the Index Data
			delete [] mIdxData;

	}

	// ------------------------------------------------------------------------
	/*! Operator = 
	*
	*   Frees the Model Data
	*/ // --------------------------------------------------------------------
	Model& Model::operator=(const Model& rhs) {

		//if we have Vertex Data
		if (mVtxData)

			//Delete the Vertex Data
			delete[] mVtxData;

		//If we have Index Data
		if (mIdxData)

			//Delete the Index Data
			delete[] mIdxData;

		//Copy the Format (Vector Assignment Operator does it for us)
		mFormat = rhs.mFormat;

		//Copy both Vertex Count and Index Count
		memcpy(&mVtxCount, &rhs.mVtxCount, sizeof(unsigned long) * 2);

		//Copy the Primitive Types
		mPrimType = rhs.mPrimType;

		//store the size of all rhs vertices
		const size_t talloc_ = mFormat.GetAttributeCount() * mVtxCount;

		//Allocate the Indexes
		mIdxData = new unsigned short[mIdxCount];

		//Allocate the data for all vertex
		mVtxData = new float[talloc_];

		//Copy the Index Data
		memcpy(mIdxData, rhs.mIdxData, mIdxCount * sizeof(unsigned short));

		//Copy the Vertex Data 
		memcpy(mVtxData, rhs.mVtxData, talloc_ * sizeof(float));

		//returns this
		return *this;
	
	}

	// ------------------------------------------------------------------------
	/*! Alloc Vtx
	*
	*   Allocates vertices
	*/ // --------------------------------------------------------------------
	f32* Model::AllocVtx(u32 vtxCount, VertexFormat format) {

		//If there is anything on mVtxData
		if(mVtxData)

			//Delete it
			delete[] mVtxData; 

		//Get the format
		mFormat = format;

		//Sets the Vertex Count
		mVtxCount = vtxCount;

		//Create a variable to store the Vertex Size
		const size_t talloc = mFormat.GetAttributeCount() * vtxCount;

		//Create space for all the vertex
		mVtxData = new float[talloc];
		
		//Set the new memory
		memset(mVtxData, INT_MAX, talloc * sizeof(float));

		//return the new Vertex Data
		return mVtxData;

	}

	// ------------------------------------------------------------------------
	/*! Alloc Idx
	*
	*   Allocates Indeces
	*/ // --------------------------------------------------------------------
	u16* Model::AllocIdx(u32 idxCount) {

		//If there was previous Index Data
		if (mIdxData)
			
			//delete said data
			delete[] mIdxData;
		
		//Sets the Index Count
		mIdxCount = idxCount;

		//Allocates the Indeces
		mIdxData = new unsigned short[idxCount];

		//Set the new allocated memory
		memset(mIdxData, INT_MAX, idxCount * sizeof(unsigned short));

		//Return the Index Data
		return mIdxData;

	}

	// ------------------------------------------------------------------------
	/*! Alloc
	*
	*   Allocates the memory for the Model Class
	*/ // --------------------------------------------------------------------
	void Model::Alloc(u32 vtxCount, u32 idxCount, VertexFormat format) {
		
		//Allocate the Indeces
		AllocIdx(idxCount);

		//Allocate the Vertices
		AllocVtx(vtxCount, format);

	}

	// ------------------------------------------------------------------------
	/*! Free Vertex Data
	*
	*   Frees the Vertex Data
	*/ // --------------------------------------------------------------------
	void Model::FreeVtxData() {

		//if we have Vertex Data
		if (mVtxData)

			//Delete the Vertex Data
			delete[] mVtxData;

		//Sets the Vertex Data to Null
		mVtxData = nullptr;

		//Sets the Vertex Count to 0
		mVtxCount = 0;

	}

	// ------------------------------------------------------------------------
	/*! Free Index Data
	*
	*   Frees the Index Data
	*/ // --------------------------------------------------------------------
	void Model::FreeIdxData() {

		//If we have Index Data
		if (mIdxData)

			//Delete the Index Data
			delete[] mIdxData;

		//Sets the Index Data to Null
		mIdxData = nullptr;

		//Sets the Index Count to 0
		mIdxCount = 0;

	}

	// ------------------------------------------------------------------------
	/*! Free
	*
	*   Frees all Model Data
	*/ // --------------------------------------------------------------------
	void Model::Free() {

		//Free Index Data
		FreeIdxData();

		//Free Vertex Data
		FreeVtxData();

	}

	// ------------------------------------------------------------------------
	/*! Get Vertex Pointer
	*
	*   Gets the Pointer of the Vertex given the attribute offset
	*/ // --------------------------------------------------------------------
	f32* Model::GetVtxPtr(u32 vtx, u32 att) const {

		//if we are going out of bounds
		if(vtx >= mVtxCount)

			//Return null pointer
			return nullptr;

		//return the attribute pointer for the vertex
		return GetVtxPtr(mFormat[att].mUsage, vtx);

	}

	// ------------------------------------------------------------------------
	/*! Get Vertex Pointer
	*
	*   Gets the Pointer of the Vertex given the usage
	*/ // --------------------------------------------------------------------
	f32* Model::GetVtxPtr(EAttributeUsage usage, u32 vtx) const {

		//For all formats
		for (AttributeDef x : mFormat)

			//If we found the usage
			if(x.mUsage == usage)
				
				//return the Attribute / Vertex
				return mVtxData + (x.mStride * vtx) + x.mStartOffset;

		//Return nullptr
		return nullptr;

	}

	// ------------------------------------------------------------------------
	/*! Get Vertex Position
	*
	*   Gets the Position of a Vertex
	*/ // --------------------------------------------------------------------
	AEVec2 Model::GetVtxPos(u32 vtx) const {

		//For all formats
		for (AttributeDef x : mFormat)

			//If we found the usage
			if (x.mUsage == EAttributeUsage::AU_POSITION)

				//return the Attribute / Vertex
				return *reinterpret_cast<AEVec2*>(mVtxData + (x.mStride * vtx) + x.mStartOffset);

		//Return not found
		return {INVALID_ACCESS, INVALID_ACCESS};

	}

	// ------------------------------------------------------------------------
	/*! Get Vertex UV
	*
	*   Gets the UV of a Vertex
	*/ // --------------------------------------------------------------------
	AEVec2 Model::GetVtxUV(u32 vtx) const {

		//For all formats
		for (AttributeDef x : mFormat)

			//If we found the usage
			if (x.mUsage == EAttributeUsage::AU_TEX_COORD)

				//Store the offset
				return *reinterpret_cast<AEVec2*>(mVtxData + (x.mStride * vtx) + x.mStartOffset);

		//Return not found
		return {INVALID_ACCESS, INVALID_ACCESS};

	}

	// ------------------------------------------------------------------------
	/*! Get Vertex Color
	*
	*   Gets the Color of a Vertex
	*/ // --------------------------------------------------------------------
	Color Model::GetVtxColor(u32 vtx) const {

		//For all formats
		for (AttributeDef x : mFormat)

			//If we found the usage
			if (x.mUsage = EAttributeUsage::AU_COLOR)

				//return the Attribute / Vertex
				return *reinterpret_cast<Color*>(mVtxData + (x.mStride * vtx) + x.mStartOffset);

		return {INVALID_ACCESS, INVALID_ACCESS, INVALID_ACCESS, INVALID_ACCESS};

	}

	// ------------------------------------------------------------------------
	/*! Set Vertex
	*
	*   Sets the Attributes of a vertex
	*/ // --------------------------------------------------------------------
	void Model::SetVtx(u32 vtx, u32 att, const f32* val) {

		SetVtxRange(vtx, vtx, att, att, val);

	}

	// ------------------------------------------------------------------------
	/*! Get Vertex Color
	*
	*   Gets the Color of a Vertex
	*/ // --------------------------------------------------------------------
	void Model::SetVtx(u32 vtx, u32 att_range_start, u32 att_range_end, const f32* val) {

		SetVtxRange(vtx, vtx, att_range_start, att_range_end, val);

	}

	// ------------------------------------------------------------------------
	/*! Set Vtx Range
	*
	*   Sets the Vertex with an array of floats
	*/ // --------------------------------------------------------------------
	void Model::SetVtxRange(u32 vtx_range_start, u32 vtx_range_end, u32 att, const f32* val) {

		//Pass to explicit Function
		SetVtxRange(vtx_range_start, vtx_range_end, att, att, val);

	}

	// ------------------------------------------------------------------------
	/*! Set Vtx Range
	*
	*   Sets the Vertex with an array of floats
	*/ // --------------------------------------------------------------------
	void Model::SetVtxRange(u32 vtx_range_start, u32 vtx_range_end, u32 att_range_start, u32 att_range_end, const f32* val) {

		//Store the vertex size, current index and range
		size_t talloc_ = 0, i = att_range_start;

		//Get the size of the range of attributes
		for(; i <= att_range_end; i++)
			talloc_ += mFormat[i].mAttributeSize;
		
		//Store a pointer to the source (we might change it later)
		float* Src_ = const_cast<float*>(val);

		//For every attribute
		for (size_t j = att_range_start; j <= att_range_end;
			Src_ += mFormat[j].mAttributeSize, j++) {

			//Update the Destiny Position
			float* const Dst_ = mVtxData + mFormat[j].mStartOffset;
			
			//Compute the size of the attribute (in Bytes)
			const size_t sz = mFormat[j].mAttributeSize * sizeof(float);

			//For every vertex
			for (i = vtx_range_start; i <= vtx_range_end; i++)
				
				//Copy the right memory into the right place in Vtx Data
				memcpy(Dst_ + (i * mFormat[j].mStride),
					Src_ + ((i - vtx_range_start) * talloc_),
					sz);

		}
	}

	// ------------------------------------------------------------------------
	/*! Set Vtx
	*
	*   Sets the Vertex attribute
	*/ // --------------------------------------------------------------------
	void Model::SetVtx(u32 vtx, EAttributeUsage usage, const f32* val) {

		//switch by usage
		switch (usage) {

		//If we are setting a position
		case Rasterizer::AU_POSITION:

			//Set the Vertex Position
			SetVtxPos(vtx, *reinterpret_cast<const AEVec2*>(val));
			
			//return
			break;

		//If we are setting a texture coordinate
		case Rasterizer::AU_TEX_COORD:

			//Set the Vertex UV
			SetVtxUV(vtx, *reinterpret_cast<const AEVec2*>(val));
			
			//return
			break;

		//If we are setting a color
		case Rasterizer::AU_COLOR:

			//Set the Vertex Color
			SetVtxColor(vtx, *reinterpret_cast<const Color*>(val));
			
			//return
			break;

		//Any other case
		default:

			//return
			break;
		}
	}

	// ------------------------------------------------------------------------
	/*! Set Vertex Position
	*
	*   Sets the Position of a Vertex
	*/ // --------------------------------------------------------------------
	void Model::SetVtxPos(u32 vtx, const AEVec2& pos) {

		//For all formats
		for (AttributeDef x : mFormat)

			//if the Attribute is a position
			if (x.mUsage == EAttributeUsage::AU_POSITION) {

				//Copy the data into the right place
				memcpy(mVtxData + (vtx * x.mStride) + x.mStartOffset, &pos, sizeof(AEVec2));

				//Return
				return;

			}
	}

	// ------------------------------------------------------------------------
	/*! Set Vertex UV
	*
	*   Sets the UV of a Vertex
	*/ // --------------------------------------------------------------------
	void Model::SetVtxUV(u32 vtx, const AEVec2& uv) {

		//For all formats
		for (AttributeDef x : mFormat)

			//if the Attribute is a UV
			if (x.mUsage == EAttributeUsage::AU_TEX_COORD) {

				//Copy the data into the right place
				memcpy(mVtxData + (vtx * x.mStride) + x.mStartOffset, &uv, sizeof(AEVec2));

				//return
				return;

			}
	}

	// ------------------------------------------------------------------------
	/*! Set Vertex Color
	*
	*   Sets the Color of a Vertex
	*/ // --------------------------------------------------------------------
	void Model::SetVtxColor(u32 vtx, const Color& col) {

		//For all formats
		for (AttributeDef x : mFormat)

			//if the Attribute is a color
			if (x.mUsage == EAttributeUsage::AU_COLOR) {

				//Copy the data into the right place
				memcpy(mVtxData + (vtx * x.mStride) + x.mStartOffset, &col, sizeof(Color));

				//return
				return;

			}
	}

	// ------------------------------------------------------------------------
	/*! Set Index
	*
	*   Sets the Index of a Vertex
	*/ // --------------------------------------------------------------------
	void Model::SetIdx(u32 idx, u16 val) {

		//Sets the index to the value
		mIdxData[idx] = val;

	}

	// ------------------------------------------------------------------------
	/*! Set Index Range
	*
	*   Sets the Indexes on a Range of a Vertex
	*/ // --------------------------------------------------------------------
	void Model::SetIdxRange(u32 idx_start, u32 idx_end, const u16* vals) {

		//copies the values on the range
		memcpy(mIdxData + idx_start, vals, ((idx_end - idx_start) + 1) * 
			sizeof(unsigned short));

	}

	// ------------------------------------------------------------------------
	/*! Get Index
	*
	*   Gets the Index of a Vertex
	*/ // --------------------------------------------------------------------
	u16 Model::GetIdx(u32 idx) {

		//If we are going out of bounds
		if(idx >= mIdxCount)

			//return invalid access
			return INVALID_ACCESS;

		//Return the Vertex Index
		return mIdxData[idx];

	}

	// ------------------------------------------------------------------------
	/*! Get Index pointer
	*
	*   Gets the Index (as a pointer) of a Vertex
	*/ // --------------------------------------------------------------------
	u16* Model::GetIdxPtr(u32 idx) {

		//return the pointer to the index
		return mIdxData + idx;
	
	}

	// ------------------------------------------------------------------------
	/*! Draw
	*
	*   Draws a Model
	*/ // --------------------------------------------------------------------
	void Model::Draw(ShaderFunc shader, void* shaderData) {

		//Switch by Primitive Type
		switch (mPrimType) {

		//If we are drawing triangles
		case Rasterizer::Model::ePT_TRIANGLE_LIST:
			
			//Draw triangles
			DrawAsTriangles(shader, shaderData);
			
			//break;
			break;

		//of we are drawing lines
		case Rasterizer::Model::ePT_LINE_LIST:
			
			//Draw lines
			DrawAsLines(shader, shaderData);
			
			//break
			break;

		//Any other case
		default:

			//Draw nothing
			break;

		}
	}

	// ------------------------------------------------------------------------
	/*! Draw As Triangles
	*
	*   Draws a Model as triangles
	*/ // --------------------------------------------------------------------
	void Model::DrawAsTriangles(ShaderFunc shader, void* shaderData) {

		//Store the Vertex Size and Offset
		size_t talloc_ = mFormat.GetAttributeCount();
		
		//Assemble parameter floats
		float* assembly = new float[talloc_ * 3];

		//For every triangle that we can draw
		for (size_t i = 0; i < mIdxCount; i += 3) {

			//Store the offset in a variable 
			size_t offset = sizeof(AEVec2) / sizeof(float);

			//For every attribute
			for (AttributeDef x : mFormat) {

				//If it's a position
				if (x.mUsage == EAttributeUsage::AU_POSITION) {

					//Copy The first vertex to the assembly data
					memcpy(assembly, mVtxData + x.mStartOffset + (mIdxData[i] * x.mStride), 
						x.mAttributeSize * sizeof(float));

				//else
				} else {

					//Copy The first vertex to the assembly data
					memcpy(assembly + offset, mVtxData + x.mStartOffset + (mIdxData[i] * x.mStride),
						x.mAttributeSize * sizeof(float));

					//Update the offset
					offset += x.mAttributeSize;

				}
			}

			//Update the offset
			offset += sizeof(AEVec2) / sizeof(float);

			//For every attribute
			for (AttributeDef x : mFormat) {

				//If it's a position
				if (x.mUsage == EAttributeUsage::AU_POSITION) {

					//Copy The first vertex to the assembly data
					memcpy(assembly + talloc_, mVtxData + x.mStartOffset + (mIdxData[i + 1] * x.mStride),
						x.mAttributeSize * sizeof(float));

				//else
				} else {

					//Copy The first vertex to the assembly data
					memcpy(assembly + offset, mVtxData + x.mStartOffset + (mIdxData[i + 1] * x.mStride),
						x.mAttributeSize * sizeof(float));

					//Update the offset
					offset += x.mAttributeSize;

				}
			}

			//Update the offset
			offset += sizeof(AEVec2) / sizeof(float);

			//For every attribute
			for (AttributeDef x : mFormat) {

				//If it's a position
				if (x.mUsage == EAttributeUsage::AU_POSITION) {

					//Copy The first vertex to the assembly data
					memcpy(assembly + (2 * talloc_), mVtxData + x.mStartOffset + (mIdxData[i + 2] * x.mStride),
						x.mAttributeSize * sizeof(float));

				//else
				} else {

					//Copy The first vertex to the assembly data
					memcpy(assembly + offset, mVtxData + x.mStartOffset + (mIdxData[i + 2] * x.mStride),
						x.mAttributeSize * sizeof(float));

					//Update the offset
					offset += x.mAttributeSize;

				}
			}

			//Draw the Shaded Triangle
			DrawTriangleShaded(assembly, talloc_, shader, shaderData);

		}

		//Destroy the Assembly Data
		delete [] assembly;

	}

	// ------------------------------------------------------------------------
	/*! Draw As Lines
	*
	*   Draws a Model as lines
	*/ // --------------------------------------------------------------------
	void Model::DrawAsLines(ShaderFunc shader, void* shaderData) {

		//Store the Vertex Size and Offset
		size_t StrideP, StrideC, offsetP_;
		int offsetC_ = -1;

		//For all formats
		for (AttributeDef x : mFormat) {

			//switch by the usage
			switch (x.mUsage) {

			//If it's a position
			case AU_POSITION:
				//Store the Position Offset
				offsetP_ = x.mStartOffset;

				//Store the Position Stride
				StrideP = x.mStride;

				//Break
				break;

			//If it's a color
			case AU_COLOR:
				//Store the Color Offset
				offsetC_ = x.mStartOffset;
				
				//Store the Color Stride
				StrideC = x.mStride;

				//break
				break;

			//Otherwise
			default:

				//break
				break;
			}
		}

		//For every triangle that we can draw
		for (size_t i = 0; i < mIdxCount; i += 2)

			//Draw the Lines
			DrawLine(*reinterpret_cast<AEVec2*>(mVtxData + (mIdxData[i] * StrideP) + offsetP_),
				*reinterpret_cast<AEVec2*>(mVtxData + (mIdxData[i + 1] * StrideP) + offsetP_),
				offsetC_ >= 0 ? *reinterpret_cast<Color*>(mVtxData + (mIdxData[i] * StrideC) + offsetC_) :
					Color());

	}

	// ------------------------------------------------------------------------
	/*! Transform
	*
	*   Transform each vertex by a certain matrix
	*/ // --------------------------------------------------------------------
	void Model::Transform(const AEMtx33& mtx) {

		//Store the index of the position attribute
		const size_t i_ = mFormat.GetAttributeIndex(AU_POSITION);

		//For each vertex
		for (size_t i = 0; i < mVtxCount; i++) {

			//Get the Index of each vertex
			AEVec2* idx = reinterpret_cast<AEVec2*>(mVtxData + (i * mFormat[i_].mStride) + mFormat[i_].mStartOffset);

			//Apply the Transformation
			*idx = mtx.MultVec(*idx);

		}
	}
}