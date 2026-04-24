#include "TitleScene.h"
#include "../SceneManager.h"

void TitleScene::Init()
{
	m_tex.Load("Texture/title.png");
	
}

void TitleScene::Update()
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
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
	}
}

void TitleScene::Draw2D()
{

	KdShaderManager::GetInstance().m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0,0,1280,720 });


	SHADER.m_spriteShader.DrawString(0, 0, "Hellow", Math::Vector4(1, 1, 0, 1));
	


}

void TitleScene::Release()
{
}
