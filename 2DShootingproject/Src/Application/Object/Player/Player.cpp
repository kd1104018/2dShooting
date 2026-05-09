#include "Player.h"
#include"../../Scene/GameScene/GameScene.h"
#include"../Bullet/Bullet.h"
#include"../Life/Life.h"	
#include"../Shield/Shield.h"
#include <algorithm>

void Player::Update()
{
	// 移動
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { m_pos.x -= 5; }
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { m_pos.x += 5; }
	if (GetAsyncKeyState(VK_UP) & 0x8000) { m_pos.y += 5; }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { m_pos.y -= 5; }

	if (m_pos.x < -600.0f) { m_pos.x = -600.0f; } // 左端より左に行ったら、左端に戻す
	if (m_pos.x > 600.0f) { m_pos.x = 600.0f; } // 右端より右に行ったら、右端に戻す

	if (m_pos.y < -320.0f) { m_pos.y = -320.0f; } // 下端より下に行ったら、下端に戻す
	if (m_pos.y > 320.0f) { m_pos.y = 320.0f; } // 上端より上に行ったら、上端に戻す
	// 弾発射


	// ----------------------------
	// 1. タイマーの更新
	// ----------------------------

	if (m_invincibleTimer > 0.0f) {
		m_invincibleTimer -= 1.0f;
	}
	if (m_shieldCooldown > 0) {
		m_shieldCooldown -= 1.0f;
	}
	// ★追加：シールドの持続時間も毎フレーム減らす
	if (m_shieldTime > 0) {
		m_shieldTime -= 1.0f;
	}

	// ----------------------------
	// 2. 当たり判定（敵との）
	// ----------------------------
	for (auto& obj : m_owner->GetObjList())
	{
		if (obj->GetObjType() == ObjectType::Enemy || obj->GetObjType() == ObjectType::EnemyBullet)
		{
			if (m_invincibleTimer > 0.0f || m_shieldTime > 0.0f) {
				continue;
			}

			Math::Vector3 v = obj->GetPos() - m_pos;
			if (v.Length() < 64.0f)
			{
				obj->OnHit();

				// ダメージを受けたので無敵時間をセット（例：2秒間 = 120フレーム）
				m_invincibleTimer = 120.0f;

				// HPを減らす処理
				for (auto& uiObj : m_owner->GetObjList()) {
					if (uiObj->GetObjType() == ObjectType::Life) {
						uiObj->OnHit();
						break;
					}
				}
			}
		}
	}

	// ----------------------------
	// 3. シールド発動チェック
	// ----------------------------
	if ((GetAsyncKeyState('D') & 0x8000) != 0) {
		// クールダウンが終了している、かつ現在シールドが出ていない場合
		if (m_shieldCooldown <= 0.0f && m_shieldTime <= 0.0f) {

			// クールダウンと持続時間を設定
			m_shieldCooldown = 900.0f; // 60fpsなら15秒
			m_shieldTime = 300.0f;     // 60fpsなら5秒

			// ※注意：ここで make_shared<Shield> をしていた処理は一旦消しています
		}
	}

	if (m_shotTimer > 0) {
		m_shotTimer--;
	}

	// 弾発射
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		// ★追加：タイマーが 0 以下のときだけ撃てるようにする！
		if (m_shotTimer <= 0)
		{
			// forループを消して、シンプルに1個だけ弾を作るように修正
			std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>();
			bullet->Init();
			bullet->SetPos(m_pos);
			m_owner->AddObject(bullet);

			// ★追加：撃ち終わったらタイマーをセット！（例：10フレーム休む）
			m_shotTimer = 10;
		}
	}
}
void Player::Draw()
{
	bool isVisible = true;

	// 無敵時間中の点滅計算
	if (m_invincibleTimer > 0) {
		// 10フレーム周期で、後半5フレームを非表示にする
		if ((int)m_invincibleTimer % 10 < 5) {
			isVisible = false;
		}
	}

	// 表示フラグが真のときだけ描画する
	if (isVisible) {
		Math::Rectangle rc = { 0, 0, 64, 64 };
		KdShaderManager::GetInstance().m_spriteShader.DrawTex(
			&m_tex, m_pos.x, m_pos.y, 64, 64, &rc);
	}
}

void Player::Init()
{
	m_tex.Load("Texture/player.png");
	m_pos = {};	// 0,0 で初期化
	m_aliveFlg = true;

	// シールド関連の初期値
	m_shieldCooldown = 0.0f;
	m_shieldTime = 0.0f;

	m_objType = ObjectType::Player;	// 種類は「プレイヤー」

	m_invincibleTimer = 0.0f; // 無敵時間の初期値
}

void Player::Release()
{
	// テクスチャはkdTexture型のデストラクタでじどうでReleaseされるのでしなくてもよい
}
