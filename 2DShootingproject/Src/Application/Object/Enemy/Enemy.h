#pragma once

// 基本的にヘッダーファイルの中で
// 別のヘッダーファイルをインクルードするのはＮＧ
// 継承元クラスのヘッダーのみインクルードが必須なので可とする
#include "../BaseObject.h"
class GameScene;

class Enemy : public BaseObject
{
public:
	Enemy() { Init(); }
	~Enemy() override { Release(); }

	void Update()override;
	void Draw()override;
	void Init()override;
	void OnHit()override;
	void SetOwner(GameScene* _owner) { m_owner = _owner; }
	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }

	void SetMoveY(float v) { m_moveY = v; }
	
	void SetAttackType(int type) { m_attackType = type; }
	
private:

	void Release()override;
	float           m_angle;
	
	float m_moveY = 5.0f;
	GameScene* m_owner = nullptr;	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを持っている
	
	int m_shotTimer = 60; // 撃つまでの時間
	int m_attackType = 0;  // 0なら「まっすぐ」、1なら「狙う」

	int m_hp = 1; 


};
