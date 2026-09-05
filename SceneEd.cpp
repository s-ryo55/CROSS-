#include "SceneEd.h"

#include "DxLib.h"


void SceneEd::Init()
{
	// ”wŒi‰æ‘œ‚Ì“Ç
	this->bg0.Load_image("ed.png");
}

/// <summary>
/// •`‰æˆ—
/// </summary>
void SceneEd::Draw()
{
	// ”wŒi0‚ğ•`‰æ
	this->bg0.Draw();
}
