#include "BossBullet.h"
#include "../../Scene/GameScene/GameScene.h"

void BossBullet::Update()
{
	
	
	m_pos += m_dir * m_speed; // 方向ベクトルに速度を掛けて移動量を計算し、位置に加算

	// 画面外に出たら消える
	if (m_pos.y < -360.0f || m_pos.x < -640.0f || m_pos.x > 640.0f || m_pos.y > 360.0f) {
		m_aliveFlg = false;
	}
}

void BossBullet::Draw()
{

	Math::Rectangle rc = { 0, 0, 16,16 };
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, m_pos.x, m_pos.y, 36, 36, &rc);
}

void BossBullet::Init()
{
	m_tex.Load("Texture/enemybullet.png"); //
	m_pos = {};
	
	m_aliveFlg = true;
	m_objType = ObjectType::BossBullet; // 種類をセット
}

void BossBullet::OnHit()
{
	m_aliveFlg = false; // プレイヤーやバリアに当たったら消える
}

void BossBullet::Release()
{
}
