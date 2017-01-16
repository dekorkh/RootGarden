#pragma once

#include "Mesh.h"
#include <Eigen/StdVector>

class MeshRectangleGradient : public Mesh
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		MeshRectangleGradient();
	~MeshRectangleGradient();

	/*  SetLayerColor:
	/	Set the color of a gradient layer using an index.
	/	Arguments:
	/		Index	-	The index of the layer to modify.
	/		Color	-	The color to set the layer to.
	*/
	void SetLayerColor(const int Index, const Vector4f& Color);
	
	/*  SetLayerPosition:
	/	Set the position of a gradient layer - 0.0 is center, 1.0 is outer edge.
	/	Arguments:
	/		Index		-	The index of the layer to modify.
	/		Position	-	The position to set the layer to. 0.0 is center, 1.0 is outer edge.
	*/
	void SetLayerPosition(const int Index, const float Position);

	/*  SetNumberOfLayers:
	/	Set the number of layers in the gradient.
	/	Arguments:
	/		NumLayers	-	Number of layers.
	*/
	void SetNumberOfLayers(int NumLayers);

	/*	GenerateMesh_Positions()
	/	Regenerate (if dirty) the positions, color, and indices
	*/
	void GenerateMesh_Positions() override;

	void GenerateMesh_Colors() override;

	void GenerateMesh_Indices() override;

private:
	vector<Vector4f, aligned_allocator<Vector4f>> LayerColors;
	vector<float> LayerPositions;
	int NumLayers;
	int NumSides;
};
