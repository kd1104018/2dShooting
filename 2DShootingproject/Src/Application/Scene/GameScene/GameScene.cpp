#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Object//Player/Player.h"

void GameScene::Init()
{
}

void GameScene::Update()
{
	m_player->Update();

	if (GetAsyncKeyState('Z') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}
}

void GameScene::Draw2D()
{

	m_player->Draw();

	

}

void GameScene::Release()
{
}
