//////////////////////////////////////////////////////////////
// セーブ用ファクトリクラス
// 2016. 12. 27	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#include	"SaveFactory.h"

#include	"../Sound/SoundBase.h"
#include	"../Sound/SoundManager.h"


ISaveObjBase* SaveFactory::Create(int classID) {
	ISaveObjBase* Obj = NULL;

	switch (classID) {
	case _SAVE_FACTORY::SAVE_FACT_SOUND:
		Obj = new SoundBase;
		break;

	case _SAVE_FACTORY::SAVE_FACT_SOUNDMANAGER:
		Obj = new SoundManager;
		break;

	default:
		break;
	};

	return Obj;
}