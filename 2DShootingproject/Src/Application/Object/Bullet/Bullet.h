#pragma once

#include "../BaseObject.h"
class GameScene;

class Bullet : public BaseObject
{
public:

	Bullet() {}
	~Bullet() { Release(); }

	void Update() override;
	void Draw()   override;
	void Init()   override;

	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }

	
private:

	void Release() override;

};