// ========================================================================
// Author		:	SyunMizuno
// 2015.10		:	�쐬
// 2015.11.22	:	����
// 2016. 1. 6	:	OBB�ǉ�
// 2016. 2. 6	:	CreateContainer�����ו������A�e�N�X�`���}�l�[�W���[�Ɠ���
// 2016. 4. 2	:	OBBTree��r��
// ========================================================================

#include	"MyAllocHierarchy.h"
#include	"../../Texture/TexManager.h"
#include	"../../Utility/System/SystemUtility.h"

/*
	memo	:
				�����o��COM�|�C���^�ŊǗ���ڎw��
*/


/*									//
//			������R�s�[			//
//									*/
LPSTR MyAllocHierarchy::CopyStr(LPCSTR name) {
	if (!name) return NULL;
	LPSTR Str = new char[strlen(name) + 1];
	strcpy_s(Str, strlen(name) + 1, name);
	return Str;
}


/*									//
//		���b�V���f�[�^�o�^			//
//									*/
HRESULT MyAllocHierarchy::RegistMeshData(const D3DXMESHDATA *pSrc, D3DXMESHDATA *pDest) {
	pDest->Type = pSrc->Type;

	// ���b�V���f�[�^�ɖ@���ǉ�
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pSrc->pMesh->GetDevice(&pDevice);	//DirectX�f�o�C�X�擾
	if (!(pSrc->pMesh->GetFVF() & D3DFVF_NORMAL)) {
		pDest->Type = D3DXMESHTYPE_MESH;
		if (FAILED(pSrc->pMesh->CloneMeshFVF(
			pSrc->pMesh->GetOptions(),
			pSrc->pMesh->GetFVF() | D3DFVF_NORMAL,
			pDevice,
			&pDest->pMesh))) {
			return E_FAIL;
		}

		// �e���_�̖@���ݒ�
		D3DXComputeNormals(pDest->pMesh, NULL);
	} else
	// �^�C�v�ʊi�[
	// �ʏ탁�b�V��
	if (pSrc->Type == D3DXMESHTYPE_MESH) {
		pDest->pMesh = pSrc->pMesh;
		pDest->pMesh->AddRef();
	} else
	// �v���O���b�V�u���b�V��
	if (pSrc->Type == D3DXMESHTYPE_PMESH) {
		pDest->pPMesh = pSrc->pPMesh;
		pDest->pPMesh->AddRef();
	}

	// �o���v�}�b�v�p����
	CloneMeshWithTangentAndBinormal(pDevice, pDest->pMesh, &pDest->pMesh);

	SAFE_RELEASE(pDevice);

	return S_OK;
}


/*									//
//			�}�e���A���o�^			//
//									*/
HRESULT MyAllocHierarchy::RegistMaterial(const D3DXMATERIAL *pSrc, DWORD num, MYMESHCONTAINER **pDest) {
	// �}�e���A���f�[�^�i�[�̈�𓮓I�m��
	(*pDest)->pMaterials = new D3DXMATERIAL[num];
	if ((*pDest)->pMaterials == NULL) return E_OUTOFMEMORY;
	memcpy((*pDest)->pMaterials, pSrc, sizeof(D3DXMATERIAL) * num);

	// �e�N�X�`���f�[�^�i�[�̈�𓮓I�m��
	(*pDest)->ppTexture = new LPDIRECT3DTEXTURE9[num];
	if ((*pDest)->ppTexture == NULL) return E_OUTOFMEMORY;
	memset((*pDest)->ppTexture, 0, sizeof(LPDIRECT3DTEXTURE9) * num);

	if (num > 0) {
		// �J�����g�f�B���N�g����ύX
		TCHAR szCurrentDir[_MAX_PATH];
		if (m_szDir[0]) {
			GetCurrentDirectory(_MAX_PATH, szCurrentDir);
			SetCurrentDirectory(m_szDir);
		}

		for (DWORD i = 0; i < num; i++) {
			// �}�e���A�����
			(*pDest)->pMaterials[i].MatD3D = pSrc[i].MatD3D;
			(*pDest)->pMaterials[i].MatD3D.Ambient = pSrc[i].MatD3D.Diffuse;
			// �e�N�X�`���t�@�C����
			(*pDest)->pMaterials[i].pTextureFilename = CopyStr(pSrc[i].pTextureFilename);
			// �e�N�X�`�������m�F
			if ((*pDest)->pMaterials[i].pTextureFilename != NULL) {
				// �e�N�X�`���ǂݍ���
				(*pDest)->ppTexture[i] = GetTexManager()->Get((*pDest)->pMaterials[i].pTextureFilename);
			}
		}

		// �J�����g�f�B���N�g�������ɖ߂�
		if (m_szDir[0])
			SetCurrentDirectory(szCurrentDir);
	} else {
		//- �}�e���A���Ȃ��̏ꍇ -//
		(*pDest)->pMaterials[0].pTextureFilename = NULL;
		memset(&(*pDest)->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		(*pDest)->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		(*pDest)->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		(*pDest)->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		(*pDest)->pMaterials[0].MatD3D.Specular = (*pDest)->pMaterials[0].MatD3D.Diffuse;
	}

	return S_OK;
}


/*									//
//			�G�t�F�N�g�o�^			//
//									*/
HRESULT MyAllocHierarchy::RegistEffect(const D3DXEFFECTINSTANCE *pSrc, D3DXEFFECTINSTANCE **ppDest) {
	// �i�[��z��̓��I�m��
	*ppDest = new D3DXEFFECTINSTANCE;
	if (*ppDest == NULL) return E_OUTOFMEMORY;

	// �G�t�F�N�g�t�@�C����
	(*ppDest)->pEffectFilename = CopyStr(pSrc->pEffectFilename);

	// �f�t�H���g��
	(*ppDest)->NumDefaults = pSrc->NumDefaults;

	// D3DXEFFECTDEFAULT�̓��I�m��
	(*ppDest)->pDefaults = new D3DXEFFECTDEFAULT[pSrc->NumDefaults];
	if ((*ppDest)->pDefaults == NULL) return E_OUTOFMEMORY;

	// �ꎞ�I�ɕϐ��ύX
	D3DXEFFECTDEFAULT *pEDSrc = pSrc->pDefaults;		// �R�s�[��
	D3DXEFFECTDEFAULT *pEDDst = (*ppDest)->pDefaults;	// �R�s�[��

	for (DWORD i = 0; i < pSrc->NumDefaults; i++) {
		pEDDst[i].pParamName = CopyStr(pEDSrc[i].pParamName);		// �p�����[�^��
		DWORD NumBytes = pEDDst[i].NumBytes = pEDSrc[i].NumBytes;	// �p�����[�^�T�C�Y
		pEDDst[i].Type = pEDSrc[i].Type;							// �p�����[�^�^�C�v
		if (pEDSrc[i].Type <= D3DXEDT_DWORD) {
			pEDDst[i].pValue = new DWORD[NumBytes];
			if (pEDDst[i].pValue == NULL) return E_OUTOFMEMORY;
			memcpy(pEDDst[i].pValue, pEDSrc[i].pValue, NumBytes);
		}
	}
	return S_OK;
}


/*									//
//		�אڃ|���S���o�^			//
//									*/
HRESULT MyAllocHierarchy::RegistAdjacency(const DWORD *pSrc, DWORD polynum, DWORD **Dest) {
	// �ʂ��ƂɎ���3�̗אڐ���񂪊i�[���ꂽDWORD�^�̃A�h���X�i�[�p(�|�C���^)�ϐ�
	*Dest = new DWORD[polynum * 3];
	if (*Dest == NULL) return E_OUTOFMEMORY;
	memcpy(*Dest, pSrc, sizeof(DWORD) * polynum * 3);
	return S_OK;
}


/*									//
//			�X�L���o�^				//
//									*/
HRESULT MyAllocHierarchy::RegistSkin(const LPD3DXSKININFO Src, MYMESHCONTAINER **Dest, LPD3DXMESH Mesh) {
	// �X�L�����b�V���L��
	(*Dest)->pSkinInfo = Src;
	(*Dest)->pSkinInfo->AddRef();

	// �{�[���̐����擾
	DWORD dwBoneNum = Src->GetNumBones();

	// �{�[���I�t�Z�b�g�s��
	(*Dest)->pMtxBoneOffset = new D3DXMATRIX[dwBoneNum];
	if ((*Dest)->pMtxBoneOffset == NULL) return E_OUTOFMEMORY;
	for (DWORD i = 0; i < dwBoneNum; i++) {
		//�p�t���[��(�{�[��)�̃I�t�Z�b�g�s����擾���Ċi�[
		memcpy(&(*Dest)->pMtxBoneOffset[i], (*Dest)->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
	}

	//- �Œ�p�C�v���C���`��p�ɕϊ� -//
	// �V�F�[�_�ŕ`�悷��ꍇ�͕ʓr�ϊ����K�v

	// ���_�P�ʂł̃u�����h�̏d�݂ƃ{�[���̑g�ݍ��킹�e�[�u����K�������V�������b�V����Ԃ��B
	// �܂�͍œK����A����
	LPD3DXBUFFER	bf;
	LPD3DXBUFFER	pBoneBuffer;
	if (FAILED((*Dest)->pSkinInfo->ConvertToBlendedMesh(
		Mesh,						//���̃��b�V���f�[�^�A�h���X
		NULL,						//�I�v�V����(���݂͎g���Ă��Ȃ�����NULL�ł���)
		(*Dest)->pAdjacency,		//���̃��b�V���̗אڐ����
		NULL,						//�o�̓��b�V���̗אڐ����
		NULL,						//�e�ʂ̐V�����C���f�b�N�X�l�i�[�p�ϐ��̃A�h���X
		&bf,						//�p���_�̐V�����C���f�b�N�X�l�i�[�p�ϐ��̃A�h���X
		&(*Dest)->dwBoneWeight,		//�{�[���̉e���̈�ʓ�����̍ő吔�i�[�p�ϐ��̃A�h���X
		&(*Dest)->dwBoneNum,		//�{�[���̑g�ݍ��킹�e�[�u���Ɋ܂܂��{�[�����i�[�p�ϐ��̃A�h���X
		&pBoneBuffer,				//�{�[���̑g�ݍ��킹�e�[�u���ւ̃|�C���^
		&(*Dest)->MeshData.pMesh	//�o�͂���郁�b�V���A�h���X�i�[�p�ϐ��̃A�h���X(�Œ�p�C�v���C���p)
	))) {
		return E_FAIL;
	}
	// ���ԕύX��̒��_�ɍ��킹�ă{�[���̉e�������X�V
	DWORD *d = (DWORD*)bf->GetBufferPointer();
	(*Dest)->pSkinInfo->Remap((*Dest)->MeshData.pMesh->GetNumVertices(), d);
	bf->Release();

	// �{�[���e�[�u������o�b�t�@�̐擪�A�h���X���擾
	(*Dest)->pBoneComb = (LPD3DXBONECOMBINATION)pBoneBuffer->GetBufferPointer();

	return S_OK;
}


/*									//
//			�t���[�������֐�		//
//									*/
HRESULT MyAllocHierarchy::CreateFrame(THIS_
	LPCSTR Name,				// ��������t���[����
	LPD3DXFRAME *ppNewFrame)	// ���������t���[���I�u�W�F�N�g��Ԃ�
{
	//----- �I�u�W�F�N�g�̐���
	MYFRAME*	pFrame = new MYFRAME;
	if (pFrame == NULL) return E_OUTOFMEMORY;	// �̈�m�ۂ̎��s���̏���
	ZeroMemory(pFrame, sizeof(MYFRAME));		// ������

	//----- ���O
	pFrame->Name = CopyStr(Name);

	// �s��̏�����
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);

	// �ǉ��F�I�t�Z�b�g�֌W������
	pFrame->OffsetID = 0xffffff;
	D3DXMatrixIdentity(&(pFrame->OffsetMat));

	// �쐬�����t���[���̃A�h���X���i�[
	*ppNewFrame = pFrame;

	return S_OK;
}


/*									//
// X�t�@�C���̂���f�B���N�g����ݒ�//
//									*/
void MyAllocHierarchy::SetDirectory(LPCTSTR pszDir) {
	if (pszDir)
		lstrcpy(m_szDir, pszDir);
	else
		m_szDir[0] = 0;
}


/*									//
//		���b�V���R���e�i����		//
//									*/
HRESULT MyAllocHierarchy::CreateMeshContainer(THIS_
	LPCSTR Name,								// �t���[����
	CONST D3DXMESHDATA *pMeshData,				// ���b�V���f�[�^
	CONST D3DXMATERIAL *pMaterials,				// �}�e���A���f�[�^
	CONST D3DXEFFECTINSTANCE *pEffectInstances,	// �G�t�F�N�g�C���X�^���X�f�[�^
	DWORD NumMaterials,							// �}�e���A���z��
	CONST DWORD *pAdjacency,					// ���b�V���̗אڐ��f�[�^
	LPD3DXSKININFO pSkinInfo,					// �X�L�����b�V���ւ̃|�C���^
	LPD3DXMESHCONTAINER *ppNewMeshContainer)	// �����������b�V���R���e�i��Ԃ�
{
	// ���b�V���R���e�i�̐���
	MYMESHCONTAINER *pMeshContainer = new MYMESHCONTAINER;
	if (pMeshContainer == NULL)	return E_OUTOFMEMORY;
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));


	//===============================================
	//----- Name
	pMeshContainer->Name = CopyStr(Name);


	//===============================================
	//----- pMeshData
	// �^�C�v�i�[
	if (S_OK != RegistMeshData(pMeshData, &pMeshContainer->MeshData))
		return E_FAIL;


	//===============================================
	//----- pMaterials
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	if (S_OK != RegistMaterial(pMaterials, NumMaterials, &pMeshContainer))
		return E_OUTOFMEMORY;


	//===============================================
	//----- pEffectInstances
	if (S_OK != RegistEffect(pEffectInstances, &pMeshContainer->pEffects))
		return E_OUTOFMEMORY;


	//===============================================
	//----- pAdjacency
	if (S_OK != RegistAdjacency(pAdjacency, pMeshData->pMesh->GetNumFaces(), &pMeshContainer->pAdjacency))
		return E_OUTOFMEMORY;


	//===============================================
	//----- pSkinInfo
	if (pSkinInfo != NULL) {
		if (S_OK != RegistSkin(pSkinInfo, &pMeshContainer, pMeshData->pMesh))
			return E_OUTOFMEMORY;
	} else {
		// �X�L�����b�V������
		pMeshContainer->pSkinInfo = NULL;
	}

	*ppNewMeshContainer = pMeshContainer;

	return S_OK;
}


/*									//
//			�t���[�����폜			//
//									*/
HRESULT MyAllocHierarchy::DestroyFrame(THIS_
	LPD3DXFRAME pFrameToFree)
{
	// ���O�̍폜
	SAFE_DELETE_ARRAY(pFrameToFree->Name);

	// �R���e�i�̍폜
	if (pFrameToFree->pMeshContainer != NULL)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	// �Z��t���[��
	if (pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	// �q�t���[��
	if (pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	SAFE_DELETE(pFrameToFree);

	return S_OK;
}


/*									//
//			�R���e�i���폜			//
//									*/
HRESULT MyAllocHierarchy::DestroyMeshContainer(THIS_
	LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerToFree;


	//===============================================
	//----- pSkinInfo
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->pMtxBoneOffset);


	//===============================================
	//----- pAdjacency
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);


	//===============================================
	//----- pEffect
	for (DWORD i = 0; i < pMeshContainer->pEffects->NumDefaults; i++) {
		SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pDefaults[i].pParamName);
		SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pDefaults[i].pValue);
	}
	SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pDefaults);
	SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pEffectFilename);
	SAFE_DELETE_ARRAY(pMeshContainer->pEffects);


	//===============================================
	//----- pMaterials
	for (DWORD i = 0; i < pMeshContainer->NumMaterials; i++) {
		if (!pMeshContainer->pMaterials[i].pTextureFilename)	continue;
		GetTexManager()->Delete(pMeshContainer->pMaterials[i].pTextureFilename);
		SAFE_DELETE_ARRAY(pMeshContainer->pMaterials[i].pTextureFilename);
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTexture);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);


	//===============================================
	//----- pMeshData�iunion�Ȃ̂�1�ł��������j
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->MeshData.pPMesh);
	SAFE_RELEASE(pMeshContainer->MeshData.pPatchMesh);


	//===============================================
	//----- Name
	SAFE_DELETE_ARRAY(pMeshContainer->Name);

	SAFE_DELETE(pMeshContainer);
	pMeshContainerToFree = NULL;

	return S_OK;
}


/*									//
//			�t���[������			//
//									*/
HRESULT MyAllocHierarchy::FindSkin(LPD3DXFRAME	_pFrameRoot, LPD3DXFRAME	_pFrame) {
	if (_pFrame->pMeshContainer != NULL) {
		// �I�t�Z�b�g�s��ݒ�
		if (_pFrame->pMeshContainer->pSkinInfo != NULL) {
			// �t���[��ID�̐ݒ�
			DWORD nCnt = SetFrameId(_pFrameRoot, _pFrame->pMeshContainer);
			if (nCnt == -1) {
				MYMESHCONTAINER*	pMeshContainer = (MYMESHCONTAINER*)_pFrame->pMeshContainer;
				nCnt = pMeshContainer->pSkinInfo->GetNumBones();
			}
			// �Z�b�g����ĂȂ�ID�B
			FindSkinLeft(_pFrameRoot, nCnt);

			return S_OK;
		}

		FindSkinLeft(_pFrameRoot, 0);
	}

	// �Z��t���[������
	if (_pFrame->pFrameSibling != NULL) {
		FindSkin(_pFrameRoot, _pFrame->pFrameSibling);
	}

	// �q�t���[������
	if (_pFrame->pFrameFirstChild != NULL) {
		FindSkin(_pFrameRoot, _pFrame->pFrameFirstChild);
	}
	return S_OK;
}


/*									//
//			���c��ID�Z�b�g			//
//									*/
int MyAllocHierarchy::FindSkinLeft(LPD3DXFRAME	_pFrameRoot, DWORD _nCnt) {
	MYFRAME*	pFrame = (MYFRAME*)_pFrameRoot;
	// ���c�肳��
	if (pFrame->OffsetID == 0xffffff) {
		pFrame->OffsetID = _nCnt++;
	}

	// �Z��t���[������
	if (pFrame->pFrameSibling != NULL) {
		_nCnt = FindSkinLeft(pFrame->pFrameSibling, _nCnt);
	}

	// �q�t���[������
	if (pFrame->pFrameFirstChild != NULL) {
		_nCnt = FindSkinLeft(pFrame->pFrameFirstChild, _nCnt);
	}
	return _nCnt;
}


/*									//
//		�t���[��ID�Z�b�g			//
//									*/
int MyAllocHierarchy::SetFrameId(LPD3DXFRAME _pFrame, LPD3DXMESHCONTAINER _pMeshContainer) {
	// �L���X�g�`
	MYMESHCONTAINER*	pMeshContainer = (MYMESHCONTAINER*)_pMeshContainer;

	DWORD dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();

	// �t���[������
	for (DWORD _Cnt = 0; _Cnt < dwBoneNum; _Cnt++) {
		MYFRAME* pFrame = (MYFRAME*)D3DXFrameFind(_pFrame, pMeshContainer->pSkinInfo->GetBoneName(_Cnt));

		// �I��
		if (pFrame == NULL)
			return _Cnt;

		pFrame->OffsetID = _Cnt;
		memcpy(&pFrame->OffsetMat, pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(_Cnt), sizeof(D3DXMATRIX));
	}

	return -1;
}

/*									//
//		Tangent��Binormal��ǉ�		//
//									*/
HRESULT MyAllocHierarchy::CloneMeshWithTangentAndBinormal(
	IDirect3DDevice9* pDevice,	// �`��f�o�C�X
	ID3DXMesh*	pSrcMesh,		// ���̃��b�V��
	ID3DXMesh**	pOutCloneMesh)	// �N���[�����b�V��
{
	// ���_�錾
	const D3DVERTEXELEMENT9 vertexDecl[] =
	{
		{ 0,   0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0,  12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0,  24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0,  32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0,  44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		D3DDECL_END()
	};

	LPD3DXMESH pTempMesh = NULL;

	// �N���[�����b�V������
	if (FAILED(pSrcMesh->CloneMesh(pSrcMesh->GetOptions(),
		vertexDecl,
		pDevice,
		pOutCloneMesh))) {
		return E_FAIL;
	}

	return D3D_OK;
}