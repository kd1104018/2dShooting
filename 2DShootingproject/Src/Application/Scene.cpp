#include "main.h"
#include "Scene.h"
#include "Scene/TitleScene/TitleScene.h"
#include "Scene/GameScene/GameScene.h"

void Scene::Draw2D()
{
	m_nowScene->Draw2D();
}

void Scene::Update()
{
	m_nowScene->Update();
}

void Scene::Init()
{
	m_nowScene = std::make_shared<TitleScene>();
}

void Scene::Release()
{
	
}

void Scene::ImGuiUpdate()
{
	
}
