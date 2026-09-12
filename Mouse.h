#pragma once
#include <DxLib.h>

class Mouse 
{
	int mouse_x;
	int mouse_y;

public:
	
	// マウスの状態を取得する関数
	void Read() {
		// マウスの座標を取得
		GetMousePoint(&this->mouse_x, &this->mouse_y);
	}

	int IsClickSprite(Sprite& sprite) {
		// スプライトの範囲内にマウスがあるかをチェック
		int sprite_x, sprite_y;
		sprite_x = sprite.Get_pos_x();
		sprite_y = sprite.Get_pos_y();
		int sprite_width, sprite_height;
		sprite.Get_size(&sprite_width, &sprite_height);
		if (this->mouse_x >= sprite_x && this->mouse_x <= sprite_x + sprite_width &&
			this->mouse_y >= sprite_y && this->mouse_y <= sprite_y + sprite_height) {
			// 左ボタンが押されている場合
			if (GetMouseInput() & MOUSE_INPUT_LEFT) {
				return 1;
			}
			else {
				return 2; // マウスが範囲内にあるが、左ボタンは押されていない
			}
		}
		return 0;
	}

};
