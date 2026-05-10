#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Object//Player/Player.h"
#include "../../Object//Enemy/Enemy.h"
#include "../../Object/Bullet/Bullet.h"
#include "../../Object/Life/Life.h"
#include "../../Object/Shield/Shield.h"
#include "../../Object/SecondEnemy/SecondEnemy.h"
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
	m_tex.Load("Texture/Game.png");
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
	m_shield = std::make_shared<Shield>();
	m_shield->Init();
	m_shield->SetOwner(this);
	m_shield->SetPos(player->GetPos());
	m_objList.push_back(m_shield);


	m_score = 0;
	m_numberTex.Load("Texture/sucore.png");
	m_shieldTex.Load("Texture/ShieldCD.png");
}
void GameScene::Update()
{
	m_bgX -= m_ScrollSpeed;
	if (m_bgX <= -1280.0f) {
		m_bgX = 0.0f;
	}
	m_alpha += m_alphaadd;
	if (m_alpha >= 1.0f)
	{
		m_alpha = 1.0f;
		m_alphaadd *= -1;
	}
	else if (m_alpha <= 0.4f)
	{
		m_alpha = 0.4f;
		m_alphaadd *= -1;
	}
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
		SceneManager::Instance().SetFinalScore(m_score);
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::GameOver);
		return;
	}
	// ここでプレイヤー追従シールドの位置を毎フレーム更新
	if (m_shield && m_player) { // m_shield は Init で保持した自機用
		m_shield->SetPos(m_player->GetPos());

		std::shared_ptr<Player> playerPtr = std::dynamic_pointer_cast<Player>(m_player);
		if (playerPtr) {
			m_shield->SetVisible(playerPtr->IsShieldActive());
		}
	}
	m_gameTimer++; // 毎フレーム時間を加算

	// 300フレーム（約5秒）経過してから敵を出し始める
	if (m_gameTimer > 300)
	{
		m_enemySpawnTimer--;

		if (m_enemySpawnTimer <= 0)
		{
			// 上から降らせる設定（X座標はランダム）
			float randomX = (float)(rand() % 1000 - 500);

			// ーーーーーーーーーーーーーーーーーーーーーーーーー
			// ★ フェーズ1：まだ普通の敵が10体出ていない時
			// ーーーーーーーーーーーーーーーーーーーーーーーーー
			if (m_spawnedEnemyCount < 2)
			{
				std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
				enemy->Init();
				enemy->SetOwner(this);
				enemy->SetPos({ randomX, 500.0f, 0.0f });
				enemy->SetAttackType(rand() % 2); // まっすぐか狙うかランダム
				m_objList.push_back(enemy);

				m_spawnedEnemyCount++; // ★敵を出したらカウントを1増やす！
			}
			// ーーーーーーーーーーーーーーーーーーーーーーーーー
			// ★ フェーズ2：普通の敵が10体以上出た後（カオス状態）
			// ーーーーーーーーーーーーーーーーーーーーーーーーー
			else
			{
				int enemyChoice = rand() % 2; // 0か1をランダムで決める

				if (enemyChoice == 0)
				{
					// 今までの敵を出す
					std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
					enemy->Init();
					enemy->SetOwner(this);
					enemy->SetPos({ randomX, 500.0f, 0.0f });
					enemy->SetAttackType(rand() % 2);
					m_objList.push_back(enemy);
				}
				else
				{
					// 新しい敵（SecondEnemy）を出す
					std::shared_ptr<SecondEnemy> secondEnemy = std::make_shared<SecondEnemy>();
					secondEnemy->Init();
					secondEnemy->SetOwner(this);
					secondEnemy->SetPos({ randomX, 500.0f, 0.0f });
					m_objList.push_back(secondEnemy);
				}
			}

			// 次の出現までの時間をセット
			m_enemySpawnTimer = m_spawnInterval;

			// ★少しずつ出現ペースを速くする（20フレーム間隔が限界）
			if (m_spawnInterval > 20) {
				m_spawnInterval -= 2;
			}
		}
	}
	if(GetAsyncKeyState('P'))
	{
		m_score += 100;
	}
	if (remainLife == 0) {
		// 遷移する前にスコアを保存！
		SceneManager::Instance().SetFinalScore(m_score);

		SceneManager::Instance().SetNextScene(SceneManager::SceneType::GameOver);
		return;
	}
}

void GameScene::Draw2D()
{

	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, (int)m_bgX, 0, 1280, 720, &Math::Rectangle{ 0, 0, 1280, 720 });

	// 2枚目の背景（1枚目のすぐ右に繋げる）
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_tex, (int)m_bgX + 1280, 0, 1280, 720, &Math::Rectangle{ 0, 0, 1280, 720 });

	for (int i = 0; i < m_objList.size(); ++i)
	{
		m_objList[i]->Draw();
	}
	unsigned long tempScore = m_score;
	for (int i = 0; i < maxDigit; i++) {
		digits[i] = tempScore % 10;
		tempScore /= 10;
	}

	// 2. 桁ごとに描画
	for (int i = 0; i < maxDigit; i++) {
		int num = digits[i];

		int width = 281;
		int height = 384;

		int column = num % 5;
		int row = num / 5;

		Math::Rectangle rc = { column * width, row * height, width, height };

		// 画面上の表示位置
		float xPos = 100 - (i * 35);
		float yPos = 230;


		float drawY = yPos;
		if (row == 1) {

			drawY -= 6.0f;
		}
		// ------------------------------------------

		KdShaderManager::GetInstance().m_spriteShader.DrawTex(
			&m_numberTex, (int)xPos, (int)drawY, 40, 55, &rc);
		if (m_player) {
			auto playerPtr = std::dynamic_pointer_cast<Player>(m_player);
			if (playerPtr) {
				float cdRate = playerPtr->GetShieldCD(); // 残り時間の割合 (0.0〜1.0)

				// 1. アイコンの土台（白：使える状態）
				Math::Vector2 iconPos = { -580, -300 };
				KdShaderManager::GetInstance().m_spriteShader.DrawTex(
					&m_shieldTex, (int)iconPos.x, (int)iconPos.y, 64, 64);

				// 2. CD中の重なり（黒い半透明）
				if (cdRate > 0.0f) {
					// 残り時間に合わせて高さを変える
					int maskH = (int)(64 * cdRate);
					// 下から上へ伸びるように表示位置を微調整
					int yOffset = -(64 - maskH) / 2;

					Math::Rectangle rc = { 0, 0, 64, maskH };
					Math::Color maskColor = { 0, 0, 0, 0.7f }; // 黒の半透明

					// ※テクスチャは適当な白い画像か、アイコン画像を使いまわして色を黒くします
					KdShaderManager::GetInstance().m_spriteShader.DrawTex(
						&m_shieldTex,
						(int)iconPos.x, (int)iconPos.y - 5 + yOffset,
						64, maskH,
						&rc, &maskColor);
				}
			}

		}
	}
}

void GameScene::Release()
{
}