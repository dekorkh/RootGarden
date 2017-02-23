#include "TransformInfo.h"
#include "Matter.h"
#include "Mesh_Instanced.h"
#include "GlobalState.h"

#include "GameStats.h"
#include "time.h"

int Mesh_Instanced::NumDrawCallsThisFrame = 0;
bool Mesh_Instanced::bDrity_Draw_Instanced = true;
vector<Mesh_Instanced*> Mesh_Instanced::InstanceList;
GLuint Mesh_Instanced::ModelMatrixBuffer = 0;
GLuint Mesh_Instanced::InstanceColorBuffer = 0;
bool Mesh_Instanced::bInitializeModelMatrixBuffer = true;
bool Mesh_Instanced::bDirty_ModelMatrices = true;
bool Mesh_Instanced::bInitializedVAO_Instanced = false;

bool Mesh_Instanced::bUseDebugData(false);
bool Mesh_Instanced::bUseDebugModelMatrix(false);
bool Mesh_Instanced::bUseDebugColors(false);
vector<GLfloat> Mesh_Instanced::DebugModelMatrixData;
vector<GLfloat> Mesh_Instanced::DebugColorsData;
vector<GLfloat> Mesh_Instanced::DebugPositionsData;
vector<GLint> Mesh_Instanced::DebugIndexData;

Mesh_Instanced::Mesh_Instanced(Matter const &InParentMatter) :
	ParentMatter(&InParentMatter),
	Mesh(3) //Number of buffers
{
	InstanceList.push_back(this);
	bDirty_ModelMatrices = true;
}

Mesh_Instanced::~Mesh_Instanced()
{
	int index = 0;
	for (vector<Mesh_Instanced*>::iterator it = InstanceList.begin(); it != InstanceList.end(); ++it)
	{
		Mesh_Instanced* CurrentInstance = *it;
		if (CurrentInstance == this)
		{
			InstanceList.erase(it);
			break;
		}
		index++;
	}
	
	bDirty_ModelMatrices = true;
	
	if (InstanceList.size() == 0)
	{
		glDeleteBuffers(1, &ModelMatrixBuffer);
		glDeleteBuffers(1, &InstanceColorBuffer);
	}
}

void Mesh_Instanced::Draw(ShaderProgram& InShaderProgram)
{
	Draw_Instanced(InShaderProgram);
}

void Mesh_Instanced::RebindModelMatrixBuffer(ShaderProgram& InShaderProgram)
{
	ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, ModelMatrixBuffer);
	ShaderProgram::glBufferData_checked(GL_ARRAY_BUFFER, SizeOfModelMatrices(), NULL, GL_STATIC_DRAW);
	
	ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, InstanceColorBuffer);
	ShaderProgram::glBufferData_checked(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16 * InstanceList.size(), NULL, GL_STATIC_DRAW);
}

void Mesh_Instanced::InitializeVAO_Instanced(ShaderProgram& InShaderProgram)
{
	Mesh_Instanced *firstmesh = InstanceList.at(0);

	ShaderProgram::glGenVertexArrays_checked(1, &firstmesh->VAO);
	ShaderProgram::glBindVertexArray_checked(firstmesh->VAO);	//vertex arrays are apparently unecessery but are useful bc they store all vertex pipeline states for a set of verts.

	ShaderProgram::glGenBuffers_checked(firstmesh->NumBuffers, firstmesh->Buffers.data());

	ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, firstmesh->Buffers[0]);
	ShaderProgram::glBufferData_checked(GL_ARRAY_BUFFER, firstmesh->SizeOfPositions() + firstmesh->SizeOfColors(), NULL, GL_STATIC_DRAW);
	ShaderProgram::glBindBuffer_checked(GL_ELEMENT_ARRAY_BUFFER, firstmesh->Buffers[1]);
	ShaderProgram::glBufferData_checked(GL_ELEMENT_ARRAY_BUFFER, firstmesh->SizeOfIndices(), NULL, GL_STATIC_DRAW);

	// Get the location of vertex attributes in the program
	GLint vpos = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, ("vPosition"));
	GLint vcol = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, ("vColor"));

	// Point the programs vertex attributes to their data in the vertex attribute array.
	ShaderProgram::glVertexAttribPointer_checked(vpos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	ShaderProgram::glVertexAttribPointer_checked(vcol, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(firstmesh->SizeOfPositions()));

	// Enable fetching from pos and col streams
	ShaderProgram::glEnableVertexAttribArray_checked(vpos);
	ShaderProgram::glEnableVertexAttribArray_checked(vcol);

	// Generate and initialize the model matrix instanced buffer
	ShaderProgram::glGenBuffers_checked(1, &ModelMatrixBuffer);
	ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, ModelMatrixBuffer);
	ShaderProgram::glBufferData_checked(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4 * 3, NULL, GL_STATIC_DRAW);

	// Generate and initialize the color instanced buffer
	ShaderProgram::glGenBuffers_checked(1, &InstanceColorBuffer);
	ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, InstanceColorBuffer);
	ShaderProgram::glBufferData_checked(GL_ARRAY_BUFFER, sizeof(float) * 4 * 3, NULL, GL_STATIC_DRAW);
}

void Mesh_Instanced::Draw_Instanced(ShaderProgram& InShaderProgram)
{
	NumDrawCallsThisFrame++;
	

	if (bDrity_Draw_Instanced)
	{		
		/////////////////////////////////////////////////
		Mesh_Instanced *thismesh = InstanceList.at(0);
		enum VAO_IDs { Triangles, NumVAOs };

		GLint prog_addr;
		glGetIntegerv(GL_CURRENT_PROGRAM, &prog_addr);

		if (!bInitializedVAO_Instanced)
		{
			InitializeVAO_Instanced(InShaderProgram);
			bInitializedVAO_Instanced = true;
		}

		// Get the location of vertex attributes in the program
		GLint vpos = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, ("vPosition"));
		GLint vcol = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, ("vColor"));
		GLint vModelMatrix = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, "vModelMatrix");
		GLint vInstanceColor = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, "vInstanceColor");

		ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, thismesh->Buffers[0]);
		ShaderProgram::glBufferData_checked(GL_ARRAY_BUFFER, thismesh->SizeOfPositions(), NULL, GL_STATIC_DRAW);
		if (bUseDebugData)
		{
			ShaderProgram::glBufferSubData_checked(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * 4, Mesh_Instanced::GetDebugPositionsData());
		}
		else
		{
			ShaderProgram::glBufferSubData_checked(GL_ARRAY_BUFFER, 0, thismesh->SizeOfPositions(), thismesh->PositionsData.data());	//Starting at begining of active buffer put data from vertices array
		}
		ShaderProgram::glVertexAttribPointer_checked(vpos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, thismesh->Buffers[1]);
		ShaderProgram::glBufferData_checked(GL_ARRAY_BUFFER, thismesh->SizeOfColors(), NULL, GL_STATIC_DRAW);
		ShaderProgram::glBufferSubData_checked(GL_ARRAY_BUFFER, 0, thismesh->SizeOfColors(), thismesh->ColorsData.data());
		
		ShaderProgram::glVertexAttribPointer_checked(vcol, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		
		ShaderProgram::glBindBuffer_checked(GL_ELEMENT_ARRAY_BUFFER, thismesh->Buffers[2]);
		ShaderProgram::glBufferData_checked(GL_ELEMENT_ARRAY_BUFFER, thismesh->SizeOfIndices(), NULL, GL_STATIC_DRAW);
		if (bUseDebugData)
		{
			ShaderProgram::glBufferSubData_checked(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLint) * 6, Mesh_Instanced::GetDebugIndexData());
		}
		else
		{
			ShaderProgram::glBufferSubData_checked(GL_ELEMENT_ARRAY_BUFFER, 0, thismesh->SizeOfIndices(), thismesh->IndexData.data());
		}
		// Get the location of vertex attributes in the program

		RebindModelMatrixBuffer(InShaderProgram);
		Draw_UpdateModelMatrices(InShaderProgram);		

		// Enable fetching from pos and col streams
		ShaderProgram::glEnableVertexAttribArray_checked(vpos);
		ShaderProgram::glEnableVertexAttribArray_checked(vcol);
		
		for (int i = 0; i < 4; i++)
		{
			ShaderProgram::glEnableVertexAttribArray_checked(vModelMatrix + i); // Enable fetching from this matrix 
			ShaderProgram::glVertexAttribDivisor_checked(vModelMatrix + i, 1); // Make instanced
		}

		ShaderProgram::glVertexAttribDivisor_checked(vInstanceColor, 1); // Make instanced
		ShaderProgram::glEnableVertexAttribArray_checked(vInstanceColor);
		
		ShaderProgram::glDrawElementsInstanced_checked(thismesh->MatterType, thismesh->NumVertIndices, GL_UNSIGNED_INT, NULL, static_cast<GLsizei>(InstanceList.size()));

		bDrity_Draw_Instanced = false;
	}
	
	if (NumDrawCallsThisFrame == InstanceList.size()) // End of frame.
	{
		NumDrawCallsThisFrame = 0;
		bDrity_Draw_Instanced = true;
	}
}

void Mesh_Instanced::Draw_UpdateModelMatrices(ShaderProgram& InShaderProgram)
{
	// Load the model matrix instanced buffer.
	GLint vModelMatrix = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, "vModelMatrix");
	ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, ModelMatrixBuffer);
	if (bUseDebugModelMatrix)
	{
		ShaderProgram::glBufferSubData_checked(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 16 * InstanceList.size(), GetDebugModelMatrix(static_cast<GLint>(InstanceList.size())));
	}
	else
	{
		for (int InstanceIdx = 0; InstanceIdx < InstanceList.size(); InstanceIdx++)
		{
			Mesh_Instanced* Instance = InstanceList.at(InstanceIdx);
			{
				ShaderProgram::glBufferSubData_checked(GL_ARRAY_BUFFER, sizeof(float) * 16 * InstanceIdx, SIZE_OF_MAT4, Instance->ParentMatter->Transform.GetModelMatrixDataPtr());
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		ShaderProgram::glVertexAttribPointer_checked(vModelMatrix + i, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, BUFFER_OFFSET(sizeof(float) * 4 * i));
	}

	// Load the color instanced buffer.
	GLint vInstanceColor = ShaderProgram::glGetAttribLocation_checked(InShaderProgram.ProgAddr, "vInstanceColor");
	ShaderProgram::ShaderProgram::glBindBuffer_checked(GL_ARRAY_BUFFER, InstanceColorBuffer);
	if (bUseDebugColors)
	{
		ShaderProgram::glBufferSubData_checked(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 4 * InstanceList.size(), GetDebugColors(static_cast<GLint>(InstanceList.size())));
	}
	else
	{
		for (int InstanceIdx = 0; InstanceIdx < InstanceList.size(); InstanceIdx++)
		{
			Mesh_Instanced* Instance = InstanceList.at(InstanceIdx);
			ShaderProgram::glBufferSubData_checked(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * InstanceIdx, sizeof(GLfloat) * 4, Instance->ColorsData.data());
		}
	}
	ShaderProgram::glVertexAttribPointer_checked(vInstanceColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}

size_t Mesh_Instanced::SizeOfModelMatrices()
{
	return SIZE_OF_MAT4 * InstanceList.size();
}

GLfloat* Mesh_Instanced::GetDebugModelMatrix(GLint NumInstances)
{
	DebugModelMatrixData.clear();
	for (GLint instanceIdx = 0; instanceIdx < NumInstances; instanceIdx++)
	{
		GLfloat xPos;
		switch (instanceIdx % 4)
		{
		case 0:
			xPos = 0.2f;
			break;
		case 1:
			xPos = 0.4f;
			break;
		case 2:
			xPos = 0.6f;
			break;
		case 3:
			xPos = 0.8f;
			break;
		}

		DebugModelMatrixData.insert(DebugModelMatrixData.end(),
		{
			1.0f * 0.1f, 0.0f, 0.0f, xPos * 2.0f - 1.0f,
			0.0f, 1.0f * 0.1f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f * 0.1f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		});
	}
	return DebugModelMatrixData.data();
}

GLfloat* Mesh_Instanced::GetDebugColors(GLint NumInstances)
{
	DebugColorsData.clear();
	for (GLint instanceIdx = 0; instanceIdx < NumInstances; instanceIdx++)
	{
		vector<GLfloat> col = { 0.0, 0.0f, 0.0, 1.0f };
		switch (instanceIdx % 3)
		{
		case 0:
			col[0] = 1.0f;
			break;
		case 1:
			col[1] = 1.0f;
			break;
		case 2:
			col[2] = 1.0f;
			break;
		}

		DebugColorsData.insert(DebugColorsData.end(), col.begin(), col.end());
	}
	return DebugColorsData.data();
}

GLfloat* Mesh_Instanced::GetDebugPositionsData()
{
	DebugPositionsData.clear();

	DebugPositionsData.insert(DebugPositionsData.end(), {
			-1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f 
	});
	
	return DebugPositionsData.data();
}

GLint* Mesh_Instanced::GetDebugIndexData()
{
	DebugIndexData.clear();

	DebugIndexData.insert(DebugIndexData.end(), { 0, 1, 2, 2, 3, 0 });

	return DebugIndexData.data();
}