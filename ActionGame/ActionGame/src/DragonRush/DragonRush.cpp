// ドラゴンの突っ込みクラス
// 2017. 2. 9	プログラム作成
// Author		SyunMizuno


#include	"DragonRush.h"
#include	"../Archives/Archives.h"
#include	"../GameFramework/framework/Texture/TexManager.h"



/*									//
//			コンストラクタ			//
//									*/
DragonRush::DragonRush() :
	m_pSmoke(NULL),
	m_pLocus(NULL)
{
	SetName("DragonRush");
	SetTag(Archives::Tag("Enemy/Attack"));

	CreateSmoke();
	CreateLocus();

	Obb* obb = new Obb;
	obb->r[0] = 3.0f;
	obb->r[1] = 10.0f;
	obb->r[2] = 3.0f;
	Aabb aabb;
	aabb.r[0] = 15.0f;
	aabb.r[1] = 15.0f;
	aabb.r[2] = 15.0f;
	m_pCollider->SetTree(aabb);
	m_pCollider->SetObb(obb);
}


/*									//
//			デストラクタ			//
//									*/
DragonRush::~DragonRush() {
	SAFE_DELETE(m_pSmoke);
	SAFE_DELETE(m_pLocus);
	GetTexManager()->Delete(Archives::Texture("Dragon/Slash"));
}


/*									//
//				煙作成				//
//									*/
void DragonRush::CreateSmoke() {
	m_pSmoke = new Particle;

	TParticleParam pp;
	pp.fDuration = 0.5f;
	pp.uMaxParticles = 200;
	pp.bLooping = true;
	pp.coord = EParticleCoord::eCartesian;
	pp.fGravity = 9.8f;
	pp.fStartSpeed = 30.0f;
	pp.fStartSpeed2 = 50.0f;
	pp.fStartSize = 0.001f;
	pp.fStartSize2 = 1.0f;
	pp.fStartRotation = 90.0f;
	pp.fStartRotation2 = 0.0f;
	pp.cStartColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	pp.cStartColor2 = pp.cStartColor;
	pp.fStartLifetime = 1.0f;
	pp.fStartLifetime2 = pp.fStartLifetime;
	pp.uRate = 20;
	pp.fCornAngle = 30.0f;
	pp.fRadius = 0.0f;
	pp.bColorOverLifetime = true;
	pp.cOverLifetime[0] = D3DCOLOR_ARGB(255, 128, 128, 128);
	pp.cOverLifetime[1] = D3DCOLOR_ARGB(0, 128, 128, 128);
	pp.bSizeOverLifetime = true;
	pp.fOverLifetime[0] = 1.0f;
	pp.fOverLifetime[1] = 1.0f;
	lstrcpy(pp.szTexFName, Archives::Texture("Particle/Cloud").c_str());
	m_pSmoke->SetParam(pp);

	m_pSmoke->PostInit();
	m_pSmoke->GetTransform()->SetParent(m_pTransform);
	m_pSmoke->SetName("DragonRush");
}


/*									//
//				軌跡作成			//
//									*/
void DragonRush::CreateLocus() {
	m_pLocus = new Locus;
	m_pLocus->SetTex(GetTexManager()->Get(Archives::Texture("Dragon/Slash")));
	m_pLocus->SetUp(-3.0f);
	m_pLocus->SetDown(3.0f);
	m_pLocus->SetRevision(false);
	m_pLocus->SetGhostSpeed(1.0f);
}


/*									//
//				更新				//
//									*/
void DragonRush::Update() {
	AttackCollisionSpace::Update();

	if(m_pLocus)
		m_pLocus->Update();
}


/*									//
//				描画				//
//									*/
void DragonRush::LateDraw() {
	AttackCollisionSpace::LateDraw();

	if(m_pLocus)
		m_pLocus->Draw();
}


/*									//
//				記録				//
//									*/
void DragonRush::Record() {
	if (m_pLocus)
		m_pLocus->SetNewPos(m_pTransform->GetWorld());
	if (m_pSmoke)
		m_pSmoke->RePlay();
}


/*									//
//				記録終了			//
//									*/
void DragonRush::End() {
	if (m_pLocus)
		m_pLocus->End();
	if (m_pSmoke)
		m_pSmoke->Stop();
}