#pragma once

#include "../BaseObject.h"
class GameScene;

class Life : public BaseObject
{
public:

	Life() {}
	~Life() { Release(); }

	void Update() override;
	void Draw()   override;
	void Init()   override;
	void OnHit()  override;
	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }


private:

	void Release() override;

};