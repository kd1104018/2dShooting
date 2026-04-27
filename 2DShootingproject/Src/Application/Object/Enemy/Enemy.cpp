
#include "Enemy.h"

void Enemy::Update()
{

	
}

void Enemy::Draw()
{
	Math::Rectangle rc;
	rc = { 0,0,64,64 };

	// 2D描画
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, m_pos.x, m_pos.y, 64, 64, &rc);
	//  テクスチャ,X座標,Y座標,幅,高さ,切り取り範囲
}

void Enemy::Init()
{
	m_tex.Load("Texture/enemy.png");
	m_pos = {};	// 0,0 で初期化
	m_aliveFlg = true;
}

void Enemy::Release()
{
	// テクスチャはkdTexture型のデストラクタでじどうでReleaseされるのでしなくてもよい
}
