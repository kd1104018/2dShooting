#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Object//Player/Player.h"
#include "../../Object//Enemy/Enemy.h"
#include "../../Object/Bullet/Bullet.h"
#include "../../Object/Life/Life.h"


void GameScene::Init()
{

	std::shared_ptr<Player> player;

	player = std::make_shared<Player>();
	player->Init();
	player->SetOwner(this);	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを渡す
	m_objList.push_back(player);

	// エネミー
	std::shared_ptr<Enemy> enemy;
	for (int i = 0; i < 10; ++i)
	{
		enemy = std::make_shared<Enemy>();	// ①インスタンスを生成
		enemy->Init();						// ②初期化
		
		enemy->SetOwner(this);	// プレイヤーはゲームシーンに所属しているので、ゲームシーンのポインタを渡す
		m_objList.push_back(enemy);
	}
	std::shared_ptr<Life> life;
	for (int i = 0; i < m_lifeCount; ++i)
	{
		life = std::make_shared<Life>();	// ①インスタンスを生成
		life->Init();						// ②初期化
		life->SetPos({ 10.0f + i * 40.0f, 10.0f, 0.0f }); // ライフの位置を設定
		m_objList.push_back(life);
	}




}

void GameScene::Update()
{
	//m_player->Update();
	//m_enemy->Update();

	/*if (GetAsyncKeyState('Z') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}
	if (GetAsyncKeyState('L') & 0x8000)
	{
		m_score += 10;
		if(m_score>9999999999)
			{
			m_score = 9999999999;
		}
	}
	if(GetAsyncKeyState('P')&0x8000)
		{

		m_score -=100;
		if(m_score<0)
			{
			m_score = 0;
		}
	}
	++i;
	unsigned long tmp = m_score;
	for (int i = maxDigit-1; i  >=0; --i)
	{
		m_digits[i] = tmp % 10;
		tmp /= 10;
	}*/
	auto it = m_objList.begin();

	while (it != m_objList.end())	// end() は 最後の要素の1個後ろを返す
	{
		// オブジェクトの有効チェック
		if ((*it)->GetAliveFlg() == false)
		{
			// 無効なオブジェクトをリストから削除
			it = m_objList.erase(it);
		}
		else
		{
			it++;	// 次の要素へイテレータを進める
		}
	}


	for (int i = 0; i < m_objList.size(); ++i)
	{
		m_objList[i]->Update();
	}
}

void GameScene::Draw2D()
{

	for (int i = 0; i < m_objList.size(); ++i)
	{
		m_objList[i]->Draw();
	}
	/*KdShaderManager::GetInstance().m_spriteShader.DrawTex(
		&m_starttex, 0,100,200,100);
for (int i = 0; i < maxDigit; ++i)
	{
		KdShaderManager::GetInstance().m_spriteShader.DrawTex(
			&m_tex, 200 + i * 20, 100, 20, 30, &Math::Rectangle(m_digits[i] * 20, 0, 20, 30));
}*/

}

void GameScene::Release()
{
}
