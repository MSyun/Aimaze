//////////////////////////////////////////////////////////////
// セーブ用ファクトリクラス(ゲーム用)
// 2016. 12. 31	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#include	"SaveGameFactory.h"

#include	"../GameController/GameControllerManager.h"


ISaveObjBase* SaveGameFactory::Create(int classID) {
	ISaveObjBase* Obj = NULL;

	switch (classID) {
	case SAVE_GAME_FACT_CONTROLLER:
		Obj = new GameController;
		break;

	case SAVE_GAME_FACT_CONTROLLERMANAGER:
//		Obj = new GameControllerManager;
		break;
	}

	return Obj;
}