// 2D�I�u�W�F�N�g�}�l�[�W���N���X
// 2017. 1.4	�v���O�����쐬
// Author		SyunMizuno


#include	"Obj2DManager.h"
#include	<tchar.h>
#include	"../../framework/Utility/System/SystemUtility.h"
#include	"../../framework/Debug/Debug.h"
#include	"../../../Archives/Archives.h"


/*
	MEMO	:
			��X4���ؓ��̏����𑼂̃N���X�Ɉϑ����Ă��悢����
*/



/*									//
//			�R���X�g���N�^			//
//									*/
Obj2DManagerBase::Obj2DManagerBase() :
	m_bHelper(true),
	m_pQuadTree(NULL)
{
	m_lstObj.clear();

	// 4���؂̋�Ԃ��쐬
	//if (!CreateTree(4, Vector2(-100, -10), Vector2(100, 30))) {
//	Debug::Log("ERROR : ���`4���؋�Ԃ̏������Ɏ��s���܂���");
	//}
}


/*									//
//			�f�X�g���N�^			//
//									*/
Obj2DManagerBase::~Obj2DManagerBase() {
	if (!m_bHelper)	return;

	while (m_lstObj.begin() != m_lstObj.end()) {
		list<_2DOBJ*>::iterator it = m_lstObj.begin();
		delete [] (*it)->pObj;
	}
	m_lstObj.clear();
	//DeleteTree();
}


/*									//
//				����				//
//									*/
// ���O
Sprite* Obj2DManagerBase::Find(const string name) {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// �g�p���ĂȂ�
		if (!(*it)->pObj->GetActive())	continue;

		if ((*it)->pObj->GetName() == name) {
			return (*it)->pObj;
		}
	}
	Debug::LogError("2D�I�u�W�F�N�g��" + name + "�͑��݂��܂���");
	return NULL;
}
// �^�O
Sprite* Obj2DManagerBase::FindWithTag(const string tag) {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// �g�p���ĂȂ�
		if (!(*it)->pObj->GetActive())	continue;

		if ((*it)->pObj->GetTag() == Archives::Tag(tag)) {
			return (*it)->pObj;
		}
	}
	Debug::LogError("2D�I�u�W�F�N�g�Ƀ^�O��" + tag + "�͑��݂��܂���");
	return NULL;
}


/*									//
//				�ǉ�				//
//									*/
void Obj2DManagerBase::Add(Sprite* Obj) {
	if (!Obj) {
		Debug::LogError("�I�u�W�F�N�g�̎��Ԃ�����܂���");
		return;
	}

	// �C���X�^���X�쐬
	_2DOBJ* regist = new _2DOBJ;
	regist->pTree = new OBJECT_FOR_TREE<Sprite*>;

	// �I�u�W�F�N�g��ݒ�
	regist->pObj = Obj;
	regist->pTree->m_pObject = &Obj;

	// �o�^
	m_lstObj.push_back(regist);
}


/*									//
//				�S�폜				//
//									*/
void Obj2DManagerBase::AllClear() {
	list<_2DOBJ*> lstBuff;
	while (m_lstObj.begin() != m_lstObj.end()) {
		list<_2DOBJ*>::iterator it = m_lstObj.begin();
		if ((*it)->pObj->GetDontDestroy()) {
			lstBuff.push_back(*it);
			m_lstObj.erase(it);
			continue;
		}
		delete [] (*it)->pObj;
	}
	m_lstObj.clear();

	for (auto it = lstBuff.begin(); it != lstBuff.end(); ++it) {
		m_lstObj.push_back(*it);
	}
}


/*									//
//				�폜				//
//	(�폜���ꂽ�Ƃ��ɌĂяo�����)	*/
void Obj2DManagerBase::Delete(Sprite* Obj) {
	if (!Obj) {
		Debug::LogError("�I�u�W�F�N�g�̎��Ԃ�����܂���");
		return;
	}

	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		if ((*it)->pObj != Obj)	continue;

		// ���X�g������
		(*it)->pTree->Remove();

		// �C���X�^���X�폜
		SAFE_DELETE((*it)->pTree);
		SAFE_DELETE(*it);

		m_lstObj.erase(it);

		return;
	}
}


/*									//
//				�X�V				//
//									*/
void Obj2DManagerBase::Update() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// �g�p�m�F
		if (!(*it)->pObj->GetActive())	continue;

		(*it)->pObj->UpdateAll();
	}
}
void Obj2DManagerBase::LateUpdate() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 4���؂�����
		//(*it)->pTree->Remove();

		// �g�p�m�F
		if (!(*it)->pObj->GetActive())	continue;

		(*it)->pObj->LateUpdateAll();

		// �ꉞ������x�o�^
		//(*it)->pTree->m_pObject = &(*it)->pObj;

		//if (!(*it)->pObj->GetCollider() ||
		//	!(*it)->pObj->GetCollider()->GetTreeColl())
		//	return;

		// 4���؂ɓo�^
		//RegistTree(*it);
	}
}


/*									//
//				�폜�m�F			//
//									*/
void Obj2DManagerBase::CheckDestroy() {
	list<_2DOBJ*>	lstobj;
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++ it) {
		if ((*it)->pObj->GetDestroy()) {
			(*it)->pObj->OnDestroy();
			lstobj.push_back(*it);
			continue;
		}
	}
	while (lstobj.begin() != lstobj.end()) {
		auto it = lstobj.begin();
		delete [] (*it)->pObj;
		lstobj.erase(it);
	}
}


/*									//
//				�����蔻��			//
//									*/
//bool Obj2DManagerBase::SkipCheck(BYTE A, BYTE B) {
//	return false;
//}
//void Obj2DManagerBase::CheckCollider() {
//
//}
//bool Obj2DManagerBase::Collision() {
//	return false;
//}


/*									//
//				�`��				//
//									*/
void Obj2DManagerBase::Draw() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		if (!(*it)->pObj->GetActive())	continue;

		(*it)->pObj->DrawAll();
	}
}
void Obj2DManagerBase::LateDraw() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		if (!(*it)->pObj->GetActive())	continue;

		(*it)->pObj->LateDrawAll();
	}
}


/*									//
//			�N�A�b�h�c���[			//
//									*/
//bool Obj2DManagerBase::CreateTree(
//	unsigned int Level,
//	Vector2& Min,
//	Vector2& Max)
//{
//	DeleteTree();
//
//	m_pQuadTree = new CLiner4TreeManager<Sprite*>;
//	return m_pQuadTree->Init(Level, Min.x, Min.y, Max.x, Max.y);
//}
//void Obj2DManagerBase::DeleteTree() {
//	SAFE_DELETE(m_pQuadTree);
//}
//void Obj2DManagerBase::RegistTree(_2DOBJ* obj) {
//	// �C���X�^���X�擾
//
//}