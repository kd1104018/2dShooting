#pragma once

// 基本的にヘッダーファイルの中で
// 別のヘッダーファイルをインクルードするのはＮＧ
// 継承元クラスのヘッダーのみインクルードが必須なので可とする
#include "../BaseObject.h"

class Player : public BaseObject
{
public:
	Player() { Init(); }
	~Player() override { Release(); }

	void Update()override;
	void Draw()override;
	void Init()override;

private:

	void Release()override;

};