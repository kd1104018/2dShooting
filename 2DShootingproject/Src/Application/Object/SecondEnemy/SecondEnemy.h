#pragma once
#include "../BaseObject.h"
class GameScene;

class SecondEnemy : public BaseObject
{
public:
	SecondEnemy() { Init(); }
	~SecondEnemy() override { Release(); }

	void Update() override;
	void Draw()   override;
	void Init()   override;
	void OnHit()  override;
	

	void SetOwner(GameScene* owner) { m_owner = owner; }
	void SetPos(Math::Vector3 pos) { m_pos = pos; }

private:
	void Release() override;
	GameScene* m_owner = nullptr;

	
	float m_angle = 0.0f;

	int m_shotTimer = 60;
};