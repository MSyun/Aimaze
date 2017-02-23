// スカイドームクラス
// 2017. 1. 4	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../Obj/3D/Obj3D.h"


class Skydome	:	public	Obj3D {
public:
#pragma region method

	Skydome();
	virtual ~Skydome();

#pragma endregion

protected:
#pragma region conceal method

	virtual void Update();
	virtual void Draw();

#pragma endregion
};