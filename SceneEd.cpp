#include "SceneEd.h"

#include "DxLib.h"


void SceneEd::Init()
{
	// Game ‚ÉƒZƒbƒg‚³‚ê‚½ŸÒ‚ğæ“¾
	this->winner = this->game_ptr->GetWinner();

	// ”wŒi‰æ‘œ‚Ì“Ç
	for (int i = 1; i < 4; i++) {
		this->bg[i].Load_image("data/WIN_" + std::to_string(i) + ".png");
	}
}

/// <summary>
/// •`‰æˆ—
/// </summary>
void SceneEd::Draw()
{
	// winner ‚É‘Î‰‚·‚é”wŒi‚ğ•`‰æi”z—ñ‚Í 1..3 ‚ğ‘z’èj
	if (this->winner >= 1 && this->winner <= 3) {
		this->bg[this->winner].Draw();
	}
}
