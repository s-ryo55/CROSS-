#pragma once
#include <string>
#include <DxLib.h>

enum class SeType
{
	SE_TYPE_OP,
	SE_TYPE_MENU,
	SE_TYPE_PLAY,
	SE_TYPE_REVERSE,
};

class Se
{
	bool se_loaded[4] = { false, false, false, false };
	int se_hnd[4];




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
		// SEファイルを読み込む
		this->Load_se();
	}
	/// <summary>
	/// SEファイルを読み込む
	/// </summary>
	/// <param name="arg_file_path">SEファイルパス</param>
	void Load_se()
	{
		// 指定されたファイルを読み込む
	
			this->se_hnd[static_cast<int>(SeType::SE_TYPE_OP)] = LoadSoundMem("data/start.mp3");
			this->se_hnd[static_cast<int>(SeType::SE_TYPE_MENU)] = LoadSoundMem("data/menu.mp3");
			this->se_hnd[static_cast<int>(SeType::SE_TYPE_PLAY)] = LoadSoundMem("data/player.mp3");
			this->se_hnd[static_cast<int>(SeType::SE_TYPE_REVERSE)] = LoadSoundMem("data/reverse.mp3");
		
	
	}

	void Update(SeType type)
	{
		// SEの状態を更新する処理が必要な場合はここに追加
		se_loaded[static_cast<int>(type)] = true;

	}

	void Play(SeType type)
	{
		//音量
		if (se_loaded[static_cast<int>(type)])
		{
			ChangeVolumeSoundMem(255, this->se_hnd[static_cast<int>(type)]);
			PlaySoundMem(this->se_hnd[static_cast<int>(type)], DX_PLAYTYPE_BACK);
		}


	}



};