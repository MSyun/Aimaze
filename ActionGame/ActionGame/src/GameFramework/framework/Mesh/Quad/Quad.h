////////////////////////////////////////////////////
// 2015.12.10	プログラム作成
// 2016. 7. 8	ビルボード機能追加
////////////////////////////////////////////////////

#pragma once

#include	<d3dx9.h>
#include	"../../../Obj/3D/Obj3D.h"
#include	"../../Convert.h"


class	Quad : public	Obj3D {
protected:
#pragma region variable

	LPDIRECT3DTEXTURE9		m_pTex;		// テクスチャ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// 頂点バッファ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// インデックスバッファ
	int			m_nNumVtx;		// 総頂点数
	int			m_nNumIdx;		// 総インデックス数
	int			m_nNumPolygon;	// 総ポリゴン数

	float		m_fSizeX;
	float		m_fSizeZ;

	bool		m_bBillboard;	// ビルボードフラグ

#pragma endregion

public:
#pragma region method

	Quad(LPDIRECT3DTEXTURE9 _Tex = NULL);
	virtual ~Quad() { Release(); }
	virtual HRESULT SetUp(
		float _SizeX, float _SizeZ,		// サイズ
		int _NumX = 1, int _NumZ = 1);	// ブロック数
	void Release();
	void SetTex(LPDIRECT3DTEXTURE9 _Tex) { m_pTex = _Tex; }
	void SetColor(Color _color);

	void SetBillFlg(bool flg) { m_bBillboard = flg; }

#pragma endregion

protected:
#pragma region conceal method

	virtual void Update();
	void LateDraw();

	// 頂点作成
	virtual HRESULT MakeVertex(
		int _NumX, int _NumZ,
		float _SizeX, float _SizeZ);

#pragma endregion
};