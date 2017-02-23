// リングクラス
// 2016. 1.24	プログラム作成
// Author	:	SyunMizuno

#pragma once


#include	"../GameFramework/Obj/3D/Obj3D.h"


class Ring	:	public	Obj3D {
private:
#pragma region variable

#define		POLYGON_NUM		36	// ポリゴン数

	struct TRing {
		Point3	Pos;
		Point2	Tex;
		float	Alpha;
	};

	TRing	m_tRing[POLYGON_NUM + 1];

	float	m_fScrollSpeed;	// スクロール速度
	float	m_fRadius;		// 半径
	float	m_fWeight;		// 太さ
	LPDIRECT3DTEXTURE9	m_pTex;

#pragma endregion

public:
#pragma region method

	Ring();
	virtual ~Ring();

	// テクスチャ設定
	void SetTex(LPDIRECT3DTEXTURE9 tex) { m_pTex = tex; }

	// 半径設定
	void SetRadius(float r);

	// 太さ設定
	void SetWeight(float w) { m_fWeight = w; }

	// スクロール速度設定
	void SetScrollSpeed(float v);

	// リセット
	void Reset();

#pragma endregion

private:
	virtual void Update();
	virtual void LateDraw();

	void PreSet();
};