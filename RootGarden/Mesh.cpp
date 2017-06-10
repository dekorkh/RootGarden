#include "Mesh.h"
#include "TextureInfo.h"
#include "GlobalState.h"

#include "GameStats.h"
#include "time.h"

Mesh::Mesh(GLuint InNumBuffers, GLuint InNumTextures, bool InbHasNormals) :
	NumVertices(0), 
	NumVertIndices(0),
	MatterType(GL_POINTS),
	NumComponentsVertColor(4),
	NumComponentsVertPosition(3),
	NumComponentsVertTexCoords(2),
	NumComponentsVertNormal(3),
	bInitializedVAO(false),
	bDirty_Positions(true),
	bDirty_Colors(true),
	bDirty_TexCoords(true),
	bDirty_Indices(true),
	bDirty_Normals(true),
	NumBuffers(InNumBuffers),
	NumTextures(InNumTextures),
	bHasNormals(InbHasNormals)
{
	/* AutoBuffer?
	Add one for each buffer activated through constructor parameters - 
	this way the number of buffers can be determined by any subclass
	which means subclasses which don't implement generate/update logic
	for a particular buffer can be handled better.  AutoBuffer.
	*/
	GLuint NumAutoBuffers = 0;
	if (bHasNormals)
	{
		NumAutoBuffers += 1;
	}

	/* 
	Initial buffer addresses.  -1 for each in order to distinguish this value as 
	a state different from that which would be returned by a call to generate them - 0.
	*/
	Buffers.assign(NumBuffers + NumAutoBuffers, -1);
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

	ShaderProgram::glBindVertexArray_checked(VAO);	//vertex arrays are apparently unecessery but are useful bc they store all vertex pipeline states for a set of verts.

	// Recopy positions, colors, and index arrays to the GPU if necessary
	if (bDirty_Positions)
	{
		ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, Buffers[0]);
		Draw_UpdatePositions();
		bDirty_Positions = false;
	}
	if (bHasNormals && InShaderProgram.bSupportsNormals && bDirty_Normals)
	{
		ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, Buffers[0]);
		Draw_UpdateNormals();
		bDirty_Normals = false;
	}
	if (bDirty_Colors)
	{
		ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, Buffers[0]);
		Draw_UpdateColors();
		bDirty_Colors = false;
	}
	if (bDirty_TexCoords)
	{
		ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, Buffers[0]);
		Draw_UpdateTexCoords();
		bDirty_TexCoords = false;
	}
	if (bDirty_Indices)
	{
		ShaderProgram::glBindBuffer_checked(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
		Draw_UpdateIndices();
		bDirty_Indices = false;
	}
	
	if (Textures.size() > 0)
	{
		glActiveTexture(GL_TEXTURE0 + ETEXTURE_COLOR);
		glBindTexture(GL_TEXTURE_2D, Textures[ETEXTURE_COLOR]);
	}

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

size_t Mesh::SizeOfTexCoords()
{
	return NumVertices * NumComponentsVertTexCoords * sizeof(GLfloat);
}

size_t Mesh::SizeOfIndices()
{
	return NumVertIndices * sizeof(GLuint);
}

size_t Mesh::SizeOfNormals()
{
	return NumVertIndices * NumComponentsVertNormal  * sizeof(GLfloat);
}

GLsizeiptr Mesh::SizeOfTextureData()
{
	GLsizeiptr result = 0;
	for (auto &texinfo : TextureInfos)
	{
		result += texinfo->Size();
	}
	return result;
}

void Mesh::GenerateMesh(GenerateMeshResult *OutGenerateMeshResult)
{
	clock_t Clock_Ms_GenerateMesh = clock();
	if (bDirty_Indices) // Indices has to be first because it sets things like number of verts.
	{
		clock_t Clock_Ms_GenerateMesh_Indices = clock();
		GenerateMesh_Indices();
		GenerateMesh_TexCoords();
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
	if (bDirty_TexCoords)
	{
		clock_t Clock_Ms_GenerateMesh_TexCoords = clock();
		GenerateMesh_TexCoords();
		OutGenerateMeshResult->Clock_GenerateMesh_TexCoords = clock() - Clock_Ms_GenerateMesh_TexCoords;
	}
	if (bDirty_Normals)
	{
		clock_t Clock_Ms_GenerateMesh_Normals = clock();
		GenerateMesh_Normals();
		OutGenerateMeshResult->Clock_GenerateMesh_Normals = clock() - Clock_Ms_GenerateMesh_Normals;
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
	/* (No textures)
	(need to have a program active as per parameter)
	Generate VAO
	BindVAO
	Generate VBO's
	Bind & Initialize VBO's
	Specify vertex attributes (VBO's must be bound)
	*/

	/* (With textures)
	Along with generating the previous buffers, also generate a "pixel unpack buffer" - this buffer will
	push our texture data until GL finds time to transfer it to the texture object associated by SetTextureObjectActive.
	Bind the pixel unpack buffer.
	Initialize the pixel unpack buffer.
	Generate texture objects
	Bind texture objects
	Set texture objects active? http://stackoverflow.com/questions/8866904/differences-and-relationship-between-glactivetexture-and-glbindtexture
	Specify texture objects' storage
	Specify the texture objects data (offset into the pixel unpack buffer)

	Generate sampler
	Bind sampler to sampler unit
	Set sampler parameters

	(Not in here, but somewhere...  we need to pass uniform ints of the texture unit index to the sampler uniform in the shader.
	*/

	// Initialize the VAO and its buffer
	ShaderProgram::glGenVertexArrays_checked(1, &VAO);
	ShaderProgram::glBindVertexArray_checked(VAO);	//vertex arrays are apparently unecessery but are useful bc they store all vertex pipeline states for a set of verts.

	// Generate and initialize a vertex attribute and an index buffer.
	ShaderProgram::glGenBuffers_checked(NumBuffers, Buffers.data());

	/*
	Bind to buffers and initialize to their size. Do their binding here get memorized and recalled by binding the VAO? No.  VAO's only
	store vertex attribute specification / data about their attribute locations.  But there are two reasons here for binding them.
	First, buffers must be bound to change or, in this case, to	initialize their data.
	*/
	ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, Buffers[EBUFFER_ARRAY]);
	size_t VertexAttributeBufferSize = SizeOfPositions() + SizeOfColors() + SizeOfTexCoords();
	if (bHasNormals && InShaderProgram.bSupportsNormals)
	{
		VertexAttributeBufferSize += SizeOfNormals();
	}
	ShaderProgram::glBufferData_checked(GL_ARRAY_BUFFER, VertexAttributeBufferSize, NULL, GL_STATIC_DRAW);

	ShaderProgram::glBindBuffer_checked(GL_ELEMENT_ARRAY_BUFFER, Buffers[EBUFFER_ELEMENT]);
	ShaderProgram::glBufferData_checked(GL_ELEMENT_ARRAY_BUFFER, SizeOfIndices(), NULL, GL_STATIC_DRAW);

	/*
	Second, The buffers must be bound to specify their attribute locations.
	Get the location of vertex attributes in the program.
	*/
	GLint vpos = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, ("vPosition"));
	GLint vcol = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, ("vColor"));
	GLint vnor = 0;
	if (bHasNormals && InShaderProgram.bSupportsNormals)
	{
		vnor = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, ("vNormal"));
	}
	
	// Point the programs vertex attributes to their data in the vertex attribute array.
	ShaderProgram::glVertexAttribPointer_checked(vpos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	ShaderProgram::glVertexAttribPointer_checked(vcol, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SizeOfPositions()));
	if (bHasNormals && InShaderProgram.bSupportsNormals)
	{
		ShaderProgram::glVertexAttribPointer_checked(vnor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SizeOfPositions() + SizeOfColors()));
	}
	
	// Enable fetching from pos and col streams.  (Is this considered part of vertex specification and so is it stored by the VAO?)
	ShaderProgram::glEnableVertexAttribArray_checked(vpos);
	ShaderProgram::glEnableVertexAttribArray_checked(vcol);
	if (bHasNormals && InShaderProgram.bSupportsNormals)
	{
		ShaderProgram::glEnableVertexAttribArray_checked(vnor);
	}

	if (NumTextures != 0)
	{
		GLint vtexCoord = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, ("vTexCoords"));
		ShaderProgram::glVertexAttribPointer_checked(vtexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SizeOfPositions() + SizeOfColors()));
		ShaderProgram::glEnableVertexAttribArray_checked(vtexCoord);
		// This must populate the texture info's since we will use that info to set up immutable texture storage next.
		Generate_TextureInfos();
		TextureInfo *texInfo = TextureInfos[ETEXTURE_COLOR];

		// Bind & Initialize the pixel unpack buffer.
		ShaderProgram::glBindBuffer_checked(GL_PIXEL_UNPACK_BUFFER, Buffers[EBUFFER_PIXEL]);
		ShaderProgram::glBufferData_checked(GL_PIXEL_UNPACK_BUFFER, SizeOfTextureData(), texInfo->Data, GL_STATIC_DRAW);
		
		// Generate the texture objects (units?)		
		ShaderProgram::glGenTextures_checked(NumTextures, Textures.data());

		// Bind the texture to it's target - only one texture can be bound per texture unit, though it can be bound to any target.
		ShaderProgram::glBindTexture_checked(GL_TEXTURE_2D, Textures[ETEXTURE_COLOR]);

		// For each texture unit
		// Set the texture active
		ShaderProgram::glActiveTexture_checked(GL_TEXTURE0 + ETEXTURE_COLOR);

		// Assign its storage
		ShaderProgram::glTexStorage2D_checked(GL_TEXTURE_2D, texInfo->Levels, texInfo->Format, texInfo->Width, texInfo->Height);

		// Point it to the texture data.
		ShaderProgram::glTexSubImage2D_checked(GL_TEXTURE_2D, 0, 0, 0, texInfo->Width, texInfo->Height, GL_RGB, GL_FLOAT, NULL);

		// Generate Sampler
		ShaderProgram::glGenSamplers_checked(NumTextures, Samplers.data());

		// Bind Sampler to Texture Unit
		ShaderProgram::glBindSampler_checked(ETEXTURE_COLOR, Samplers[ETEXTURE_COLOR]);
	}
}

void Mesh::DestroyVAO()
{
	glDeleteBuffers(NumBuffers, Buffers.data());
	glDeleteTextures(NumTextures, Textures.data());
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::GenerateMesh_Positions()
{
}

void Mesh::GenerateMesh_Normals()
{
}

void Mesh::GenerateMesh_Colors()
{
}

void Mesh::GenerateMesh_Indices()
{
}

void Mesh::GenerateMesh_TexCoords()
{
}

void Mesh::Generate_TextureInfos()
{
	TextureInfos.insert(TextureInfos.end(), new TextureInfo()); // ETEXTURE_COLOR
	Textures.insert(Textures.end(), -1);						// ETEXTURE_COLOR
	Samplers.insert(Samplers.end(), -1);						// ETEXTURE_COLOR
}

void Mesh::Draw_UpdatePositions()
{	
	ShaderProgram::glBufferSubData_checked(GL_ARRAY_BUFFER, 0, SizeOfPositions(), PositionsData.data());	//Starting at begining of active buffer put data from vertices array
}

void Mesh::Draw_UpdateNormals()
{
	ShaderProgram::glBufferSubData_checked(GL_ARRAY_BUFFER, SizeOfPositions() + SizeOfColors() + SizeOfTexCoords(), SizeOfNormals(), NormalsData.data());
}

void Mesh::Draw_UpdateColors()
{
	ShaderProgram::glBufferSubData_checked(GL_ARRAY_BUFFER, SizeOfPositions(), SizeOfColors(), ColorsData.data());		//Starting at end of vertices array of active buffer put data from color array
}

void Mesh::Draw_UpdateTexCoords()
{
	ShaderProgram::glBufferSubData_checked(GL_ARRAY_BUFFER, SizeOfPositions() + SizeOfColors(), SizeOfTexCoords(), TexCoordsData.data());
}

void Mesh::Draw_UpdateIndices()
{
	ShaderProgram::glBufferSubData_checked(GL_ELEMENT_ARRAY_BUFFER, 0, SizeOfIndices(), IndexData.data());
}