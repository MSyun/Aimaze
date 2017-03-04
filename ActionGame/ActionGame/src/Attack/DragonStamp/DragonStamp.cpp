// ドラゴンの踏みつぶしクラス
// 2017. 2.14	プログラム作成
// Author		SyunMizuno


#include	"DragonStamp.h"
#include	"../../Archives/Archives.h"



/*									//
//			コンストラクタ			//
//									*/
DragonStamp::DragonStamp() :
	m_pSmoke(NULL)
{
	SetName("DragonStamp");
	SetTag(Archives::Tag("Enemy/Attack"));

	CreateSmoke();

	Obb* obb = new Obb;
	obb->r[0] = 10.0f;
	obb->r[1] = 3.0f;
	obb->r[2] = 10.0f;
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
DragonStamp::~DragonStamp() {
	SAFE_DELETE(m_pSmoke);
}


/*									//
//				煙作成				//
//									*/
void DragonStamp::CreateSmoke() {
	m_pSmoke = new Particle;

	TParticleParam pp;
	pp.fDuration = 0.1f;
	pp.uMaxParticles = 200;
	pp.bLooping = false;
	pp.coord = EParticleCoord::eCartesian;
	pp.fGravity = 9.8f;
	pp.fStartSpeed = 30.0f;
	pp.fStartSpeed2 = 50.0f;
	pp.fStartSize = 1.0f;
	pp.fStartSize2 = 1.0f;
	pp.fStartRotation = 90.0f;
	pp.fStartRotation2 = 0.0f;
	pp.cStartColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	pp.cStartColor2 = pp.cStartColor;
	pp.fStartLifetime = 1.0f;
	pp.fStartLifetime2 = pp.fStartLifetime;
	pp.uRate = 200;
	pp.fCornAngle = 180.0f;
	pp.fRadius = 0.0f;
	pp.bColorOverLifetime = true;
	pp.cOverLifetime[0] = D3DCOLOR_ARGB(255, 128, 128, 128);
	pp.cOverLifetime[1] = D3DCOLOR_ARGB(0, 128, 128, 128);
	pp.bSizeOverLifetime = true;
	pp.fOverLifetime[0] = 5.0f;
	pp.fOverLifetime[1] = 5.0f;
	lstrcpy(pp.szTexFName, Archives::Texture("Particle/Cloud").c_str());
	m_pSmoke->SetParam(pp);

	m_pSmoke->PostInit();
	m_pSmoke->GetTransform()->SetParent(m_pTransform);
	m_pSmoke->SetName("DragonRush");
}


void DragonStamp::Stamp() {
	Reset();
	m_pSmoke->RePlay();
}