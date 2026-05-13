
#include "Enemy.h"
#include"../../Object/EnemyBullet/EnemyBullet.h"
#include"../../Scene/GameScene/GameScene.h"
#include"../../Object/Explosion/Explosion.h"


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
	m_hp = 1;

}
void Enemy::OnHit()
{
	
	
		m_hp--; // HPを1減らす

		// ダメージを受けたら、少しの間（例：10フレーム）無敵にする
		

		// HPが0以下になったら消滅
		if (m_hp <= 0)
		{
			m_aliveFlg = false;
			m_owner->m_score += 100;

			if (m_owner) {
				auto exp = std::make_shared<Explosion>();
				exp->Init();
				exp->SetPos(m_pos); // 敵がいた場所にセット

				
				m_owner->AddObject(exp);

				// スコア加算も忘れずに
				m_owner->AddScore(100);
			}
		}
	
}

void Enemy::Release()
{
	// テクスチャはkdTexture型のデストラクタでじどうでReleaseされるのでしなくてもよい
}

void Enemy::Update()
{

	
	if (!m_aliveFlg) return;

	
	
	
	// X移動（ラップ）
	if (-640 - 36 > m_pos.x)
	{
		m_pos.x = 640 + 32;
	}
	else
	{
		m_pos.x -= 10.0f;
	}

	
	m_pos.y += m_moveY;

	// 範囲を超えたら方向を反転
	if (m_pos.y > 300.0f)
	{
		m_moveY = -std::abs(m_moveY);
	}
	else if (m_pos.y < -300.0f)
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
			if (v.Length() < 48.0f)
			{
				obj->OnHit();	// 当たったときの処理
				OnHit();			// 敵に当たったときの処理

			}
		}
	}

	m_shotTimer--; // タイマーを減らす

	if (m_shotTimer <= 0)
	{
		std::shared_ptr<EnemyBullet> bullet = std::make_shared<EnemyBullet>();
		bullet->Init();
		bullet->SetOwner(m_owner);
		bullet->SetPos(m_pos);

		if (m_attackType == 0) {
			
			bullet->SetDir({ -3.0f, 0.0f, 0.0f });
			bullet->SetSpeed(5.0f);
		}
		else {
			// 【タイプ1】プレイヤーを狙って撃つ
			Math::Vector3 playerPos = m_pos;
			for (auto& obj : m_owner->GetObjList()) {
				if (obj->GetObjType() == ObjectType::Player) {
					playerPos = obj->GetPos();
					break;
				}
			}

			Math::Vector3 dir = playerPos - m_pos;
			if (dir.Length() > 0) {
				dir.Normalize();

				
				bullet->SetDir(dir);      // 向きをセット
				bullet->SetSpeed(10.0f);   // スピードをセット
			}
		}

		m_owner->AddObject(bullet);
		m_shotTimer = 120;
	}
}
