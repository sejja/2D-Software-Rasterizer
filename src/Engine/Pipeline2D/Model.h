#ifndef _MODEL_H_
#define _MODEL_H_

#include "../Rasterizer/DrawTriangle.h"

namespace Rasterizer
{
	enum EAttributeUsage { AU_POSITION, AU_TEX_COORD, AU_COLOR, AU_CUSTOM };

	/// PROVIDED
	/// -----------------------------------------------------------------------
	/// \class	AttributeDef
	/// \brief	Stores the necessary information to retrieve any vertex
	///			attributes from a 1-D array of floats.
	struct AttributeDef
	{
		u32 mStartOffset;			// Where is the first element of the attribute?
		u32 mAttributeSize;			// Attribute size (in floats)
		u32 mStride;				// Stride to the next element (in floats)
		EAttributeUsage mUsage; 	// Usage of the attribute (what does this attribute represent?) 
	};

	/// PROVIDED
	/// -----------------------------------------------------------------------
	/// \class	VertexFormat
	/// \brief	Container of AttributeDef class. It is implemetend as a derived
	///			class of an std::vector<> and so inherits all its functionality.
	///			Additionally, a method is provided which returns the total count 
	///			in floats that make up one vertex.  
	struct VertexFormat : public std::vector<AttributeDef>
	{

		/// -------------------------------------------------------------------
		/// \brief	Returns the total count (in floats) that make up one vertex.
		///			E.g. if the vertex contains position and color (regardless 
		///			of whether they are laid out in memory in an interleaved or 
		///			SOA manner), GetAttributeCount() will return 6 (2 for XY and 
		///			4 for RGBA).
		u32 GetAttributeCount() const
		{
			u32 attCount = 0;
			for (u32 i = 0; i < size(); ++i)
			{
				attCount += (*this)[i].mAttributeSize;
			}
			return attCount;
		}

		//	\brief	Returns the index of the attribute matching the specified usage
		//			If none is found, it will return unsigned -1 (0xFFFFFFFF). 
		u32 GetAttributeIndex(EAttributeUsage usage) const
		{
			// loop to find the usage
			for (u32 i = 0; i < size(); ++i)
			{
				if ((*this)[i].mUsage == usage)
					return i;
			}

			// -1 is 0xFFFFFFFF for a u32, so if we return this value, 
			// it means that the attribute doesn't exist
			return -1;
		}
	};

	/// PROVIDED
	/// -----------------------------------------------------------------------
	/// \class	Model
	/// \brief	Interface for an indexed vertex array defined in model space. 
	///			It uses the above VertexFormat class to allow users to define
	///			any vertex format. 
	class Model
	{
	public:
		/// PROVIDED
		/// -------------------------------------------------------------------
		/// \enum	EPrimitiveType
		/// \brief	Defines how the model should be drawn. See the Draw... function
		///			for more details.
		enum EPrimitiveType { ePT_TRIANGLE_LIST, ePT_LINE_LIST };

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Sets the primitive type to the specified one. Sets all other
		///			data to 0 and NULL. 
		Model(EPrimitiveType primType = ePT_TRIANGLE_LIST);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Deep copies (i.e. it allocates its own memory) the given model 
		///			into this model.
		Model(const Model& rhs);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Frees allocated vertex and index memory, if necessary. 
		~Model();

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	assignment operator. Copies the given model into this one.
		Model& operator =(const Model& rhs);

		/// TODO
		/// -------------------------------------------------------------------
		/// \brief	Allocates vertex data (float array) based on the specified
		///			vertex count and format. 
		/// \note	This should also free any current vertex data, if any
		f32* AllocVtx(u32 vtxCount, VertexFormat format);

		/// TODO
		/// -------------------------------------------------------------------
		/// \brief	Allocates index data (u16 array) based on the specified
		///			index count.
		/// \note	This should also free any current vertex data, if any
		u16* AllocIdx(u32 idxCount);

		/// TODO
		/// -------------------------------------------------------------------
		/// \brief	Allocates vertex and index data at once. Essentially a 
		///			wrapper around the two functions above. 
		void Alloc(u32 vtxCount, u32 idxCount, VertexFormat format);

		/// TODO
		/// -------------------------------------------------------------------
		/// \brief	Frees vertex data, if any.
		void FreeVtxData();

		/// TODO
		/// -------------------------------------------------------------------
		/// \brief	Frees index data, if any.
		void FreeIdxData();

		/// TODO
		/// -------------------------------------------------------------------
		/// \brief	Frees both vertex and index data. Essentially a wrapper 
		///			function around the two functions above
		void Free();

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Returns the address of the vertex specified by 'vtx' and
		///			'att'.
		f32* GetVtxPtr(u32 vtx = 0, u32 att = 0) const;

		// TODO
		/// -------------------------------------------------------------------
		// versions of the getters from the functions above. 
		f32* GetVtxPtr(EAttributeUsage usage, u32 vtx = 0) const;
		AEVec2 GetVtxPos(u32 vtx = 0) const;
		AEVec2 GetVtxUV(u32 vtx = 0) const;
		Color GetVtxColor(u32 vtx = 0) const;

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Sets the attribute value specified by 'att' of the vertex
		///			specified by 'vtx'. 
		/// \note	This function MUST CALL SetVtxRange below.
		void SetVtx(u32 vtx, u32 att, const f32* val);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Sets the values of the specified range of attributes of the 
		///			vertex specified by 'vtx'. 
		/// \note	This function MUST CALL SetVtxRange below.
		void SetVtx(u32 vtx, u32 att_range_start, u32 att_range_end, const f32* val);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Sets the values of the specified attribute on the range of 
		///			vertices. 
		/// \note	This function MUST CALL SetVtxRange below.
		void SetVtxRange(u32 vtx_range_start, u32 vtx_range_end, u32 att, const f32* val);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Sets the values of the range of attributes on the range of 
		///			vertices.
		///			You are safe to assume that val represents the vertex data 
		///			in an interleaved format and consecutive. To be clear, this 
		///			format is independent from the Model format. So it's possible 
		///			that, internally the vertex data of the model is laid out as
		///			SOA, however, 'val' will always be interleaved. 
		///			Also, vtx_range_start corresponds to vertex index 0 in 'val'
		///			Similarly, att_range_start corresponds to attribute index 0 in 
		///			the vertices of 'val'.
		void SetVtxRange(u32 vtx_range_start, u32 vtx_range_end, u32 att_range_start, u32 att_range_end, const f32* val);

		// TODO
		// -------------------------------------------------------------------
		// Versions of the functions above using attribute usage. 
		void SetVtx(u32 vtx, EAttributeUsage usage, const f32* val);
		void SetVtxPos(u32 vtx, const AEVec2& pos);
		void SetVtxUV(u32 vtx, const AEVec2& uv);
		void SetVtxColor(u32 vtx, const Color& col);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Sets the index buffer at index 'idx' to 'val'.
		void SetIdx(u32 idx, u16 val);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Sets the range of indices specified by idx_start and idx_end
		///			Note that idx_start corresponds to index 0 in 'vals'.
		void SetIdxRange(u32 idx_start, u32 idx_end, const u16* vals);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Returns the value of the index buffer at index 'idx'.
		u16  GetIdx(u32 idx);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	returns the memory address of an element in the index buffer.
		u16* GetIdxPtr(u32 idx = 0);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Draws the model using any of the function below based on the
		///			internal primitive type. Essentially a wrapper function.
		void Draw(ShaderFunc shader = NULL, void* shaderData = NULL);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Draws the model as a triangle list. Each triangle is 
		///			"assembled" from the index buffer [i, i+1, i+2]. 
		///			Since this function calls DrawTriangleShaded, the vertex data
		///			must be copied into an interleaved vertex array before 
		///			passing it to DrawTriangleShaded. 
		void DrawAsTriangles(ShaderFunc shader, void* shaderData);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Draws the model as a line list. Each line is "assembled"
		///			from the index buffer [i, i+1]. 
		///			For the sake of simplicity, this function calls DrawLine
		///			using a black color by default. 
		///			'shader' and 'shaderData' are ignored.
		void DrawAsLines(ShaderFunc shader, void* shaderData);

		/// TODO
		/// -------------------------------------------------------------------
		///	\brief	Transforms each of the vertices xy coordinates by the provided
		///			matrix. 
		void Transform(const AEMtx33& mtx);

		/// PROVIDED
		/// -------------------------------------------------------------------
		/// \brief GETTERS AND SETTERS
		EPrimitiveType GetPrimitiveType() const { return mPrimType; }
		EPrimitiveType GetPrimitiveType() { return mPrimType; }
		void SetPrimitiveType(EPrimitiveType primType) { mPrimType = primType; }
		u32  GetVtxCount()const { return mVtxCount; }
		u32  GetIdxCount()const { return mIdxCount; }
		const VertexFormat& GetFormat()const { return mFormat; }

	private:
		f32* mVtxData;
		u16* mIdxData;
		u32				mVtxCount;
		u32				mIdxCount;
		VertexFormat	mFormat;
		EPrimitiveType	mPrimType;
	};
}

#endif