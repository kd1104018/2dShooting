#pragma once 
#include "../BaseScene/BaseScene.h"


class GameScene : public BaseScene
{
public:

	GameScene() { Init(); }
	~GameScene() {}

	//オーバーライドは基底クラスの関数を上書きする
	void Init()override;
	void Update()override;
	void Draw2D()override;
	void Release()override;
private:

	float m_alpha = 1.0f;
	float m_alphaadd = 0.01f;
	KdTexture m_tex;
	KdTexture m_starttex;
};