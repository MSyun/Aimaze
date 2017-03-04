// ドラゴンの炎クラス
// 2017. 2. 7	プログラム作成
// Author		SyunMizuno


#include	"DragonBreath.h"
#include	"../../Archives/Archives.h"
#include	"../../GameFramework/framework/Mesh/SkinMesh/SkinMeshManager.h"
#include	"../../GameFramework/framework/Time/Time.h"



/*									//
//			コンストラクタ			//
//									*/
DragonBreath::DragonBreath() :
	m_vMove(0.0f, 0.0f, 0.0f),
	m_pSmoke(NULL),
	m_pLocus(NULL),
	m_bSmoke(false)
{
	SetName("DragonBreath");
	SetTag(Archives::Tag("Enemy/Attack"));

	SetModel(GetSkinMeshManager()->Get(Archives::Mesh("ItemBox")));

	CreateLocus();
	CreateSmoke();

	Sphere* sphere = new Sphere;
	sphere->r = 3.0f;
	m_pCollider->SetSphere(sphere);
}


/*									//
//			デストラクタ			//
//									*/
DragonBreath::~DragonBreath() {
	SAFE_DELETE(m_pSmoke);
	SAFE_DELETE(m_pLocus);
}


/*									//
//				軌跡作成			//
//									*/
void DragonBreath::CreateLocus() {
	// 軌跡
	m_pLocus = new Particle;

	TParticleParam pp;
	pp.fDuration = 1.0f;
	pp.uMaxParticles = 50;
	pp.bLooping = true;
	pp.coord = EParticleCoord::eCartesian;
	pp.fGravity = -5.0f;
	pp.fStartSpeed = 2.0f;
	pp.fStartSpeed2 = 5.0f;
	pp.fStartSize = 0.001f;
	pp.fStartSize2 = 1.0f;
	pp.fStartRotation = 90.0f;
	pp.fStartRotation2 = 0.0f;
	pp.cStartColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	pp.cStartColor2 = pp.cStartColor;
	pp.fStartLifetime = 1.0f;
	pp.fStartLifetime2 = pp.fStartLifetime;
	pp.uRate = 20;
	pp.fCornAngle = 180.0f;
	pp.fRadius = 0.0f;
	pp.bColorOverLifetime = true;
	pp.cOverLifetime[0] = D3DCOLOR_ARGB(255, 255, 69, 0);
	pp.cOverLifetime[1] = D3DCOLOR_ARGB(0, 255, 69, 0);
	pp.bSizeOverLifetime = true;
	pp.fOverLifetime[0] = 3.0f;
	pp.fOverLifetime[1] = 1.0f;
	lstrcpy(pp.szTexFName, Archives::Texture("Particle/Cloud").c_str());
	m_pLocus->SetParam(pp);

	m_pLocus->PostInit();
	m_pLocus->Stop();
	m_pLocus->SetName("DragonBreath");
}


/*									//
//				煙作成				//
//									*/
void DragonBreath::CreateSmoke() {
	// 煙
	m_pSmoke = new Particle;

	TParticleParam pp;
	pp.fDuration = 0.1f;
	pp.uMaxParticles = 50;
	pp.bLooping = false;
	pp.coord = EParticleCoord::eCartesian;
	pp.fGravity = 0.0f;
	pp.fStartSpeed = 10.0f;
	pp.fStartSpeed2 = 5.0f;
	pp.fStartSize = 0.001f;
	pp.fStartSize2 = 1.0f;
	pp.fStartRotation = 90.0f;
	pp.fStartRotation2 = 180.0f;
	pp.cStartColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	pp.cStartColor2 = pp.cStartColor;
	pp.fStartLifetime = 0.5f;
	pp.fStartLifetime2 = pp.fStartLifetime;
	pp.uRate = 200;
	pp.fCornAngle = 180.0f;
	pp.fRadius = 1.0f;
	pp.bColorOverLifetime = true;
	pp.cOverLifetime[0] = D3DCOLOR_ARGB(255, 255, 69, 0);
	pp.cOverLifetime[1] = D3DCOLOR_ARGB(0, 255, 69, 0);
	pp.bSizeOverLifetime = true;
	pp.fOverLifetime[0] = 3.0f;
	pp.fOverLifetime[1] = 1.0f;
	lstrcpy(pp.szTexFName, Archives::Texture("Particle/Cloud").c_str());
	m_pSmoke->SetParam(pp);

	m_pSmoke->PostInit();
	m_pSmoke->Stop();
	m_pSmoke->GetTransform()->SetPos(0.0f, 3.0f, -50.0f);
	m_pSmoke->SetName("DragonBreath");
}


/*									//
//				射出				//
//									*/
void DragonBreath::Shot(Transform* shooter, float speed) {
	m_vMove = shooter->GetForward() * speed;
	m_pTransform->SetEularAngles(0.0f, 0.0f, 0.0f);
	m_pTransform->SetLocalPos(shooter->GetPos());
	m_bSmoke = false;
	m_pLocus->RePlay();
	SetActive(true);
}


/*									//
//				更新				//
//									*/
void DragonBreath::Update() {
	AttackCollisionSpace::Update();

	m_pTransform->Translate(m_vMove * Time::GetDeltaTime(), m_pTransform);
	m_pLocus->GetTransform()->SetPos(m_pTransform->GetPos());

	if (!m_bSmoke && m_pTransform->GetPos().y <= 0.0f) {
		Burn();
	}
}


/*									//
//				更新				//
//									*/
void DragonBreath::Burn() {
	m_pSmoke->GetTransform()->SetPos(m_pTransform->GetPos());
	m_pSmoke->RePlay();
	m_bSmoke = true;
	m_pLocus->Stop();
	SetActive(false);
}


/*									//
//			あたり判定				//
//									*/
void DragonBreath::OnCollisionStay(Obj3D* obj) {
	AttackCollisionSpace::OnCollisionStay(obj);

	Burn();
}