// �A�C�e���}�l�[�W���[�N���X
// 2017. 1.25	�v���O�����쐬
// Author		SyunMizuno


#pragma once


#include	"ItemBox.h"
#include	"../../GameFramework/framework/Singleton/Singleton.h"
#include	<vector>
using namespace std;


class ItemBoxManagerBase {
protected:
	vector<ItemBox*>	m_aItemBox;

public:
#pragma region method

	ItemBoxManagerBase();
	virtual ~ItemBoxManagerBase();

	// �w�萔�쐬
	void Create(int num);

	// ���Z�b�g�i���X�^�[�g���Ɏg�p������
	void Reset();

	// �A�C�e���{�b�N�X�ɓo�^
	void RecordItem(Obj3D* obj);

#pragma endregion

private:
	// ���g�p������
	ItemBox* Search();
};

class ItemBoxManager	:	public	ItemBoxManagerBase,	public	Singleton<ItemBoxManager> {
};

inline ItemBoxManager* GetItemBoxManager() {
	return ItemBoxManager::Instance();
};