// ���
// 2017. 2. 10	�v���O�����쐬
// Author		SyunMizuno


#pragma once


#include	"../GameFramework/Obj/3D/Obj3D.h"



class Allow	:	public	Obj3D {
public:
	Allow();
	virtual ~Allow();

	// �������Ă����
	virtual void OnCollisionTrigger(Obj3D* obj);

protected:
	// ������
	virtual void Init();
	// �X�V
	virtual void Update();
};