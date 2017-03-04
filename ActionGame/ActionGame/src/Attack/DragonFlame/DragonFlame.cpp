// ドラゴンの炎クラス
// 2017. 2. 7	プログラム作成
// Author		SyunMizuno


#include	"DragonFlame.h"
#include	"../../Archives/Archives.h"
#include	"../../GameFramework/framework/Time/Time.h"



/*									//
//			コンストラクタ			//
//									*/
DragonFlame::DragonFlame() :
	m_pParticle(NULL)
{
	SetName("DragonFlame");
	SetTag(Archives::Tag("Enemy/Attack"));

	m_pParticle = new Particle;

	TParticleParam pp;
	pp.fDuration = 1.0f;
	pp.uMaxParticles = 1000;
	pp.bLooping = true;
	pp.coord = EParticleCoord::eCartesian;
	pp.fGravity = -9.8f;
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
	pp.uRate = 100;
	pp.fCornAngle = 30.0f;
	pp.fRadius = 0.0f;
	pp.bColorOverLifetime = true;
	pp.cOverLifetime[0] = D3DCOLOR_ARGB(255, 255, 102, 0);
	pp.cOverLifetime[1] = D3DCOLOR_ARGB(0, 255, 102, 0);
	pp.bSizeOverLifetime = true;
	pp.fOverLifetime[0] = 3.0f;
	pp.fOverLifetime[1] = 1.0f;
	lstrcpy(pp.szTexFName, Archives::Texture("Particle/Cloud").c_str());
	m_pParticle->SetParam(pp);

	m_pParticle->PostInit();
	m_pParticle->GetTransform()->SetParent(m_pTransform);
	m_pParticle->SetName("DragonFlame");

	Obb* obb = new Obb;
	obb->r[0] = 3.0f;
	obb->r[1] = 30.0f;
	obb->r[2] = 10.0f;
	obb->p_d[1] = 30.0f;
	obb->p_d[2] = 5.0f;
	m_pCollider->SetObb(obb);

	Aabb aabb;
	for (int i = 0; i < 3; i ++) {
		aabb.r[i] = 50.0f;
	}
	m_pCollider->SetTree(aabb);
}


/*									//
//			デストラクタ			//
//									*/
DragonFlame::~DragonFlame() {
	SAFE_DELETE(m_pParticle);
}