#pragma once 


#include "../BaseScene/BaseScene.h"
class TitleScene :public BaseScene
{
public:

	TitleScene() {}
	~TitleScene() {}



	// ‰Šúİ’è
	void Init()override;
	// ‰ğ•ú
	void Release()override;
	// XVˆ—
	void Update()override;
	// •`‰æˆ—
	void Draw2D()override;
	// GUIˆ—
	

private:

	KdTexture m_titleTex;


};