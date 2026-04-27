#pragma once

// 全てのゲームオブジェクトの基礎クラス
class BaseObject
{
public:
	BaseObject() {}
	virtual~BaseObject() { Release(); }

	virtual void Update();
	virtual void Init();
	virtual void Draw();

	// protected : このクラスと派生先のクラス内からのみアクセス可能
protected:

	virtual	void Release();

	KdTexture		m_tex;				// テクスチャ
	Math::Vector3	m_pos = {};				// 座標
	bool			m_aliveFlg = true;	// 生存フラグ

};