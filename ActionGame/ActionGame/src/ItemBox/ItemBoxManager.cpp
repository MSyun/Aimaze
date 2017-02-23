// アイテムマネージャークラス
// 2017. 1.25	プログラム作成
// Author		SyunMizuno


#include	"ItemBoxManager.h"
#include	"../GameFramework/framework/Mesh/SkinMesh/SkinMeshManager.h"
#include	"../Archives/Archives.h"
#include	"../GameFramework/framework/Debug/Debug.h"
#include	"../GameFramework/framework/Texture/TexManager.h"



/*									//
//			コンストラクタ			//
//									*/
ItemBoxManagerBase::ItemBoxManagerBase() {
	GetSkinMeshManager()->Create(Archives::Mesh("ItemBox"));
}


/*									//
//			デストラクタ			//
//									*/
ItemBoxManagerBase::~ItemBoxManagerBase() {
	GetSkinMeshManager()->Delete(Archives::Mesh("ItemBox"));
}


/*									//
//				指定数作成			//
//									*/
void ItemBoxManagerBase::Create(int num) {
	ItemBox* pItem;
	for (int i = 0; i < num; ++ i) {
		pItem = new ItemBox;
		pItem->SetActive(false);
		m_aItemBox.push_back(pItem);
	}
}


/*									//
//				リセット			//
//									*/
void ItemBoxManagerBase::Reset() {
	for (auto it = m_aItemBox.begin(); it != m_aItemBox.end(); ++it) {
		(*it)->SetActive(false);
	}
}


/*									//
//		アイテムボックスに登録		//
//									*/
void ItemBoxManagerBase::RecordItem(Obj3D* obj) {
	if (!obj) {
		Debug::LogError("インスタンスがありません");
		return;
	}

	ItemBox* pItem = Search();

	if (!pItem)	return;

	pItem->SetActive(true);
	pItem->GetTransform()->SetPos(obj->GetTransform()->GetPos());
	pItem->Begin(GetTexManager()->Get(Archives::Texture("Ring")));
	pItem->SetItem(obj);
}


/*									//
//			未使用を検索			//
//									*/
ItemBox* ItemBoxManagerBase::Search() {
	// 検索♪検索～♪
	for (auto it = m_aItemBox.begin(); it != m_aItemBox.end(); ++it) {
		if (!(*it)->GetActive())	return (*it);
	}

	// 全部使用しとるやんけ…
	Debug::LogError("未使用のItemBoxがありません");
	return NULL;
}