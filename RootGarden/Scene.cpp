#include "Scene.h"
#include "GlobalState.h"


Scene::Scene() : BackgroundColor(Vector4f::Constant(0.5f)), ElapsedSeconds(0.0f)
{
}

void Scene::HandleInput(int Key, int x, int y, bool down)
{
	if (MODE_PRINT_INPUT)
	{
		printf_s("Scene::HandleInput(%d, %d, %d, %i\n)", Key, x, y, down);
	}
}

void Scene::EndScene()
{
	GameStats::GetGameStats()->WriteToFile("GameStats.csv");
	glutLeaveMainLoop();
}