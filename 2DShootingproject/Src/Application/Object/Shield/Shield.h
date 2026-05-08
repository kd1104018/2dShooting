#pragma once

#include "../BaseObject.h"
class GameScene;

class Shield : public BaseObject
{
public:

	Shield() { Init(); }
	~Shield() { Release(); }

	void Update() override;
	void Draw()   override;
	void Init()   override;
	void OnHit()  override;
		void SetOwner(GameScene* _owner) { m_owner = _owner; }

private:

	void Release()override;
	
	GameScene* m_owner = nullptr;	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを持っている

};