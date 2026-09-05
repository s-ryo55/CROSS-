#include "Jiki.h"
#include "SceneTest.h"

/// <summary>
/// 移動
/// </summary>
/// <param name="arg_diff_x">横位置の差分</param>
/// <param name="arg_diff_y">縦位置の差分</param>
void Jiki::Move(int arg_diff_x, int arg_diff_y)
{
	// 親クラスメソッド呼び出し
	Sprite::Move(arg_diff_x, arg_diff_y);

	// Sceneをダウンキャスト
	SceneTest* scene_ptr = dynamic_cast<SceneTest*>(this->scene_ptr);

	// ダウンキャストが成功している場合
	if(scene_ptr != nullptr)
	{
		// 自機位置チェック
		if(this->pos_x < 1280 / 3)
		{
			// 左側
			scene_ptr->Select_tekki_dir(3);
		}
		else if (this->pos_x > 1280 - (1280 / 3))
		{
			// 右側
			scene_ptr->Select_tekki_dir(1);
		}
	}
}



