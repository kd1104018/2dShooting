#include "main.h"
#include "Scene.h"
#include "Scene/Title.h"

void Scene::Draw2D()
{
	m_titleScene->Draw2D();
}

void Scene::Update()
{
	
}

void Scene::Init()
{

	m_titleScene->Init();	
}

void Scene::Release()
{
	
}

void Scene::ImGuiUpdate()
{
	
}
