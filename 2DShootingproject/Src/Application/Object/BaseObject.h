#pragma once

// 全てのゲームオブジェクトの基礎クラス
class BaseObject
{
public:
	enum class ObjectType
	{
		None,
		Player,
		Enemy,
		Bullet,
		SecondEnemy,
		EnemyBullet,
		Life,
		Shield,
		Explosion,
		Boss,
		BossBullet,
	};

	BaseObject() {}
	virtual~BaseObject() { Release(); }

	virtual void Update();
	virtual void Init();
	virtual void Draw();
	virtual void OnHit() {}	// 当たったときの処理
	ObjectType GetObjType() { return m_objType; }
	Math::Vector3 GetPos() { return m_pos; }
	bool GetAliveFlg() { return m_aliveFlg; }

	//セットポス
	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }

	bool GetAliveFlg() const { return m_aliveFlg; }
	void SetAliveFlg(bool _flag) { m_aliveFlg = _flag; }
	// protected : このクラスと派生先のクラス内からのみアクセス可能
protected:

	virtual void Release();

	KdTexture		m_tex;				// テクスチャ
	Math::Vector3	m_pos = {};				// 座標
	bool			m_aliveFlg = true;	// 生存フラグ
	ObjectType		m_objType = ObjectType::None;
};