#include "TitleScene.h"
#include "../SceneManager.h"

void TitleScene::Init()
{
	m_tex.Load("Texture/title.png");
	m_starttex.Load("Texture/startbtn.png");
	m_rogotex.Load("Texture/rogo.png");

	m_logoPos = { 0, 100 };

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
	if (m_inputWaitTimer > 0) {
		m_inputWaitTimer--;
	}

	// 2. タイマーが0のときだけ、ボタン判定を行う
	if (m_inputWaitTimer <= 0) {
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
		}
	}

	Math::Vector2 btnPos = { 0, -100 };
	Math::Vector2 btnSize = { 200, 80 };
	
	

	

	
	

	// ボタンをゆっくり点滅させる演出
	m_btnalpha = 0.7f + std::sin(m_timer * 0.1f) * 0.3f;
	m_timer++;
}

void TitleScene::Draw2D()
{

	KdShaderManager::GetInstance().m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0,0,1280,720 });

	Math::Color color = { 1, 1, 1, m_btnalpha };
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_starttex,
		0, -100,                     // 位置
		(int)(800 * m_btnscale),     // 幅（スケール適用）
		(int)(400 * m_btnscale),      // 高さ（スケール適用）
		nullptr, &color);
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(&m_rogotex,
		(int)m_logoPos.x, (int)m_logoPos.y,
		512, 128,    // ロゴの表示サイズ（画像の比率に合わせて調整してください）
		nullptr);



}

void TitleScene::Release()
{
}
