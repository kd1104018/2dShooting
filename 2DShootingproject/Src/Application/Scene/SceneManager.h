#pragma once
//前方宣言	

class BaseScene;
class SceneManager

{
public:
	//シーン情報
	enum SceneType
	{
		//int 型の列挙体を定義
		Title,
		Game,
		GameOver,
		Result,
	};
	
	void Update();
	void Draw();

	void SetNextScene(SceneType _nextScene)
	{
		m_nextSceneType = _nextScene;
	}
	SceneType GetcurrentScene()const
	{
		return m_currentSceneType;
	}
	void SetFinalScore(unsigned long _score) { m_finalScore = _score; }
	unsigned long GetFinalScore() const { return m_finalScore; }

private:

	void Init();
	void Release();
	void ChangeScene(SceneType _Scenetype);


	//変数
	std::shared_ptr<BaseScene> m_CurrentScene;
	SceneType m_currentSceneType = SceneType::Title;

	SceneType m_nextSceneType = m_currentSceneType;

	unsigned long m_finalScore = 0;
	

	//シングルトンパターン
	// シングルトンパターンはクラスのインスタンスが1つしか存在しないことを保証するデザインパターン	

private:
	SceneManager() { Init(); }
	~SceneManager() { Release(); }
public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}
};