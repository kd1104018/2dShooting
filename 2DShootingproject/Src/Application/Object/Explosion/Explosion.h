#pragma once

#include "../BaseObject.h"

class Explosion : public BaseObject {
public:
    Explosion() { Init(); }
    ~Explosion() { Release(); }
    void Update() override;
    void Draw() override;
	void Init() override;
    
private:
    KdTexture m_tex;
    int       m_animeCount; // フレーム数カウント
    int       m_animeIdx;   // 現在のコマ番号
    int       m_maxAnime;   // 最大コマ数
    int       m_animeSpeed; // アニメーション速度
    
};