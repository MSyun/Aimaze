// �`��}�l�[�W���[
// 2017. 2. 23	�v���O�����쐬
// author		SyunMizuno


#include	"DrawManager.h"



/*									//
//			�R���X�g���N�^			//
//									*/
DrawManagerBase::DrawManagerBase() {

}


/*									//
//			�f�X�g���N�^			//
//									*/
DrawManagerBase::~DrawManagerBase() {

}


/*									//
//			���X�g�擾				//
//									*/
void DrawManagerBase::SetList(Obj3DManagerBase* mana) {
	m_pObjList = mana->GetList();
}


/*									//
//				�`��				//
//									*/
void DrawManagerBase::Draw() {
	for (auto it = m_pObjList->begin(); it != m_pObjList->end(); ++it) {
		// �e�̊m�F
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->DrawAll();
	}
}


/*									//
//				��`��				//
//									*/
void DrawManagerBase::LateDraw() {
	for (auto it = m_pObjList->begin(); it != m_pObjList->end(); ++it) {
		// �e�̊m�F
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->LateDrawAll();
	}
}


/*									//
//				���o				//
//									*/
void DrawManagerBase::Extract() {
	CheckFrustum();
	CheckOclusion();
}


/*									//
//				�e�쐬				//
//									*/
void DrawManagerBase::CreateShadow() {

}


/*									//
//				������				//
//									*/
void DrawManagerBase::CheckFrustum() {

}


/*									//
//			�I�N���[�W����			//
//									*/
void DrawManagerBase::CheckOclusion() {

}


/*									//
//			�t�B���^�`��			//
//									*/
void DrawManagerBase::FilterDraw() {

}