#include "SceneManager.h"
#include"TitleScene/TitleScene.h"
#include"GameScene/GameScene.h"

void SceneManager::Update()
{
	if (m_currentSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}
	m_CurrentScene->Update();
	
}

void SceneManager::Draw()
{
	m_CurrentScene->Draw2D();
}

void SceneManager::Init()
{
	ChangeScene(m_currentSceneType);
}

void SceneManager::Release()
{
}

void SceneManager::ChangeScene(SceneType _Scenetype)
{
	//次のシーンを作成し、現在のシーンにする
	switch (_Scenetype)
	{
	case SceneType::Title:
	{
		m_CurrentScene = std::make_shared<TitleScene>();
		break;
	}
	case SceneType::Game:
	{
		m_CurrentScene = std::make_shared<GameScene>();
		break;
	}
	}

	//2 シーン管理を更新
	m_currentSceneType = _Scenetype;
}