// 2Dオブジェクトマネージャクラス
// 2017. 1.4	プログラム作成
// Author		SyunMizuno


#include	"Obj2DManager.h"
#include	<tchar.h>
#include	"../../framework/Utility/System/SystemUtility.h"
#include	"../../framework/Debug/Debug.h"
#include	"../../../Archives/Archives.h"


/*
	MEMO	:
			後々4分木等の処理を他のクラスに委託してもよいかも
*/



/*									//
//			コンストラクタ			//
//									*/
Obj2DManagerBase::Obj2DManagerBase() :
	m_bHelper(true),
	m_pQuadTree(NULL)
{
	m_lstObj.clear();

	// 4分木の空間を作成
	//if (!CreateTree(4, Vector2(-100, -10), Vector2(100, 30))) {
//	Debug::Log("ERROR : 線形4分木空間の初期化に失敗しました");
	//}
}


/*									//
//			デストラクタ			//
//									*/
Obj2DManagerBase::~Obj2DManagerBase() {
	if (!m_bHelper)	return;

	while (m_lstObj.begin() != m_lstObj.end()) {
		list<_2DOBJ*>::iterator it = m_lstObj.begin();
		delete [] (*it)->pObj;
	}
	m_lstObj.clear();
	//DeleteTree();
}


/*									//
//				検索				//
//									*/
// 名前
Sprite* Obj2DManagerBase::Find(const string name) {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 使用してない
		if (!(*it)->pObj->GetActive())	continue;

		if ((*it)->pObj->GetName() == name) {
			return (*it)->pObj;
		}
	}
	Debug::LogError("2Dオブジェクト名" + name + "は存在しません");
	return NULL;
}
// タグ
Sprite* Obj2DManagerBase::FindWithTag(const string tag) {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 使用してない
		if (!(*it)->pObj->GetActive())	continue;

		if ((*it)->pObj->GetTag() == Archives::Tag(tag)) {
			return (*it)->pObj;
		}
	}
	Debug::LogError("2Dオブジェクトにタグ名" + tag + "は存在しません");
	return NULL;
}


/*									//
//				追加				//
//									*/
void Obj2DManagerBase::Add(Sprite* Obj) {
	if (!Obj) {
		Debug::LogError("オブジェクトの実態がありません");
		return;
	}

	// インスタンス作成
	_2DOBJ* regist = new _2DOBJ;
	regist->pTree = new OBJECT_FOR_TREE<Sprite*>;

	// オブジェクトを設定
	regist->pObj = Obj;
	regist->pTree->m_pObject = &Obj;

	// 登録
	m_lstObj.push_back(regist);
}


/*									//
//				全削除				//
//									*/
void Obj2DManagerBase::AllClear() {
	list<_2DOBJ*> lstBuff;
	while (m_lstObj.begin() != m_lstObj.end()) {
		list<_2DOBJ*>::iterator it = m_lstObj.begin();
		if ((*it)->pObj->GetDontDestroy()) {
			lstBuff.push_back(*it);
			m_lstObj.erase(it);
			continue;
		}
		delete [] (*it)->pObj;
	}
	m_lstObj.clear();

	for (auto it = lstBuff.begin(); it != lstBuff.end(); ++it) {
		m_lstObj.push_back(*it);
	}
}


/*									//
//				削除				//
//	(削除されたときに呼び出される)	*/
void Obj2DManagerBase::Delete(Sprite* Obj) {
	if (!Obj) {
		Debug::LogError("オブジェクトの実態がありません");
		return;
	}

	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		if ((*it)->pObj != Obj)	continue;

		// リストから解放
		(*it)->pTree->Remove();

		// インスタンス削除
		SAFE_DELETE((*it)->pTree);
		SAFE_DELETE(*it);

		m_lstObj.erase(it);

		return;
	}
}


/*									//
//				更新				//
//									*/
void Obj2DManagerBase::Update() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 使用確認
		if (!(*it)->pObj->GetActive())	continue;

		(*it)->pObj->UpdateAll();
	}
}
void Obj2DManagerBase::LateUpdate() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 4分木から解放
		//(*it)->pTree->Remove();

		// 使用確認
		if (!(*it)->pObj->GetActive())	continue;

		(*it)->pObj->LateUpdateAll();

		// 一応もう一度登録
		//(*it)->pTree->m_pObject = &(*it)->pObj;

		//if (!(*it)->pObj->GetCollider() ||
		//	!(*it)->pObj->GetCollider()->GetTreeColl())
		//	return;

		// 4分木に登録
		//RegistTree(*it);
	}
}


/*									//
//				削除確認			//
//									*/
void Obj2DManagerBase::CheckDestroy() {
	list<_2DOBJ*>	lstobj;
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++ it) {
		if ((*it)->pObj->GetDestroy()) {
			(*it)->pObj->OnDestroy();
			lstobj.push_back(*it);
			continue;
		}
	}
	while (lstobj.begin() != lstobj.end()) {
		auto it = lstobj.begin();
		delete [] (*it)->pObj;
		lstobj.erase(it);
	}
}


/*									//
//				あたり判定			//
//									*/
//bool Obj2DManagerBase::SkipCheck(BYTE A, BYTE B) {
//	return false;
//}
//void Obj2DManagerBase::CheckCollider() {
//
//}
//bool Obj2DManagerBase::Collision() {
//	return false;
//}


/*									//
//				描画				//
//									*/
void Obj2DManagerBase::Draw() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		if (!(*it)->pObj->GetActive())	continue;

		(*it)->pObj->DrawAll();
	}
}
void Obj2DManagerBase::LateDraw() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		if (!(*it)->pObj->GetActive())	continue;

		(*it)->pObj->LateDrawAll();
	}
}


/*									//
//			クアッドツリー			//
//									*/
//bool Obj2DManagerBase::CreateTree(
//	unsigned int Level,
//	Vector2& Min,
//	Vector2& Max)
//{
//	DeleteTree();
//
//	m_pQuadTree = new CLiner4TreeManager<Sprite*>;
//	return m_pQuadTree->Init(Level, Min.x, Min.y, Max.x, Max.y);
//}
//void Obj2DManagerBase::DeleteTree() {
//	SAFE_DELETE(m_pQuadTree);
//}
//void Obj2DManagerBase::RegistTree(_2DOBJ* obj) {
//	// インスタンス取得
//
//}