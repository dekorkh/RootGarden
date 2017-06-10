////////////////////////////////////////////////////////////////
//	
//	Triangles.cpp
//	
////////////////////////////////////////////////////////////////

#include "Main.h"

//--------------------------------------------------------------
//
// init

/*IsWGLExtensionSupported()
/*Retrieves a string of supported windows gl extensions and returns true if the passed name
/*is in the string of supported extensions.
*/
bool IsWGLExtensionSupported(const char* WGLExtensionName)
{
	PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionStr = NULL;	//ptr to a function that will return a pointer to a string of supported extensions
	_wglGetExtensionStr = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
	if (strstr(_wglGetExtensionStr(), WGLExtensionName) == NULL)
		return false;
	return true;
}


Game::Game() :
	DeltaSeconds_Display(0.0f),
	ClockLastFrame(CLOCKS_PER_SEC / 60),
	ClockMinFrame(0),
	ClockFrameLimiter(0),
	FrameIndex(0),
	AspectRatio(1.0f)
{
	pGameStats = GameStats::GetGameStats();

	ShaderManager* pShaderManager = ShaderManager::GetShaderManager();
	pShaderManager->AddShader("basic_vert", "basic.vert", ESHADER_TYPE::SHADER_TYPE_VERTEX);
	pShaderManager->AddShader("basic_lit_vert", "basic_lit.vert", ESHADER_TYPE::SHADER_TYPE_VERTEX);
	pShaderManager->AddShader("param_alpha_vert", "param_alpha.vert", ESHADER_TYPE::SHADER_TYPE_VERTEX);
	pShaderManager->AddShader("basic_instanced_vert", "basic_instanced.vert", ESHADER_TYPE::SHADER_TYPE_VERTEX);
	pShaderManager->AddShader("basic_frag", "basic.frag", ESHADER_TYPE::SHADER_TYPE_FRAGMENT);
	pShaderManager->AddShader("msaa_frag", "msaa.frag", ESHADER_TYPE::SHADER_TYPE_FRAGMENT);
	
	pShaderManager->AddShader("textureColor_vert", "textureColor.vert", ESHADER_TYPE::SHADER_TYPE_VERTEX);
	pShaderManager->AddShader("textureColor_frag", "textureColor.frag", ESHADER_TYPE::SHADER_TYPE_FRAGMENT);

	pShaderManager->AddShaderProgram("basic_prog", "basic_vert", "basic_frag");
	pShaderManager->AddShaderProgram("basic_lit_prog", "basic_lit_vert", "basic_frag");
	pShaderManager->AddShaderProgram("msaa_prog", "basic_vert", "msaa_frag");
	pShaderManager->AddShaderProgram("param_alpha_prog", "param_alpha_vert", "basic_frag");
	pShaderManager->AddShaderProgram("basic_instanced_prog", "basic_instanced_vert", "basic_frag");
	pShaderManager->AddShaderProgram("msaa_instanced_prog", "basic_instanced_vert", "msaa_frag");
	pShaderManager->AddShaderProgram("textureColor_prog", "textureColor_vert", "textureColor_frag");
	pShaderManager->GetShaderProgramByName("param_alpha_prog")->AddUniform("ParamAlpha", UNIFORM_F1);

	ShaderProgram::AddGlobalUniform("AspectRatio", UNIFORM_F1);
	ShaderProgram::AddGlobalUniform("ViewMatrix", UNIFORM_M4);

	ActiveScene = new Scene_Blocks();

	ViewMode = EViewMode::E_MODE_OCCLUSION_DEPTH;

	Log_Frame = new ofstream("Log_Frame.txt");
	*Log_Frame << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << endl;
	*Log_Frame << "CLOCKS_MIN_FRAME: " << ClockMinFrame << endl;
}

Game::~Game()
{
	Log_Frame->close();
	delete Log_Frame;
	delete ActiveScene;
}

void
Game::HandleInput(int Key, int x, int y, bool down)
{
	ActiveScene->HandleInput(Key, x, y, down);
}

void
Game::InitGraphics(Vector2i WindowSize)
{
	//Check & Handle extensions (like VSYNC)
	bool bWGLExtensionSupported = IsWGLExtensionSupported("WGL_EXT_swap_control");
	printf_s("WGL_EXT_swap_control: %d\n", bWGLExtensionSupported);
	if (bWGLExtensionSupported)
	{
		glSwapInterval = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
		glGetSwapInterval = (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress("wglGetSwapIntervalEXT");
		glSwapInterval(0); //VSYNC
	}

	//Check MSAA
	GLint bSupportsMSAA;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &bSupportsMSAA);
	cout << "MSAA framebuffer support: " << bSupportsMSAA << endl;
	cout << "\tBe sure to also check whether it is enabled during rendering." << endl;

	GLint MSAASamples;
	glGetIntegerv(GL_SAMPLES, &MSAASamples); // Check the number of MSAA samples per pixel.
	cout << "MSAA Samples: " << MSAASamples << "\n";
	
	switch (0)
	{
	case 0: // Normal
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case 1: // Subtract
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendEquation(GL_FUNC_SUBTRACT);
		break;
	}

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(MAXUINT);

	glEnable(GL_STENCIL_TEST);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, 1920, 1200);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//	Final fragment operation using incoming values and those which already exist in the buffer
	switch (0)
	{
	case 0:
		break;
	case 1:
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_EQUIV);  
		break;
	}

	//	Compute aspect ratio
	AspectRatio = static_cast<float>(WindowSize(0)) / WindowSize(1);
	ShaderProgram::SetGlobalUniform("AspectRatio", &AspectRatio);
	ShaderProgram::MarkGlobalUniformDirty("AspectRatio", true);

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error after InitGraphics(): " << err;
		throw err;
	}
};

//--------------------------------------------------------------
//
// display
//
void
Game::Display(void)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at enter Display(), frame : ";
		cout << pGameStats->pThisFrame->Index << " ";
		cout << "error :" << err << "\n";
		throw err;
	}

	DeltaSeconds_Display = static_cast<double>(clock() - ClockLastFrame) / CLOCKS_PER_SEC;
	ClockLastFrame = clock();
	pGameStats->AddFrame();
	pGameStats->pThisFrame->Index->Set(pGameStats->NumFrames());

	clock_t Clock_Tick = clock();
	ActiveScene->Tick(DeltaSeconds_Display);
	pGameStats->pThisFrame->Ms_Tick->Increment(TICKS_TO_MS(clock() - Clock_Tick));

	vector<SceneComponent*>* AllPrims = SceneComponent::GetPrims();
	SetupPrimitives(*AllPrims);
	
	SetupCamera(*ActiveScene->ActiveCamera);

	/////////////// EARLY DEPTH PASS //////////////////////////////////////////////////
	if (false)
	{
		glDisable(GL_BLEND);
		glDisable(GL_MULTISAMPLE_ARB);

		glDisable(GL_STENCIL_TEST);

		glEnable(GL_DEPTH_TEST);  // In fact render this first.  


		glColorMask(false, false, false, false);
		glDepthMask(true);

		glClearDepth(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (vector<SceneComponent*>::iterator it = AllPrims->begin(); it != AllPrims->end(); ++it)
		{
			clock_t Clock_DynamicCastMatter = clock();
			Matter* ActiveMatter = dynamic_cast<Matter*>(*it);
			pGameStats->pThisFrame->Ms_DynamicCastMatter->Increment(TICKS_TO_MS(clock() - Clock_DynamicCastMatter));

			if (ActiveMatter != nullptr && ActiveMatter->bIsOcclusion)
			{
				ActiveMatter->Draw(*ActiveScene);
			}
		}
	}
	/////////////// OCCLUSION QUERY ///////////////////////////////////////////////////////
	// Will fail with early depth because no samples will pass
	if (false)
	{
		glDisable(GL_BLEND);
		glDisable(GL_MULTISAMPLE_ARB);
		glDisable(GL_STENCIL_TEST);

		glEnable(GL_DEPTH_TEST);  // In fact render this first.  

		glDepthMask(true);

		vector<GLuint>* OcclusionQueries = SceneComponent::GetOcclusionQueries();
		OcclusionQueries->clear();
		OcclusionQueries->assign(AllPrims->size(), 0);
		ShaderProgram::glGenQueries_checked(static_cast<GLsizei>(AllPrims->size()), OcclusionQueries->data());

		pGameStats->pThisFrame->OcclusionQueries_Tested->Set(static_cast<int>(OcclusionQueries->size()));

		for (vector<SceneComponent*>::iterator it = AllPrims->begin(); it != AllPrims->end(); ++it)
		{
			clock_t Clock_DynamicCastMatter = clock();
			Matter* ActiveMatter = dynamic_cast<Matter*>(*it);
			pGameStats->pThisFrame->Ms_DynamicCastMatter->Increment(TICKS_TO_MS(clock() - Clock_DynamicCastMatter));

			if (ActiveMatter != nullptr && ActiveMatter->bIsOcclusion)
			{
				GLuint QueryIndex = it - AllPrims->begin();
				GLuint Query = OcclusionQueries->at(QueryIndex);
				ShaderProgram::glBeginQuery_checked(GL_SAMPLES_PASSED, Query);
				ActiveMatter->Draw(*ActiveScene);
				ShaderProgram::glEndQuery_checked(GL_SAMPLES_PASSED);
			}
		}
	
		//HARVEST OCCLUSION QUERIES
		GLint QueryReady;
		GLuint QueryResult;
		for (auto const &Query : *OcclusionQueries)
		{
			// Not all queries generated will result in valid objects since they are generated for AllPrims and not all of them are occlusion objects.
			if (ShaderProgram::glIsQuery_checked(Query) == GL_FALSE)
			{
				continue;
			}

			int sanity = 1000;
			QueryReady = false;
			while (!QueryReady && sanity--)
			{
				ShaderProgram::glGetQueryObjectiv_checked(Query, GL_QUERY_RESULT_AVAILABLE, &QueryReady);
			}

			if (QueryReady)
			{
				pGameStats->pThisFrame->OcclusionQueries_Collected->Increment(1);
				ShaderProgram::glGetQueryObjectuiv_checked(Query, GL_QUERY_RESULT, &QueryResult);
				pGameStats->pThisFrame->OcclusionSamples_Passed->Increment(QueryResult);
			}
		}
	}

	/////////////// STENCIL PASS /////////////////////////////////////////////////////////////
	if (false)
	{
		glDisable(GL_BLEND);
		glDisable(GL_MULTISAMPLE_ARB);
		
		glEnable(GL_STENCIL_TEST);
				
		glColorMask(false, false, false, false);
		glDepthMask(false);
		glClearStencil(0x0);
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 0x1, 0x1); //pass always, ref val is 1, mask is 1
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		
		for (vector<SceneComponent*>::iterator it = AllPrims->begin(); it != AllPrims->end(); ++it)
		{
			clock_t Clock_DynamicCastMatter = clock();
			Matter* ActiveMatter = dynamic_cast<Matter*>(*it);
			pGameStats->pThisFrame->Ms_DynamicCastMatter->Increment(TICKS_TO_MS(clock() - Clock_DynamicCastMatter));

			if (ActiveMatter != nullptr && ActiveMatter->bIsStencil)
			{
				//Issue the draw (stream the vert info if needed.)
				ActiveMatter->Draw(*ActiveScene);
			}
		}
	}

	/////////////// BASE PASS /////////////////////////////////////////////////////////////
	if (true)
	{
		glEnable(GL_BLEND);
		
		glEnable(GL_MULTISAMPLE_ARB); // MSAA \o/
		glMinSampleShading(1.0f);

		glClearColor(0.2f, 0.18f, 0.17f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);

		// Issue the drawcalls
		if (false)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_EQUAL, 0x0, 0x1); //if val is equal to ref, ref val is 1, mask is 1
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}

		glColorMask(true, true, true, true);
		glDepthMask(true);

		for (vector<SceneComponent*>::iterator it = AllPrims->begin(); it != AllPrims->end(); ++it)
		{
			clock_t Clock_DynamicCastMatter = clock();
			Matter* ActiveMatter = dynamic_cast<Matter*>(*it);
			pGameStats->pThisFrame->Ms_DynamicCastMatter->Increment(TICKS_TO_MS(clock() - Clock_DynamicCastMatter));
		
			if (ActiveMatter != nullptr && !ActiveMatter->bIsStencil)
			{	
				//Issue the draw (stream the vert info if needed.)
				ActiveMatter->Draw(*ActiveScene);
			}
		}
	}
	// Set LastProg address to -1 to go through prog switch codepath and cause 
	// global state changes to refresh between frames.  Their update is bundled 
	// with program switching.
	ShaderProgram::LastProgAddr = -1;


	clock_t Clock_Swap = clock();
	glutSwapBuffers();	// swap buffers
	GameStats::GetGameStats()->pThisFrame->Ms_Swap->Set(TICKS_TO_MS(clock() - Clock_Swap));

	while ((clock() - ClockLastFrame) < ClockMinFrame)
		1;
	
	pGameStats->pThisFrame->Ms_Total->Set(TICKS_TO_MS(clock() - ClockLastFrame));


	glutPostRedisplay(); //call redraw window
}

void Game::SetupPrimitives(vector<SceneComponent*> const &InPrims) const
{
	GameStatsFrame* pThisFrame = GameStats::GetGameStats()->pThisFrame;	

	//  Futures are used to harvest the dispatched threads
	vector<future<void>> BuildParametersFutures;
	vector<future<void>> UpdateModelMatrixFutures;
	vector<future<void>> GenerateMeshFutures;
	
	GenerateMeshFutures.reserve(InPrims.size());	
	UpdateModelMatrixFutures.reserve(InPrims.size());	
	BuildParametersFutures.reserve(InPrims.size());

	//	Results are attached to each dispatched thread and are used to return information about the work done
	vector<GenerateMeshResult> GenerateMeshResults;
	GenerateMeshResults.resize(InPrims.size());
	
	clock_t Clock_Ms_BuildParameters = clock();
	for (GLuint i = 0; i < InPrims.size(); ++i)
	{
		SceneComponent* ActiveComponent = InPrims.at(i);
		if (ActiveComponent->IsDirtyComponentParameters())
			BuildParametersFutures.push_back(async(&SceneComponent::Build, ActiveComponent));
	}
	
	//	Harvest BuildParameters - async
	for (GLuint i = 0; i < BuildParametersFutures.size(); ++i)
	{
		BuildParametersFutures.at(i).get();
	}
	pThisFrame->Ms_BuildParameters->Set(TICKS_TO_MS(clock() - Clock_Ms_BuildParameters));

	// Kick off generate meshes - async (compute model matrix is rolled in here as well.)
	clock_t Clock_Ms_GenerateMesh = clock();

	GLuint iMatter = 0;
	for (GLuint i = 0; i < InPrims.size(); ++i)
	{
		SceneComponent* ActiveComponent = InPrims.at(i);
		Matter* ActiveMatter = dynamic_cast<Matter*>(ActiveComponent);
		if (ActiveMatter != nullptr)
		{
			if (ActiveMatter->IsMeshDirty())
				GenerateMeshFutures.push_back(async(&Matter::UpdateMesh, ActiveMatter, &GenerateMeshResults[iMatter]));
			if (ActiveMatter->IsModelMatrixDirty())
				UpdateModelMatrixFutures.push_back(async(&Matter::UpdateModelMatrix, ActiveMatter));
			iMatter++;
		}	
	}

	clock_t Clock_Total_Positions = 0;
	clock_t Clock_Total_Colors = 0;
	clock_t Clock_Total_Indices = 0;
	clock_t Clock_Total_Normals = 0;
	clock_t Clock_Total_ModelMatrix = 0;

	// Harvest generate meshes - async
	for (GLuint i = 0; i < GenerateMeshFutures.size(); ++i)
	{
		GenerateMeshFutures.at(i).get();
		Clock_Total_Positions += GenerateMeshResults.at(i).Clock_GenerateMesh_Positions;
		Clock_Total_Normals += GenerateMeshResults.at(i).Clock_GenerateMesh_Normals;
		Clock_Total_Colors += GenerateMeshResults.at(i).Clock_GenerateMesh_Colors;
		Clock_Total_Indices += GenerateMeshResults.at(i).Clock_GenerateMesh_Indices;		
	}

	Clock_Ms_GenerateMesh = clock() - Clock_Ms_GenerateMesh;

	clock_t Clock_SumMeshThreads = Clock_Total_Positions + Clock_Total_Colors + Clock_Total_Indices + Clock_Total_Normals;
 	double Pct_Positions = static_cast<double>(Clock_Total_Positions) / fmax(Clock_SumMeshThreads, 1);
	double Pct_Normals = static_cast<double>(Clock_Total_Normals) / fmax(Clock_SumMeshThreads, 1);
	double Pct_Colors = static_cast<double>(Clock_Total_Colors) / fmax(Clock_SumMeshThreads, 1);
	double Pct_Indices = static_cast<double>(Clock_Total_Indices) / fmax(Clock_SumMeshThreads, 1);
	
	float Ms_Total_Mesh = TICKS_TO_MS(clock() - Clock_Ms_GenerateMesh);
	pThisFrame->Ms_GenerateMesh->Set(static_cast<float>(Clock_Ms_GenerateMesh));
	pThisFrame->Ms_GenerateMesh_Positions->Set(static_cast<float>(Clock_Total_Positions));
	pThisFrame->Ms_GenerateMesh_Normals->Set(static_cast<float>(Clock_Total_Normals));
	pThisFrame->Ms_GenerateMesh_Colors->Set(static_cast<float>(Clock_Total_Colors));
	pThisFrame->Ms_GenerateMesh_Indices->Set(static_cast<float>(Clock_Total_Indices));

	clock_t Clock_ComputeModelMatrix = clock();
	// Harvest compute model matrix - async
	for (GLuint i = 0; i < UpdateModelMatrixFutures.size(); ++i)
	{
		UpdateModelMatrixFutures.at(i).get();
	}
	pThisFrame->Ms_ComputeModelMatrix->Set(TICKS_TO_MS(clock() - Clock_ComputeModelMatrix));
}

void Game::SetupCamera(Camera &InCamera) const
{
	InCamera.Transform.ComputeModelMatrix();
}

Game* GameInstancePtr; // Forward declare

void Display()
{
	GameInstancePtr->Display();
	glutPostRedisplay();
}

void Keyboard(unsigned char Key, int x, int y)
{
	GameInstancePtr->HandleInput(Key, x, y, true);
}

void Keyboard2_down(int Key, int x, int y)
{
	GameInstancePtr->HandleInput(Key, x, y, true);
}

void Keyboard2_up(int Key, int x, int y)
{
	GameInstancePtr->HandleInput(Key, x, y, false);
}

//--------------------------------------------------------------
//
// main
//
int
main(int argc, char** argv)
{
	//Set-up random gen
	srand(time(NULL));

	glutInit(&argc, argv);				//? ***
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);		//? ** these are frame buffer properties

	glutInitWindowSize(819 * 1.5, 512 * 1.5);
	glutInitWindowPosition(0, 0);		
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);	//OpenGL has different profiles and extensions since 3.2 that allow different, sometime esoteric compatibility.
	glutCreateWindow(argv[0]);
	GLenum err;
	//glutFullScreen();

	glewExperimental = GL_TRUE;	// -_- (https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library)
	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit(EXIT_FAILURE);
	}
	while ((err = glGetError()) != GL_NO_ERROR)
		cout << "Error at glewInit() : possibly due to https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library and safe to ignore." << endl;

	//Pass window size into init, obviously important drawing to screen
	Vector2i WindowSize = Vector2i(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	
	GameInstancePtr = new Game();
	GameInstancePtr->InitGraphics(WindowSize);
	
	//Register window's update loop function, runs indefinitely once glut main loop is called.  There's an Idle function as well for capping framerate
	glutDisplayFunc(Display);
	//Register keyboard input callback
	glutKeyboardFunc(Keyboard);		// alpha numeric keys
	glutSpecialFunc(Keyboard2_down); // special keys such arrow keys (callback for on down)
	glutSpecialUpFunc(Keyboard2_up); // special keys such arrow keys (callback for on up)
	
	glutMainLoop();
	
	// Cleanup
	delete GameInstancePtr;
}