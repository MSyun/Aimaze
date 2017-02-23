// スプライトクラス
// 2016. 1. 3	プログラム作成
// 2017. 1. 4	Objを継承
// author		SyunMizuno


#pragma once

#include	"../../framework/Graphic/3dgraphic.h"
#include	"../../framework/Convert.h"
#include	"../../Obj/Obj.h"
#include	"../../RectTransform/RectTransform.h"


class Sprite	:	public	IObj {
private:
#pragma region variable
	RectTransform*		m_pRectTransform;

	Color				m_Color;	// 色
	LPDIRECT3DTEXTURE9	m_pTex;		// テクスチャ

	Vector2				m_vTexPos;	// テクスチャ座標
	Vector2				m_vTexSize;	// テクスチャサイズ

protected:
	VERTEX_2D			m_aVtx[NUM_VERTEX];

#pragma endregion

public:
#pragma region method

	Sprite();
	virtual ~Sprite();

	virtual void UpdateAll() {
		IObj::UpdateAll();
		if (GetOperate())	Input();
		Update();
	}
	virtual void LateUpdateAll() { LateUpdate(); }
	virtual void DrawAll() { Draw(); }
	virtual void LateDrawAll() { LateDraw(); }

#pragma region Setter
	// セッター
	virtual void SetColor( const Color& _color )				{ m_Color	= _color; }
	virtual void SetColor( float x, float y, float z, float a )	{ m_Color	= Color(x,y,z,a); }
	virtual void SetColor( float val )							{ m_Color	= Color( val, val, val, val ); }
	virtual void SetTexture( const LPDIRECT3DTEXTURE9 _tex )	{ m_pTex	= _tex; this->SetSizeDefault(); }
	virtual void SetTexPos(float x, float y) { m_vTexPos.x = x; m_vTexPos.y = y; }
	virtual void SetTexPos(const Vector2& pos) { m_vTexPos = pos; }
	virtual void SetTexSize(float x, float y) { m_vTexSize.x = x; m_vTexSize.y = y; }
	virtual void SetTexSize(const Vector2& pos) { m_vTexSize = pos; }
#pragma endregion

#pragma region Getter
	// ゲッター
	virtual RectTransform* GetRectTransform() { return m_pRectTransform; }
	virtual Color	GetColor()				{ return m_Color; }
	virtual IDirect3DTexture9* GetTexture()	{return m_pTex;}
#pragma endregion

#pragma endregion

protected:
#pragma region conceal method

	virtual void Update();
	virtual void Draw();

	void MakeVtx();
	virtual void SetVtx();
	virtual void SetTex();
	void SetColor();

	// テクスチャサイズの取得
	void SetSizeDefault();

#pragma endregion
};