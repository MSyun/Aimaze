// �X�v���C�g�N���X
// 2016. 1. 3	�v���O�����쐬
// 2017. 1. 4	Obj���p��
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

	Color				m_Color;	// �F
	LPDIRECT3DTEXTURE9	m_pTex;		// �e�N�X�`��

	Vector2				m_vTexPos;	// �e�N�X�`�����W
	Vector2				m_vTexSize;	// �e�N�X�`���T�C�Y

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
	// �Z�b�^�[
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
	// �Q�b�^�[
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

	// �e�N�X�`���T�C�Y�̎擾
	void SetSizeDefault();

#pragma endregion
};