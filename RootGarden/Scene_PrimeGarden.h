#pragma once

#include "Scene.h"

class Scene_PrimeGarden : public Scene
{
public:
	Scene_PrimeGarden();
	~Scene_PrimeGarden();

	static bool IsPrime(int n);
};