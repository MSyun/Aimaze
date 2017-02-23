// スカイドームクラス
// 2017. 1. 4	プログラム作成
// Author		SyunMizuno


#include	"Skydome.h"
#include	"../framework/Mesh/SkinMesh/SkinMeshManager.h"
#include	"../../Archives/Archives.h"
#include	"../framework/Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
Skydome::Skydome() {
	SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Skydome")));

	SetShaderType(_shadertype::SHADER_NONE);
	SetName("SkyDome");
}


/*									//
//			デストラクタ			//
//									*/
Skydome::~Skydome() {
	GetSkinMeshManager()->Delete(Archives::Mesh("Skydome"));
}


/*									//
//				更新				//
//									*/
void Skydome::Update() {
	Obj3D::Update();
}


/*									//
//				描画				//
//									*/
void Skydome::Draw() {
	DWORD Z, L;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &Z);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->GetRenderState(D3DRS_LIGHTING, &L);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	Obj3D::Draw();
	pDevice->SetRenderState(D3DRS_LIGHTING, L);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, Z);
}