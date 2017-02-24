// 描画マネージャー
// 2017. 2. 23	プログラム作成
// author		SyunMizuno


#pragma once

#include	"../Obj/3D/Obj3DManager.h"




class DrawManagerBase {
protected:
	list<_3DOBJ*>*	m_pObjList;

public:
	DrawManagerBase();
	virtual ~DrawManagerBase();

	// 描画
	void Draw();
	void LateDraw();

	// 抽出
	void Extract();

	void SetList(Obj3DManagerBase* mana);

protected:
	// 影作成
	void CreateShadow();

	// 視錐台
	void CheckFrustum();

	// オクルージョン
	void CheckOclusion();

	// フィルタ描画
	void FilterDraw();
};


class DrawManager	:	public	DrawManagerBase,	public	Singleton<DrawManager> {};


inline DrawManager* GetDrawManager(void) {
	return DrawManager::Instance();
}