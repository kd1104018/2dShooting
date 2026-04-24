#include "GameScene.h"
#include "../SceneManager.h"
void GameScene::Init()
{
	m_tex.Load("Asset/Textures/Game.png");
	m_starttex.Load("Asset/Textures/start.png");
}

void GameScene::Update()
{
	/*if(GetAsyncKeyState(VK_CLEAR))*/
	m_alpha += m_alphaadd;
	if (m_alpha >= 1.0f)
	{
		m_alpha = 1.0f;
		m_alphaadd *= -1;
	}
	else if (m_alpha <= 0.4f)
	{
		m_alpha = 0.4f;
		m_alphaadd *= -1;
	}
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}
}

void GameScene::Draw2D()
{

	KdShaderManager::GetInstance().m_spriteShader.DrawCircle(100, 200, 100);



}

void GameScene::Release()
{
}
