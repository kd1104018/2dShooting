#pragma once

// 基本的にヘッダーファイルの中で
// 別のヘッダーファイルをインクルードするのはＮＧ
// 継承元クラスのヘッダーのみインクルードが必須なので可とする
#include "../BaseObject.h"

class Player : public BaseObject
{
public:
	Player() { Init(); }
	~Player() { Release(); }

	void Update();
	void Draw();
	void Init();

private:

	void Release();

};