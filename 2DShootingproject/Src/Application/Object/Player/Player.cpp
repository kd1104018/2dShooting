#include "Player.h"

void Player::Update()
{

	// 移動
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_movex -= 2; // 左に移動
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_movex += 2; // 右に移動
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_movey += 2; // 上に移動
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_movey -= 2; // 下に移動
	}
	else
	{
		m_pos.x *= 0.5f;
		m_pos.y *= 0.5f;


		if (m_movex >= -0.8f && m_movex <= 0.8f) {
			m_movex = 0;
		}
		if (m_movey >= -0.8f && m_movey <= 0.8f) {
			m_movey = 0;
		}
	}
	
	m_pos.x += m_movex;
	m_pos.y += m_movey;
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
}

void Player::Release()
{
	// テクスチャはkdTexture型のデストラクタでじどうでReleaseされるのでしなくてもよい
}
