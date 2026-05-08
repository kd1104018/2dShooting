
#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Object//Player/Player.h"
#include "../../Object//Enemy/Enemy.h"
#include "../../Object/Bullet/Bullet.h"
#include "../../Object/Life/Life.h"
#include "../../Object/Shield/Shield.h"
#include <random>

void GameScene::Init()
{

	std::shared_ptr<Player> player;

	player = std::make_shared<Player>();
	player->Init();
	player->SetOwner(this);	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを渡す
	m_objList.push_back(player);
	m_player = player; // 追加：プレイヤーをメンバに保持しておく

	std::random_device rd;
	std::mt19937 mt(rd());
	// 画面幅／高さに合わせて範囲を調整してください
	std::uniform_real_distribution<float> distX(-600.0f, 600.0f); // X 範囲
	std::uniform_real_distribution<float> distY(-500.0f, 500.0f); // Y 範囲

	// エネミー
	std::shared_ptr<Enemy> enemy;
	for (int i = 0; i < 10; ++i)
	{
		enemy = std::make_shared<Enemy>();	// ①インスタンスを生成
		enemy->Init();						// ②初期化

		enemy->SetOwner(this);	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを渡す
		const float rx = distX(mt);
		const float ry = distY(mt);
		enemy->SetPos({ rx, ry, 0.0f });

		m_objList.push_back(enemy);

	}
	std::shared_ptr<Life> life;
	for (int i = 0; i < m_lifeCount; ++i)
	{
		life = std::make_shared<Life>();	// ①インスタンスを生成
		life->Init();						// ②初期化
		life->SetPos({ 500.0f + i * 40.0f, 300.0f, 0.0f }); // ライフの位置を設定
		m_objList.push_back(life);
	}
	//シールドを追加
	//CD表示用と時期にかぶせる形で追加
	for (int i = 0; i < 10; ++i)
	{
		std::shared_ptr<Shield> shield;
		shield = std::make_shared<Shield>();	// ①インスタンスを生成
		shield->Init();						// ②初期化
		shield->SetOwner(this);	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを渡す
		m_objList.push_back(shield);
	}
}

// 更新処理: 詳細な擬似コード（日本語）
// 1. m_objList のイテレータで全要素を走査し、GetAliveFlg() が false のオブジェクトを erase する。
//    - erase はイテレータを無効化するので、戻り値（次の有効なイテレータ）を使って継続する。
// 2. 残った全オブジェクトに対して Update() を呼ぶ。
// 3. リスト中の Life オブジェクトの数をカウントする。
//    - BaseObject 内にネストされた enum class ObjectType を使うので、比較は BaseObject::ObjectType::Life のように完全修飾する。
// 4. ライフが 0 なら Player オブジェクトをリストから探して削除し、シーン遷移を設定する。
//    - Player の比較も BaseObject::ObjectType::Player を使う。
//    - プレイヤー削除後は SceneManager に次シーンを設定し、早期 return する。
// 実装上の注意:
// - m_objList は std::vector<std::shared_ptr<BaseObject>> なので、イテレータの扱いに注意する。
// - erase を行うループとオブジェクトの Update を行うループは分離している（erase の後に Update を行う）。
// - GetObjType() の返り値は BaseObject::ObjectType を想定しているため、スコープ修飾子を付ける必要がある。

void GameScene::Update()
{
	auto it = m_objList.begin();

	// 1) 無効オブジェクトの削除
	while (it != m_objList.end())
	{
		if ((*it)->GetAliveFlg() == false)
		{
			it = m_objList.erase(it);
		}
		else
		{
			++it;
		}
	}

	// 2) 全オブジェクトの更新
	for (size_t i = 0; i < m_objList.size(); ++i)
	{
		m_objList[i]->Update();
	}

		// 3) ライフ残数チェック（BaseObject にネストされた enum を完全修飾して比較）
	int remainLife = 0;
	for (const auto& obj : m_objList) {
		if (obj->GetObjType() == BaseObject::ObjectType::Life) ++remainLife;
	}

	// 4) ライフが0ならプレイヤーをリストから削除してシーン遷移をセット
	if (remainLife == 0) {
		for (auto it2 = m_objList.begin(); it2 != m_objList.end(); ++it2) {
			if ((*it2)->GetObjType() == BaseObject::ObjectType::Player) {
				m_objList.erase(it2);
				break;
			}
		}
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::GameOver);
		return;
	}
}

void GameScene::Draw2D()
{

	for (int i = 0; i < m_objList.size(); ++i)
	{
		m_objList[i]->Draw();
	}
}

void GameScene::Release()
{
}