//	�R���C�_�[
// 2017. 1. 2	�v���O�����쐬
// Author		SyunMizuno


#include	"Collider.h"
#include	"../framework/Utility/System/SystemUtility.h"
#include	"../framework/Debug/Debug.h"



/*									//
//			�R���X�g���N�^			//
//									*/
Collider::Collider(IObj* object) :
	m_bEnable(true),
	m_pObject(object),
	m_pTree(NULL),
	m_CollisionColor(0, 255, 255, 255),
	m_TreeColor(255, 255, 0, 255),
	m_CurrentHit(0)
{
	if (!object) {
		Debug::Log("Collider : �I�u�W�F�N�g�����݂��܂���");
	}
}


/*									//
//			�f�X�g���N�^			//
//									*/
Collider::~Collider() {
	DeleteSphere();
	DeleteCapsule();
	DeleteObb();
	DeleteAabb();
	DeleteTree();
	for (int i = 0; i < 2; ++i)
		m_aHitObj[i].clear();
}


/*									//
//				�X�V				//
//									*/
void Collider::Update(Matrix& mat) {
	HitUpdate();

	for (auto it = m_aSphere.begin(); it != m_aSphere.end(); ++it)
		UpdateCollision(**it, mat);
	for (auto it = m_aCapsule.begin(); it != m_aCapsule.end(); ++it)
		UpdateCollision(**it, mat);
	for (auto it = m_aObb.begin(); it != m_aObb.end(); ++it)
		UpdateCollision(**it, mat);
	for (auto it = m_aAabb.begin(); it != m_aAabb.end(); ++it)
		UpdateCollision(**it, mat);
	if (m_pTree)
		UpdateCollision(*m_pTree, mat);
}


/*									//
//				�`��				//
//									*/
void Collider::Draw() {
	if (!m_bEnable)	return;

	for (auto it = m_aSphere.begin(); it != m_aSphere.end(); ++it)
		DrawCollision(**it, m_CollisionColor);
	for (auto it = m_aCapsule.begin(); it != m_aCapsule.end(); ++it)
		DrawCollision(**it, m_CollisionColor);
	for (auto it = m_aObb.begin(); it != m_aObb.end(); ++it)
		DrawCollision(**it, m_CollisionColor);
	for (auto it = m_aAabb.begin(); it != m_aAabb.end(); ++it)
		DrawCollision(**it, m_CollisionColor);
	// Tree
	if (m_pTree)
		DrawCollision(*m_pTree, m_TreeColor);
}

#pragma region Hit
/*									//
//			�q�b�g�̒ʒm			//
//									*/
void Collider::Hit(Collider* target) {
	if (!m_bEnable)	return;
	m_aHitObj[m_CurrentHit].push_back(target);
}


/*									//
//			�q�b�g�̏�����			//
//									*/
void Collider::ResetHit() {
	for (int i = 0; i < 2; ++i)
		m_aHitObj[i].clear();
}


/*									//
//			�q�b�g�̍X�V			//
//									*/
void Collider::HitUpdate() {
	m_CurrentHit = (m_CurrentHit + 1) % 2;
	m_aHitObj[m_CurrentHit].clear();
}
#pragma endregion

#pragma region Check
/*									//
//			�q�b�g�̃g���K�[		//
//									*/
bool Collider::CheckCollTrigger(Collider* target) {
	if (!m_bEnable)	return false;

	if (CheckCurrentHit(target) &&
		!CheckPrevHit(target)) {
		return true;
	}

	return false;
}


/*									//
//			�q�b�g�̃X�e�C			//
//									*/
bool Collider::CheckCollStay(Collider* target) {
	if (!m_bEnable)	return false;

	if (CheckCurrentHit(target) &&
		CheckPrevHit(target)) {
		return true;
	}

	return false;
}


/*									//
//			�q�b�g�̃����[�X		//
//									*/
bool Collider::CheckCollRelease(Collider* target) {
	if (!m_bEnable)	return false;

	if (!CheckCurrentHit(target) &&
		CheckPrevHit(target)) {
		return true;
	}

	return false;
}
#pragma endregion

#pragma region Set
/*									//
//				���ݒ�				//
//									*/
void Collider::SetSphere(Sphere* sphere) {
	if (!sphere)	return;

	m_aSphere.push_back(sphere);
	CreateTree();
}


/*									//
//			�J�v�Z���ݒ�			//
//									*/
void Collider::SetCapsule(Capsule* capsule) {
	if (!capsule)	return;

	m_aCapsule.push_back(capsule);
	CreateTree();
}


/*									//
//				OBB�ݒ�				//
//									*/
void Collider::SetObb(Obb* obb) {
	if (!obb)	return;

	m_aObb.push_back(obb);
	CreateTree();
}


/*									//
//				AABB�ݒ�			//
//									*/
void Collider::SetAabb(Aabb* aabb) {
	if (!aabb)	return;

	m_aAabb.push_back(aabb);
	CreateTree();
}


/*									//
//				8���ؐݒ�			//
//									*/
void Collider::SetTree(const Aabb& aabb) {
	CreateTree();
	m_pTree->p_d = aabb.p_d;
	for (int i = 0; i < 3; i ++)
		m_pTree->r[i] = aabb.r[i];
}
#pragma endregion

#pragma region Color
/*									//
//			�����蔻��̐F�ݒ�		//
//									*/
void Collider::SetCollisionColor(Color color) {
	m_CollisionColor = color;
}


/*									//
//			�����蔻��̐F�ݒ�		//
//									*/
void Collider::SetCollisionColor(float r, float g, float b, float a) {
	m_CollisionColor.r = r;
	m_CollisionColor.g = g;
	m_CollisionColor.b = b;
	m_CollisionColor.a = a;
}


/*									//
//		8���؂̂����蔻��̐F�ݒ�	//
//									*/
void Collider::SetTreeColor(Color color) {
	m_TreeColor = color;
}


/*									//
//		8���؂̂����蔻��̐F�ݒ�	//
//									*/
void Collider::SetTreeColor(float r, float g, float b, float a) {
	m_TreeColor.r = r;
	m_TreeColor.g = g;
	m_TreeColor.b = b;
	m_TreeColor.a = a;
}
#pragma endregion

#pragma region conceal method
/*									//
//		���t���[���̃q�b�g�m�F		//
//									*/
bool Collider::CheckCurrentHit(Collider* target) {
	if (!m_bEnable)	return false;

	for (auto it = m_aHitObj[m_CurrentHit].begin(); it != m_aHitObj[m_CurrentHit].end(); ++it)
		if ((*it) == target)
			return true;

	return false;
}

/*									//
//		�O�t���[���̃q�b�g�m�F		//
//									*/
bool Collider::CheckPrevHit(Collider* target) {
	if (!m_bEnable)	return false;

	int Num = (m_CurrentHit + 1) % 2;
	for (auto it = m_aHitObj[Num].begin(); it != m_aHitObj[Num].end(); ++it)
		if ((*it) == target)
			return true;

	return false;
}

/*									//
//				8���؂̍쐬			//
//									*/
void Collider::CreateTree() {
	if (m_pTree)	return;
	m_pTree = new Aabb;

	//m_pTreeColl->SetMat(&GetWorld());

	// �������a�ݒ�
	for (int i = 0; i < 3; i ++)
		m_pTree->r[i] = 3.0f;
}

#pragma region Delete
/*									//
//				���폜				//
//									*/
void Collider::DeleteSphere() {
	for (auto it = m_aSphere.begin(); it != m_aSphere.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_aSphere.clear();
}


/*									//
//			�J�v�Z���폜			//
//									*/
void Collider::DeleteCapsule() {
	for (auto it = m_aCapsule.begin(); it != m_aCapsule.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_aCapsule.clear();
}


/*									//
//				OBB�폜				//
//									*/
void Collider::DeleteObb() {
	for (auto it = m_aObb.begin(); it != m_aObb.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_aObb.clear();
}


/*									//
//				AABB�폜			//
//									*/
void Collider::DeleteAabb() {
	for (auto it = m_aAabb.begin(); it != m_aAabb.end(); ++it) {
		SAFE_DELETE(*it);
	}
	m_aAabb.clear();
}


/*									//
//				8���؍폜			//
//									*/
void Collider::DeleteTree() {
	SAFE_DELETE(m_pTree);
}
#pragma endregion

#pragma endregion