// 3Dオブジェクトマネージャクラス
// 2016. 8.19	プログラム作成
// 2016. 9. 7	オクツリー実装開始
// 2016. 9.14	オクツリー仮実装完了
// 2017. 1. 4	DontDestroy関係追加
// Author		SyunMizuno

#include	"Obj3DManager.h"
#include	<tchar.h>
#include	"../../framework/Utility/System/SystemUtility.h"
#include	"../../framework/Debug/Debug.h"
#include	"../../../Archives/Archives.h"


/*
	MEMO	:
			後々8分木等の処理を他のクラスに委託してもよいかも
*/


/*									//
//			コンストラクタ			//
//									*/
Obj3DManagerBase::Obj3DManagerBase() :
	m_bHelper(true),
	m_pOcTree(NULL)
{
	m_lstObj.clear();

	// 8分木の空間を作成
	if (!CreateTree(6, Vector3(-300, -10, -300), Vector3(300, 30, 300))) {
		Debug::LogError("線形8分木空間の初期化に失敗しました");
	}
}


/*									//
//			デストラクタ			//
//									*/
Obj3DManagerBase::~Obj3DManagerBase() {
	if (!m_bHelper)	return;

	while (m_lstObj.begin() != m_lstObj.end()) {
		list<_3DOBJ*>::iterator it = m_lstObj.begin();
		delete [] (*it)->pObj;
	}
	m_lstObj.clear();

	DeleteTree();
}


/*									//
//				検索				//
//									*/
// 名前
Obj3D* Obj3DManagerBase::Find(const string name) {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 使用してない
		if (!(*it)->pObj->GetActive())	continue;

		if ((*it)->pObj->GetName() == name) {
			return (*it)->pObj;
		}
	}
	Debug::LogError("3Dオブジェクト名" + name + "は存在しません");
	return NULL;
}
// タグ
Obj3D* Obj3DManagerBase::FindWithTag(const string tag) {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 使用してない
		if (!(*it)->pObj->GetActive())	continue;

		if ((*it)->pObj->GetTag() == Archives::Tag(tag)) {
			return (*it)->pObj;
		}
	}
	Debug::LogError("3Dオブジェクトにタグ名" + tag + "は存在しません");
	return NULL;
}


/*									//
//				追加				//
//	(生成されたときに呼び出される)	*/
void Obj3DManagerBase::Add(Obj3D* Obj) {
	if (!Obj) {
		Debug::LogError("オブジェクトの実態がありません");
		return;
	}

	// インスタンス作成
	_3DOBJ* regist = new _3DOBJ;
	regist->pTree = new OBJECT_FOR_TREE<Obj3D*>;

	// オブジェクトを設定
	regist->pObj = Obj;
	regist->pTree->m_pObject = &Obj;

	// 登録
	m_lstObj.push_back(regist);
}


/*									//
//				全削除				//
//									*/
void Obj3DManagerBase::AllClear() {
	list<_3DOBJ*> lstBuff;
	while (m_lstObj.begin() != m_lstObj.end()) {
		list<_3DOBJ*>::iterator it = m_lstObj.begin();
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

	DeleteTree();
	// 8分木の空間を作成
	if (!CreateTree(6, Vector3(-300, -10, -300), Vector3(300, 30, 300))) {
		Debug::LogError("線形8分木空間の初期化に失敗しました");
	}
}


/*									//
//				削除				//
//	(削除されたときに呼び出される)	*/
void Obj3DManagerBase::Delete(Obj3D* Obj) {
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
void Obj3DManagerBase::Update() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 親の確認
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->UpdateAll();
	}
}
void Obj3DManagerBase::LateUpdate() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 親の確認
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->LateUpdateAll();
	}
}


/*									//
//				削除確認			//
//									*/
void Obj3DManagerBase::CheckDestroy() {
	list<_3DOBJ*>	lstobj;
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
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
//			あたり判定				//
//									*/
void Obj3DManagerBase::RegistCollider() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 8分木から解放
		(*it)->pTree->Remove();

		if (!(*it)->pObj->GetActive())	continue;

		(*it)->pTree->m_pObject = &(*it)->pObj;

		if (!(*it)->pObj->GetCollider() ||
			!(*it)->pObj->GetCollider()->GetTreeColl())
			continue;

		RegistTree(*it);
	}
}
bool Obj3DManagerBase::SkipCheck( BYTE A, BYTE B ) {
	// アイテム vs 自機
	unsigned char Player = Archives::Tag("Player");
	unsigned char ItemBox = Archives::Tag("ItemBox");
	unsigned char Enemy = Archives::Tag("Enemy");
	unsigned char PlayerAttack = Archives::Tag("Player/Attack");
	unsigned char EnemyAttack = Archives::Tag("Enemy/Attack");
	unsigned char Allow = Archives::Tag("Allow");
	if ((A == Player && B == ItemBox) ||
		(B == Player && A == ItemBox))
		return true;
	if ((A == Player && B == EnemyAttack) ||
		(B == Player && A == EnemyAttack))
		return true;
	if ((A == PlayerAttack && B == Enemy) ||
		(B == PlayerAttack && A == Enemy))
		return true;
	if ((A == Allow && B == Player) ||
		(B == Allow && A == Player))
		return true;

	return false;
}
void Obj3DManagerBase::CheckCollider() {
	RegistCollider();

	// あたり判定の個数
	int	nColNum = 0;
	// 衝突対象リスト
	std::vector<Obj3D*>	aCol;

	// 衝突対象リスト取得
	nColNum = m_pOcTree->GetAllCollisionList(aCol);

	// 衝突判定
	nColNum /= 2;	// 2で割るのはペアになっているため

	for (int n = 0; n < nColNum; ++n) {
		// 当たるオブジェクト
		Obj3D* A = aCol[n * 2];
		Obj3D* B = aCol[n * 2 + 1];

		// スキップ確認
		if (!SkipCheck(A->GetTag(), B->GetTag()))
			continue;

		Collider* AColl = A->GetCollider();
		Collider* BColl = B->GetCollider();

		// ツリー用あたり判定
		if(!CheckCollision(*AColl->GetTreeColl(), *BColl->GetTreeColl()))
			continue;

		// ここであたり判定
		if(Collision(AColl, BColl)) {
			AColl->Hit(BColl);
			BColl->Hit(AColl);

			// トリガ(どちらかが良ければいいはず)
			if (AColl->CheckCollTrigger(BColl))
				A->OnCollisionTrigger(B);
			if (BColl->CheckCollTrigger(AColl))
				B->OnCollisionTrigger(A);
			// ステイ(どちらかが良ければいいはず)
			if (AColl->CheckCollStay(BColl))	A->OnCollisionStay(B);
			if (BColl->CheckCollStay(AColl))	B->OnCollisionStay(A);
			// リリース(どちらかが良ければいいはず)
			if (AColl->CheckCollRelease(BColl))	A->OnCollisionRelease(B);
			if (BColl->CheckCollRelease(AColl))	B->OnCollisionRelease(A);
		}
	}
}
bool Obj3DManagerBase::Collision(Collider* A, Collider* B) {
	vector<Sphere*>	aSphere1 = A->GetSphere();
	vector<Sphere*>	aSphere2 = B->GetSphere();
	vector<Obb*>	aObb1 = A->GetObb();
	vector<Obb*>	aObb2 = B->GetObb();
	for (auto A = aSphere1.begin(); A != aSphere1.end(); ++A) {
		for (auto B = aSphere2.begin(); B != aSphere2.end(); ++B) {
			if (CheckCollision(*(*A), *(*B)))	return true;
		}
		for (auto B = aObb2.begin(); B != aObb2.end(); ++B) {
			if (CheckCollision(*(*A), *(*B)))	return true;
		}
	}
	for (auto A = aObb1.begin(); A != aObb1.end(); ++A) {
		for (auto B = aSphere2.begin(); B != aSphere2.end(); ++B) {
			if (CheckCollision(*(*A), *(*B)))	return true;
		}
		for (auto B = aObb2.begin(); B != aObb2.end(); ++B) {
			if (CheckCollision(*(*A), *(*B)))	return true;
		}
	}

	return false;
}


/*									//
//				描画				//
//									*/
void Obj3DManagerBase::Draw() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 親の確認
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->DrawAll();
	}
}
void Obj3DManagerBase::LateDraw() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 親の確認
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->LateDrawAll();
	}
}



/*									//
//			オクツリー				//
//									*/
bool Obj3DManagerBase::CreateTree(
	unsigned int Level,
	Vector3& Min,
	Vector3& Max)
{
	DeleteTree();

	m_pOcTree = new CLiner8TreeManager<Obj3D*>;
	return m_pOcTree->Init(Level, Min, Max);
}
void Obj3DManagerBase::DeleteTree() {
	SAFE_DELETE(m_pOcTree);
}
void Obj3DManagerBase::RegistTree(_3DOBJ* obj) {
	// インスタンス取得
	Aabb* aabb = obj->pObj->GetCollider()->GetTreeColl();

	// インスタンス確認
	if (!aabb)	return;

	// 値設定
	Point3 pos = aabb->p;
	Vector3 Min(pos[0] - aabb->r[0], pos[1] - aabb->r[1], pos[2] - aabb->r[2]);
	Vector3 Max(pos[0] + aabb->r[0], pos[1] + aabb->r[1], pos[2] + aabb->r[2]);
	m_pOcTree->Regist(&Min, &Max, obj->pTree);
}