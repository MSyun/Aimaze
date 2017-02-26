//////////////////////////////////////////////////////////////
// 3D�I�u�W�F�N�g�N���X
// 2016. 5.19	�v���O�����쐬
// 2016. 9.14	�I�N�c���[�p�R���W�����ǉ�
// 2017. 1.23	�A�j���[�^�[�ǉ�
// Author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"../Obj.h"
#include	"../../framework/Convert.h"
#include	"../../framework/Mesh/SkinMesh/SkinMesh.h"
#include	"../../Transform/Transform.h"
#include	"../../Collider/Collider.h"
#include	"../../Animator/IAnimator.h"


using namespace std;

/*
	memo	:
			HLSL�̃^�C�v��ݒ肷�邱��
			�A�j���[�^�[�̓��b�V���̒��ɉB�������ق����悢
*/


class	Obj3D : public	IObj {
private:
#pragma region conceal variable

	_shadertype m_ShaderType;	// �V�F�[�_�^�C�v

#pragma endregion

protected:
#pragma region variable

	Transform*	m_pTransform;	// �s���
	Collider*	m_pCollider;	// �����蔻��
	SkinMesh*	m_pMesh;		// ���b�V��
	IAnimator*	m_pAnimator;	// �A�j���[�^�[

#pragma endregion

public:
#pragma region method

	Obj3D();
	virtual ~Obj3D();

	virtual void UpdateAll();
	virtual void LateUpdateAll();
	virtual void DrawAll();
	virtual void LateDrawAll();
	virtual void DrawShadow();

	Transform* GetTransform() { return m_pTransform; }
	Collider* GetCollider() { return m_pCollider; }
	SkinMesh* GetMesh() { return m_pMesh; }

	virtual void OnCollisionTrigger(Obj3D* obj) {}
	virtual void OnCollisionStay(Obj3D* obj) {}
	virtual void OnCollisionRelease(Obj3D* obj) {}


#pragma region Collider
	void CreateCollider();
	void DeleteCollider();
#pragma endregion

#pragma region Mesh
	void CreateMesh();				// �쐬
	void DeleteMesh();				// �폜
	void SetModel(_tSkin* pSkin, IAnimator* pAnimator = NULL);	// ���f���̐ݒ�
#pragma endregion

#pragma region Shader Type
	_shadertype GetShaderType() { return m_ShaderType; }			// �V�F�[�_�^�C�v�擾
	void SetShaderType(_shadertype type) { m_ShaderType = type; }	// �V�F�[�_�^�C�v�ݒ�
#pragma endregion

	virtual void Destroy();

	virtual void SetActive(bool active);

#pragma endregion

protected:
#pragma region conceal method

	virtual void Init() {
		if(m_pAnimator)
			m_pAnimator->AnimatorSetting(&m_pMesh->GetCont());
	};
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void Draw() {}
	virtual void LateDraw() {}

	// ���b�V���̍X�V
	void UpdateMesh();

#pragma endregion
};