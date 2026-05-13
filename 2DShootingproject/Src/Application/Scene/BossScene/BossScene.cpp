#include "BossScene.h"
#include "../SceneManager.h"
#include "../../Object//Player/Player.h"
#include "../../Object//Enemy/Enemy.h"
#include "../../Object/Bullet/Bullet.h"
#include "../../Object/Life/Life.h"
#include "../../Object/Shield/Shield.h"
#include "../../Object/SecondEnemy/SecondEnemy.h"
#include <random>



void BossScene::Init() {
    // 背景やスコア用テクスチャのロード（GameSceneと同様）
    m_hpBarTex.Load("Texture/White.png"); // 1x1の白い画像でOK
    std::shared_ptr<Player> player;

    player = std::make_shared<Player>();
   
    player = std::make_shared<Player>();
    player->Init();
    player->SetBossOwner(this);
    m_objList.push_back(player);

    std::shared_ptr<Boss>boss
    // ボス生成
    m_boss = std::make_shared<Boss>();
    m_boss->Init();
    m_boss->SetOwner(this);
    // 885x768の右側に配置 (x=300付近)
    m_boss->SetPos({ 300.0f, 0.0f, 0.0f });
    m_objList.push_back(m_boss);
}

void BossScene::Update() {
    // 1. オブジェクトの更新と不要なものの削除
    auto it = m_objList.begin();
    while (it != m_objList.end()) {
        if (!(*it)->GetAliveFlg()) {
            it = m_objList.erase(it);
        }
        else {
            (*it)->Update();
            ++it;
        }
    }

    // 2. ボスの死亡チェック
    if (m_boss && m_boss->GetHp() <= 0 && !m_isBossDead) {
        m_isBossDead = true;
        // スコア加算（ボーナス）
        m_score += 5000;
    }

    // 3. クリア演出後の遷移
    if (m_isBossDead) {
        m_clearTimer++;
        if (m_clearTimer > 180) { // 3秒待ってからクリア画面へ
            SceneManager::Instance().SetFinalScore(m_score);
            SceneManager::Instance().SetNextScene(SceneManager::SceneType::GameClear);
        }
    }
}

void BossScene::Draw2D() {
    // 背景の描画（省略）

    // 全オブジェクトの描画
    for (auto& obj : m_objList) {
        obj->Draw();
    }

    // --- ボスHPバーの描画 ---
    if (m_boss && !m_isBossDead) {
        float hpRate = (float)m_boss->GetHp() / m_boss->GetMaxHp();

        // バーの土台（黒）
        KdShaderManager::GetInstance().m_spriteShader.DrawTex(
            &m_hpBarTex, 0, 330, 700, 20, nullptr, &Math::Color{ 0,0,0,0.5f });

        // バーの本体（赤）
        // 885ピクセル幅に対して700ピクセルのバーを中央に配置
        Math::Rectangle rc = { 0, 0, 1, 1 }; // 白画像を伸ばして使う
        Math::Color hpColor = { 1.0f, 0.2f, 0.2f, 1.0f };

        KdShaderManager::GetInstance().m_spriteShader.DrawTex(
            &m_hpBarTex,
            (int)(-350 * (1.0f - hpRate)), // 左詰めに見せるための位置計算
            330,
            (int)(700 * hpRate), 20,
            &rc, &hpColor);
    }
}
void BossScene::Release()
{
}
}