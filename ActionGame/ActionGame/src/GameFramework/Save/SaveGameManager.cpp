//////////////////////////////////////////////////////////////
// ゲーム用セーブマネージャー
// 2016.12.31	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"SaveGameManager.h"
#include	"SaveGameFactory.h"


/*									//
//			ファクトリ関数			//
//		（ここは継承すること前提）	*/
ISaveObjBase* SaveGameManager::CreateObj(int classID) {
	return SaveGameFactory::Create(classID);	// 指定のクラスがない
}