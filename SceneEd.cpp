#include "SceneEd.h"

#include "DxLib.h"


void SceneEd::Init()
{
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
	
		this->bg[board_state.].Draw();
	
}
