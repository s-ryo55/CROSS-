#pragma once
#include "DxLib.h"		// DxLib

/// <summary>
/// キーリーダークラス
/// </summary>
class KeyReader
{
	/// <summary>
	/// キーの状態配列
	/// </summary>
	char key_state_arr[256];

	/// <summary>
	/// キーが押されたか
	/// </summary>
	bool key_press = false;

public:

	/// <summary>
	/// コンストラクター
	/// </summary>
	KeyReader()
	{
		// キー状態配列を初期化
		for(int cnt = 0;cnt < 256; cnt++)
		{
			this->key_state_arr[cnt] = 0;
		}
	}

	/// <summary>
	/// キー読込（一括）
	/// </summary>
	void Read()
	{
		GetHitKeyStateAll(this->key_state_arr);

		this->key_press = false;
		for (int cnt = 0; cnt < 256; cnt++)
		{
			if (this->key_state_arr[cnt] != 0)
			{
				this->key_press = true;
			}
		}
	}

	/// <summary>
	/// キーの状態確認
	/// </summary>
	/// <param name="arg_key_code">確認するキーのコード</param>
	/// <returns>キーの状態</returns>
	bool CheckKey(int arg_key_code){
		// 指定されたキーの状態をチェック
		if(this->key_state_arr[arg_key_code] == 1)
		{
			// 押されていればtrueを返す
			return true;
		}
		// 押されていなければfalseを返す
		return false;
	}

	/// <summary>
	/// キーが押されたかのチェック
	/// </summary>
	/// <returns>状態</returns>
	bool KeyPress()
	{
		return this->key_press;
	}
};
