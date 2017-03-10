// �A�C�e���̔��N���X
// 2016. 8. 4	�v���O�����쐬
// Author	:	SyunMizuno

#pragma once


#include	"../../GameFramework/Obj/3D/Obj3D.h"
#include	"../Ring/Ring.h"



class ItemBox	:	public	Obj3D {
private:
	Ring*	m_pRing;	// �����O
	Obj3D*	m_pBox;		// ��
	Obj3D*	m_pItem;	// ���g

public:
	ItemBox();
	virtual ~ItemBox();

	Obj3D* GetItem() { return m_pItem; }
	void SetItem(Obj3D* item);

	// �A�j���[�V����
	void Begin(LPDIRECT3DTEXTURE9 tex);

	// ���g��n��
	Obj3D* Transfer();

private:
	virtual void Update();
};