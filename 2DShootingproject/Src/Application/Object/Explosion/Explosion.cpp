#include "Explosion.h"
#include"../../Scene/GameScene/GameScene.h"


// 初期化
void Explosion::Init()
{
    // 爆発テクスチャの読み込み
    m_tex.Load("Texture/explosion.png");

    // アニメーション用変数の初期化
    m_animeCount = 0;       // 現在の経過フレーム
    m_animeIdx = 0;       // 現在何コマ目か
    m_maxAnime = 5;       // 全体のコマ数（画像に合わせて調整）
    m_animeSpeed = 4;       // 1コマ進むのにかかるフレーム数（小さいほど速い）

    m_aliveFlg = true;
}

// 更新処理
void Explosion::Update()
{
    if (!m_aliveFlg) return;

    // アニメーションの進行
    m_animeCount++;
    if (m_animeCount >= m_animeSpeed)
    {
        m_animeCount = 0;
        m_animeIdx++;

        // 全てのコマを表示し終えたら消滅
        if (m_animeIdx >= m_maxAnime)
        {
            m_aliveFlg = false;
        }
    }
}

// 描画処理
void Explosion::Draw()
{
    if (!m_aliveFlg) return;

    // 1コマのサイズ（例：画像全体が500pxで5コマなら1コマ100px）
    // 画像の実際のサイズに合わせて数値を変更してください
    int chipW = 100;
    int chipH = 100;

    // 現在のコマに応じた切り取り範囲を計算（横並びの場合）
    Math::Rectangle rc = {
        m_animeIdx * chipW, // 切り取り開始X
        0,                  // 切り取り開始Y
        chipW,              // 切り取り幅
        chipH               // 切り取り高さ
    };

    // 描画（少し大きく見せたい場合は 128, 128 などのサイズを指定）
    KdShaderManager::GetInstance().m_spriteShader.DrawTex(
        &m_tex,
        (int)m_pos.x, (int)m_pos.y,
        120,120,
        &rc
    );
}