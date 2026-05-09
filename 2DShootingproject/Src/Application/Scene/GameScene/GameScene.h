
#pragma once 
#include "../BaseScene/BaseScene.h"
class BaseObject;
class Shield; // 追加：前方宣言

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
	// シールドを専用に保持（Player 追従用）
	std::shared_ptr<Shield> m_shield = nullptr;




	unsigned long score = 0;
	static const int maxDigit = 10;
	int digits[maxDigit] = {};
	KdTexture m_lifeTex;
	int m_lifeCount = 3;

	// 敵スポーン制御（既存のまま）
	int m_spawnCounter = 0;
	int m_spawnIntervalFrames = 60;
	int m_spawnedEnemies = 0;
	int m_spawnMaxEnemies = 10;
	float m_spawnBaseX = 600.0f;
	float m_spawnSpacing = 80.0f;
	float m_spawnOffsetMax = 400.0f;
	float m_spawnYMin = -500.0f;
	float m_spawnYMax = 500.0f;
	float firstspown = 0.0f;
	
	int m_gameTimer = 0;           // ゲーム開始からの合計時間
	int m_enemySpawnTimer = 0;     // 敵を出すためのカウントダウン
	int m_spawnInterval = 120;     // 敵を出す間隔（最初は2秒に1匹くらい）
	const int START_DELAY = 300;   // 5秒間の猶予（60fps × 5秒 = 300）

	int m_enemyCount = 0;          // 今までに出した普通の敵の数
	int m_enemyTimer = 60;         // 普通の敵を出すタイマー
	int m_secondEnemyTimer = 120;  // 新しい敵を出すタイマー
	int m_spawnedEnemyCount=0;
};