//////////////////////////////////////////////////////////////
// 3Dオブジェクトマネージャクラス
// 2016. 8.19	プログラム作成
// Author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"Obj3D.h"
#include	"../../framework/Singleton/Singleton.h"
#include	<list>
#include	"../../framework/Collision/Cell.h"

using namespace std;


// あたり判定と完全分離できる可能性のため構造体を使用
typedef struct {
	Obj3D*						pObj;
	OBJECT_FOR_TREE<Obj3D*>*	pTree;
} _3DOBJ;


class	Obj3DManagerBase {
protected:
#pragma region variable

	// 8分木
	CLiner8TreeManager<Obj3D*>*	m_pOcTree;

	// オブジェクト
	list<_3DOBJ*>	m_lstObj;

	bool	m_bHelper;

#pragma endregion

public:
#pragma region method

	Obj3DManagerBase();
	virtual ~Obj3DManagerBase();

	// 検索(名前
	Obj3D* Find(const string name);
	// 検索(タグ
	Obj3D* FindWithTag(const string tag);

	// 追加
	void Add(Obj3D* Obj);

	// 全削除
	void AllClear();

	// 削除
	void Delete(Obj3D* Obj);

	//----- 更新
	void Update();
	void LateUpdate();

	void CheckDestroy();

	// あたり判定
	void CheckCollider();

	void SetHelper(bool helper) { m_bHelper = helper; }

	// オクツリー
	bool CreateTree(unsigned int Level, Vector3& Min, Vector3& Max);

	// リスト
	list<_3DOBJ*>* GetList() { return &m_lstObj; }

#pragma endregion

protected:
#pragma region conceal method

	//----- あたり判定
	void RegistCollider();
	bool Collision(Collider* A, Collider* B);
	bool SkipCheck(BYTE A, BYTE B);

	//----- オクツリー
	void DeleteTree();
	void RegistTree(_3DOBJ* obj);

#pragma endregion
};


class	Obj3DManager : public	Obj3DManagerBase, public	Singleton<Obj3DManager> {};

inline Obj3DManager* GetObj3DManager(void) {
	return Obj3DManager::Instance();
}