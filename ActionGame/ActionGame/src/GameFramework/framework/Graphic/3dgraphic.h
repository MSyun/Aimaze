#pragma once

#include	"../Convert.h"


//=================================================================
// ���C�u�����̃����N
//=================================================================
#if 0	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̃t�@�C��"�ɑΏۃ��C�u������ݒ肷�� ]
#pragma comment ( lib, "d3d9.lib" )		// �`�揈���ɕK�v
#pragma comment ( lib, "d3dx9.lib" )	// [d3d9.lib]�̊g�����C�u����
#pragma comment ( lib, "dxguid.lib" )	// DirectX�R���|�[�l���g�g�p�ɕK�v
#endif


//*****************************************************************
// �}�N����`
//*****************************************************************
#define	NUM_VERTEX			( 4 )				// ���_��
#define	NUM_POLYGON			( 2 )				// �|���S����


// ���_�t�H�[�}�b�g�i���_���W[2D] / ���_�J���[�j
#define	FVF_VERTEX_2D		( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define	FVF_VERTEX_3D		( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

// ��L���_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct {
	D3DXVECTOR3	pos;	// ���_���W
	float		rhw;	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2	tex;	// �e�N�X�`�����W
} VERTEX_2D;

typedef struct {
	D3DXVECTOR3	vtx;
	D3DXVECTOR3	normal;
	D3DCOLOR	diffuse;
	D3DXVECTOR2	tex;
} VERTEX_3D;