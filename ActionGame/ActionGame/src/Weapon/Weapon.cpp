// ����N���X
// 2017. 1.24	�v���O�����쐬
// Author		SyunMizuno



#include	"Weapon.h"
#include	"../Archives/Archives.h"
#include	"../ItemBox/ItemBoxManager.h"



Weapon::Weapon() :
	m_pOwner(NULL)
{
}


/*									//
//			�f�X�g���N�^			//
//									*/
Weapon::~Weapon() {

}


/*									//
//				�X�V				//
//									*/
void Weapon::LateUpdate() {
	GetTransform()->SetScale(1.f, 1.f, 1.f);
}


/*									//
//				������ݒ�			//
//									*/
void Weapon::SetOwner(Character* obj) {
	m_pOwner = obj;
	if (!m_pOwner)
		GetItemBoxManager()->RecordItem(this);
}