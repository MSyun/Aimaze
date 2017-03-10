// アイテムマネージャークラス
// 2017. 1.25	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"ItemBox.h"
#include	"../../GameFramework/framework/Singleton/Singleton.h"
#include	<vector>
using namespace std;


class ItemBoxManagerBase {
protected:
	vector<ItemBox*>	m_aItemBox;

public:
#pragma region method

	ItemBoxManagerBase();
	virtual ~ItemBoxManagerBase();

	// 指定数作成
	void Create(int num);

	// リセット（リスタート時に使用したい
	void Reset();

	// アイテムボックスに登録
	void RecordItem(Obj3D* obj);

#pragma endregion

private:
	// 未使用を検索
	ItemBox* Search();
};

class ItemBoxManager	:	public	ItemBoxManagerBase,	public	Singleton<ItemBoxManager> {
};

inline ItemBoxManager* GetItemBoxManager() {
	return ItemBoxManager::Instance();
};