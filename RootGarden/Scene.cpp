#include "Scene.h"

Scene::Scene() : 
	BackgroundColor(Vector4f::Constant(0.5f)), 
	ElapsedSeconds(0.0f),
	ActiveCamera(new Camera())
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

void Scene::Tick(double deltaSeconds)
{
	SceneComponent::Tick(deltaSeconds);
	ActiveCamera->Tick(deltaSeconds);
}