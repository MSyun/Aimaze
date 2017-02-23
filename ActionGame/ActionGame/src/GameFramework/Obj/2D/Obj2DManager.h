// 2D�I�u�W�F�N�g�}�l�[�W���N���X
// 2017. 1.4	�v���O�����쐬
// Author		SyunMizuno


#pragma once


#include	"Sprite.h"
#include	"../../framework/Singleton/Singleton.h"
#include	<list>
#include	"../../framework/Collision/Cell.h"

using namespace std;


typedef struct {
	Sprite*						pObj;
	OBJECT_FOR_TREE<Sprite*>*	pTree;
} _2DOBJ;


class Obj2DManagerBase {
protected:
#pragma region variable

	// 4����
	CLiner4TreeManager<Sprite*>*	m_pQuadTree;

	// �I�u�W�F�N�g
	list<_2DOBJ*>	m_lstObj;

	bool	m_bHelper;

#pragma endregion

public:
#pragma region method

	Obj2DManagerBase();
	virtual ~Obj2DManagerBase();

	// ����(���O
	Sprite* Find(const string name);
	// ����(�^�O
	Sprite* FindWithTag(const string tag);

	// �ǉ�
	void Add(Sprite* Obj);

	// �S�폜
	void AllClear();

	// �폜
	void Delete(Sprite* Obj);

	//----- �X�V
	void Update();
	void LateUpdate();

	void CheckDestroy();

	// �����蔻��
	//void CheckCollider();

	//----- �`��
	void Draw();
	void LateDraw();

	void SetHelper(bool helper) { m_bHelper = helper; }

	// �N�A�b�h�c���[
	//bool CreateTree(unsigned int Level, Vector2& Min, Vector2& Max);

#pragma endregion

private:
#pragma region conceal method

	//----- �����蔻��
	//bool Collision();
	//bool SkipCheck(BYTE A, BYTE B);

	//----- �N�A�b�h�c���[
	//void DeleteTree();
	//void RegistTree(_2DOBJ* obj);

#pragma endregion
};


class	Obj2DManager	:	public	Obj2DManagerBase,	public	Singleton<Obj2DManager> {};

inline Obj2DManager* GetObj2DManager(void) {
	return Obj2DManager::Instance();
}