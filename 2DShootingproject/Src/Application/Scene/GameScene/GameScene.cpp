#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Object//Player/Player.h"
#include "../../Object//Enemy/Enemy.h"
#include "../../Object/Bullet/Bullet.h"
#include "../../Object/Life/Life.h"
#include "../../Object/Shield/Shield.h"
#include <random>


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

// 実装方針（詳細な擬似コード）
// 1. 敵の出現 X 座標にランダムなオフセットとインデックスに基づく間隔を与えることで、右端から来るタイミングをずらす。
//    - baseSpawnX = screenRightX を基準にする。
//    - spawnSpacing を用いて各敵を段階的に右へずらす（i * spawnSpacing）。
//    - distOffset を 0..maxOffset の範囲でランダムに生成してさらにばらつかせる。  
//    - 最終的な開始 X = baseSpawnX + i * spawnSpacing + offset。
// 2. Y 座標は既存の distY(mt) を使ってランダムに決める（変更なし）。
// 3. 他の初期化処理（Init, SetOwner, SetPos, m_objList.push_back）は従来どおり行う。
// 4. この変更は Init() 内の敵生成ループのみを置き換える（ほかのロジックは維持）。
//
// 注意点:
// - 画面外に出過ぎる場合は maxOffset を適切に調整してください（遅く来てほしいなら大きめに）。
// - 必要なら敵ごとに移動速度を変えてさらに到達タイミングを調整できますが、ここでは開始 X の差で実現。

void GameScene::Init()
{

	std::shared_ptr<Player> player;

	player = std::make_shared<Player>();
	player->Init();
	player->SetOwner(this);	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを渡す
	m_objList.push_back(player);
	m_player = player; // 追加：プレイヤーをメンバに保持しておく

	// プレイヤー追従用シールドを作成して登録
	m_shield = std::make_shared<Shield>();
	m_shield->Init();
	m_shield->SetOwner(this);
	// 初期位置をプレイヤーに合わせる
	m_shield->SetPos(player->GetPos());
	m_objList.push_back(m_shield);

	std::random_device rd;
	std::mt19937 mt(rd());
	// 画面幅／高さに合わせて範囲を調整してください
	// 敵は画面の右端からスタートさせたいので X は固定値にする
	const float screenRightX = 600.0f; // 画面右端の X 座標（必要に応じて変更）
	std::uniform_real_distribution<float> distY(-500.0f, 500.0f); // Y 範囲

	// エネミー（出現タイミングをずらすために X にオフセットと間隔を付与）
	std::shared_ptr<Enemy> enemy;
	// 敵の出現を段階的に遅らせるための間隔（ピクセル単位）
	const float spawnSpacing = 80.0f;
	// さらにランダムなオフセットを付与してばらつかせる最大幅
	std::uniform_real_distribution<float> distOffset(-400.0f, 400.0f);

	for (int i = 0; i < 10; ++i)
	{
		enemy = std::make_shared<Enemy>();	// ①インスタンスを生成
		enemy->Init();						// ②初期化

		enemy->SetOwner(this);	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを渡す

		// 右端固定を基準に、インデックスとランダムオフセットで開始 X をずらす
		const float rx = screenRightX + i * spawnSpacing + distOffset(mt);
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
	//CD表示用と自機にかぶせる形で追加
	for (int i = 0; i < 10; ++i)
	{
		std::shared_ptr<Shield> shield;
		shield = std::make_shared<Shield>();	// ①インスタンスを生成
		shield->Init();						// ②初期化
		shield->SetOwner(this);	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを渡す
		m_objList.push_back(shield);
	}
}
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

	// ここでプレイヤー追従シールドの位置を毎フレーム更新
	if (m_shield && m_player) {
		m_shield->SetPos(m_player->GetPos());
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