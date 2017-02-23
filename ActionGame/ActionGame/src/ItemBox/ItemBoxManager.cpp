// �A�C�e���}�l�[�W���[�N���X
// 2017. 1.25	�v���O�����쐬
// Author		SyunMizuno


#include	"ItemBoxManager.h"
#include	"../GameFramework/framework/Mesh/SkinMesh/SkinMeshManager.h"
#include	"../Archives/Archives.h"
#include	"../GameFramework/framework/Debug/Debug.h"
#include	"../GameFramework/framework/Texture/TexManager.h"



/*									//
//			�R���X�g���N�^			//
//									*/
ItemBoxManagerBase::ItemBoxManagerBase() {
	GetSkinMeshManager()->Create(Archives::Mesh("ItemBox"));
}


/*									//
//			�f�X�g���N�^			//
//									*/
ItemBoxManagerBase::~ItemBoxManagerBase() {
	GetSkinMeshManager()->Delete(Archives::Mesh("ItemBox"));
}


/*									//
//				�w�萔�쐬			//
//									*/
void ItemBoxManagerBase::Create(int num) {
	ItemBox* pItem;
	for (int i = 0; i < num; ++ i) {
		pItem = new ItemBox;
		pItem->SetActive(false);
		m_aItemBox.push_back(pItem);
	}
}


/*									//
//				���Z�b�g			//
//									*/
void ItemBoxManagerBase::Reset() {
	for (auto it = m_aItemBox.begin(); it != m_aItemBox.end(); ++it) {
		(*it)->SetActive(false);
	}
}


/*									//
//		�A�C�e���{�b�N�X�ɓo�^		//
//									*/
void ItemBoxManagerBase::RecordItem(Obj3D* obj) {
	if (!obj) {
		Debug::LogError("�C���X�^���X������܂���");
		return;
	}

	ItemBox* pItem = Search();

	if (!pItem)	return;

	pItem->SetActive(true);
	pItem->GetTransform()->SetPos(obj->GetTransform()->GetPos());
	pItem->Begin(GetTexManager()->Get(Archives::Texture("Ring")));
	pItem->SetItem(obj);
}


/*									//
//			���g�p������			//
//									*/
ItemBox* ItemBoxManagerBase::Search() {
	// �����􌟍��`��
	for (auto it = m_aItemBox.begin(); it != m_aItemBox.end(); ++it) {
		if (!(*it)->GetActive())	return (*it);
	}

	// �S���g�p���Ƃ��񂯁c
	Debug::LogError("���g�p��ItemBox������܂���");
	return NULL;
}