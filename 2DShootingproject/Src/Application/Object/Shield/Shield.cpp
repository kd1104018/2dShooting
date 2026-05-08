#include "Shield.h"
#include"../../Scene/GameScene/GameScene.h"

void Shield::Update()
{


}

void Shield::Draw()
{

	if (m_aliveFlg = true)
	{
		Math::Rectangle rc = { 0,0,80,80 };

		KdShaderManager::GetInstance().m_spriteShader.DrawTex(
			&m_tex, m_pos.x, m_pos.y, 80, 80, &rc);
	}
	/*Math::Rectangle rc2 = { 0,0,80,80 };
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, m_pos.x, m_pos.y, 80, 80, &rc2);*/

}

void Shield::Init()
{
	m_tex.Load("Texture/Shield.png");
	m_pos = {};
	m_aliveFlg = false;
	m_objType = ObjectType::Shield;		// 種類は「盾」
}

void Shield::OnHit()
{


}

void Shield::Release()
{
}