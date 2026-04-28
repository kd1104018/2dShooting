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
	if (GetAsyncKeyState('L') & 0x8000)
	{
		m_score += 10;
		if(m_score>9999999999)
			{
			m_score = 9999999999;
		}
	}
	if(GetAsyncKeyState('P')&0x8000)
		{

		m_score -=100;
		if(m_score<0)
			{
			m_score = 0;
		}
	}
	unsigned long tmp = m_score;
	for (int i = maxDigit-1; i  >=0; --i)
	{
		m_digits[i] = tmp % 10;
		tmp /= 10;
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
