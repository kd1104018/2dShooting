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
	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }
	void SetOwner(GameScene* _owner) { m_owner = _owner; }
	void SetAliveFlg(bool flg) { m_aliveFlg = flg; }

private:

	void Release()override;

	GameScene* m_owner = nullptr;	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを持っている

};