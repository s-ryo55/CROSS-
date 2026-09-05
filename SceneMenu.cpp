#include "SceneMenu.h"

#include "DxLib.h"


void SceneMenu::Init()
{
	// ”wŒi‰æ‘œ‚Ì“Ç
	this->bg0.Load_image("data/menu.png");
}

/// <summary>
/// •`‰æˆ—
/// </summary>
void SceneMenu::Draw()
{
	// ”wŒi0‚ğ•`‰æ
	this->bg0.Draw();
}