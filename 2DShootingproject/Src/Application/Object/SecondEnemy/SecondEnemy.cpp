#include "SecondEnemy.h"
#include "../../Scene/GameScene/GameScene.h"
#include"../../Object/EnemyBullet/EnemyBullet.h"
#include"../../Object/Explosion/Explosion.h"
#include <cmath> // ★追加：sin関数を使うため

void SecondEnemy::Update()
{

	m_pos.x -= 5.0f;
	
	m_angle += 0.1f;

	// 画面の下に出たら消える

	

	
	m_shotTimer--;

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
	for (auto& obj : m_owner->GetObjList())
	{
		// 相手が「プレイヤーの弾」だった場合
		if (obj->GetObjType() == BaseObject::ObjectType::Bullet)
		{
			// 自分(SecondEnemy)と弾の距離を計算
			Math::Vector3 v = obj->GetPos() - m_pos;
			float distance = v.Length();

			// 判定距離（自分32 + 弾8 くらいで 40.0f）
			if (distance < 48.0f)
			{
				// 相手（弾）に当たった通知を送る
				obj->OnHit();

				// 自分も当たった処理（スコア加算して消える）を呼ぶ
				this->OnHit();

				// 1発当たれば十分なのでループを抜ける
				break;
			}
		}
	}


	if (m_pos.y < -400.0f) {
		m_aliveFlg = false;
	}
}

void SecondEnemy::Draw()
{
	
	Math::Rectangle rc = { 0, 0, 64, 64 };
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, m_pos.x, m_pos.y, 64, 64, &rc);
}

void SecondEnemy::Init()
{

	m_tex.Load("Texture/secondenemy.png");
	m_pos = {};
	m_aliveFlg = true;
	m_angle = 0.0f;
	m_shotTimer = 60; // 最初は1秒後（60フレーム後）に撃つ
	m_objType = ObjectType::Enemy;
}

void SecondEnemy::OnHit()
{
	// 弾やプレイヤーに当たったら消える
	m_aliveFlg = false;
	m_owner->m_score += 100;
	if (m_owner) {
		auto exp = std::make_shared<Explosion>();
		exp->Init();
		exp->SetPos(m_pos); // 敵がいた場所にセット

		// GameSceneのオブジェクトリストに追加してもらう
		// (GameSceneにAddObjectのような関数を作っておくと楽です)
		m_owner->AddObject(exp);

		// スコア加算も忘れずに
		m_owner->AddScore(100);
	}
}

void SecondEnemy::Release() {}
