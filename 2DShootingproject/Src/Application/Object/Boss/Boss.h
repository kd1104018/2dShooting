#pragma once

// 基本的にヘッダーファイルの中で
// 別のヘッダーファイルをインクルードするのはＮＧ
// 継承元クラスのヘッダーのみインクルードが必須なので可とする
#include "../BaseObject.h"
class GameScene;

class Boss : public BaseObject
{
public:
	Boss() { Init(); }
	~Boss() override { Release(); }

	void Update()override;
	void Draw()override;
	void Init()override;
	void OnHit()override;
	void SetOwner(GameScene* _owner) { m_owner = _owner; }
	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }


	void Shoot(); // ボスの攻撃パターンを実装する関数
	void SetMoveY(float v) { m_moveY = v; }
	
	void SetAttackType(int type) { m_attackType = type; }

	void ShootTarget(); // 自機を狙う攻撃
	void ShootSpread(); // 拡散攻撃

	
private:

	void Release()override;
	float           m_angle;
	// 追加: 上下移動の速度（符号で上下方向を表す）
	float m_moveY = 5.0f;
	GameScene* m_owner = nullptr;	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを持っている
	
	int m_shotTimer = 120; // 撃つまでの時間
	int m_attackType = 0;  // 0なら「まっすぐ」、1なら「狙う」

	int m_hp = 2; 
	
	int   m_maxHp;
	
	int   m_phase;      // 第1形態、第2形態などの管理
	int   m_attackTimer; // 攻撃間隔の管理

	KdTexture m_boss; // ボス本体の画像
	int m_animeCount;   // アニメーションの経過フレームカウント
	int m_animeIdx;     // 現在何コマ目か
	int m_maxAnime;     // そのモーションの最大コマ数
	int m_animeSpeed;   // アニメーション速度（小さいほど速い）
	int m_currentLine;

	int m_attackPhase = 0;    // 0: 待機, 1: 連射攻撃, 2: 拡散攻撃
	
	int m_shotCount = 0;      // 連射した数を数える
	int m_shotInterval = 0;

	const int CHIP_W = 100;
	const int CHIP_H = 100;
};
