#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Object//Player/Player.h"
#include "../../Object//Enemy/Enemy.h"


void GameScene::Init()
{
	// ②初期化	
	m_player = std::make_shared<Player>();	// ①インスタンス確保と初期化	
	m_player->Init();							// ②初期化	
	//m_enemy = std::make_shared<Enemy>();	// ①インスタンス確保と初期化	
	//m_enemy->Init();							// ②初期化	
	m_enemy = std::make_shared<Enemy>();	// ①インスタンス確保と初期化	
	m_enemy->Init();
}

void GameScene::Update()
{
	m_player->Update();
	m_enemy->Update();

	if (GetAsyncKeyState('Z') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}
}

void GameScene::Draw2D()
{

	m_player->Draw();
	m_enemy->Draw();



}

void GameScene::Release()
{
}
