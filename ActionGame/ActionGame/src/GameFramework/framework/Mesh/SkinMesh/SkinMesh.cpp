// ========================================================================
// Author		:	SyunMizuno
// 2015.10		:	�쐬
// 2015.11.22	:	���������[�N�폜
// 2015.11.28	:	Manager�֐��ǉ�
// 2016. 2. 6	:	�X�L�����b�V���}�l�[�W���[�Ƃ̓���
// 2016. 5.10	:	���C�g�ƃ}�e���A���̐F������
// ========================================================================

#include	"SkinMesh.h"

// �V�F�[�_�p
#include	"../../Light/LightManager.h"
#include	"../../Camera/CameraManager.h"
#include	<tchar.h>
#include	"../../Debug/Debug.h"
#include	"../../Utility/System/SystemUtility.h"
#include	"../../Graphic/Graphics.h"

/*
	MEMO_SPACE:
		DrawMeshContainer���ōs��v�Z���s���Ă���̂ŗv�C��

		Lambert��Phong�ł܂��Ȃ���̂ŕK�v�Ȃ�����

		�ŏI�I�Ƀ��C�g�͂ق��œo�^����͂�
*/


//*									//
//			�R���X�g���N�^			//
//									*/
SkinMesh::SkinMesh() :
	m_bLoad(false)
{
	m_aFrame.clear();
	m_aInContainer.clear();
	m_aTransform.clear();
}


/*									//
//			�f�X�g���N�^			//
//									*/
SkinMesh::~SkinMesh() {
	Release();
}


/*									//
//				���				//
//									*/
void SkinMesh::Release() {
	if (m_bLoad) {
		MyAllocHierarchy Hierarchy;
		if(m_aFrame.size() > 0)
			Hierarchy.DestroyFrame(m_aFrame[0]);

		m_bLoad = false;
	}
	for (auto it = m_aTransform.begin(); it != m_aTransform.end(); ++it) {
		SAFE_DELETE(*it);
	}
}

#pragma region Load
/*									//
//		�X�L�����b�V���ǂݍ���		//
//									*/
HRESULT SkinMesh::Load(const string filename) {
	HRESULT hr;
	D3DXFRAME* pFrameRoot;
	MyAllocHierarchy	Hierarchy;
	LPD3DXANIMATIONCONTROLLER	Controller = NULL;

	Release();

	// �f�B���N�g���𒊏o
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(filename.c_str(), szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	Hierarchy.SetDirectory(szDir);

	// X�t�@�C������A�j���[�V�������b�V����ǂݍ��ݍ쐬����
	hr = D3DXLoadMeshHierarchyFromX(
		filename.c_str(),
		D3DXMESH_MANAGED,
		GetGraphics()->GetDevice(),
		&Hierarchy,
		NULL,
		(D3DXFRAME**)&pFrameRoot,
		&Controller);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("�X�L�����b�V���̓ǂݍ��݂Ɏ��s���܂���"), filename.c_str(), MB_OK);
		return hr;
	}

	// �t���[�����̐���
	Hierarchy.FindSkin(pFrameRoot, pFrameRoot);

	CreateFrameArray(pFrameRoot);

	// �R���g���[���[�̃R�s�[
	m_AnimCont.SetAnimCont(Controller);
	Controller->Release();

	m_bLoad = true;

	return hr;
}

/*									//
//		�}�l�[�W���[���ǂݍ���	//
//									*/
void SkinMesh::ManagerGet(_tSkin* pSkin) {
	if (!pSkin) {
		Debug::LogError("�n���ꂽ���b�V��������܂���");
		return;
	}

	CreateFrameArray(pSkin->pFrameRoot);
	if(pSkin->pController)
		m_AnimCont.SetAnimCont(pSkin->pController);
}
#pragma endregion

/*									//
//				�X�V				//
//									*/
void SkinMesh::Update(Matrix& world) {
	if (m_aFrame.empty())	return;

	UpdateFrameMatrices(m_aFrame[0], &world);

	for (auto it = m_aTransform.begin(); it != m_aTransform.end(); ++it) {
		vector<Transform*>* child = (*it)->GetChilds();
		for (auto itr = child->begin(); itr != child->end(); ++itr) {
			(*itr)->GetGameObject()->UpdateAll();
		}
	}
}
void SkinMesh::LateUpdate() {
	for (auto it = m_aTransform.begin(); it != m_aTransform.end(); ++it) {
		vector<Transform*>* child = (*it)->GetChilds();
		for (auto itr = child->begin(); itr != child->end(); ++itr) {
			(*itr)->GetGameObject()->LateUpdateAll();
		}
	}
}


/*									//
//			�����s��X�V			//
//									*/
void SkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, Matrix* pParentMatrix) {
	MYFRAME*	pFrame = (MYFRAME*)pFrameBase;
	if (pParentMatrix) {
		// CombinedTransformationMatrix�ɍŏI�s����i�[
		Matrix world;
		D3DXMatrixMultiply(&world, &pFrame->TransformationMatrix, pParentMatrix);
		m_aTransform[pFrame->OffsetID]->SetWorld(world);
	} else {
		// CombinedTransformationMatrix�ɍŏI�s����i�[
		m_aTransform[pFrame->OffsetID]->SetWorld(pFrame->TransformationMatrix);
	}

	// �Z��t���[��
	if (pFrame->pFrameSibling)
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);

	// �q�t���[��
	if (pFrame->pFrameFirstChild)
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &m_aTransform[pFrame->OffsetID]->GetWorld());
}

#pragma region Draw
/*									//
//				�`��				//
//									*/
void SkinMesh::Draw(_shadertype type) {
	m_eAlpha = _eAlphaCheck::CHECK_NOALPHA;
	DrawFrame(type);
	for (auto it = m_aTransform.begin(); it != m_aTransform.end(); ++it) {
		vector<Transform*>* child = (*it)->GetChilds();
		for (auto itr = child->begin(); itr != child->end(); ++itr) {
			(*itr)->GetGameObject()->DrawAll();
		}
	}
}
void SkinMesh::LateDraw(_shadertype type) {
	m_eAlpha = _eAlphaCheck::CHECK_ALPHA;
	DrawFrame(type);
	for (auto it = m_aTransform.begin(); it != m_aTransform.end(); ++it) {
		vector<Transform*>* child = (*it)->GetChilds();
		for (auto itr = child->begin(); itr != child->end(); ++itr) {
			(*itr)->GetGameObject()->LateDrawAll();
		}
	}
}
void SkinMesh::DrawShadow() {
	for (auto it = m_aInContainer.begin(); it != m_aInContainer.end(); ++it) {
		MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)(*it)->pMeshContainer;
		while (pMeshContainer) {
			DrawShadowContainer(pMeshContainer, (*it));

			pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
		}
	}
}


/*									//
//			�t���[���̕`��			//
//									*/
void SkinMesh::DrawFrame(_shadertype type) {
	for (auto it = m_aInContainer.begin(); it != m_aInContainer.end(); ++it) {
		MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)(*it)->pMeshContainer;
		while (pMeshContainer) {
			//----- �`��^�C�v�̑I��
			if (type == SHADER_NONE)
				DrawMeshContainer(pMeshContainer, (*it));
			else
				DrawShaderContainer(pMeshContainer, (*it), type);

			pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
		}
	}
}


/*									//
//			���Z�����`�F�b�N		//
//									*/
bool SkinMesh::CheckAlpha(float alpha) {
	//----- ���Z����
	switch (m_eAlpha) {
	case CHECK_ALL:
		return true;

	case CHECK_ALPHA:
		if (alpha < 1.0f)
			return true;
		break;

	case CHECK_NOALPHA:
		if (alpha >= 1.0f)
			return true;
		break;
	};

	return false;
}

#pragma region DrawContainer
/*									//
//			�R���e�i���`��			//
//									*/
void SkinMesh::DrawMeshContainer(
	MYMESHCONTAINER* pMeshContainer,
	MYFRAME* pFrame)
{
	// �X�L�����b�V��
	if (pMeshContainer->pSkinInfo != NULL) {
		// �X�L�j���O�v�Z
		for (DWORD i = 0; i < pMeshContainer->dwBoneNum; ++i) {
			DWORD AttribID = pMeshContainer->pBoneComb[i].AttribId;
			//----- �����`�F�b�N
			if (!this->CheckAlpha(pMeshContainer->pMaterials[AttribID].MatD3D.Diffuse.a)) {
				continue;
			}

			DWORD dwBlendMatrixNum = 0;
			// �e�����Ă���s�񐔎擾
			for (DWORD k = 0; k < pMeshContainer->dwBoneWeight; ++k) {
				UINT mtxIndex = pMeshContainer->pBoneComb[i].BoneId[k];
				if (mtxIndex != UINT_MAX) {
					// mStack�ɃI�t�Z�b�g�s��*�{�[�������s����i�[
					Matrix mStack = pMeshContainer->pMtxBoneOffset[mtxIndex] * m_aTransform[mtxIndex]->GetWorld();
					// �s��X�^�b�N�Ɋi�[
					GetGraphics()->GetDevice()->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
					// ���݉e�����󂯂Ă���{�[���̐�
					++dwBlendMatrixNum;
				}
			}

			// �W�I���g���u�����f�B���O���g�p���邽�߂ɍs��̌����w��
			GetGraphics()->GetDevice()->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixNum - 1);

			// ���b�V���̕`��
			GetGraphics()->GetDevice()->SetMaterial(&pMeshContainer->pMaterials[AttribID].MatD3D);
			GetGraphics()->GetDevice()->SetTexture(0, pMeshContainer->ppTexture[AttribID]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	} else {
		// �ʏ탁�b�V��
		GetGraphics()->GetDevice()->SetTransform(D3DTS_WORLD, &m_aTransform[pFrame->OffsetID]->GetWorld());
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; ++i) {
			//----- �����`�F�b�N
			if (!this->CheckAlpha(pMeshContainer->pMaterials[i].MatD3D.Diffuse.a)) {
				continue;
			}
			GetGraphics()->GetDevice()->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);	// �}�e���A���Z�b�g
			GetGraphics()->GetDevice()->SetTexture(0, pMeshContainer->ppTexture[i]);			// �e�N�X�`���Z�b�g
			pMeshContainer->MeshData.pMesh->DrawSubset(i);					// �`��
		}
	}
}


/*									//
//		�R���e�i���`��i�V�F�[�_	//
//									*/
void SkinMesh::DrawShaderContainer(
	MYMESHCONTAINER* pMeshContainer,
	MYFRAME* pFrame,
	_shadertype type)
{
	//----- �V�F�[�_
	IHlslDrawBase* pShader = (IHlslDrawBase*)GetShaderManager()->Get(type);
	pShader->Begin();

	//////////////////////////////////////
	// �����͌�ŏC��
	Light* light = GetLightManager()->Get("Directional");
	pShader->SetLight(&light->GetDirection4());
	//pShader->SetLightView(light->GetView());
	//pShader->SetLightProj(light->GetProj());
	//ShadowMap* shadow = (ShadowMap*)GetShaderManager()->Get(SHADER_SHADOW);
	//pShader->SetShadowMap(shadow->GetShadowTex());
	//////////////////////////////////////

	pShader->SetCamera(&GetCameraManager()->CurrentCamera()->GetTransform()->GetPos());
	pShader->SetSpecular(light->GetSpecular());

	// �X�L�����b�V��
	if (pMeshContainer->pSkinInfo != NULL) {
		// �X�L�j���O�v�Z
		for (DWORD i = 0; i < pMeshContainer->dwBoneNum; ++i) {
			DWORD AttribID = pMeshContainer->pBoneComb[i].AttribId;

			//----- �����`�F�b�N
			if (!this->CheckAlpha(pMeshContainer->pMaterials[AttribID].MatD3D.Diffuse.a)) {
				continue;
			}

			DWORD dwBlendMatrixNum = 0;
			// �e�����Ă���s�񐔎擾
			for (DWORD k = 0; k < pMeshContainer->dwBoneWeight; ++k) {
				UINT mtxIndex = pMeshContainer->pBoneComb[i].BoneId[k];
				if (mtxIndex != UINT_MAX) {
					// mStack�ɃI�t�Z�b�g�s��*�{�[�������s����i�[
					Matrix mStack = pMeshContainer->pMtxBoneOffset[mtxIndex] * m_aTransform[mtxIndex]->GetWorld();
					// �s��X�^�b�N�Ɋi�[
					pShader->SetWorldMatrix(&mStack, k);
					// ���݉e�����󂯂Ă���{�[���̐�
					++dwBlendMatrixNum;
				}
			}
			pShader->SetMatrix();
			// �W�I���g���u�����f�B���O���g�p���邽�߂ɍs��̌����w��
			pShader->SetBlendNum(dwBlendMatrixNum);

			// ���b�V���̕`��
			//----- �}�e���A��
			// ���C�g�̐F
			Color light = GetLightManager()->Get("Directional")->GetColor();
			// ���b�V���̐F
			D3DCOLORVALUE Col = pMeshContainer->pMaterials[AttribID].MatD3D.Ambient;
			// ����
			Vector4 A(Col.r*light.r, Col.g*light.g, Col.b*light.b, Col.a*light.a);
			pShader->SetAmbient(&A);
			// �e�N�X�`��
			pShader->SetDecalMap(pMeshContainer->ppTexture[AttribID]);
			// �p�X�̊J�n
			if (pMeshContainer->ppTexture[AttribID])
				pShader->BeginPass(2);
			else
				pShader->BeginPass(3);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
			pShader->EndPass();	// �p�X�̏I��
		}
	} else {
		// �ʏ탁�b�V��
		pShader->SetWorldMatrix(&m_aTransform[pFrame->OffsetID]->GetWorld());
		pShader->SetMatrix();
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; ++i) {
			//----- �����`�F�b�N
			if (!this->CheckAlpha(pMeshContainer->pMaterials[i].MatD3D.Diffuse.a)) {
				continue;
			}

			//----- �}�e���A��
			// ���C�g�̐F
			Color light = GetLightManager()->Get("Directional")->GetColor();
			// ���b�V���̐F
			D3DCOLORVALUE Col = pMeshContainer->pMaterials[i].MatD3D.Ambient;
			// ����
			Vector4 A(Col.r*light.r, Col.g*light.g, Col.b*light.b, Col.a*light.a);
			pShader->SetAmbient(&A);
			// �e�N�X�`��
			pShader->SetDecalMap(pMeshContainer->ppTexture[i]);
			// �p�X�̊J�n
			if (pMeshContainer->ppTexture[i] != NULL)
				pShader->BeginPass(0);
			else
				pShader->BeginPass(1);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);					// �`��
			pShader->EndPass();	// �p�X�̏I��
		}
	}
	pShader->End();
}


/*									//
//			�R���e�i���`��i�e		//
//									*/
void SkinMesh::DrawShadowContainer(
	MYMESHCONTAINER* pMeshContainer,
	MYFRAME* pFrame)
{
	//----- �V�F�[�_
	IHlslDrawBase* pShader = (IHlslDrawBase*)GetShaderManager()->Get(SHADER_SHADOW);

	// �X�L�����b�V��
	if (pMeshContainer->pSkinInfo != NULL) {
		// �X�L�j���O�v�Z
		for (DWORD i = 0; i < pMeshContainer->dwBoneNum; ++i) {
			DWORD AttribID = pMeshContainer->pBoneComb[i].AttribId;

			DWORD dwBlendMatrixNum = 0;
			// �e�����Ă���s�񐔎擾
			for (DWORD k = 0; k < pMeshContainer->dwBoneWeight; ++k) {
				UINT mtxIndex = pMeshContainer->pBoneComb[i].BoneId[k];
				if (mtxIndex != UINT_MAX) {
					// mStack�ɃI�t�Z�b�g�s��*�{�[�������s����i�[
					Matrix mStack = pMeshContainer->pMtxBoneOffset[mtxIndex] * m_aTransform[mtxIndex]->GetWorld();
					// �s��X�^�b�N�Ɋi�[
					pShader->SetWorldMatrix(&mStack, k);
					// ���݉e�����󂯂Ă���{�[���̐�
					++dwBlendMatrixNum;
				}
			}
			pShader->SetMatrix();
			// �W�I���g���u�����f�B���O���g�p���邽�߂ɍs��̌����w��
			pShader->SetBlendNum(dwBlendMatrixNum);

			// �p�X�̊J�n
			pShader->BeginPass(1);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
			pShader->EndPass();	// �p�X�̏I��
		}
	} else {
		// �ʏ탁�b�V��
		pShader->SetWorldMatrix(&m_aTransform[pFrame->OffsetID]->GetWorld());
		pShader->SetMatrix();
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; ++i) {
			// �p�X�̊J�n
			pShader->BeginPass(0);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);					// �`��
			pShader->EndPass();	// �p�X�̏I��
		}
	}
}
#pragma endregion
#pragma endregion

#pragma region Bone
/*									//
//		�{�[���̍����s��擾		//
//									*/
Transform* SkinMesh::GetBoneTransform(const string _BoneName) {
	MYFRAME* pFrame = SearchBoneFrame(_BoneName);

	if (pFrame)
		return m_aTransform[pFrame->OffsetID];
	else {
		return NULL;
	}
}


/*									//
//			�Ώۂ̃{�[������		//
//									*/
MYFRAME* SkinMesh::SearchBoneFrame(const string _BoneName) {
	for (auto it = m_aFrame.begin(); it != m_aFrame.end(); ++it) {
		if (strcmp((*it)->Name, _BoneName.c_str()) == 0)
			return (*it);
	}

	Debug::LogError("���������{�[�� : " + _BoneName + "��������܂���ł���");
	return NULL;
}
#pragma endregion

/*									//
//			�t���[���z��쐬		//
//									*/
void SkinMesh::CreateFrameArray(LPD3DXFRAME _pFrame) {
	MYFRAME* pFrame = (MYFRAME*)_pFrame;

	m_aFrame.push_back(pFrame);
	m_aTransform.push_back(new Transform(NULL));
	if (pFrame->pMeshContainer)	m_aInContainer.push_back(pFrame);

	if (pFrame->pFrameSibling)		CreateFrameArray(pFrame->pFrameSibling);
	if (pFrame->pFrameFirstChild)	CreateFrameArray(pFrame->pFrameFirstChild);
}

#pragma region Collision
/*									//
//			�����蔻��				//
//									*/
bool SkinMesh::Intersect(
	Vector3& L0,		// �n�_
	Vector3& L1,		// �I�_
	bool bRay,			// ���C�t���O
	Vector3* pCross,	// �����_
	Vector3* pNormal)	// �ڐG���̖@���x�N�g��
{
	// �����̕����x�N�g�������߂�
	Vector3 W;
	if (bRay) {
		W = L1;
	} else {
		W = L1 - L0;
	}
	return IntersectFrame(L0, W, bRay, pCross, pNormal);
}
// �t���[��
bool SkinMesh::IntersectFrame(
	Vector3& L0,
	Vector3& W,
	bool bRay,
	Vector3* pCross,
	Vector3* pNormal)
{
	bool bResult = false;

	for (auto it = m_aInContainer.begin(); it != m_aInContainer.end(); ++it) {
		bResult = IntersectMeshContainer(
			(*it)->pMeshContainer,
			L0,
			W,
			bRay,
			pCross,
			pNormal);
		if (bResult)
			return bResult;
	}

	return bResult;
}
// �R���e�i
bool SkinMesh::IntersectMeshContainer(
	LPD3DXMESHCONTAINER pMeshContainer,
	Vector3& L0,
	Vector3& W,
	bool bRay,
	Vector3* pCross,
	Vector3* pNormal)
{
	bool bResult = false;

	// ���b�V���̎擾
	LPD3DXMESH pMesh = pMeshContainer->MeshData.pMesh;
	LPBYTE pVtx;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
	WORD* pIdx;
	pMesh->LockIndexBuffer(0, (LPVOID*)&pIdx);
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwIdx = pMesh->GetNumFaces();
	for (DWORD i = 0; i < dwIdx; i++) {
		// �O�p�`�̒��_�擾
		Vector3 P0 = *(LPD3DXVECTOR3)(pVtx + dwStride * *pIdx++);
		Vector3 P1 = *(LPD3DXVECTOR3)(pVtx + dwStride * *pIdx++);
		Vector3 P2 = *(LPD3DXVECTOR3)(pVtx + dwStride * *pIdx++);
		// �O�p�`�̕ӂ̃x�N�g���擾
		Vector3 P0P1 = P1 - P0;
		Vector3 P1P2 = P2 - P1;
		Vector3 P2P0 = P0 - P2;
		// �O�p�`�̕ӂƐ����̍��ʂ̖@���x�N�g���擾
		Vector3 N;
		D3DXVec3Cross(&N, &P0P1, &W);
		if (D3DXVec3Dot(&N, &(L0 - P0)) < 0.0f) {
			continue;
		}
		D3DXVec3Cross(&N, &P1P2, &W);
		if (D3DXVec3Dot(&N, &(L0 - P1)) < 0.0f) {
			continue;
		}
		D3DXVec3Cross(&N, &P2P0, &W);
		if (D3DXVec3Dot(&N, &(L0 - P2)) < 0.0f) {
			continue;
		}
		// �O�p�`�̖@���x�N�g���擾�A���K��
		D3DXVec3Cross(&N, &P0P1, &P1P2);
		D3DXVec3Normalize(&N, &N);

		// �}��ϐ�t�����߂�O�̕�����v�Z
		float base = D3DXVec3Dot(&N, &W);
		if (base == 0.0f) {
			continue;	// �����Ɩʂ����s
		}
		// t�����߂�
		float t = D3DXVec3Dot(&N, &(P0 - L0)) / base;
		if (t < 0.0f) {
			continue;	// ��_���n�_�����
		}
		if (!bRay && t > 1.0f) {
			continue;	// ��_���I�_���O��
		}
		// ��_�����߂�
		if (pCross) {
			*pCross = L0 + t * W;
		}
		// �@���x�N�g����Ԃ�
		if (pNormal) {
			*pNormal = N;
		}
		// ��_����������
		bResult = true;
		break;
	}
	pMesh->UnlockIndexBuffer();
	pMesh->UnlockVertexBuffer();

	return bResult;
}
#pragma endregion