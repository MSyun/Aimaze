// 2Dオブジェクトマネージャクラス
// 2017. 1.4	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"Sprite.h"
#include	"../../framework/Singleton/Singleton.h"
#include	<list>
#include	"../../framework/Collision/Cell.h"

using namespace std;


typedef struct {
	Sprite*						pObj;
	OBJECT_FOR_TREE<Sprite*>*	pTree;
} _2DOBJ;


class Obj2DManagerBase {
protected:
#pragma region variable

	// 4分木
	CLiner4TreeManager<Sprite*>*	m_pQuadTree;

	// オブジェクト
	list<_2DOBJ*>	m_lstObj;

	bool	m_bHelper;

#pragma endregion

public:
#pragma region method

	Obj2DManagerBase();
	virtual ~Obj2DManagerBase();

	// 検索(名前
	Sprite* Find(const string name);
	// 検索(タグ
	Sprite* FindWithTag(const string tag);

	// 追加
	void Add(Sprite* Obj);

	// 全削除
	void AllClear();

	// 削除
	void Delete(Sprite* Obj);

	//----- 更新
	void Update();
	void LateUpdate();

	void CheckDestroy();

	// あたり判定
	//void CheckCollider();

	//----- 描画
	void Draw();
	void LateDraw();

	void SetHelper(bool helper) { m_bHelper = helper; }

	// クアッドツリー
	//bool CreateTree(unsigned int Level, Vector2& Min, Vector2& Max);

#pragma endregion

private:
#pragma region conceal method

	//----- あたり判定
	//bool Collision();
	//bool SkipCheck(BYTE A, BYTE B);

	//----- クアッドツリー
	//void DeleteTree();
	//void RegistTree(_2DOBJ* obj);

#pragma endregion
};


class	Obj2DManager	:	public	Obj2DManagerBase,	public	Singleton<Obj2DManager> {};

inline Obj2DManager* GetObj2DManager(void) {
	return Obj2DManager::Instance();
}