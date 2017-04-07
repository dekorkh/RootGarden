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

class TextureInfo;

enum ETEXTURE
{
	ETEXTURE_COLOR = 0,
	NUM_ETEXTURES
};

enum EBUFFER
{
	EBUFFER_ARRAY = 0,
	EBUFFER_ELEMENT,
	EBUFFER_PIXEL,
	NUM_EBUFFERS
};

struct GenerateMeshResult
{
	clock_t Clock_GenerateMesh;
	clock_t Clock_GenerateMesh_Positions;
	clock_t Clock_GenerateMesh_TexCoords;
	clock_t Clock_GenerateMesh_Colors;
	clock_t Clock_GenerateMesh_Indices;
};

class Mesh
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	Mesh(GLuint InNumBuffers = NUM_EBUFFERS, GLuint InNumTextures = 0);
	virtual ~Mesh();

	/* Draw
	/	Generates and binds the VAO for the Mesh,
	/	Generates, binds, and initializes the buffer for vertex info,
	/	Copies position data into a continous array and the copies it to the buffer,
	/	Same for color,
	/	Indices are already contiguous so just copies them to the buffer
	/	Calls draw elements.
	/	Arguments:
	/		ProgramAddress	-	The integer address of the gpu program used to querry the vert attribute location.
	*/
	virtual void Draw(ShaderProgram& InShaderProgram);

	/* GenerateMesh
	/	Regenerate any vertex attributes marked dirty.
	*/
	void GenerateMesh(GenerateMeshResult *OutGenerateMeshResult);

	bool bDirty_Positions;
	bool bDirty_TexCoords;
	bool bDirty_Colors;
	bool bDirty_Indices;

	GLenum MatterType;

protected:
	size_t SizeOfPositions();
	size_t SizeOfColors();
	size_t SizeOfTexCoords();
	size_t SizeOfIndices();
	GLsizeiptr SizeOfTextureData();

	/* InitializeVAO
	/	Generate the Vertex Array Object.
	*/
	virtual void InitializeVAO(ShaderProgram& InShaderProgram);

	/* DestroyVAO 
	/	Destroy the Vertex Array Object.
	*/
	void DestroyVAO();

	/* BuildRectangle
	/	Helper static function adds a rectangle to the vertex attribute arrays for the Mesh.
	*/
	static void BuildRectangle_Positions(vector<GLfloat> &PositionsData, Vector2f UpperLeft, Vector2f LowerRight);
	static void BuildRectangle_Indices(vector<GLuint> &IndexData, GLuint StartIndex);

	vector<GLfloat> PositionsData;	// Positions formatted into a contiguous array for GPU
	vector<GLfloat> ColorsData;		// Colors formatted into a contigous array for GPU
	vector<GLfloat> TexCoordsData;	// Texture coordinates
	vector<GLuint> IndexData;
	

	GLuint NumVertices;
	GLuint NumVertIndices;

	virtual void GenerateMesh_Positions();
	virtual void GenerateMesh_Colors();
	virtual void GenerateMesh_Indices();
	virtual void GenerateMesh_TexCoords();
	virtual void Generate_TextureInfos();

	GLuint NumBuffers;
	vector<GLuint> Buffers;
	GLuint NumTextures;
	vector<GLuint> Textures;
	vector<TextureInfo*> TextureInfos;
	vector<GLuint> Samplers;
	GLuint VAO;

	void Draw_UpdateColors();
	void Draw_UpdatePositions();
	void Draw_UpdateTexCoords();
	void Draw_UpdateIndices();

	bool bInitializedVAO;	//Graphics eng. inits after scene&meshes are created so meshes init once during first draw using this check.

private:

	size_t const NumComponentsVertPosition;
	size_t const NumComponentsVertColor;
	size_t const NumComponentsVertTexCoords;
};

