//////////////////////////////////////////////////////////////
// セーブ用ファクトリクラス
// 2016. 12. 27	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"SaveObjBase.h"


typedef enum {
	SAVE_FACT_SOUND = 0,
	SAVE_FACT_SOUNDMANAGER,
	SAVE_FACT_MAX,
}_SAVE_FACTORY;


class SaveFactory {
public:
	SaveFactory() {}
	virtual ~SaveFactory() {}
	static ISaveObjBase* Create(int classID);
};