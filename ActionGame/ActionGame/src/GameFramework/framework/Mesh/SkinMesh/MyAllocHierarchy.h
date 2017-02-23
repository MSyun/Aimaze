// ========================================================================
// Author		:	SyunMizuno
// 2015.10		:	�쐬
// 2016. 1. 6	:	OBB�ǉ�
// 2016. 1.30	:	�R���e�i�̃����o��LPD3DXBUFFER����D3DXBONECOMBINATION��
// 2016. 4. 2	:	OBBTree��r��
// ========================================================================

#pragma once

#include	<d3dx9.h>
#include	"../../Convert.h"


/*
	memo:
		�t���[�����̃I�t�Z�b�g�s��͐�������Ă��邪
		�g�p����Ă��Ȃ�
		�������̖��ʁA���������Ȃ�
*/


/*									//
//			�t���[���\����			//
//									*/
struct MYFRAME : public	D3DXFRAME {
	Matrix	OffsetMat;						// �{�[���I�t�Z�b�g�s��
	DWORD	OffsetID;						// �{�[���I�t�Z�b�g�s��ID
};


/*									//
//			�R���e�i�\����			//
//									*/
struct MYMESHCONTAINER : public	D3DXMESHCONTAINER {
	LPDIRECT3DTEXTURE9*		ppTexture;		// �e�N�X�`��
	DWORD					dwBoneWeight;	// �d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���j
	DWORD					dwBoneNum;		// �{�[���̐�
	LPD3DXBONECOMBINATION	pBoneComb;		// �{�[���R���r�l�[�V�����\���̔z��ւ̃|�C���^
	Matrix*					pMtxBoneOffset;	// �I�t�Z�b�g�s��

	// �R���X�g���N�^
	MYMESHCONTAINER() {
		ppTexture = NULL;
		pBoneComb = NULL;
		pMtxBoneOffset = NULL;
	}
};


/*									//
//		�q�G�����L�[�N���X			//
//									*/
class MyAllocHierarchy : public	ID3DXAllocateHierarchy {
private:
	TCHAR m_szDir[_MAX_PATH];	// X�t�@�C���̂���f�B���N�g��

public:
	MyAllocHierarchy() { m_szDir[0] = 0; }
	~MyAllocHierarchy() {}
	// �t���[���𐶐�����
	STDMETHOD(CreateFrame)(THIS_
		LPCSTR Name,
		LPD3DXFRAME *ppNewFrame
		);

	// �R���e�i�𐶐�����
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer
		);

	// �t���[�����폜����
	STDMETHOD(DestroyFrame)(THIS_
		LPD3DXFRAME pFrameToFree
		);

	// �R���e�i���폜����
	STDMETHOD(DestroyMeshContainer)(THIS_
		LPD3DXMESHCONTAINER pMeshContainerToFree
		);

	// �t���[������
	HRESULT FindSkin(LPD3DXFRAME	_pFrameRoot, LPD3DXFRAME	_pFrame);

	// �f�B���N�g���̐ݒ�
	void SetDirectory(LPCTSTR pszDir);

protected:
	// ������R�s�[
	virtual LPSTR CopyStr(LPCSTR name);

	// ���b�V���f�[�^�o�^
	virtual HRESULT RegistMeshData(const D3DXMESHDATA *pSrc, D3DXMESHDATA *pDest);

	// �}�e���A���o�^
	virtual HRESULT RegistMaterial(const D3DXMATERIAL *pSrc, DWORD num, MYMESHCONTAINER **pDest);

	// �G�t�F�N�g�o�^
	virtual HRESULT RegistEffect(const D3DXEFFECTINSTANCE *pSrc, D3DXEFFECTINSTANCE **ppDest);

	// �אڃ|���S���o�^
	virtual HRESULT RegistAdjacency(const DWORD *pSrc, DWORD polynum, DWORD **Dest);

	// �X�L���o�^
	virtual HRESULT RegistSkin(const LPD3DXSKININFO Src, MYMESHCONTAINER **Dest, LPD3DXMESH Mesh);

	// ���c��ID�Z�b�g
	int FindSkinLeft(LPD3DXFRAME _pFrameRoot, DWORD _nCnt);
	// �t���[��ID�Z�b�g
	int SetFrameId(LPD3DXFRAME _pFrame, LPD3DXMESHCONTAINER _pMeshContainer);

	// Tangent��Binormal��ǉ�
	HRESULT CloneMeshWithTangentAndBinormal(
		IDirect3DDevice9* pDevice,		// �`��f�o�C�X
		ID3DXMesh*	pSrcMesh,			// ���̃��b�V��
		ID3DXMesh**	pOutCloneMesh);	// �N���[�����b�V��
};