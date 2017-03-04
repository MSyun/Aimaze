// 魔法攻撃クラス
// 2017. 1.31	プログラム作成
// Author		SyunMizuno



#include	"AttackMagic.h"
#include	"../../Archives/Archives.h"
#include	"../../GameFramework/framework/Time/Time.h"



const int MAX_PARTICLE = 2;


/*									//
//			コンストラクタ			//
//									*/
AttackMagic::AttackMagic() :
	m_fSpeed(0.5f),
	m_fTime(0.0f),
	m_fDefaultTime(0.0f),
	m_pParticle(NULL)
{
	SetName("AttackMagic");

	m_pParticle = new Particle[MAX_PARTICLE];

	// 0
	TParticleParam pp;
	pp.fDuration = 0.1f;
	pp.uMaxParticles = 100;
	pp.bLooping = true;
	pp.coord = eCartesian;
	pp.fGravity = 0.0f;
	pp.fStartSpeed = 5.0f;
	pp.fStartSpeed2 = 7.0f;
	pp.fStartSize = 0.001f;
	pp.fStartSize2 = pp.fStartSize;
	pp.fStartRotation = 90.0f;
	pp.fStartRotation2 = 0.0f;
	pp.cStartColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	pp.cStartColor2 = pp.cStartColor;
	pp.fStartLifetime = 1.0f;
	pp.fStartLifetime2 = pp.fStartLifetime;
	pp.uRate = 10;
	pp.fCornAngle = 360.0f;
	pp.fRadius = 0.0f;
	pp.bColorOverLifetime = true;
	pp.cOverLifetime[0] = D3DCOLOR_ARGB(255, 200, 0, 128);
	pp.cOverLifetime[1] = D3DCOLOR_ARGB(0, 200, 0, 128);
	pp.bSizeOverLifetime = true;
	pp.fOverLifetime[0] = 3.0f;
	pp.fOverLifetime[1] = 1.0f;
	lstrcpy(pp.szTexFName, Archives::Texture("Particle/Magic").c_str());
	m_pParticle[0].SetParam(pp);
	// 1
	pp.fDuration = 0.1f;
	pp.uMaxParticles = 30;
	pp.bLooping = true;
	pp.coord = eCartesian;
	pp.fGravity = 0.0f;
	pp.fStartSpeed = 1.0f;
	pp.fStartSpeed2 = 2.0f;
	pp.fStartSize = 0.01f;
	pp.fStartSize2 = pp.fStartSize;
	pp.fStartRotation = 90.0f;
	pp.fStartRotation2 = 0.0f;
	pp.cStartColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	pp.cStartColor2 = pp.cStartColor;
	pp.fStartLifetime = 0.5f;
	pp.fStartLifetime2 = pp.fStartLifetime;
	pp.uRate = 10;
	pp.fCornAngle = 360.0f;
	pp.fRadius = 0.0f;
	pp.bColorOverLifetime = true;
	pp.cOverLifetime[0] = D3DCOLOR_ARGB(255, 255, 255, 255);
	pp.cOverLifetime[1] = D3DCOLOR_ARGB(0, 255, 255, 255);
	pp.bSizeOverLifetime = true;
	pp.fOverLifetime[0] = 3.0f;
	pp.fOverLifetime[1] = 1.0f;
	lstrcpy(pp.szTexFName, Archives::Texture("Particle/Magic").c_str());
	m_pParticle[1].SetParam(pp);

	for (int i = 0; i < MAX_PARTICLE; ++i) {
		m_pParticle[i].PostInit();
		m_pParticle[i].GetTransform()->SetParent(m_pTransform);
		m_pParticle[i].SetName("AttackMagicParticle");
	}

	SetTag(Archives::Tag("Enemy/Attack"));

	Sphere* sphere = new Sphere;
	sphere->r = 1.0f;
	m_pCollider->SetSphere(sphere);
	m_pCollider->SetCollisionColor(1.0f, 0.0f, 0.0f, 1.0f);
}


/*									//
//			デストラクタ			//
//									*/
AttackMagic::~AttackMagic() {

}


/*									//
//				更新				//
//									*/
void AttackMagic::Update() {
	float delta = Time::GetDeltaTime();
	m_fTime += delta;

	if (m_fTime >= m_fDefaultTime) {
		SetActive(false);
		return;
	}

	m_pTransform->Translate(0.0f, 0.0f, m_fSpeed * delta, m_pTransform);
}


/*									//
//				射出				//
//									*/
void AttackMagic::Shot() {
	m_fTime = 0.0f;
	SetActive(true);
}