// ���
// 2017. 2. 10	�v���O�����쐬
// Author		SyunMizuno


#include	"Allow.h"
#include	"../Archives/Archives.h"
#include	"../GameFramework/framework/Mesh/SkinMesh/SkinMeshManager.h"
#include	"../GameFramework/framework/Scene/SceneManagerWin.h"



/*									//
//			�R���X�g���N�^			//
//									*/
Allow::Allow() {
	SetName("Allow");
	SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Allow")));
	SetTag(Archives::Tag("Allow"));
}


/*									//
//			�f�X�g���N�^			//
//									*/
Allow::~Allow() {

}


/*									//
//				������				//
//									*/
void Allow::Init() {
	Obj3D::Init();

	m_pTransform->SetPos(-35.0f, 3.0f, 81.0f);
	m_pTransform->Rotate(0.0f, 0.0f, 90.0f);

	CreateCollider();
	Sphere* sphere = new Sphere;
	sphere->r = 25.0f;
	sphere->p_d[1] = 13.0f;
	m_pCollider->SetSphere(sphere);
	Aabb tree;
	tree.r[0] = 10.0f;
	tree.r[1] = 5.0f;
	tree.r[2] = 25.0f;
	tree.p_d[1] = 13.0f;
	m_pCollider->SetTree(tree);
}


/*									//
//				�X�V				//
//									*/
void Allow::Update() {
	Obj3D::Update();

	m_pTransform->Rotate(0.0f, 90.0f * Time::GetDeltaTime(), 0.0f);
}


/*									//
//				�����蔻��			//
//									*/
void Allow::OnCollisionTrigger(Obj3D* obj) {
	Obj3D::OnCollisionTrigger(obj);

	GetSceneManagerWins()->ChangeLoadCommand("Boss");
}