// グラフィックインターフェース
// 2017. 2. 23	プログラム作成
// author		SyunMizuno


#pragma once

#include	"../../Convert.h"

/*
	Memo	:
			このままではGet/SetDeviceがよくないので要修正
*/



__interface IGraphics {
public:
	virtual void SetView(Matrix& view);
	virtual Matrix GetView();
	virtual void SetProjection(Matrix& proj);
	virtual Matrix GetProjection();
};