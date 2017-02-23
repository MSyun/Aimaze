// アイテムの箱クラス
// 2016. 8. 4	プログラム作成
// Author	:	SyunMizuno


#include	"ItemBox.h"
#include	"../GameFramework/framework/Mesh/SkinMesh/SkinMeshManager.h"
#include	"../Archives/Archives.h"
#include	"../GameFramework/framework/Texture/TexManager.h"
#include	"../GameFramework/framework/Time/Time.h"



/*									//
//			コンストラクタ			//
//									*/
ItemBox::ItemBox():
	m_pRing(NULL),
	m_pBox(NULL)
{
	SetName("ItemBox");
	SetTag(Archives::Tag("ItemBox"));
	CreateCollider();
	Sphere* sphere = new Sphere;
	sphere->r = 1.5f;
	m_pCollider->SetSphere(sphere);

	// Ring
	m_pRing = new Ring;
	m_pRing->SetScrollSpeed(1.0f);
	m_pRing->SetWeight(0.5f);
	m_pRing->SetRadius(1.5f);
	m_pRing->SetTex(GetTexManager()->Get(Archives::Texture("Ring")));

	// Box
	m_pBox = new Obj3D;
	m_pBox->SetShaderType(SHADER_PHONG);
	m_pBox->SetModel(GetSkinMeshManager()->Get(Archives::Mesh("ItemBox")));

	Transform* transform = GetTransform();

	m_pRing->GetTransform()->SetParent(transform);
	m_pBox->GetTransform()->SetParent(transform);

	m_pRing->GetTransform()->SetEularAngles(0.0f, 0.0f, 45.0f);
}


/*									//
//			デストラクタ			//
//									*/
ItemBox::~ItemBox() {

}


/*									//
//				更新				//
//									*/
void ItemBox::Update() {
	float delta = Time::GetDeltaTime();
	m_pRing->GetTransform()->Rotate(0.0f, 30.0f * delta, 0.0f);

	if (m_pItem) {
		Transform* transform = m_pItem->GetTransform();
		transform->SetScale(0.3f, 0.3f, 0.3f);
		transform->Rotate(0.0f, 30.0f * delta, 0.0f, true);
	}
}


/*									//
//				開始				//
//									*/
void ItemBox::Begin(LPDIRECT3DTEXTURE9 tex) {
	m_pRing->SetTex(tex);
	m_pRing->Reset();
}


/*									//
//			アイテムの登録			//
//									*/
void ItemBox::SetItem(Obj3D* item) {
	if (m_pItem) {
		m_pItem->GetTransform()->RemoveParent();
		m_pItem->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	}
	m_pItem = item;

	if (!item)	return;

	m_pItem->GetTransform()->SetParent(GetTransform());
	m_pItem->GetTransform()->SetEularAngles(0.0f, 0.0f, 45.0f);

}


/*									//
//				中身を渡す			//
//									*/
Obj3D* ItemBox::Transfer() {
	Obj3D* Item = m_pItem;

	SetItem(NULL);
	SetActive(false);

	return Item;
}