#pragma once

#include "ShaderManager.h"

#include "Eigen/StdVector"
#include "Eigen/Dense"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <time.h>

using namespace std;
using namespace Eigen;

#define BUFFER_OFFSET(x) ((const void*) (x))	//See "Red" book pg. 27

#define SHADER_ATTR_POSITION	"vPosition"
#define SHADER_ATTR_COLOR		"vColor"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, IndexBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

struct GenerateMeshResult
{
	clock_t Clock_GenerateMesh;
	clock_t Clock_GenerateMesh_Positions;
	clock_t Clock_GenerateMesh_Colors;
	clock_t Clock_GenerateMesh_Indices;
};

class Mesh
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	Mesh();
	~Mesh();

	/* Draw
	/	Generates and binds the VAO for the mesh,
	/	Generates, binds, and initializes the buffer for vertex info,
	/	Copies position data into a continous array and the copies it to the buffer,
	/	Same for color,
	/	Indices are already contiguous so just copies them to the buffer
	/	Calls draw elements.
	/	Arguments:
	/		ProgramAddress	-	The integer address of the gpu program used to querry the vert attribute location.
	*/
	void Draw(ShaderProgram& InShaderProgram);

	/* GenerateMesh
	/	Regenerate any vertex attributes marked dirty.
	*/
	void GenerateMesh(GenerateMeshResult *OutGenerateMeshResult);

	bool bDirty_Positions;
	bool bDirty_Colors;
	bool bDirty_Indices;

protected:
	size_t SizeOfPositions();
	size_t SizeOfColors();
	size_t SizeOfIndices();

	/* InitializeVAO
	/	Generate the Vertex Array Object.
	*/
	void InitializeVAO(ShaderProgram& InShaderProgram);

	/* DestroyVAO 
	/	Destroy the Vertex Array Object.
	*/
	void DestroyVAO();

	/* BuildRectangle
	/	Helper static function adds a rectangle to the vertex attribute arrays for the mesh.
	*/
	static void BuildRectangle_Positions(vector<GLfloat> &PositionsData, Vector2f UpperLeft, Vector2f LowerRight);
	static void BuildRectangle_Indices(vector<GLuint> &IndexData, GLuint StartIndex);

	vector<Vector3f, aligned_allocator<Vector3f>> Positions;
	vector<GLfloat> PositionsData; // Positions formatted into a contiguous array for GPU
	vector<Vector4f, aligned_allocator<Vector4f>> Colors;
	vector<GLfloat> ColorsData; // Colors formatted into a contigous array for GPU

	vector<GLuint> IndexData;

	GLuint NumVertices;
	GLuint NumVertIndices;

	GLenum MatterType;

	virtual void GenerateMesh_Positions();
	virtual void GenerateMesh_Colors();
	virtual void GenerateMesh_Indices();

private:

	void Draw_UpdateColors();
	void Draw_UpdatePositions();
	void Draw_UpdateIndices();

	GLuint Buffers[NumBuffers];
	GLuint VAOs[NumVAOs];

	size_t const NumComponentsVertPosition;
	size_t const NumComponentsVertColor;

	bool bInitializedVAO;	//Graphics eng. inits after scene&meshes are created so meshes init once during first draw using this check.
};