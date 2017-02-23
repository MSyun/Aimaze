// �X�J�C�h�[���N���X
// 2017. 1. 4	�v���O�����쐬
// Author		SyunMizuno


#include	"Skydome.h"
#include	"../framework/Mesh/SkinMesh/SkinMeshManager.h"
#include	"../../Archives/Archives.h"
#include	"../framework/Graphic/Graphics.h"


/*									//
//			�R���X�g���N�^			//
//									*/
Skydome::Skydome() {
	SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Skydome")));

	SetShaderType(_shadertype::SHADER_NONE);
	SetName("SkyDome");
}


/*									//
//			�f�X�g���N�^			//
//									*/
Skydome::~Skydome() {
	GetSkinMeshManager()->Delete(Archives::Mesh("Skydome"));
}


/*									//
//				�X�V				//
//									*/
void Skydome::Update() {
	Obj3D::Update();
}


/*									//
//				�`��				//
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