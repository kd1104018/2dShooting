
#include "Enemy.h"

//void Enemy::Update()
//{
//	if (!m_aliveFlg) return;
//
//	// 1フレームごとに2度回転させる
//	m_angle += 2.0f;
//
//	// 360度を超えたらリセット（必須ではないですが、数値の肥大化防止）
//	if (m_angle >= 360.0f) m_angle -= 360.0f;
//	if (-640 - 36 > m_pos.x)
//	{
//		m_pos.x = 640 + 32; // 左に移動
//	}
//	else
//	{
//		m_pos.x -= 10.0f; // 左に移動
//	}
//	//上下運動
//	float moveY = 5.0f; // 上下の移動量
//	m_pos.y += moveY; // 上に移動
//	if (m_pos.y > 100)
//	{
//		moveY *= -1.0f; // 下に移動
//	}
//	else if (m_pos.y < -100)
//	{
//		moveY *= -1.0f; // 上に移動
//	}
//}
void Enemy::Draw()
{
	if (!m_aliveFlg) return;

	Math::Rectangle rc = { 0, 0, 64, 64, };

	// 第7引数に m_angle を追加
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(

		&m_tex, m_pos.x, m_pos.y, 64, 64, &rc);

	// テクスチャ,X座標,Y座標,幅,高さ,切り取り範囲

}


void Enemy::Init()
{
	m_tex.Load("Texture/enemy.png");
	m_pos = { 640,0 };
	m_angle = 0.0f;    // 0度で初期化
	m_aliveFlg = true;
}
void Enemy::Release()
{
	// テクスチャはkdTexture型のデストラクタでじどうでReleaseされるのでしなくてもよい
}
#include "Enemy.h"

void Enemy::Update()
{
	if (!m_aliveFlg) return;

	// 角度更新
	m_angle += 2.0f;
	if (m_angle >= 360.0f) m_angle -= 360.0f;

	// X移動（ラップ）
	if (-640 - 36 > m_pos.x)
	{
		m_pos.x = 640 + 32;
	}
	else
	{
		m_pos.x -= 10.0f;
	}

	// Y上下運動（メンバ変数 m_moveY を使って状態を保持）
	m_pos.y += m_moveY;

	// 範囲を超えたら方向を反転
	if (m_pos.y > 100.0f)
	{
		m_moveY = -std::abs(m_moveY);
	}
	else if (m_pos.y < -100.0f)
	{
		m_moveY = std::abs(m_moveY);
	}
}
