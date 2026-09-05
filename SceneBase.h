#pragma once

class SceneBase{
protected:

	/// <summary>
	/// 更新カウンター
	/// </summary>
	int update_cnt = 0;

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 入力処理
	/// </summary>
	 virtual void Input() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	 virtual void Update() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	 virtual void Draw() = 0;

	/// <summary>
	/// 音声再生処理
	/// </summary>
	 virtual void Sound_play() = 0;

};
