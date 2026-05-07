#pragma once 
#include "../BaseScene/BaseScene.h"
class BaseObject;

class GameScene : public BaseScene
{
public:

	GameScene() { Init(); }
	~GameScene() {}

	//オーバーライドは基底クラスの関数を上書きする
	void Init()override;
	void Update()override;
	void Draw2D()override;
	void Release()override;

	std::vector<std::shared_ptr<BaseObject>> GetObjList()
	{
		return m_objList;
	}
	void AddObject(std::shared_ptr<BaseObject> obj)
	{
		m_objList.push_back(obj);
	}
	std::vector<std::shared_ptr<BaseObject>>m_objList;
private:

	float m_alpha = 1.0f;
	float m_alphaadd = 0.01f;
	KdTexture m_tex;
	
	KdTexture m_starttex;


	std::shared_ptr<BaseObject> m_player = nullptr;
	std::shared_ptr<BaseObject> m_enemy = nullptr;
	std::shared_ptr<BaseObject> m_bullet = nullptr;
	
	

	unsigned long score = 0;
	static const int maxDigit = 10;
	int digits[maxDigit] = {};
	KdTexture m_lifeTex;
	int m_lifeCount = 3;
};