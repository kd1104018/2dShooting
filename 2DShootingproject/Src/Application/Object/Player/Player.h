#pragma once

// 基本的にヘッダーファイルの中で
// 別のヘッダーファイルをインクルードするのはＮＧ
// 継承元クラスのヘッダーのみインクルードが必須なので可とする
#include "../BaseObject.h"
class GameScene;
class Player : public BaseObject
{
public:
	Player() { Init(); }
	~Player() override { Release(); }

	void Update()override;
	void Draw()override;
	void Init()override;
	
	void SetOwner(GameScene* _owner) { m_owner = _owner; }

	bool IsShieldActive() const { return m_shieldTime > 0; }

private:

	void Release()override;
	
	GameScene* m_owner = nullptr;	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを持っている
	float m_shieldTime = 5.0f; // シールドの残り時間
	float m_shieldCooldown = 0.0f; // シールドのクールダウン時間
	int m_shotTimer = 0; // 弾を撃つまでの待ち時間
	float m_invincibleTimer = 0.0f;

};