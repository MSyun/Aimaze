// 3D�I�u�W�F�N�g�}�l�[�W���N���X
// 2016. 8.19	�v���O�����쐬
// 2016. 9. 7	�I�N�c���[�����J�n
// 2016. 9.14	�I�N�c���[����������
// 2017. 1. 4	DontDestroy�֌W�ǉ�
// Author		SyunMizuno

#include	"Obj3DManager.h"
#include	<tchar.h>
#include	"../../framework/Utility/System/SystemUtility.h"
#include	"../../framework/Debug/Debug.h"
#include	"../../../Archives/Archives.h"


/*
	MEMO	:
			��X8���ؓ��̏����𑼂̃N���X�Ɉϑ����Ă��悢����
*/


/*									//
//			�R���X�g���N�^			//
//									*/
Obj3DManagerBase::Obj3DManagerBase() :
	m_bHelper(true),
	m_pOcTree(NULL)
{
	m_lstObj.clear();

	// 8���؂̋�Ԃ��쐬
	if (!CreateTree(6, Vector3(-300, -10, -300), Vector3(300, 30, 300))) {
		Debug::LogError("���`8���؋�Ԃ̏������Ɏ��s���܂���");
	}
}


/*									//
//			�f�X�g���N�^			//
//									*/
Obj3DManagerBase::~Obj3DManagerBase() {
	if (!m_bHelper)	return;

	while (m_lstObj.begin() != m_lstObj.end()) {
		list<_3DOBJ*>::iterator it = m_lstObj.begin();
		delete [] (*it)->pObj;
	}
	m_lstObj.clear();

	DeleteTree();
}


/*									//
//				����				//
//									*/
// ���O
Obj3D* Obj3DManagerBase::Find(const string name) {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// �g�p���ĂȂ�
		if (!(*it)->pObj->GetActive())	continue;

		if ((*it)->pObj->GetName() == name) {
			return (*it)->pObj;
		}
	}
	Debug::LogError("3D�I�u�W�F�N�g��" + name + "�͑��݂��܂���");
	return NULL;
}
// �^�O
Obj3D* Obj3DManagerBase::FindWithTag(const string tag) {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// �g�p���ĂȂ�
		if (!(*it)->pObj->GetActive())	continue;

		if ((*it)->pObj->GetTag() == Archives::Tag(tag)) {
			return (*it)->pObj;
		}
	}
	Debug::LogError("3D�I�u�W�F�N�g�Ƀ^�O��" + tag + "�͑��݂��܂���");
	return NULL;
}


/*									//
//				�ǉ�				//
//	(�������ꂽ�Ƃ��ɌĂяo�����)	*/
void Obj3DManagerBase::Add(Obj3D* Obj) {
	if (!Obj) {
		Debug::LogError("�I�u�W�F�N�g�̎��Ԃ�����܂���");
		return;
	}

	// �C���X�^���X�쐬
	_3DOBJ* regist = new _3DOBJ;
	regist->pTree = new OBJECT_FOR_TREE<Obj3D*>;

	// �I�u�W�F�N�g��ݒ�
	regist->pObj = Obj;
	regist->pTree->m_pObject = &Obj;

	// �o�^
	m_lstObj.push_back(regist);
}


/*									//
//				�S�폜				//
//									*/
void Obj3DManagerBase::AllClear() {
	list<_3DOBJ*> lstBuff;
	while (m_lstObj.begin() != m_lstObj.end()) {
		list<_3DOBJ*>::iterator it = m_lstObj.begin();
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

	DeleteTree();
	// 8���؂̋�Ԃ��쐬
	if (!CreateTree(6, Vector3(-300, -10, -300), Vector3(300, 30, 300))) {
		Debug::LogError("���`8���؋�Ԃ̏������Ɏ��s���܂���");
	}
}


/*									//
//				�폜				//
//	(�폜���ꂽ�Ƃ��ɌĂяo�����)	*/
void Obj3DManagerBase::Delete(Obj3D* Obj) {
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
void Obj3DManagerBase::Update() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// �e�̊m�F
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->UpdateAll();
	}
}
void Obj3DManagerBase::LateUpdate() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// �e�̊m�F
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->LateUpdateAll();
	}
}


/*									//
//				�폜�m�F			//
//									*/
void Obj3DManagerBase::CheckDestroy() {
	list<_3DOBJ*>	lstobj;
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
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
//			�����蔻��				//
//									*/
void Obj3DManagerBase::RegistCollider() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// 8���؂�����
		(*it)->pTree->Remove();

		if (!(*it)->pObj->GetActive())	continue;

		(*it)->pTree->m_pObject = &(*it)->pObj;

		if (!(*it)->pObj->GetCollider() ||
			!(*it)->pObj->GetCollider()->GetTreeColl())
			continue;

		RegistTree(*it);
	}
}
bool Obj3DManagerBase::SkipCheck( BYTE A, BYTE B ) {
	// �A�C�e�� vs ���@
	unsigned char Player = Archives::Tag("Player");
	unsigned char ItemBox = Archives::Tag("ItemBox");
	unsigned char Enemy = Archives::Tag("Enemy");
	unsigned char PlayerAttack = Archives::Tag("Player/Attack");
	unsigned char EnemyAttack = Archives::Tag("Enemy/Attack");
	unsigned char Allow = Archives::Tag("Allow");
	if ((A == Player && B == ItemBox) ||
		(B == Player && A == ItemBox))
		return true;
	if ((A == Player && B == EnemyAttack) ||
		(B == Player && A == EnemyAttack))
		return true;
	if ((A == PlayerAttack && B == Enemy) ||
		(B == PlayerAttack && A == Enemy))
		return true;
	if ((A == Allow && B == Player) ||
		(B == Allow && A == Player))
		return true;

	return false;
}
void Obj3DManagerBase::CheckCollider() {
	RegistCollider();

	// �����蔻��̌�
	int	nColNum = 0;
	// �ՓˑΏۃ��X�g
	std::vector<Obj3D*>	aCol;

	// �ՓˑΏۃ��X�g�擾
	nColNum = m_pOcTree->GetAllCollisionList(aCol);

	// �Փ˔���
	nColNum /= 2;	// 2�Ŋ���̂̓y�A�ɂȂ��Ă��邽��

	for (int n = 0; n < nColNum; ++n) {
		// ������I�u�W�F�N�g
		Obj3D* A = aCol[n * 2];
		Obj3D* B = aCol[n * 2 + 1];

		// �X�L�b�v�m�F
		if (!SkipCheck(A->GetTag(), B->GetTag()))
			continue;

		Collider* AColl = A->GetCollider();
		Collider* BColl = B->GetCollider();

		// �c���[�p�����蔻��
		if(!CheckCollision(*AColl->GetTreeColl(), *BColl->GetTreeColl()))
			continue;

		// �����ł����蔻��
		if(Collision(AColl, BColl)) {
			AColl->Hit(BColl);
			BColl->Hit(AColl);

			// �g���K(�ǂ��炩���ǂ���΂����͂�)
			if (AColl->CheckCollTrigger(BColl))
				A->OnCollisionTrigger(B);
			if (BColl->CheckCollTrigger(AColl))
				B->OnCollisionTrigger(A);
			// �X�e�C(�ǂ��炩���ǂ���΂����͂�)
			if (AColl->CheckCollStay(BColl))	A->OnCollisionStay(B);
			if (BColl->CheckCollStay(AColl))	B->OnCollisionStay(A);
			// �����[�X(�ǂ��炩���ǂ���΂����͂�)
			if (AColl->CheckCollRelease(BColl))	A->OnCollisionRelease(B);
			if (BColl->CheckCollRelease(AColl))	B->OnCollisionRelease(A);
		}
	}
}
bool Obj3DManagerBase::Collision(Collider* A, Collider* B) {
	vector<Sphere*>	aSphere1 = A->GetSphere();
	vector<Sphere*>	aSphere2 = B->GetSphere();
	vector<Obb*>	aObb1 = A->GetObb();
	vector<Obb*>	aObb2 = B->GetObb();
	for (auto A = aSphere1.begin(); A != aSphere1.end(); ++A) {
		for (auto B = aSphere2.begin(); B != aSphere2.end(); ++B) {
			if (CheckCollision(*(*A), *(*B)))	return true;
		}
		for (auto B = aObb2.begin(); B != aObb2.end(); ++B) {
			if (CheckCollision(*(*A), *(*B)))	return true;
		}
	}
	for (auto A = aObb1.begin(); A != aObb1.end(); ++A) {
		for (auto B = aSphere2.begin(); B != aSphere2.end(); ++B) {
			if (CheckCollision(*(*A), *(*B)))	return true;
		}
		for (auto B = aObb2.begin(); B != aObb2.end(); ++B) {
			if (CheckCollision(*(*A), *(*B)))	return true;
		}
	}

	return false;
}


/*									//
//				�`��				//
//									*/
void Obj3DManagerBase::Draw() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// �e�̊m�F
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->DrawAll();
	}
}
void Obj3DManagerBase::LateDraw() {
	for (auto it = m_lstObj.begin(); it != m_lstObj.end(); ++it) {
		// �e�̊m�F
		if ((*it)->pObj->GetTransform()->CheckParent())
			continue;

		(*it)->pObj->LateDrawAll();
	}
}



/*									//
//			�I�N�c���[				//
//									*/
bool Obj3DManagerBase::CreateTree(
	unsigned int Level,
	Vector3& Min,
	Vector3& Max)
{
	DeleteTree();

	m_pOcTree = new CLiner8TreeManager<Obj3D*>;
	return m_pOcTree->Init(Level, Min, Max);
}
void Obj3DManagerBase::DeleteTree() {
	SAFE_DELETE(m_pOcTree);
}
void Obj3DManagerBase::RegistTree(_3DOBJ* obj) {
	// �C���X�^���X�擾
	Aabb* aabb = obj->pObj->GetCollider()->GetTreeColl();

	// �C���X�^���X�m�F
	if (!aabb)	return;

	// �l�ݒ�
	Point3 pos = aabb->p;
	Vector3 Min(pos[0] - aabb->r[0], pos[1] - aabb->r[1], pos[2] - aabb->r[2]);
	Vector3 Max(pos[0] + aabb->r[0], pos[1] + aabb->r[1], pos[2] + aabb->r[2]);
	m_pOcTree->Regist(&Min, &Max, obj->pTree);
}