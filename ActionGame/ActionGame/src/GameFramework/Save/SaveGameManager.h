//////////////////////////////////////////////////////////////
// ゲーム用セーブマネージャー
// 2016.12.31	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"../framework/Save/SaveManagerBase.h"


class SaveGameManager	:	public	SaveManagerBase {
public:
	SaveGameManager() {};
	virtual ~SaveGameManager() {};

private:
	virtual ISaveObjBase* CreateObj(int classID);
};