//	ランバートクラス
// 2016. 3.12	プログラム作成
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"../HlslDrawBase.h"

/*
	memo:
		フォンと併用する場合このクラスはフォンで代用できるため必要なし
		(フォンで利用していれば)
*/

class	HalfLambert : public	IHlslDrawBase {
public:
#pragma region method

	HalfLambert();
	virtual ~HalfLambert();
	virtual HRESULT Load();

#pragma endregion

protected:
	// ハンドルの接続
	virtual void ConnectHandle();
};