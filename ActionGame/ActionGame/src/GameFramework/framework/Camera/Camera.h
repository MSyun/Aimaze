// �J�����N���X
// 2017. 1. 4	�v���O�����쐬
// Author		SyunMizuno

#pragma once


#include	"../Convert.h"
#include	"../../Skydome/Skydome.h"
#include	"../../Obj/3D/Obj3D.h"


class	Camera : public	Obj3D {
protected:
#pragma region variable

	// �r���[�֌W
	Matrix	m_matView;
	float	m_fLookLength;	// �����_�܂ł̋���

	// �v���W�F�N�V�����֌W
	Matrix		m_matProj;
	float		m_fFovy;	// ����p
	float		m_fAspect;	// �A�X�y�N�g��
	float		m_fNear;	// �O���N���b�v
	float		m_fFar;		// ����N���b�v

	Skydome*	m_pSky;	// �X�J�C�{�b�N�X

#pragma endregion

public:
#pragma region method

	Camera();
	virtual ~Camera();

	void Set();

	void SetLookLength(float len) { m_fLookLength = len; }
	void AddLookLength(float len) { m_fLookLength += len; }

	Matrix GetView() { return m_matView; }
	Matrix GetProj() { return m_matProj; }

	// �v���W�F�N�V����
	void SetFovy(float Fovy) { m_fFovy = Fovy; }
	void SetAspect(float Aspect) { m_fAspect = Aspect; }
	void SetNear(float Near) { m_fNear = Near; }
	void SetFar(float Far) { m_fFar = Far; }

	void CreateSky();
	void DeleteSky();

#pragma endregion

protected:
#pragma region conceal method

	virtual void Update();

#pragma endregion
};