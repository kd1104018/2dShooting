#include"TitleScene.h"
void TitleScene::Draw2D()
{
	Math::Rectangle rect(0, 0, 1280, 720);
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(&m_titleTex, 0, 0,&rect);
}

void TitleScene::Update()
{

}
void TitleScene::Init()
{
	m_titleTex.Load("../../../../Texture/title.png");
}
void TitleScene::Release()
{
}