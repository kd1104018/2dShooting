#include "Life.h"
#include"../../Scene/GameScene/GameScene.h"

void Life::Update()
{
	//HPを減らすときに呼び出す

}

void Life::Draw()
{
	Math::Rectangle rc = { 0,0,32,32 };

	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, m_pos.x, m_pos.y, 32, 32, &rc);
}

void Life::Init()
{
	m_tex.Load("Texture/Life.png");
	m_pos = {};
	m_aliveFlg = true;
	m_objType = ObjectType::Life;		// 種類は「弾」
}

void Life::OnHit()
{
	m_aliveFlg = false;
	
}

void Life::Release()
{
}