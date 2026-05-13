#pragma once
#include "../BaseObject.h"
class GameScene;

class BossBullet : public BaseObject
{
public:
	BossBullet() { Init(); }
	~BossBullet() override { Release(); }

	void Update() override;
	void Draw()   override;
	void Init()   override;
	void OnHit()  override;

	void SetOwner(GameScene* owner) { m_owner = owner; }
	void SetPos(Math::Vector3 pos) { m_pos = pos; }


	void SetMoveVec(Math::Vector3 vec) { m_moveVec = vec; }

	void SetDir(Math::Vector3 _dir) { m_dir = _dir; }
	void SetSpeed(float _speed) { m_speed = _speed; }
private:
	void Release() override;
	GameScene* m_owner = nullptr;
	Math::Vector3 m_moveVec; // 弾が飛ぶ方向とスピード

	Math::Vector3 m_dir = { -1.0f, 0.0f, 0.0f }; // デフォルトは左方向
	float         m_speed = 5.0f;
};