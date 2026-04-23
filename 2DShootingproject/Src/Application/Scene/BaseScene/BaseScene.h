#pragma once 

//各シーンの基底クラス（親クラス）
class BaseScene
{
public:


	BaseScene() { Init(); }
	virtual ~BaseScene() {}

	//仮想関数...継承先で上書きされることを前定とした関数
	virtual void Init();
	virtual void Update();
	virtual void Draw2D();
	virtual void Release();
private:


};