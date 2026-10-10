#pragma once
#include <string>
#include <DxLib.h>

class Se
{
	std::string file_path = "";
	bool is_playing = false;
	int se_hud;



public:
	/// <summary>
	/// デフォルトコンストラクター
	/// </summary>
	Se(){}
	/// <summary>
	/// コンストラクター
	/// </summary>
	/// <param name="arg_file_path">初期SEファイルパス</param>
	Se(std::string arg_file_path){
;
	}
	/// <summary>
	/// SEファイルを読み込む
	/// </summary>
	/// <param name="arg_file_path">SEファイルパス</param>
	void Load_se(std::string arg_file_path)
	{

		se_hud = LoadMusicMem(arg_file_path.c_str());
	}

	void Set_playing()
	{
		is_playing = true;
	}

	void Play()
	{
		if (is_playing) 
		{
			PlayMusicMem(se_hud, DX_PLAYTYPE_BACK);
			is_playing = false;
		}
	}

	void Play_bgm()
	{
		if (is_playing)
		{
			PlayMusicMem(se_hud, DX_PLAYTYPE_LOOP);
			is_playing = false;
		}
	}

	void Stop()
	{
		if (is_playing)
		{
			StopMusicMem(se_hud);
			is_playing = false;
		}
	}




};