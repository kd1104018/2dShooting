
#include "Boss.h"
#include"../../Object/BossBullet/BossBullet.h"
#include"../../Scene/GameScene/GameScene.h"
#include"../../Object/Explosion/Explosion.h"


void Boss::Draw()
{
	if (!m_aliveFlg) return;

	// 現在のコマに応じた切り取り範囲（Rectangle）を計算
	Math::Rectangle rc = {
		m_animeIdx * CHIP_W,   // 切り取り開始X
		m_currentLine * CHIP_H, // 切り取り開始Y
		CHIP_W,               // 切り取り幅
		CHIP_H                // 切り取り高さ
	};

	// 描画（少し大きく見せる）
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_boss,
		(int)m_pos.x, (int)m_pos.y,
		300, 300,  // 表示サイズ
		&rc        // 切り取り範囲
	);

}


void Boss::Init() 
{
	m_boss.Load("Texture/Boss.png");
	m_pos = { 300.0f, 0.0f, 0.0f }; // 最初は画面のさらに右に配置
	m_aliveFlg = true;
	m_hp = 20; // ボスのHPを設定

	m_animeCount = 0;
	m_animeIdx = 0;
	m_maxAnime = 10;   // 1行目のコマ数
	m_animeSpeed = 6;   // アニメーション速度
	m_currentLine = 0;
}
void Boss::OnHit()
{
	
	
		m_hp--; // HPを1減らす

		// ダメージを受けたら、少しの間（例：10フレーム）無敵にする
		

		// HPが0以下になったら消滅
		if (m_hp <= 0)
		{
			m_aliveFlg = false;
			

			if (m_owner) {
				auto exp = std::make_shared<Explosion>();
				exp->Init();
				exp->SetPos(m_pos); // 敵がいた場所にセット

				// GameSceneのオブジェクトリストに追加してもらう
				// (GameSceneにAddObjectのような関数を作っておくと楽です)
				m_owner->AddObject(exp);

				// スコア加算も忘れずに
				
			}
		}
	
}
void Boss::Release()
{
	// テクスチャはkdTexture型のデストラクタでじどうでReleaseされるのでしなくてもよい
}

void Boss::Update()
{
	m_animeCount++;
	if (m_animeCount >= m_animeSpeed) {
		m_animeCount = 0;
		m_animeIdx++;

		// ループ再生
		if (m_animeIdx >= m_maxAnime) {
			m_animeIdx = 0;
		}
	}

	// 1. 登場シーン（ゆっくり左へ移動して定位置へ）
	if (m_pos.x > 300.0f) {
		m_pos.x -= 2.0f;
		return;
	}

	if (m_attackTimer <= 0)
	{
		// 弾を生成
		Shoot();

		// 次の攻撃までの待ち時間（120フレーム = 2秒に1回）
		m_attackTimer = 90;
	}

	m_attackTimer--;

	if (m_attackTimer <= 0) {
		
		m_attackPhase = (rand() % 2) + 1;

		if (m_attackPhase == 1) {
			m_shotCount = 0;       // 連射カウントリセット
			m_attackTimer = 180;   // 3秒間(180F)の攻撃時間を確保
		}
		else {
			ShootSpread();         // 拡散弾は一瞬で撃つ
			m_attackTimer = 120;   // 撃った後の硬直時間
		}
	}

	// --- 連射攻撃(Phase 1)の実行中処理 ---
	if (m_attackPhase == 1 && m_attackTimer > 0) {
		m_shotInterval--;
		if (m_shotInterval <= 0) {
			ShootTarget();         // 自機を狙う弾
			m_shotInterval = 10;   // 10フレームに1発（秒間6発）
			m_shotCount++;
		}
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
			if (v.Length() < 60.0f)
			{
				obj->OnHit();	// 当たったときの処理
				OnHit();			// 敵に当たったときの処理

			}
		}
	}
}
void Boss::Shoot()
{
	if (!m_owner) return;

	// 敵の弾（EnemyBullet）を作成
	std::shared_ptr<BossBullet> bullet = std::make_shared<BossBullet>();
	bullet->Init();
	
	// ボスの現在地から発射
	bullet->SetPos(m_pos);
	
	// GameSceneのリストに追加
	m_owner->AddObject(bullet);
}
void Boss::ShootTarget() {
	if (!m_owner || !m_owner->GetPlayer()) return;

	auto bullet = std::make_shared<BossBullet>();
	bullet->Init();
	bullet->SetPos(m_pos);

	// 自機への方向ベクトルを計算
	Math::Vector3 targetPos = m_owner->GetPlayer()->GetPos();
	Math::Vector3 dir = targetPos - m_pos;
	dir.Normalize(); // 長さを1にする

	// 弾に方向をセット（EnemyBullet側にSetDir関数などを作っておく）
	bullet->SetDir(dir);
	bullet->SetSpeed(8.0f); // 少し速め

	m_owner->AddObject(bullet);
}
void Boss::ShootSpread() {
	if (!m_owner) return;

	// 5方向くらいに広げる
	for (int i = -2; i <= 2; i++) {
		auto bullet = std::make_shared<BossBullet>();
		bullet->Init();
		bullet->SetPos(m_pos);

		// 基本は左方向（-1, 0, 0）
		// yに少し変化をつけて拡散させる
		Math::Vector3 dir = { -1.0f, i * 0.2f, 0.0f };
		dir.Normalize();

		bullet->SetDir(dir);
		bullet->SetSpeed(5.0f); // 拡散弾は避けやすいように少し遅め

		m_owner->AddObject(bullet);
	}
}