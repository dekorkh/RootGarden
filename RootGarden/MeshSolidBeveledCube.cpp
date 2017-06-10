#include "MeshSolidBeveledCube.h"

#define _USE_MATH_DEFINES
#include "math.h"

MeshSolidBeveledCube::MeshSolidBeveledCube() :
	Mesh(3, 0, true),
	Color(Vector4f::Constant(1.0f))
{
	Vector4f NewColor;
	NewColor << 1.0f, 0.0f, 0.0f, 1.0f;
	SetColor(NewColor);
	MatterType = GL_POINTS;

	// We will need at least as many verts as we have normals. (4 per side)
	NumSides = 1;
	NumVertices = 4 * NumSides; //sides * vert normals
	NumVertIndices = 6 * NumSides; //sides * 3 indices per tri * 2 tris per face
}


MeshSolidBeveledCube::~MeshSolidBeveledCube()
{
}

void MeshSolidBeveledCube::SetColor(const Vector4f& InColor)
{
	Color = InColor;
}

void MeshSolidBeveledCube::GenerateMesh_Colors()
{
	ColorsData.clear();
	for (GLuint i = 0; i < NumVertices; i++)
	{
		ColorsData.insert(ColorsData.end(), { Color(0), Color(1), Color(2), Color(3) });
	}
}

void MeshSolidBeveledCube::GenerateMesh_Indices()
{
	IndexData.clear();

	IndexData.insert(IndexData.end(), {
		 0,  1,  2,
		 2,  3,  0,
/*
		 4,  5,  6,
		 6,  7,  4,

		 8,  9, 10,
		10, 11,  8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
*/
	});
}

void MeshSolidBeveledCube::GenerateMesh_Positions()
{
	PositionsData.clear();
	
	// FRONT FACE
	PositionsData.insert(PositionsData.end(), { -1.0f, -1.0f, -1.0f }); // 0
	PositionsData.insert(PositionsData.end(), { -1.0f,  1.0f, -1.0f });	// 1
	PositionsData.insert(PositionsData.end(), {  1.0f,  1.0f, -1.0f }); // 2
	PositionsData.insert(PositionsData.end(), {  1.0f, -1.0f, -1.0f }); // 3

	/*
	// RIGHT FACE
	PositionsData.insert(PositionsData.end(), { 1.0f, -1.0f, -1.0f }); //---4 = 1
	PositionsData.insert(PositionsData.end(), { 1.0f, -1.0f, 1.0f });  //---5 = 5
	PositionsData.insert(PositionsData.end(), { 1.0f,  1.0f, 1.0f });  //---6 = 6
	PositionsData.insert(PositionsData.end(), { 1.0f, -1.0f, -1.0f }); //---7 = 1

	// BACK FACE
	PositionsData.insert(PositionsData.end(), { 1.0f, -1.0f, 1.0f });  //---8 = 5
	PositionsData.insert(PositionsData.end(), { -1.0f, -1.0f, 1.0f }); //---9 = 4
	PositionsData.insert(PositionsData.end(), { -1.0f,  1.0f, 1.0f }); //--10 = 7
	PositionsData.insert(PositionsData.end(), { 1.0f,  1.0f, 1.0f });  //--11 = 6

	// LEFT FACE
	PositionsData.insert(PositionsData.end(), { -1.0f, -1.0f, 1.0f });  //--12 = 4
	PositionsData.insert(PositionsData.end(), { -1.0f, -1.0f, -1.0f }); //--13 = 0
	PositionsData.insert(PositionsData.end(), { -1.0f,  1.0f, -1.0f }); //--14 = 3
	PositionsData.insert(PositionsData.end(), { -1.0f,  1.0f, 1.0f });  //--15 = 7

	// TOP FACE
	PositionsData.insert(PositionsData.end(), { -1.0f,  1.0f, -1.0f }); //--16 = 3
	PositionsData.insert(PositionsData.end(), { 1.0f,  1.0f, -1.0f });  //--17 = 2
	PositionsData.insert(PositionsData.end(), { 1.0f,  1.0f, 1.0f });   //--18 = 6
	PositionsData.insert(PositionsData.end(), { -1.0f,  1.0f, 1.0f });  //--19 = 7

	// BOTTOM FACE
	PositionsData.insert(PositionsData.end(), { -1.0f, -1.0f, 1.0f });	//--20 = 4
	PositionsData.insert(PositionsData.end(), { 1.0f, -1.0f, 1.0f });	//--21 = 5
	PositionsData.insert(PositionsData.end(), { 1.0f, -1.0f, -1.0f });	//--22 = 1
	PositionsData.insert(PositionsData.end(), { -1.0f, -1.0f, -1.0f }); //--23 = 0
	*/
}

void MeshSolidBeveledCube::GenerateMesh_Normals()
{
	NormalsData.clear();

	// Map eigen vectors to positions
	vector<Map<Vector3f>> PositionsAsEigenVectors;
	PositionsAsEigenVectors.reserve(NumVertices);
	for (GLuint i = 0; i < NumVertices; ++i)
	{
		Map<Vector3f> PositionEigenVec(PositionsData.data() + i * NumComponentsVertPosition);
		PositionsAsEigenVectors.push_back(PositionEigenVec);
	}

	// For each triangle, add a normal computed as v0_v1 X v0_v2;
	for (GLuint i = 0; i < NumVertices / 4; ++i)
	{
		GLuint v0_i = i * 4;
		GLuint v1_i = i * 4 + 1;
		GLuint v2_i = i * 4 + 2;
		GLuint v3_i = i * 4 + 3;

		Vector3f T1_VecA = (PositionsAsEigenVectors[v1_i] - PositionsAsEigenVectors[v0_i]).normalized();
		Vector3f T1_VecB = (PositionsAsEigenVectors[v2_i] - PositionsAsEigenVectors[v0_i]).normalized();
		
		Vector3f T2_VecA = (PositionsAsEigenVectors[v3_i] - PositionsAsEigenVectors[v2_i]).normalized();
		Vector3f T2_VecB = (PositionsAsEigenVectors[v0_i] - PositionsAsEigenVectors[v2_i]).normalized();
		
		Vector3f T1_N = T1_VecA.cross(T1_VecB).normalized();
		Vector3f T2_N = T2_VecA.cross(T2_VecB).normalized(); // Unnecessary since each side's normals are same and can be set to T1.

		for (GLuint j = 0; j < 4; ++j)
		{
			NormalsData.insert(NormalsData.end(), T1_N.data(), T1_N.data() + 3);
		}
	}	
}