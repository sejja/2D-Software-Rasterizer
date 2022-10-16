//
//  Camera2D.cpp
//   2D Rasterizer
//
//  Created by Diego Revilla on 08/07/2020
//  Copyright � 2020 . All rights reserved.
//

#include <AEEngine.h>
#include "..\Rasterizer\Rasterizer.h"
#include "Model.h"
#include "Viewport.h"
#include "Camera2D.h"
#include <vector>
#include <array>

namespace Rasterizer {

	// ------------------------------------------------------------------------
	/*! Move By
	*
	*   Adds a certain translation to the Camera
	*/ // --------------------------------------------------------------------
	void Camera2D::MoveBy(const AEVec2& translation) {

		//Add a translation to the Position of the Camera
		mPosition += translation;

	}

	// ------------------------------------------------------------------------
	/*! Compute Matrix
	*
	*   Computes the Matrix of a Camera
	*/ // --------------------------------------------------------------------
	AEMtx33 Camera2D::ComputeMatrix() {

		//Computes the Inverse of the transformation concatenation
		//given by: Translation(Inverse) * Rotation(Inverse) * Scale(Inverse)
		return AEMtx33::Scale(1.f / mViewRect.x, 1.f / mViewRect.y) *
			AEMtx33::RotRad(-mOrientation) *
			AEMtx33::Translate(-mPosition.x, -mPosition.y);

	}

	// ------------------------------------------------------------------------
	/*! Compute Inverse Matrix
	*
	*   Computes the Inverse Matrix of the Camera
	*/ // --------------------------------------------------------------------
	AEMtx33 Camera2D::ComputeInvMatrix() {

		//Computes the transformation concatenation
		//given by: Scale * Rotation * Translation
		return AEMtx33::Translate(mPosition.x, mPosition.y) *
			AEMtx33::RotRad(mOrientation) *
			AEMtx33::Scale(mViewRect.x, mViewRect.y);

	}

	//Make defines to make more readable code
#define INVALID_ACCESS	-1
#define X				 0
#define Y				 (X + 1)
#define R				 (Y + 1)
#define G				 (R + 1)
#define B				 (G + 1)
#define A				 (B + 1)

	//Create some typedef (just to make things cleaner)
	using edge = unsigned;
	using flag = BYTE;
	using Att_Tranf = std::array<float, 6>;
	using VertexData = std::pair<Att_Tranf, flag>;
	using Segment = std::pair<Att_Tranf, Att_Tranf>;

	/*********************************************/
	//
	//	Create Constexpr Expression for the edge
	//  bitwise operations
	//
	/*********************************************/
	static constexpr const edge LEFT_BTW = 1;
	static constexpr const edge RIGHT_BTW = LEFT_BTW << 1;
	static constexpr const edge BOTTOM_BTW = RIGHT_BTW << 1;
	static constexpr const edge TOP_BTW = BOTTOM_BTW << 1;
	
	// ------------------------------------------------------------------------
	/*! Compute Vertex Flags
	*
	*   Computes the Flags of the vertices given a list
	*/ // --------------------------------------------------------------------
	static const void ComputeVertexFlags(const std::vector<Att_Tranf>& in_list,
		std::vector<flag>& in_flags) {

		//Retrieve the size of the vector
		const size_t size_ = in_list.size();

		//resize the flag list to the size of the vertex list
		in_flags.resize(size_);

		//for every vertex in the list
		for (size_t i = 0; i < size_; i++) {

			//Retrieve the vertex list at the desired position
			Att_Tranf const& v = in_list[i];

			//Retrieve the flag too
			flag& f = in_flags[i];

			//SETS THE FLAG TO 0!!
			f = 0;

			/***********************************************/
			//
			//	CASES:
			//
			//  We are on the left side - Add 0001 to our flag
			//  We are on the right side - Add 0010 to our flag
			//  We are on the lower side - Add 0100 to our flag
			//  We are on the upper side - Add 1000 to our flag
			//
			/***********************************************/
			if (v[0] < -.5f)
				f |= LEFT_BTW;

			if (v[0] > .5f)
				f |= RIGHT_BTW;

			if (v[1] < -.5f)
				f |= BOTTOM_BTW;

			if (v[1] > .5f)
				f |= TOP_BTW;

		}
	}

	// ------------------------------------------------------------------------
	/*! Is Flag Outside Edge
	*
	*   Check if a vertex is outside the edges
	*/ // --------------------------------------------------------------------
	constexpr const bool inline IsFlagOutsideEdge(const flag flag, const edge ed) {

		/*given the flag, compute if its inside or outside with bitwise
			operations*/
		return (flag & ed) != 0;

	}

	// ------------------------------------------------------------------------
	/*! Trivial Acceptance
	*
	*   Return true if all the vertices are contained within the view
	*/ // --------------------------------------------------------------------
	const bool inline TrivialAcceptance(const std::vector<flag>& flags) {

		//For every flag in the flag list
		for (flag x : flags)

			//if it's a value different than 0
			if (x)

				//return false
				return false;

		//if all flags are 0, return true
		return true;

	}

	// ------------------------------------------------------------------------
	/*! Trivial Rejection
	*
	*   If all the vertices are outside a certain edge, return true
	*/ // --------------------------------------------------------------------
	constexpr const bool TrivialRejection(const std::vector<flag>& flags) {

		//For each Corner
		for (size_t i = 1; i <= TOP_BTW; i = i << 1) {

			//Store if a vertex is detected to be inside
			bool detected = false;

			//loop through every flag
			for (flag x : flags)

				//If the flag is not outside the edge
				if (!IsFlagOutsideEdge(x, i)) {

					//We detected a vertex inside our view
					detected = true;

					//Break this loop
					break;

				}

			//If we did not detect a vertex
			if (!detected)

				//return found
				return true;

		}

		//return not found
		return false;

	}

	// ------------------------------------------------------------------------
	/*! Operator |=
	*
	*   If all the vertices are outside a certain edge, return true
	*/ // --------------------------------------------------------------------
	const AEVec2 operator|=(const Segment& rhs, const Segment& lhs) {

		//Create two Normal vectors from the segments
		const AEVec2 N1(rhs.second[1] - rhs.first[1], rhs.first[0] - rhs.second[0]),
			N2(lhs.second[1] - lhs.first[1], lhs.first[0] - lhs.second[0]);

		//calculate the determinant between both normals
		const float determinant = (N1.x * N2.y) - (N2.x * N1.y);

		//If te determinant is not 0
		if (determinant)

			//return the intersection
			return (AEVec2(
				N2.y * (N1.x * (rhs.first[0]) + N1.y * (rhs.first[1])) -
				N1.y * (N2.x * (lhs.first[0]) + N2.y * (lhs.first[1])),
				N1.x * (N2.x * (lhs.first[0]) + N2.y * (lhs.first[1])) -
				N2.x * (N1.x * (rhs.first[0]) + N1.y * (rhs.first[1])))
				/ determinant);

		//else
		else

			//Return a generic Vector
			return AEVec2(INVALID_ACCESS, INVALID_ACCESS);

	}

	// ------------------------------------------------------------------------
	/*! Compute Intersection
	*
	*   Compute the Intersection between a line and the border
	*/ // --------------------------------------------------------------------
	Att_Tranf ComputeIntersection(const Segment& s, const edge e) {

		Att_Tranf a_;
		AEVec2 b_;
		float t_;

		//Switch by the edge we are checking
		switch (e) {

			//If we are on the left edge
		case LEFT_BTW:
		{

			//Get the intersection between the two segments
			b_ = Segment(s.second, s.first) |= Segment({ -.5f, -.5f }, { -.5f, .5f });

			//Get the T interpolating value
			t_ = (b_.x - s.second[X]) / (s.first[X] - s.second[X]);

			//Copy the position into the attribute transfer
			memcpy(&a_, &b_, sizeof(AEVec2));

			//Interpolate for the Red Value
			a_[R] = s.first[R] + (s.second[R] - s.first[R]) * t_;

			//Interpolate for the Green Value
			a_[G] = s.first[G] + (s.second[G] - s.first[G]) * t_;

			//Interpolate for the Blue Value
			a_[B] = s.first[B] + (s.second[B] - s.first[B]) * t_;

			//Interpolate for the Alpha Value
			a_[A] = s.first[A] + (s.second[A] - s.first[A]) * t_;

			//Return the intersection between the two segments
			return a_;

		}

		//If we are on the right edge
		case RIGHT_BTW:
		{

			//Get the intersection between the two segments
			b_ = s |= Segment({ .5f, .5f }, { .5f, -.5f });

			//Get the T interpolating value
			t_ = (b_.x - s.first[X]) / (s.second[X] - s.first[X]);

			//Copy the position into the attribute transfer
			memcpy(&a_, &b_, sizeof(AEVec2));

			//Interpolate for the Red Value
			a_[R] = s.first[R] + (s.second[R] - s.first[R]) * t_;

			//Interpolate for the Green Value
			a_[G] = s.first[G] + (s.second[G] - s.first[G]) * t_;

			//Interpolate for the Blue Value
			a_[B] = s.first[B] + (s.second[B] - s.first[B]) * t_;

			//Interpolate for the Alpha Value
			a_[A] = s.first[A] + (s.second[A] - s.first[A]) * t_;

			//Return the intersection between the two segments
			return a_;

		}

		//If we are on the bottom edge
		case BOTTOM_BTW:
		{

			//Get the intersection between the two segments
			b_ =  s |= Segment({ -.5f, -.5f }, { .5f, -.5f });

			//Get the T interpolating value
			t_ = (b_.x - s.first[Y]) / (s.second[Y] - s.first[Y]);

			//Copy the position into the attribute transfer
			memcpy(&a_, &b_, sizeof(AEVec2));
			
			//Interpolate for the Red Value
			a_[R] = s.first[R] + (s.second[R] - s.first[R]) * t_;

			//Interpolate for the Green Value
			a_[G] = s.first[G] + (s.second[G] - s.first[G]) * t_;

			//Interpolate for the Blue Value
			a_[B] = s.first[B] + (s.second[B] - s.first[B]) * t_;

			//Interpolate for the Alpha Value
			a_[A] = s.first[A] + (s.second[A] - s.first[A]) * t_;

			//Return the intersection between the two segments
			return a_;

		}

		//If we are on the upper edge
		default:
		{

			//Get the intersection between the two segments
			b_ =  s |= Segment({ -.5f, .5f }, { .5f, .5f });

			//Get the T interpolating value
			t_ = (b_.x - s.first[Y]) / (s.second[Y] - s.first[Y]);

			//Copy the position into the attribute transfer
			memcpy(&a_, &b_, sizeof(AEVec2));
			
			//Interpolate for the Red Value
			a_[R] = s.first[R] + (s.second[R] - s.first[R]) * t_;

			//Interpolate for the Green Value
			a_[G] = s.first[G] + (s.second[G] - s.first[G]) * t_;

			//Interpolate for the Blue Value
			a_[B] = s.first[B] + (s.second[B] - s.first[B]) * t_;

			//Interpolate for the Alpha Value
			a_[A] = s.first[A] + (s.second[A] - s.first[A]) * t_;

			//Return the intersection between the two segments
			return a_;

		}
		}
	}

	// ------------------------------------------------------------------------
	/*! Add All Vertices in Model
	*
	*   Adds All the Vertices to the output list
	*/ // --------------------------------------------------------------------
	void AddAllVerticesInModel(std::vector<Att_Tranf>& in_list,
		std::vector<Att_Tranf>& out_list) {

		//for every Vector in the input list
		for (Att_Tranf x : in_list)

			//Add said vector to the output list
			out_list.push_back(x);

	}

	// ------------------------------------------------------------------------
	/*! Simple Triangulation
	*
	*   Triangulates a list of Vertices
	*/ // --------------------------------------------------------------------
	std::vector<Att_Tranf> SimpleTriangulation(const std::vector<Att_Tranf>& in) {

		//Create an output list
		std::vector<Att_Tranf> out_list_;

		//Retrieve the traversal anchor (which is the size of the passed list
		//	minus one)
		size_t anchor_ = in.size() - 1;

		//Traversal
		for (size_t i = 0; i < anchor_; i++) {

			//Push back the first Vertex
			out_list_.push_back(in[0]);

			//Push back the iteration-matched Vertex
			out_list_.push_back(in[i]);

			//Push back the next vertex
			out_list_.push_back(in[i + 1]);

		}

		//Return the output list
		return out_list_;

	}

	// ------------------------------------------------------------------------
	/*! Clip
	*
	*   Clips a Model
	*/ // --------------------------------------------------------------------
	void Camera2D::Clip(Model& model) {

		//A list to save the output vertices
		std::vector<Att_Tranf> out_list;

		//for every index triangle in the model
		for (size_t i = 0; i < model.GetIdxCount(); i += 3) {

			//Declare two Attribute Transfer classes
			Att_Tranf v0, v1, v2;

			//Copy the Data into the Attribute Transfer classes
			memcpy(v0.data(), model.GetVtxPtr(model.GetIdx(i)), sizeof(float) * 6);
			memcpy(v1.data(), model.GetVtxPtr(model.GetIdx(i + 1)), sizeof(float) * 6);
			memcpy(v2.data(), model.GetVtxPtr(model.GetIdx(i + 2)), sizeof(float) * 6);

			//Create two list of attribute Transfer Classes
			std::vector<Att_Tranf> in_list, out_temp;

			//Create a list of flags
			std::vector<flag>  in_flags;

			//Push to the first attribute transfer classes the vertices
			in_list.push_back(v0);
			in_list.push_back(v1);
			in_list.push_back(v2);

			//Compute thte vertice flags
			ComputeVertexFlags(in_list, in_flags);

			//If all the vertices are all within the view
			if (TrivialAcceptance(in_flags)) {

				//Adds them to the output list
				AddAllVerticesInModel(in_list, out_list);
				
				//Continues with the next one
				continue;

			}

			//If the vertices are outside the view
			if (TrivialRejection(in_flags))

				//Skip this triangle and continue
				continue;

			//For each view anchor to analyze
			for (size_t fl = 1; fl <= TOP_BTW; fl = fl << 1) {

				//For each vertex
				for (size_t e = 0; e < in_list.size(); e++) {

					//Construct two vertex data (The segment start and end)
					VertexData vs(in_list[e], in_flags[e]),
						ve = (e == in_flags.size() - 1 ?
							VertexData(in_list[0], in_flags[0]) :
							VertexData(in_list[e + 1], in_flags[e + 1]));

					//Store wether each vertex is inside or outside the view
					bool vs_in = !IsFlagOutsideEdge(vs.second, fl);
					bool ve_in = !IsFlagOutsideEdge(ve.second, fl);

					//If both are outside
					if (!vs_in && !ve_in)

						//Discard this segment
						continue;

					//If they are both inside
					if (vs_in && ve_in)

						//Push back the first vertex
						out_temp.push_back(ve.first);

					//If the first is outside but the second
					if (vs_in && !ve_in)

						//Push back the intersection
						out_temp.push_back(
							ComputeIntersection(Segment(vs.first, ve.first), fl));

					//If the second is outside but the first
					if (!vs_in && ve_in) {

						//Compute the intersection and push it back
						out_temp.push_back(
							ComputeIntersection(Segment(vs.first, ve.first), fl));
						
						//Push back the first vertex
						out_temp.push_back(ve.first);

					}
				}

				//If we haven't pushed any vertices
				if (out_temp.empty())

					//Go for the next set of vertices
					break;

				//IF we are not on the last edge
				if (fl != TOP_BTW) {

					//Copy the contents of the out list to the in list
					in_list = out_temp;

					//Clear the out list
					out_temp.clear();

					//Compute the Vertex Flags
					ComputeVertexFlags(in_list, in_flags);

				}
			}

			//If we have out list of size 3
			if (out_temp.size() == 3)

				//Add the vertices to the final model
				AddAllVerticesInModel(out_temp, out_list);

			//else
			else {

				//Do a triangulation
				std::vector<Att_Tranf> temp(SimpleTriangulation(out_temp));
				
				//Concatenate the out list with the triangulated list
				out_list.insert(out_list.end(), temp.begin(), temp.end());

			}
		}

		//Free the Model
		model.Free();

		//Alocate the number of vertices that we have
		model.Alloc(out_list.size(), out_list.size(), model.GetFormat());

		//For every single vertex that has to go on the model
		for (size_t i = 0; i < out_list.size(); i++) {

			//Copy it to the model
			memcpy(model.GetVtxPtr(i), &out_list[i], sizeof(float) * 6);
			
			//Set the index right
			model.SetIdx(i, static_cast<unsigned short>(i));

		}
	}
}