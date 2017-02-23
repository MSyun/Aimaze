//////////////////////////////////////////////////////////////
// CSaveObjBase クラス
// 2016. 9. 24	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"SaveObjBase.h"


/*									//
//				セーブ				//
//									*/
int ISaveObjBase::Save(SaveManagerBase* mgr) {
	return mgr->Write(GetDataRecord(), this);	// 書き込み
}


/*									//
//				ロード				//
//									*/
int ISaveObjBase::Load(SaveManagerBase* mgr) {
	return mgr->Read(GetDataRecord(), this);	// 読み込み
}


/*									//
//			ポインタ接続			//
//									*/
int ISaveObjBase::ConnectPtr(SaveManagerBase* mgr) {
	return mgr->ConnectPtr(GetDataRecord(), this);
}