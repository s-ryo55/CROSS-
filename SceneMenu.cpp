#include "SceneMenu.h"

#include "DxLib.h"


void SceneMenu::Init()
{
	// ”wŒi‰æ‘œ‚Ì“Ç
	this->bg0.Load_image("ed.png");
	this->game_start.Load_image( "game_start.png");
	this->game_rule.Load_image( "game_rule.png");
	this->game_exit.Load_image( "game_exit.png");

}

/// <summary>
/// •`‰æˆ—
/// </summary>
void SceneMenu::Draw()
{
	// ”wŒi0‚ğ•`‰æ
	this->bg0.Draw();
	// ƒQ[ƒ€ƒXƒ^[ƒgƒXƒvƒ‰ƒCƒg‚ğ•`‰æ
	this->game_start.Draw();
}