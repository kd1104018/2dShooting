#pragma once
#include "../BaseObject.h"
class GameScene;

class EnemyBullet : public BaseObject
{
public:
	EnemyBullet() { Init(); }
	~EnemyBullet() override { Release(); }

	void Update() override;
	void Draw()   override;
	void Init()   override;
	void OnHit()  override;

	void SetOwner(GameScene* owner) { m_owner = owner; }
	void SetPos(Math::Vector3 pos) { m_pos = pos; }


	void SetMoveVec(Math::Vector3 vec) { m_moveVec = vec; }

private:
	void Release() override;
	GameScene* m_owner = nullptr;
	Math::Vector3 m_moveVec; // 弾が飛ぶ方向とスピード
};