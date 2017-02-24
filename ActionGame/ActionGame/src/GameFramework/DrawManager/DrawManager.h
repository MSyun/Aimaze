// �`��}�l�[�W���[
// 2017. 2. 23	�v���O�����쐬
// author		SyunMizuno


#pragma once

#include	"../Obj/3D/Obj3DManager.h"




class DrawManagerBase {
protected:
	list<_3DOBJ*>*	m_pObjList;

public:
	DrawManagerBase();
	virtual ~DrawManagerBase();

	// �`��
	void Draw();
	void LateDraw();

	// ���o
	void Extract();

	void SetList(Obj3DManagerBase* mana);

protected:
	// �e�쐬
	void CreateShadow();

	// ������
	void CheckFrustum();

	// �I�N���[�W����
	void CheckOclusion();

	// �t�B���^�`��
	void FilterDraw();
};


class DrawManager	:	public	DrawManagerBase,	public	Singleton<DrawManager> {};


inline DrawManager* GetDrawManager(void) {
	return DrawManager::Instance();
}