#include "Bullet.h"
#include"../../Scene/GameScene/GameScene.h"

void Bullet::Update()
{
	m_pos.x += 10.0f;

	//// 画面外に出たら寿命切れ
	if (m_pos.x>640.0f)
	{
		m_aliveFlg = false;
	}
	
}

void Bullet::Draw()
{
	Math::Rectangle rc = { 0,0,37,13 };

	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, m_pos.x, m_pos.y, 37, 13, &rc);
}

void Bullet::Init()
{
	m_tex.Load("Texture/bullet.png");
	m_pos = {};
	m_aliveFlg = true;
	m_objType = ObjectType::Bullet;		// 種類は「弾」
}

void Bullet::OnHit()
{
	m_aliveFlg = false;	// 当たったときに消える
}

void Bullet::Release()
{
}