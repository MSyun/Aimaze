//////////////////////////////////////////////////////////////
// SaveManagerBase クラス
// 2016. 9. 24	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"../Obj/SaveObjBase.h"
#include	<string>
#include	<vector>
#include	<fstream>

using namespace std;


class	ISaveObjBase;
struct	DATARECORD;

enum SAVE_FLAG {
	SAVE_OK,
	LOAD_OK,
	NO_SAVEFILE,
	CONNECT_OK,
};

#define	NO_ELEM		(-1)
#define	TYPE_END	(0)		// 終了
#define	TYPE_LOCAL	(1)		// ローカル
#define	TYPE_PTR	(2)		// ポインタ
#define	TYPE_MEM	(3)		// メモリブロック
#define	TYPE_BASE	(4)		// 親クラスデータ


#define	DATA_END										{	TYPE_END,	0,											0										}
#define	DATA_LOCAL( CLASSNAME, MEMBERNAME )				{	TYPE_LOCAL,	((__int64)&((CLASSNAME*)0)->MEMBERNAME),	sizeof( ((CLASSNAME*)0)->MEMBERNAME )	}
#define	DATA_PTR( CLASSNAME, MEMBERNAME )				{	TYPE_PTR,	((__int64)&((CLASSNAME*)0)->MEMBERNAME),	sizeof(int)								}
#define	DATA_MEM( CLASSNAME, POINTERNAME, SIZENAME )	{	TYPE_MEM,	((__int64)&((CLASSNAME*)0)->POINTERNAME),	((__int64)&((CLASSNAME*)0)->SIZENAME)	}
#define	DATA_BASE( TABLEPTR )							{	TYPE_BASE,	((__int64)TABLEPTR),						0										}


class	SaveManagerBase {
public:
	SaveManagerBase(void) {};
	virtual ~SaveManagerBase(void) {};

	/////////////////////////////////////////////////////////
	// 外で使用するのはここだけ
	int Save(const string filename);		// セーブ
	int Load(const string filename);		// ロード
	void AddSaveData(ISaveObjBase* pobj);	// セーブオブジェクト追加
	/////////////////////////////////////////////////////////

	int Write(DATARECORD* list, ISaveObjBase* pobj);
	int Read(DATARECORD* list, ISaveObjBase* pobj);
	int ConnectPtr(DATARECORD* list, ISaveObjBase* pobj);	// オブジェクトのポインタを結合

protected:
	vector< ISaveObjBase* >	m_ObjList;
	fstream	m_fs;

	int LoadSkip();									// 読み込みデータを1つスキップ
	int ElemFromPtr(void* pos);						// ポインタを要素番号に変換
	virtual ISaveObjBase* CreateObj(int classID);	// オブジェクト生成
};