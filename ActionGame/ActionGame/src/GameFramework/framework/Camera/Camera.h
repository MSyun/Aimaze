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
	Matrix	m_matView;		// �r���[�s��
	float	m_fLookLength;	// �����_�܂ł̋���

	// �v���W�F�N�V�����֌W
	Matrix		m_matProj;	// �v���W�F�N�V�����s��
	float		m_fFovy;	// ����p
	float		m_fAspect;	// �A�X�y�N�g��
	float		m_fNear;	// �O���N���b�v
	float		m_fFar;		// ����N���b�v

	Skydome*	m_pSky;	// �X�J�C�{�b�N�X

#pragma endregion

public:
#pragma region method

	/* �R���X�g���N�^ */
	Camera();

	/* �f�X�g���N�^ */
	virtual ~Camera();

	/* �J�����̐ݒ� */
	void Set();

	/* �����_�̐ݒ�
	// len	:	�ݒ肷�钷��
	*/
	void SetLookLength(float len) { m_fLookLength = len; }
	void AddLookLength(float len) { m_fLookLength += len; }

	/* �r���[�s��̎擾 */
	Matrix GetView() { return m_matView; }

	/* �v���W�F�N�V�����s��̎擾 */
	Matrix GetProj() { return m_matProj; }

	/* ����p�̐ݒ�
	// Fovy	:	�ݒ肵��������p
	*/
	void SetFovy(float Fovy) { m_fFovy = Fovy; }

	/* �A�X�y�N�g��̐ݒ�
	// Aspect	:	�ݒ肵�����A�X�y�N�g��̐ݒ�
	*/
	void SetAspect(float Aspect) { m_fAspect = Aspect; }

	/* �j�A�N���b�v�̐ݒ�
	// Near	:	�ݒ肵�����j�A�N���b�v
	*/
	void SetNear(float Near) { m_fNear = Near; }

	/* �t�@�[�N���b�v�̐ݒ�
	// Far	:	�ݒ肵�����t�@�[�N���b�v
	*/
	void SetFar(float Far) { m_fFar = Far; }

	/* �X�J�C�h�[���̍쐬 */
	void CreateSky();

	/* �X�J�C�h�[���̍폜 */
	void DeleteSky();

#pragma endregion

protected:
#pragma region conceal method

	/* �X�V */
	virtual void Update();

#pragma endregion
};