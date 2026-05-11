#pragma once 

//継承の場合のみヘッダー内インクルード許可!!
#include "../BaseScene/BaseScene.h"

class GameClearScene : public BaseScene
{
public:

	GameClearScene() { Init(); }
	~GameClearScene() {}


	//オーバーライドは基底クラスの関数を上書きする
	void Init()override;
	void Update()override;
	void Draw2D()override;
	void Release()override;
	void SetFinalScore(unsigned long _score) { m_finalScore = _score; }
	unsigned long GetFinalScore() const { return m_finalScore; }
private:


	float m_alpha = 1.0f;
	float m_alphaadd = 0.01f;
	KdTexture m_tex;
	KdTexture m_cntnewtex;
	float m_btnalpha = 1.0f;
	float m_btnscale = 1.0f;
	float m_timer = 0.0f;

	KdTexture m_numberTex;
	static const int maxDigit = 8;
	int digits[maxDigit] = {};

	unsigned long m_finalScore = 0; // ここにスコアを蓄える
};