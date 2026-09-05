#pragma once
#include <string>
#include "DxLib.h"		// DxLib

/// /// <summary>
/// 背景クラス
/// </summary>
class Background
{
protected:

	/// <summary>
	/// 画像ファイルパス
	/// </summary>
	std::string file_path = "";

	/// <summary>
	/// 画像ハンドル
	/// </summary>
	int image_hnd = -1;

public:

	/// <summary>
	/// デフォルトコンストラクター
	/// </summary>
	Background(){}

	/// <summary>
	/// コンストラクター
	/// </summary>
	/// <param name="arg_file_path">初期画像ファイルパス</param>
	Background(std::string arg_file_path){
		// 画像ファイルを読み込む
		this->Load_image(arg_file_path);
	}

	/// <summary>
	/// 画像ファイルを読み込む
	/// </summary>
	/// <param name="arg_file_path">画像ファイルパス</param>
	void Load_image(std::string arg_file_path)
	{
		// 画像ファイルパスを保持
		this->file_path = arg_file_path;

		// 指定されたファイルを読み込む
		this->image_hnd = LoadGraph(this->file_path.c_str());
	}

	/// <summary>
	/// 背景を描画
	/// 　透過無し
	/// </summary>
	virtual void Draw()
	{
		// 背景を描画
		DrawGraph(0, 0, this->image_hnd, false);	
	}
};



