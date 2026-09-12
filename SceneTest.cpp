#include "SceneTest.h"
#include "DxLib.h"





void SceneTest::Init()
{
	// ”wŒi‰æ‘œ‚Ì“Ç
	this->bg0.Load_image("data/ingame.png");

	for(int x = 0; x <7; x++){
		for(int y = 0; y < 7; y++){
			for(int state = 0; state < 4; state++)
			{
				// ƒXƒvƒ‰ƒCƒg‚Ì‰Šú‰»
				this->board_surface[x][y][state].Load_image("data/board_" + std::to_string(state) + ".png");
			}
		}
	}



}


/// <summary>
/// “ü—Íˆ—
/// </summary>
void SceneTest::Input()
{

}

/// <summary>
/// XVˆ—
/// </summary>
void SceneTest::Update()
{
	

}

/// <summary>
/// •`‰æˆ—
/// </summary>
void SceneTest::Draw()
{
	// ”wŒi0‚ğ•`‰æ
	this->bg0.Draw();



}

/// <summary>
/// ‰¹ºÄ¶ˆ—
/// </summary>
void SceneTest::Sound_play()
{


}

/// <summary>
/// “G‹@•ûŒüw’è
/// </summary>
/// <param name="arg_dir">“G‹@•ûŒü</param>
void SceneTest::Select_tekki_dir(int arg_dir)
{
	
	
}


