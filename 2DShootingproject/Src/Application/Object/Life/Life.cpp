#include "Life.h"
#include"../../Scene/GameScene/GameScene.h"


void Life::Update()
{
	

	
}

void Life::Draw()
{
	Math::Rectangle rc;
	rc = { 0,0,16,16 };

	// 2D描画
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, m_pos.x, m_pos.y, 16, 16, &rc);
	//  テクスチャ,X座標,Y座標,幅,高さ,切り取り範囲
}

void Life::Init()
{
	m_tex.Load("Texture/Life.png");
	m_pos = {};	// 0,0 で初期化
	m_aliveFlg = true;
	m_objType = ObjectType::Life;	
}

void Life::Release()
{
	// テクスチャはkdTexture型のデストラクタでじどうでReleaseされるのでしなくてもよい
}
