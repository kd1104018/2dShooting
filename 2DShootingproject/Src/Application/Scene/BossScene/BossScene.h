
#pragma once 
#include "../BaseScene/BaseScene.h"
class BaseObject;
class Shield; // 追加：前方宣言

class BossScene : public BaseScene 
{
public:

    BossScene() { Init(); }
    ~BossScene() {}

    void Init() override;
    void Update() override;
    void Draw2D() override;

    void Release() override;
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
    std::shared_ptr<BaseObject> m_boss = nullptr;      // ボスへのポインタ
    std::shared_ptr<BaseObject> m_player = nullptr;  // プレイヤーへのポインタ

    KdTexture m_hpBarTex;              // HPバー用の白い画像
    int m_clearTimer = 0;              // クリア後の余韻タイマー
    bool m_isBossDead = false;         // ボス撃破フラグ
};