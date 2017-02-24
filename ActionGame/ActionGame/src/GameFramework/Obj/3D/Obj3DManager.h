//////////////////////////////////////////////////////////////
// 3D�I�u�W�F�N�g�}�l�[�W���N���X
// 2016. 8.19	�v���O�����쐬
// Author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"Obj3D.h"
#include	"../../framework/Singleton/Singleton.h"
#include	<list>
#include	"../../framework/Collision/Cell.h"

using namespace std;


// �����蔻��Ɗ��S�����ł���\���̂��ߍ\���̂��g�p
typedef struct {
	Obj3D*						pObj;
	OBJECT_FOR_TREE<Obj3D*>*	pTree;
} _3DOBJ;


class	Obj3DManagerBase {
protected:
#pragma region variable

	// 8����
	CLiner8TreeManager<Obj3D*>*	m_pOcTree;

	// �I�u�W�F�N�g
	list<_3DOBJ*>	m_lstObj;

	bool	m_bHelper;

#pragma endregion

public:
#pragma region method

	Obj3DManagerBase();
	virtual ~Obj3DManagerBase();

	// ����(���O
	Obj3D* Find(const string name);
	// ����(�^�O
	Obj3D* FindWithTag(const string tag);

	// �ǉ�
	void Add(Obj3D* Obj);

	// �S�폜
	void AllClear();

	// �폜
	void Delete(Obj3D* Obj);

	//----- �X�V
	void Update();
	void LateUpdate();

	void CheckDestroy();

	// �����蔻��
	void CheckCollider();

	void SetHelper(bool helper) { m_bHelper = helper; }

	// �I�N�c���[
	bool CreateTree(unsigned int Level, Vector3& Min, Vector3& Max);

	// ���X�g
	list<_3DOBJ*>* GetList() { return &m_lstObj; }

#pragma endregion

protected:
#pragma region conceal method

	//----- �����蔻��
	void RegistCollider();
	bool Collision(Collider* A, Collider* B);
	bool SkipCheck(BYTE A, BYTE B);

	//----- �I�N�c���[
	void DeleteTree();
	void RegistTree(_3DOBJ* obj);

#pragma endregion
};


class	Obj3DManager : public	Obj3DManagerBase, public	Singleton<Obj3DManager> {};

inline Obj3DManager* GetObj3DManager(void) {
	return Obj3DManager::Instance();
}