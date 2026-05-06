#include "Bullet.h"

void Bullet::Update()
{
	m_pos.y += 10.0f;

	// 画面外に出たら寿命切れ
	if (m_pos.x <620 )
	{
		m_aliveFlg = false;
	}
}

void Bullet::Draw()
{
	Math::Rectangle rc = { 0,0,13,37 };

	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, m_pos.x, m_pos.y, 13, 37, &rc);
}

void Bullet::Init()
{
	m_tex.Load("Asset/Textures/bullet.png");
	m_pos = {};
	m_aliveFlg = true;
	m_objType = ObjectType::Bullet;		// 種類は「弾」
}

void Bullet::Release()
{
}