// アイテムの箱クラス
// 2016. 8. 4	プログラム作成
// Author	:	SyunMizuno

#pragma once


#include	"../../GameFramework/Obj/3D/Obj3D.h"
#include	"../Ring/Ring.h"



class ItemBox	:	public	Obj3D {
private:
	Ring*	m_pRing;	// リング
	Obj3D*	m_pBox;		// 箱
	Obj3D*	m_pItem;	// 中身

public:
	ItemBox();
	virtual ~ItemBox();

	Obj3D* GetItem() { return m_pItem; }
	void SetItem(Obj3D* item);

	// アニメーション
	void Begin(LPDIRECT3DTEXTURE9 tex);

	// 中身を渡す
	Obj3D* Transfer();

private:
	virtual void Update();
};