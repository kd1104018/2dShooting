#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Object//Player/Player.h"
#include "../../Object//Enemy/Enemy.h"
#include "../../Object/Bullet/Bullet.h"
#include "../../Object/Life/Life.h"
#include "../../Object/Shield/Shield.h"
#include "../../Object/SecondEnemy/SecondEnemy.h"
#include "../../Object/Boss/Boss.h"
#include "../../Object/BossBullet/BossBullet.h"
#include <random>



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
	while (it != m_objList.end()) {
		if (!(*it) || (*it)->GetAliveFlg() == false) {
			it = m_objList.erase(it);
		}
		else {
			++it;
		}
	}

	
	for (size_t i = 0; i < m_objList.size(); ++i) {
		if (m_objList[i]) m_objList[i]->Update();
	}

	// --- 3. ライフとゲームオーバー判定 ---
	int remainLife = 0;
	for (const auto& obj : m_objList) {
		if (obj && obj->GetObjType() == BaseObject::ObjectType::Life) ++remainLife;
	}

	if (remainLife == 0) {
		SceneManager::Instance().SetFinalScore(m_score);
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::GameOver);
		return; // ゲームオーバーならここで終了
	}

	// --- 4. シールド追従など ---
	if (m_shield && m_player) {
		m_shield->SetPos(m_player->GetPos());
		auto playerPtr = std::dynamic_pointer_cast<Player>(m_player);
		if (playerPtr) m_shield->SetVisible(playerPtr->IsShieldActive());
	}

	// --- 5. 敵の出現管理 (整理しました) ---
	m_gameTimer++;

	if (!m_isBossMode && m_gameTimer > 300) {
		m_enemySpawnTimer--;
		if (m_enemySpawnTimer <= 0) {
			// 出現率の調整
			int dice = rand() % 100;
			float randomY = (float)(rand() % 400 - 200);

			if (dice < 30) {
				SpawnWallFormation();      // 30%で壁(SecondEnemy含む)
			}
			else if (dice < 60) {
				SpawnArrowFormation(randomY); // 30%で矢印
			}
			else {
				SpawnEnemy(0, randomY);    // 40%で単体
			}

			m_enemySpawnTimer = 90; // 次回までのインターバル
		}
	}

	// --- 6. ボスモードへの移行 ---
	if (!m_isBossMode && m_score >= 10000) {
		m_isBossMode = true;

		// ザコ敵を全滅させる
		for (auto& obj : m_objList) {
			if (!obj) continue;
			if (obj->GetObjType() == BaseObject::ObjectType::Enemy ||
				obj->GetObjType() == BaseObject::ObjectType::SecondEnemy) {
				obj->OnHit(); // 爆発させる
			}
		}

		// ボス生成
		auto boss = std::make_shared<Boss>();
		boss->Init();
		boss->SetOwner(this);
		boss->SetPos({ 800.0f, 0.0f, 0.0f });
		m_objList.push_back(boss);

		// メンバ変数の m_boss に覚えさせておく（生存チェック用）
		m_boss = boss;
	}

	// --- 7. クリア判定 (ボス登場時のみチェック) ---
	if (m_isBossMode && m_boss) {
		if (!m_boss->GetAliveFlg()) {
			SceneManager::Instance().SetFinalScore(m_score);
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::GameClear);
		}
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
		float yPos = 300;


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
				Math::Vector2 iconPos = { -580, 300 };
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
	//スコアを増やすデバッグきー
	if (GetAsyncKeyState('S') & 0x8000) 
	{
		m_score += 1000;
	}
}


void GameScene::Release()
{
}
void GameScene::SpawnEnemy(float offsetX, float y) {
	auto enemy = std::make_shared<Enemy>();
	enemy->Init();
	enemy->SetOwner(this);

	// Xは画面右端(700) + 陣形ごとのズレ(offsetX)
	// Yは引数で指定された高さ
	enemy->SetPos({ 700.0f + offsetX, y, 0.0f });
	enemy->SetAttackType(rand() % 2);
	m_objList.push_back(enemy);
}
void GameScene::SpawnWallFormation() {
	float x = 0.0f;
	// 上下に2体ずつ配置（中央をあける）
	SpawnEnemy(x, 200.0f);
	SpawnEnemy(x, 100.0f);

	SpawnEnemy(x, -100.0f);
	SpawnEnemy(x, -200.0f);
}

void GameScene::SpawnVFormation(float y) {
	SpawnEnemy(0.0f, y);          // 先頭（中央）
	SpawnEnemy(100.0f, y + 100.0f); // 右上後ろ
	SpawnEnemy(100.0f, y - 100.0f); // 右下後ろ
}

// --- 2. 上下を塞ぐ壁型（中央だけ空ける） ---


// --- 3. 矢印型（より鋭い突撃陣形） ---
void GameScene::SpawnArrowFormation(float y) {
	SpawnSecondEnemy(0.0f, y);          // 先頭
	SpawnSecondEnemy(80.0f, y + 60.0f);
	SpawnSecondEnemy(80.0f, y - 60.0f);
	SpawnSecondEnemy(160.0f, y + 120.0f);
	SpawnSecondEnemy(160.0f, y - 120.0f);
}
void GameScene::SpawnSecondEnemy(float offsetX, float y) {
	auto sEnemy = std::make_shared<SecondEnemy>();
	sEnemy->Init();
	sEnemy->SetOwner(this);
	sEnemy->SetPos({ 720.0f + offsetX, y, 0.0f });

	// SecondEnemy にも攻撃タイプをセット
	sEnemy->SetAttackType(rand() % 2);

	m_objList.push_back(sEnemy);
}