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
	void SetVisible(bool flg) { m_isVisible = flg; }

	// 追加：シールドを発動する関数
	void Activate();
	// 追加：シールドが今展開中か（プレイヤーのダメージを無効化できるか）確認する関数
	bool IsActive() { return m_aliveFlg; }

private:
	void Release() override;

	GameScene* m_owner = nullptr;

	// 追加：タイマー管理用の変数
	int m_activeTimer = 0;   // 展開中の残り時間
	int m_cooldownTimer = 0; // クールタイムの残り時間

	bool m_isVisible = false;
};