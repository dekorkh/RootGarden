#pragma once

#include "Mesh.h"

class TransformInfo;
class Matter;

class Mesh_Instanced
	: public Mesh
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	Mesh_Instanced(Matter const &InParentMatter );
	virtual ~Mesh_Instanced();

	/* Draw
	/	Generates and binds the VAO for the Mesh_Instanced,
	/	Generates, binds, and initializes the buffer for vertex info,
	/	Copies position data into a continous array and the copies it to the buffer,
	/	Same for color,
	/	Indices are already contiguous so just copies them to the buffer
	/	Calls draw elements.
	/	Arguments:
	/		ProgramAddress	-	The integer address of the gpu program used to querry the vert attribute location.
	*/
	void Draw(ShaderProgram& InShaderProgram) override;

	// Will cause the model matrices to be recopied to the gpu.
	static bool bDirty_ModelMatrices;

protected:
	static void InitializeVAO_Instanced(ShaderProgram& InShaderProgram);

private:
	/* Draw_Instanced
	/ Every time an instance's Draw() is called it calls on this static version.
	/ Draws all the instances during the first call for that frame and simply returns the for rest.
	*/
	static void Draw_Instanced(ShaderProgram& InShaderProgram);

	/* ReinitializeModelMatrixBuffer
	/ Every time an instance is added/removed this rebinds the model matrix buffer
	/ so that it can be expanded/shrunk.
	*/
	static void RebindModelMatrixBuffer(ShaderProgram& InShaderProgram);

	/* Draw_UpdateModelMatrices
	/ Sends the model matrix data for each instance to the gpu buffer.
	*/
	static void Draw_UpdateModelMatrices(ShaderProgram& InShaderProgram);

	static size_t SizeOfModelMatrices();

	// Need reference to owning matter in order to have access to the model matrix.
	const Matter* ParentMatter;

	// Number of times draw has been called for this mesh type this frame.
	static int NumDrawCallsThisFrame;
	static bool bDrity_Draw_Instanced;

	// Contains the list of instances of this mesh.
	static vector<Mesh_Instanced*> InstanceList;
	static vector<class TransformInfo*> TransformList;

	static GLuint ModelMatrixBuffer;
	static GLuint InstanceColorBuffer;

	static bool bInitializedVAO_Instanced;

	static bool bInitializeModelMatrixBuffer;

	
};