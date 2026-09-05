#pragma once
#include <DxLib.h>

class Mouse 
{
public:
	// マウスの状態を取得する関数
	static bool GetMouseState(int& x, int& y, int& leftButton, int& rightButton) 
	{
		// DxLibの関数を使用してマウスの状態を取得
		GetMousePoint(&x, &y);
		leftButton = GetMouseInput() & MOUSE_INPUT_LEFT;
		rightButton = GetMouseInput() & MOUSE_INPUT_RIGHT;

		if (leftButton) {
			 // 左ボタンが押されている場合
			return true;
		}
		
	}


};
