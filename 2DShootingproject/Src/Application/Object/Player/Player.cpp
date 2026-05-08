#include "Player.h"
#include"../../Scene/GameScene/GameScene.h"
#include"../Bullet/Bullet.h"
#include"../Life/Life.h"	

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
			std::shared_ptr<Bullet> bullet;
			bullet = std::make_shared<Bullet>();	// ①生成
			bullet->Init();							// ②初期化
			bullet->SetPos(m_pos);					// ③プレイヤーと同じ座標にセット
			m_owner->AddObject(bullet);				// ④リストに追加
		}
	}
	for (auto& obj : m_owner->GetObjList())
	{

		// オブジェクトに対する処理
		if (obj->GetObjType() == ObjectType::Enemy)
		{

			// 対象の座標（ベクトル） - 自分の座標（ベクトル） = 対象へのベクトル（矢印）
			Math::Vector3 v;
			v = obj->GetPos() - m_pos;
			//弾判定...ベクトルの長さで判定
			if (v.Length() < 64.0f)
			{
				obj->OnHit();	// 当たったときの処理
				//HPを減らす処理
				for (auto& uiObj : m_owner->GetObjList()) {
					if (uiObj->GetObjType() == ObjectType::Life) {
						uiObj->OnHit(); // Life::OnHit() が m_aliveFlg を false にする
						break; // 1つだけ減らす

					}
				}
			}
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

	m_objType = ObjectType::Player;	// 種類は「プレイヤー」
}

void Player::Release()
{
	// テクスチャはkdTexture型のデストラクタでじどうでReleaseされるのでしなくてもよい
}
