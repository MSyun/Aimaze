//	ランバートクラス
// 2016. 3.12	プログラム作成
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"../HlslDrawBase.h"

/*
	memo:
		フォンと併用する場合このクラスはフォンで代用できるため必要なし
*/

class	Lambert : public	IHlslDrawBase {
public:
	Lambert();
	virtual ~Lambert();
	virtual HRESULT Load();

protected:
	// ハンドルの接続
	virtual void ConnectHandle();
};