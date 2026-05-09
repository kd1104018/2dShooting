#include "Shield.h"
#include"../../Scene/GameScene/GameScene.h"

void Shield::Update()
{
	
}

void Shield::Draw()
{
	
	// 修正：m_aliveFlg ではなく m_isVisible で判定する！
	if (m_isVisible == true)
	{
		Math::Rectangle rc = { 0,0,80,80 };
		KdShaderManager::GetInstance().m_spriteShader.DrawTex(
			&m_tex, m_pos.x, m_pos.y, 80, 80, &rc);
	}
}

void Shield::Init()
{
	m_tex.Load("Texture/Shield.png");
	m_pos = {};
	m_aliveFlg = true; // ★ずっとtrueのままにします！
	m_objType = ObjectType::Shield;
	m_isVisible = false; // 最初は非表示
}

// 追加：シールド発動処理


void Shield::OnHit()
{
}

void Shield::Release()
{
}