//	�R���C�_�[
// 2017. 1. 2	�v���O�����쐬
// Author		SyunMizuno



#pragma once


#include	<vector>
#include	"../framework/Collision/Collision.h"
#include	"../framework/Convert.h"
#include	"../Obj/Obj.h"


using namespace std;


class Collider {
private:
#pragma region variable

	bool	m_bEnable;

	IObj*	m_pObject;

	int		m_CurrentHit;
	vector<Collider*>	m_aHitObj[2];

	vector<Sphere*>		m_aSphere;
	vector<Capsule*>	m_aCapsule;
	vector<Obb*>		m_aObb;
	vector<Aabb*>		m_aAabb;

	Color	m_CollisionColor;

	Aabb*	m_pTree;
	Color	m_TreeColor;

#pragma endregion

public:
#pragma region method

	Collider(IObj* object);
	virtual ~Collider();

	// �X�V
	void Update(Matrix& mat);

	// �`��
	void Draw();

	IObj* GetGameObject() { return m_pObject; }

#pragma region Hit
	// �q�b�g�̒ʒm
	void Hit(Collider* target);
	// �q�b�g�̏�����
	void ResetHit();
	// �q�b�g�̍X�V
	void HitUpdate();
#pragma endregion

#pragma region Check
	// �q�b�g�̃g���K�[
	bool CheckCollTrigger(Collider* target);
	// �q�b�g�̃X�e�C
	bool CheckCollStay(Collider* target);
	// �q�b�g�̃����[�X
	bool CheckCollRelease(Collider* target);
#pragma endregion

#pragma region Get
	// ���擾
	vector<Sphere*> GetSphere() { return m_aSphere; }
	// �J�v�Z���擾
	vector<Capsule*> GetCapsule() { return m_aCapsule; }
	// OBB�擾
	vector<Obb*> GetObb() { return m_aObb; }
	// AABB�擾
	vector<Aabb*> GetAabb() { return m_aAabb; }
	// 8���؎擾
	Aabb* GetTreeColl() { return m_pTree; }
#pragma endregion

#pragma region Set
	// ���ݒ�
	void SetSphere(Sphere* sphere);
	// OBB�ݒ�
	void SetObb(Obb* obb);
	// �J�v�Z���ݒ�
	void SetCapsule(Capsule* capsule);
	// AABB�ݒ�
	void SetAabb(Aabb* aabb);
	// 8���ؐݒ�
	void SetTree(const Aabb& aabb);

	void SetEnable(bool flg) { m_bEnable = flg; }
#pragma endregion

#pragma region Color
	// �����蔻��̐F�ݒ�
	void SetCollisionColor(Color color);
	void SetCollisionColor(float r, float g, float b, float a);
	// 8���؂̂����蔻��̐F�ݒ�
	void SetTreeColor(Color color);
	void SetTreeColor(float r, float g, float b, float a);
#pragma endregion

#pragma endregion

private:
#pragma region conceal method
	// ���t���[���̃q�b�g�m�F
	bool CheckCurrentHit(Collider* target);
	// �O�t���[���̃q�b�g�m�F
	bool CheckPrevHit(Collider* target);
	// 8���؂̍쐬
	void CreateTree();

#pragma region Delete
	// ���폜
	void DeleteSphere();
	// OBB�폜
	void DeleteObb();
	// �J�v�Z���폜
	void DeleteCapsule();
	// AABB�폜
	void DeleteAabb();
	// 8���؍폜
	void DeleteTree();
#pragma endregion

#pragma endregion
};