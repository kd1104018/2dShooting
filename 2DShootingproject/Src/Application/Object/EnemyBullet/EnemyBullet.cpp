#include "EnemyBullet.h"
#include "../../Scene/GameScene/GameScene.h"

void EnemyBullet::Update()
{
	// セットされた方向に飛ぶ
	m_pos += m_moveVec;

	// 画面外に出たら消える
	if (m_pos.y < -360.0f || m_pos.x < -640.0f || m_pos.x > 640.0f || m_pos.y > 360.0f) {
		m_aliveFlg = false;
	}
}

void EnemyBullet::Draw()
{

	Math::Rectangle rc = { 0, 0, 16,16 };
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, m_pos.x, m_pos.y, 16, 16, &rc);
}

void EnemyBullet::Init()
{
	m_tex.Load("Texture/EnemyBullet.png"); //
	m_pos = {};
	m_moveVec = { 0.0f, -15.0f, 0.0f }; // デフォルトは下
	m_aliveFlg = true;
	m_objType = ObjectType::EnemyBullet; // 種類をセット
}

void EnemyBullet::OnHit()
{
	m_aliveFlg = false; // プレイヤーやバリアに当たったら消える
}

void EnemyBullet::Release()
{
}
