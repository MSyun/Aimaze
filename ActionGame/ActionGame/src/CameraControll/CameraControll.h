// �J��������N���X
// 2017. 1.30	�v���O�����쐬
// Author		SyunMizuno


#pragma once


#include	"../GameFramework/Obj/3D/Obj3D.h"
#include	"../GameFramework/framework/Camera/Camera.h"


class CameraControll	:	public	Obj3D {
private:
	Transform*	m_Player;
	Camera*		m_Camera;

	//----- sin�v�f
	Vector3	m_vShake;
	bool	m_bShake;		// �h��m�F
	float	m_fCounter;		// �v���p
	float	m_fDeviation;	// �U�ꕝ
	float	m_fCyCle;		// �����i1�T�C�N������Ԋu

public:
	CameraControll();
	virtual ~CameraControll() {};

	// �h��
	void Shake(float time, float deviat);

private:
	virtual void Init();
	virtual void LateUpdate();
	virtual void Input();

	void ShakeCount();
};