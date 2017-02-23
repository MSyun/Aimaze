// 武器クラス
// 2017. 1.24	プログラム作成
// Author		SyunMizuno



#include	"Weapon.h"
#include	"../Archives/Archives.h"
#include	"../ItemBox/ItemBoxManager.h"



Weapon::Weapon() :
	m_pOwner(NULL)
{
}


/*									//
//			デストラクタ			//
//									*/
Weapon::~Weapon() {

}


/*									//
//				更新				//
//									*/
void Weapon::LateUpdate() {
	GetTransform()->SetScale(1.f, 1.f, 1.f);
}


/*									//
//				持ち主設定			//
//									*/
void Weapon::SetOwner(Character* obj) {
	m_pOwner = obj;
	if (!m_pOwner)
		GetItemBoxManager()->RecordItem(this);
}