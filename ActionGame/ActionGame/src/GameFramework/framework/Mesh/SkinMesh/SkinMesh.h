// ========================================================================
// Author		:	SyunMizuno
// 2015.10		:	�쐬
// 2015.11.22	:	���������[�N�폜
// 2016. 4. 2	:	OBB�̒ǉ�
// 2016. 5. 9	:	�n�[�t�����o�[�g����
// ========================================================================

#pragma once

#include	<vector>
#include	"MyAllocHierarchy.h"
#include	"HighLevelAnimController.h"
#include	"../../Convert.h"
#include	"../../../Transform/Transform.h"
#include	"../../Shader/ShaderManager.h"

using namespace std;

/*
	MEMO_SPACE:
*/


//----- ���b�V���p�\����(Manager�Ŏg�p
typedef struct _tSkin {
	LPD3DXFRAME					pFrameRoot;		// �t���[��
	LPD3DXANIMATIONCONTROLLER	pController;	// �R���g���[���[
} _tSkin;


class	SkinMesh {
public:
#pragma region enum

	// ���Z�����`�F�b�N
	typedef enum {
		CHECK_ALL = 0,
		CHECK_ALPHA,
		CHECK_NOALPHA,
	} _eAlphaCheck;

#pragma endregion

protected:
#pragma region variable

	vector<MYFRAME*>		m_aFrame;		// �t���[���z��
	vector<MYFRAME*>		m_aInContainer;	// �R���e�i�z��
	vector<Transform*>		m_aTransform;	// Transform

	HighLevelAnimController	m_AnimCont;		// �R���g���[���[
	bool					m_bLoad;		// ���̃N���X�ō쐬�����t���O
	_eAlphaCheck			m_eAlpha;		// ���Z�����`�F�b�N

#pragma endregion

public:
#pragma region method

	SkinMesh();
	virtual ~SkinMesh();

	//----- �X�V
	virtual void Update(Matrix& world);
	virtual void LateUpdate();

	//----- �`��
	virtual void Draw(
		_shadertype type = SHADER_NONE);
	virtual void LateDraw(
		_shadertype type = SHADER_NONE);

	//----- ���
	void Release();

#pragma region Load
	// �ǂݍ���
	HRESULT	Load(const string filename);
	// �}�l�[�W���[����̓ǂݍ���
	void ManagerGet(_tSkin* pSkin);
#pragma endregion

	// �{�[���s��擾
	Transform* GetBoneTransform(const string _BoneName);

	//----- �R���g���[��
	HighLevelAnimController& GetCont() { return m_AnimCont; }

	//----- �����蔻��
	bool Intersect(
		Vector3& L0,
		Vector3& L1,
		bool bRay = false,
		Vector3* pCross = NULL,
		Vector3* pNormal = NULL);

#pragma endregion

protected:
#pragma region conceal method

	//----- �X�V
	virtual void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, Matrix* pParentMatrix);

	//----- �`��
	// ���Z�����`�F�b�N
	bool CheckAlpha(float alpha);

#pragma region Draw
	// �t���[��
	void DrawFrame(_shadertype type);
	// �R���e�i
	void DrawMeshContainer(
		MYMESHCONTAINER* pMeshContainer,
		MYFRAME* pFrame);
	// �R���e�i�i�V�F�[�_
	void DrawShaderContainer(
		MYMESHCONTAINER* pMeshContainer,
		MYFRAME* pFrame,
		_shadertype type);
#pragma endregion

	// �Ώۂ̃{�[��������
	MYFRAME* SearchBoneFrame(const string _BoneName);

	// �t���[���z��쐬
	virtual void CreateFrameArray(LPD3DXFRAME _pFrame);

#pragma region Collision
	//----- �����蔻��
	bool IntersectFrame(
		Vector3& L0,
		Vector3& W,
		bool bRay,
		Vector3* pCross,
		Vector3* pNormal);

	bool IntersectMeshContainer(
		LPD3DXMESHCONTAINER pMeshContainer,
		Vector3& L0,
		Vector3& W,
		bool bRay,
		Vector3* pCross,
		Vector3* pNormal);
#pragma endregion

#pragma endregion
};