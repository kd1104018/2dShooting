#include "GameClear.h"
#include "../SceneManager.h"

void GameClearScene::Init()
{
	m_tex.Load("Texture/Gameclear.png");
	m_cntnewtex.Load("Texture/cntnew.png");
	m_numberTex.Load("Texture/sucore.png");

}

void GameClearScene::Update()
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
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}

	Math::Vector2 btnPos = { 0, -100 };
	Math::Vector2 btnSize = { 200, 80 };

	// ボタンをゆっくり点滅させる演出
	m_btnalpha = 0.7f + std::sin(m_timer * 0.1f) * 0.3f;
	m_timer++;
}

void GameClearScene::Draw2D()
{

	KdShaderManager::GetInstance().m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0,0,1280,720 });

	Math::Color color = { 1, 1, 1, m_btnalpha };
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_cntnewtex,
		0, -100,                     // 位置
		(int)(800 * m_btnscale),     // 幅（スケール適用）
		(int)(400 * m_btnscale),      // 高さ（スケール適用）
		nullptr, &color);
	unsigned long score = SceneManager::Instance().GetFinalScore();
	int digits[10];
	int maxDigit = 8;
	unsigned long tempScore = score;
	for (int i = 0; i < maxDigit; i++)
	{
		digits[i] = tempScore % 10;
		tempScore /= 10;
	}
	for (int i = 0; i < maxDigit; i++) {
		int num = digits[i];

		int width = 281;
		int height = 384;

		int column = num % 5;
		int row = num / 5;

		Math::Rectangle rc = { column * width, row * height, width, height };

		// 画面上の表示位置
		float xPos = 100 - (i * 35);
		float yPos = -300;


		float drawY = yPos;
		if (row == 1) {

			drawY -= 6.0f;
		}
		KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_numberTex, (int)xPos, (int)drawY, 40, 55, &rc);
	}

	

}

void GameClearScene::Release()
{
}
