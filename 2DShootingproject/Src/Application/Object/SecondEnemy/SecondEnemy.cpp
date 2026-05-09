#include "SecondEnemy.h"
#include "../../Scene/GameScene/GameScene.h"
#include"../../Object/EnemyBullet/EnemyBullet.h"
#include <cmath> // ★追加：sin関数を使うため

void SecondEnemy::Update()
{
	// ーーーーー 既存の移動処理 ーーーーー
	m_pos.y -= 3.0f;
	m_pos.x += std::sin(m_angle) * 5.0f;
	m_angle += 0.1f;

	// 画面の下に出たら消える
	if (m_pos.y < -500.0f) {
		m_aliveFlg = false;
	}

	// ーーーーー ★追加：弾の発射処理 ーーーーー
	m_shotTimer--;

	if (m_shotTimer <= 0)
	{
		// 弾を作る
		std::shared_ptr<EnemyBullet> bullet = std::make_shared<EnemyBullet>();
		bullet->Init();
		bullet->SetOwner(m_owner);
		bullet->SetPos(m_pos); // 自分の位置から発射

		// 弾の飛んでいく方向をセット（例：まっすぐ下に向かってスピード5.0fで飛ぶ）
		bullet->SetMoveVec({ 0.0f, -5.0f, 0.0f });

		// シーンに弾を追加
		m_owner->AddObject(bullet);

		// 次に撃つまでの時間をセット（120フレーム＝2秒）
		m_shotTimer = 120;
	}

	if (m_pos.y < -500.0f) {
		m_aliveFlg = false;
	}
}

void SecondEnemy::Draw()
{
	// ★"Texture/SecondEnemy.png" など、新しい画像を用意するか、
	// 既存の敵画像を使い回してもOKです！
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
}

void SecondEnemy::Release() {}