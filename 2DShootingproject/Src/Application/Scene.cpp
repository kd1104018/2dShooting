#include "main.h"
#include "Scene.h"
#include "Scene/SceneManager.h"
void Scene::Draw2D()
{
	SceneManager::Instance().Draw();
	
}

void Scene::Update()
{
	SceneManager::Instance().Update();
}

void Scene::Init()
{
	
}


void Scene::Release()
{
	
}

void Scene::ImGuiUpdate()
{
	
}
