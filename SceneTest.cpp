#include "SceneTest.h"
#include "DxLib.h"






void SceneTest::Init()
{
	// 背景画像の読込
	this->bg0.Load_image("data/ingame.png");
	this->reverse_select[0].Load_image("data/reverse.png");
	this->reverse_select[1].Load_image("data/reverse_select_lock.png");
	this->reach_select[0].Load_image("data/reach.png");
	this->reach_select[1].Load_image("data/reach_lock.png");

	for(int x = 0; x <7; x++){
		for(int y = 0; y < 7; y++){
			for(int state = 0; state < 4; state++)
			{
				// スプライトの初期化
				this->board_surface[x][y][state].Load_image("data/board_" + std::to_string(state) + ".png");
			}

			// スプライトの表示位置を設定
			this->board_surface[x][y][0].Set_pos(100 + x * 72, 200 + y * 63);

		}
	}

	for(int i = 0; i < 2; i++) {
		this->reverse_select[i].Set_pos(15, 100);
		this->reach_select[i].Set_pos(150, 100);
	}



}


/// <summary>
/// 入力処理
/// </summary>
void SceneTest::Input()
{
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
	if (this->mouse.IsClickSprite(this->board_surface[x][y][0]) == 1)
	{
		// クリックされたときの処理
		
	}

		}
	}	

/// <summary>
/// 更新処理
/// </summary>
void SceneTest::Update()
{
	

}

/// <summary>
/// 描画処理
/// </summary>
void SceneTest::Draw()
{
	// 背景0を描画
	this->bg0.Draw();
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
				this->board_surface[x][y][board_state.getBoardState(x, y)].Draw();
		}
	}


	

}

/// <summary>
/// 音声再生処理
/// </summary>
void SceneTest::Sound_play()
{


}

/// <summary>
/// 敵機方向指定
/// </summary>
/// <param name="arg_dir">敵機方向</param>
void SceneTest::Select_tekki_dir(int arg_dir)
{
	
	
}


