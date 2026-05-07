
#include "Enemy.h"
#include"../../Scene/GameScene/GameScene.h"


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
	m_objType = ObjectType::Enemy;		// 種類は「敵」
}
void Enemy::OnHit()
{
	m_aliveFlg = false;	// 当たったときは生存フラグをfalseにする
}
void Enemy::Release()
{
	// テクスチャはkdTexture型のデストラクタでじどうでReleaseされるのでしなくてもよい
}

void Enemy::Update()
{
	if (!m_aliveFlg) return;

	// 角度更新
	m_angle += 5.0f;    // 5度ずつ回転

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
	for (auto& obj : m_owner->GetObjList())
	{

		// オブジェクトに対する処理
		if (obj->GetObjType() == ObjectType::Bullet)
		{

			// 対象の座標（ベクトル） - 自分の座標（ベクトル） = 対象へのベクトル（矢印）
			Math::Vector3 v;
			v = obj->GetPos() - m_pos;
			//弾判定...ベクトルの長さで判定
			if (v.Length() < 64.0f)
			{
				obj->OnHit();	// 当たったときの処理
				m_aliveFlg = false;	// 敵は当たったときに消える

			}
		}
	}
}
