#include "Player.h"
#include"../../Scene/GameScene/GameScene.h"
#include"../Bullet/Bullet.h"
#include"../Life/Life.h"	
#include"../Shield/Shield.h"
#include <algorithm>

void Player::Update()
{
	// 移動
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pos.x -= 5;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pos.x += 5;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_pos.y += 5;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_pos.y -= 5;
	}

	// 弾発射
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		for (int i = 0; i < 10; ++i)
		{
			// 弾１個分のインスタンスを生成 & 初期化してリストへ追加
			std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>();
			bullet->Init();
			bullet->SetPos(m_pos);
			m_owner->AddObject(bullet);
		}
	}

	// 当たり判定（敵との）
	for (auto& obj : m_owner->GetObjList())
	{
		if (obj->GetObjType() == ObjectType::Enemy)
		{
			Math::Vector3 v = obj->GetPos() - m_pos;
			if (v.Length() < 64.0f)
			{
				obj->OnHit();	// 当たったときの処理

				// HPを減らす処理（ライフ表示オブジェクトを一つ消す）
				for (auto& uiObj : m_owner->GetObjList()) {
					if (uiObj->GetObjType() == ObjectType::Life) {
						uiObj->OnHit();
						break; // 1つだけ減らす
					}
				}
			}
		}
	}

	// ----------------------------
	// Shield 管理（整理版）
	// 毎フレームクールダウン/持続時間を更新（フレーム単位で扱う）
	// m_shieldCooldown : 発動までの残りフレーム（<=0 で発動可能）
	// m_shieldTime     : シールド残りフレーム（>0 の間シールド有効）
	// ----------------------------


	// 1. クールダウンを減らす（0以下にならないように制限）
	if (m_shieldCooldown > 0) {
		m_shieldCooldown -= 1.0f;
	}

	// 2. シールド発動チェック
	if ((GetAsyncKeyState('D') & 0x8000) != 0) {
		// クールダウンが終了している、かつ現在シールドが出ていない場合
		if (m_shieldCooldown <= 0.0f) {

			// シールド生成（1つだけ生成）
			std::shared_ptr<Shield> shield = std::make_shared<Shield>();
			shield->Init();
			shield->SetOwner(m_owner);
			shield->SetPos(m_pos);
			shield->SetAliveFlg(true); // シールドを有効にする
			m_owner->AddObject(shield);

			// クールダウンを再設定（900フレーム = 60fpsなら15秒）
			m_shieldCooldown = 900.0f;

			// シールドの持続時間を設定（もし必要なら）
			m_shieldTime = 300.0f;
		}
	}
}

void Player::Draw()
{
	Math::Rectangle rc;
	rc = { 0,0,64,64 };

	// 2D描画
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, m_pos.x, m_pos.y, 64, 64, &rc);
	//  テクスチャ,X座標,Y座標,幅,高さ,切り取り範囲
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
}

void Player::Release()
{
	// テクスチャはkdTexture型のデストラクタでじどうでReleaseされるのでしなくてもよい
}
