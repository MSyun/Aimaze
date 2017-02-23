////////////////////////////////////////////////////
// 2015.12.10	�v���O�����쐬
// 2016. 7. 8	�r���{�[�h�@�\�ǉ�
////////////////////////////////////////////////////

#pragma once

#include	<d3dx9.h>
#include	"../../../Obj/3D/Obj3D.h"
#include	"../../Convert.h"


class	Quad : public	Obj3D {
protected:
#pragma region variable

	LPDIRECT3DTEXTURE9		m_pTex;		// �e�N�X�`��
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@
	int			m_nNumVtx;		// �����_��
	int			m_nNumIdx;		// ���C���f�b�N�X��
	int			m_nNumPolygon;	// ���|���S����

	float		m_fSizeX;
	float		m_fSizeZ;

	bool		m_bBillboard;	// �r���{�[�h�t���O

#pragma endregion

public:
#pragma region method

	Quad(LPDIRECT3DTEXTURE9 _Tex = NULL);
	virtual ~Quad() { Release(); }
	virtual HRESULT SetUp(
		float _SizeX, float _SizeZ,		// �T�C�Y
		int _NumX = 1, int _NumZ = 1);	// �u���b�N��
	void Release();
	void SetTex(LPDIRECT3DTEXTURE9 _Tex) { m_pTex = _Tex; }
	void SetColor(Color _color);

	void SetBillFlg(bool flg) { m_bBillboard = flg; }

#pragma endregion

protected:
#pragma region conceal method

	virtual void Update();
	void LateDraw();

	// ���_�쐬
	virtual HRESULT MakeVertex(
		int _NumX, int _NumZ,
		float _SizeX, float _SizeZ);

#pragma endregion
};