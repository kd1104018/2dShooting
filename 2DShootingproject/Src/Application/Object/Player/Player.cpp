#include "Player.h"
#include"../../Scene/GameScene/GameScene.h"
#include"../Bullet/Bullet.h"
#include"../Life/Life.h"	
#include"../Shield/Shield.h"
#include <algorithm>

void Player::Update()
{
	// 移動
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { m_pos.x -= 10; }
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { m_pos.x += 10; }
	if (GetAsyncKeyState(VK_UP) & 0x8000) { m_pos.y += 10; }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { m_pos.y -= 10; }

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
	if (m_shieldCD > 0) {
		m_shieldCD -= 1.0f;
	}
	
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

			// 無敵状態なら判定をスキップ
			if (m_invincibleTimer > 0.0f || m_shieldTime > 0.0f) {
				continue;
			}
			if(m_isInvincible==true)
			{
				continue;
			}

			Math::Vector3 v = obj->GetPos() - m_pos;
			float distance = v.Length(); // 距離を計算

			// --- 種類ごとに判定距離を決定 ---
			float hitRange = 0.0f;
			if (obj->GetObjType() == ObjectType::Enemy) {
				hitRange = 64.0f; // 敵本体用
			}
			else {
				hitRange = 16.0f; // 弾用
			}

			// ★ここを distance < hitRange だけにする（さっきの 64.0f の if は不要）
			if (distance < hitRange)
			{
				obj->OnHit();

				m_invincibleTimer = 120.0f;

				// HP（ライフUI）を減らす処理
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
	if (m_shieldCD > 0) {
		m_shieldCD--;
	}

	// スペースキーなどでシールド発動！
	if (GetAsyncKeyState('D') & 0x8000) { // 例：左クリ
		if (m_shieldCD <= 0) {
			m_shieldTime = 120.0f; // 2秒間発動
			m_shieldCD = 720;      // 秒間のクールタイム開始
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
	if (GetAsyncKeyState('L') & 0x8000) {
		m_isInvincible = true;
	}
	else {
		m_isInvincible = false;
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

	m_shieldTime = 0.0f;

	m_objType = ObjectType::Player;	// 種類は「プレイヤー」

	m_invincibleTimer = 0.0f; // 無敵時間の初期値

	m_shieldCD = 0.0f; // シールドのクールダウン初期値
}

void Player::Release()
{
	// テクスチャはkdTexture型のデストラクタでじどうでReleaseされるのでしなくてもよい
}
