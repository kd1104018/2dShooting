#pragma once

// 基本的にヘッダーファイルの中で
// 別のヘッダーファイルをインクルードするのはＮＧ
// 継承元クラスのヘッダーのみインクルードが必須なので可とする
#include "../BaseObject.h"
class GameScene;
class BossScene;
class Player : public BaseObject
{
public:
	Player() { Init(); }
	~Player() override { Release(); }

	void Update()override;
	void Draw()override;
	void Init()override;
	
	void SetOwner(GameScene* _owner) { m_owner = _owner; }
	void SetBossOwner(BossScene* _bossowner) { m_bossowner = _bossowner; }

	bool IsShieldActive() const { return m_shieldTime > 0; }
	float GetShieldCD() const {
		if (m_shieldCD <= 0) return 0.0f;
		return (float)m_shieldCD / 600.0f; // 600は最大CDフレーム数
	}
	bool IsInvincible() const { return m_isInvincible; }


private:

	void Release()override;
	
	GameScene* m_owner = nullptr;	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを持っている
	BossScene* m_bossowner = nullptr; // ボスシーンでのプレイヤー用（必要に応じて使い分ける）
	float m_shieldTime = 5.0f; // シールドの残り時間
	
	int m_shotTimer = 0; // 弾を撃つまでの待ち時間
	float m_invincibleTimer = 0.0f;
	float m_shieldCD = 0.0f; // シールドのクールダウン時間（フレーム数で管理）
	bool m_isInvincible = false;
};