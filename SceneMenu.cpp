#include "SceneMenu.h"

#include "DxLib.h"


void SceneMenu::Init()
{
	// ”wŒi‰æ‘œ‚Ì“Ç
	this->bg0.Load_image("data/menu.png");
	this->game_start.Load_image("data/start.png");
	this->game_rule.Load_image("data/asobikata.png");
	this->game_exit.Load_image("data/end.png");
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
	this->game_rule.Draw();
	this->game_exit.Draw();
}