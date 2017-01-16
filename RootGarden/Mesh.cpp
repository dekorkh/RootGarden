#include "Mesh.h"
#include "GlobalState.h"

#include "GameStats.h"
#include "time.h"

Mesh::Mesh() : 
	NumVertices(0), 
	NumVertIndices(0),
	MatterType(GL_POINTS),
	NumComponentsVertColor(4),
	NumComponentsVertPosition(3),
	bInitializedVAO(false),
	bDirty_Positions(true),
	bDirty_Colors(true),
	bDirty_Indices(true)
{
}


Mesh::~Mesh()
{
	DestroyVAO();
}

void Mesh::Draw(ShaderProgram& InShaderProgram)
{
	//Graphics eng. inits after scene&meshes are created so meshes init once during first draw instead of during construction.
	if (!bInitializedVAO)
	{
		InitializeVAO(InShaderProgram);
		bInitializedVAO = true;
	}

	glBindVertexArray(VAOs[Triangles]);	//vertex arrays are apparently unecessery but are useful bc they store all vertex pipeline states for a set of verts.

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[IndexBuffer]);

	// Recopy positions, colors, and index arrays to the GPU if necessary
	if (bDirty_Positions)
	{
		Draw_UpdatePositions();
		bDirty_Positions = false;
	}
	if (bDirty_Colors)
	{
		Draw_UpdateColors();
		bDirty_Colors = false;
	}
	if (bDirty_Indices)
	{
		Draw_UpdateIndices();
		bDirty_Indices = false;
	}

	// Get the location of vertex attributes in the program
	GLint vpos = InShaderProgram.GetVertexAttribLocation("vPosition");
	GLint vcol = InShaderProgram.GetVertexAttribLocation("vColor");

	// Enable the program to fetch from vertex arrays
	glEnableVertexAttribArray(vpos);
	glEnableVertexAttribArray(vcol);

	//glBindVertexArray(VAOs[Triangles]);
	//glDrawArrays(MatterType, 0, ActiveMatter->NumVertices);
	glDrawElements(MatterType, NumVertIndices, GL_UNSIGNED_INT, NULL);
}

size_t Mesh::SizeOfPositions()
{
	return NumVertices * NumComponentsVertPosition * sizeof(GLfloat);
}

size_t Mesh::SizeOfColors()
{
	return NumVertices * NumComponentsVertColor * sizeof(GLfloat);
}

size_t Mesh::SizeOfIndices()
{
	return NumVertIndices * sizeof(GLuint);
}

void Mesh::GenerateMesh(GenerateMeshResult *OutGenerateMeshResult)
{
	clock_t Clock_Ms_GenerateMesh = clock();
	if (bDirty_Indices) // Indices has to be first because it sets things like number of verts.
	{
		clock_t Clock_Ms_GenerateMesh_Indices = clock();
		GenerateMesh_Indices();
		OutGenerateMeshResult->Clock_GenerateMesh_Indices = clock() - Clock_Ms_GenerateMesh_Indices;
	}
	if (bDirty_Positions)
	{
		clock_t Clock_Ms_GenerateMesh_Positions = clock();
		GenerateMesh_Positions();
		OutGenerateMeshResult->Clock_GenerateMesh_Positions = clock() - Clock_Ms_GenerateMesh_Positions;
	}
	if (bDirty_Colors)
	{
		clock_t Clock_Ms_GenerateMesh_Colors = clock();
		GenerateMesh_Colors();
		OutGenerateMeshResult->Clock_GenerateMesh_Colors = clock() - Clock_Ms_GenerateMesh_Colors;
	}
	OutGenerateMeshResult->Clock_GenerateMesh = clock() - Clock_Ms_GenerateMesh;
}

void Mesh::BuildRectangle_Indices(vector<GLuint> &IndexData, GLuint StartIndex)
{
	IndexData.insert(IndexData.end(), { StartIndex , StartIndex + 1, StartIndex + 2, StartIndex + 2, StartIndex + 3, StartIndex });
}

void Mesh::BuildRectangle_Positions(vector<GLfloat> &PositionsData, Vector2f UpperLeft, Vector2f LowerRight)
{
	PositionsData.insert(PositionsData.end(), { UpperLeft(0), UpperLeft(1), 0.0f });
	PositionsData.insert(PositionsData.end(), { LowerRight(0), UpperLeft(1), 0.0f });
	PositionsData.insert(PositionsData.end(), { LowerRight(0), LowerRight(1), 0.0f });
	PositionsData.insert(PositionsData.end(), { UpperLeft(0), LowerRight(1), 0.0f });
}

void Mesh::InitializeVAO(ShaderProgram& InShaderProgram)
{
	// Initialize the VAO and its buffer
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);	//vertex arrays are apparently unecessery but are useful bc they store all vertex pipeline states for a set of verts.

	// Generate and initialize a vertex attribute and an index buffer.
	glGenBuffers(NumBuffers, Buffers);

	// Bind to both buffers and initialize to their size
	// Do their binding here get memorized and recalled by binding the VAO?
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, SizeOfPositions() + SizeOfColors(), NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[IndexBuffer]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeOfIndices(), NULL, GL_STATIC_DRAW);

	// Get the location of vertex attributes in the program
	GLint vpos = InShaderProgram.GetVertexAttribLocation("vPosition");
	GLint vcol = InShaderProgram.GetVertexAttribLocation("vColor");

	// Point the programs vertex attributes to their data in the vertex attribute array.
	glVertexAttribPointer(vpos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(vcol, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SizeOfPositions()));
}

void Mesh::DestroyVAO()
{
	glDeleteBuffers(NumBuffers, Buffers);
	glDeleteVertexArrays(NumVAOs, VAOs);
}

void Mesh::GenerateMesh_Positions()
{
}

void Mesh::GenerateMesh_Colors()
{
}

void Mesh::GenerateMesh_Indices()
{
}

void Mesh::Draw_UpdatePositions()
{	
	glBufferSubData(GL_ARRAY_BUFFER, 0, SizeOfPositions(), PositionsData.data());	//Starting at begining of active buffer put data from vertices array
}

void Mesh::Draw_UpdateColors()
{
	glBufferSubData(GL_ARRAY_BUFFER, SizeOfPositions(), SizeOfColors(), ColorsData.data());		//Starting at end of vertices array of active buffer put data from color array
}

void Mesh::Draw_UpdateIndices()
{
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, SizeOfIndices(), IndexData.data());
}