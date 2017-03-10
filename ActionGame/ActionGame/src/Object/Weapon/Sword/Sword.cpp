// 剣クラス
// 2017. 1.26	プログラム作成
// Author		SyunMizuno



#include	"Sword.h"
#include	"../../../Archives/Archives.h"
#include	"../../../GameFramework/framework/Mesh/SkinMesh/SkinMeshManager.h"



Sword::Sword() {
	SetName("Sword");

	//CreateCollider();
	//Obb* obb;
	//obb = new Obb;
	//obb->r[0] = 1.0f;
	//obb->r[1] = 2.5f;
	//obb->r[2] = 1.0f;
	//obb->p_d.y = 2.0f;
	//m_pCollider->SetObb(obb);

	SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Sword")));
	SetWeaponType(0);
	GetStatus().SetMaxLife(10);
	GetStatus().SetPower(4);

	SetShaderType(SHADER_PHONG);
}


Sword::~Sword()
{
}