// 死亡演出クラス
// 2017. 2. 7	プログラム作成
// Author		SyunMizuno


#include	"DeathEffect.h"
#include	"../../GameFramework/framework/Texture/TexManager.h"
#include	"../../Archives/Archives.h"



/*									//
//			コンストラクタ			//
//									*/
DeathEffect::DeathEffect() :
	m_pSmoke(NULL),
	m_fScale(0.0f),
	m_bPlay(false)
{
	SetTex(GetTexManager()->Get(Archives::Texture("DeathEffect")));
	CreateSmoke();
}


/*									//
//			デストラクタ			//
//									*/
DeathEffect::~DeathEffect() {
	SAFE_DELETE(m_pSmoke);
}


/*									//
//				更新				//
//									*/
void DeathEffect::Update() {
	Quad::Update();

	if (!m_bPlay)	return;

	m_fScale += Time::GetDeltaTime() * 20.0f;

	m_pTransform->SetScale(m_fScale, m_fScale, m_fScale);
	m_pSmoke->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

	if (m_fScale >= 10.0f) {
		m_bPlay = false;
		SetColor(Color(1.0f, 1.0f, 1.0f, 0.0f));
		m_pSmoke->Stop();
	}
}


/*									//
//				開始				//
//									*/
void DeathEffect::Play() {
	m_bPlay = true;
	m_pTransform->SetScale(0.1f, 0.1f, 0.1f);
	m_pSmoke->RePlay();
}


/*									//
//				煙作成				//
//									*/
void DeathEffect::CreateSmoke() {
	m_pSmoke = new Particle;

	TParticleParam pp;
	pp.fDuration = 1.0f;
	pp.uMaxParticles = 30;
	pp.bLooping = false;
	pp.coord = eCartesian;
	pp.fGravity = 9.8f;
	pp.fStartSpeed = 5.0f;
	pp.fStartSpeed2 = 15.0f;
	pp.fStartSize = 0.01f;
	pp.fStartSize2 = pp.fStartSize;
	pp.fStartRotation = 0.0f;
	pp.fStartRotation2 = 90.0f;
	pp.cStartColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	pp.cStartColor2 = pp.cStartColor;
	pp.fStartLifetime = 1.0f;
	pp.fStartLifetime2 = pp.fStartLifetime;
	pp.uRate = 60;
	pp.fCornAngle = 360.0f;
	pp.fRadius = 0.0f;
	pp.bColorOverLifetime = true;
	pp.cOverLifetime[0] = D3DCOLOR_ARGB(255, 50, 255, 255);
	pp.cOverLifetime[1] = D3DCOLOR_ARGB(0, 0, 255, 255);
	pp.bSizeOverLifetime = true;
	pp.fOverLifetime[0] = 5.0f;
	pp.fOverLifetime[1] = 1.0f;
	lstrcpy(pp.szTexFName, Archives::Texture("Particle/Magic").c_str());
	m_pSmoke->SetParam(pp);
	m_pSmoke->PostInit();
	m_pSmoke->Stop();
	m_pSmoke->GetTransform()->SetParent(m_pTransform);
}