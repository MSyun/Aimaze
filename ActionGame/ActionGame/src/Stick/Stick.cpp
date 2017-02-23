// 杖クラス
// 2017. 1.26	プログラム作成
// Author		SyunMizuno


#include	"Stick.h"
#include	"../Archives/Archives.h"
#include	"../GameFramework/framework/Mesh/SkinMesh/SkinMeshManager.h"



Stick::Stick() {
	SetName("Stick");

	CreateCollider();
	Obb* obb;
	obb = new Obb;
	obb->r[0] = 1.0f;
	obb->r[1] = 2.5f;
	obb->r[2] = 1.0f;
	m_pCollider->SetObb(obb);

	SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Stick")));
	SetWeaponType(0);
	GetStatus().SetMaxLife(10);
	GetStatus().SetPower(4);

	SetShaderType(SHADER_PHONG);
}


Stick::~Stick() {

}