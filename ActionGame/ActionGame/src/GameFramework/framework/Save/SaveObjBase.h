//////////////////////////////////////////////////////////////
// CSaveObjBase クラス
// 2016. 9. 24	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once

#include	"SaveManagerBase.h"


// セーブデータオフセット構造体
struct DATARECORD {
	char	type;	// データタイプ
	int		offset;	// オフセット値
	int		size;	// サイズ
};


class	SaveManagerBase;

class	ISaveObjBase {
	friend class SaveManagerBase;

public:
	virtual DATARECORD* GetDataRecord() = 0;	// データ配列を取得
	virtual int GetClassID() = 0;				// クラスIDを取得

protected:
	virtual int Save(SaveManagerBase* mgr);
	virtual int Load(SaveManagerBase* mgr);
	int ConnectPtr(SaveManagerBase* mgr);
};