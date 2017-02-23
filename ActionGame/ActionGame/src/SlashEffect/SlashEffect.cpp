// ヒットエフェクトクラス
// 2017. 2.15	プログラム作成
// Author		SyunMizuno


#include	"SlashEffect.h"
#include	"../Archives/Archives.h"




SlashEffect::SlashEffect() {
	SetName("SlashEffect");

	TParticleParam pp;
	pp.fDuration = 0.1f;
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
	pp.cOverLifetime[0] = D3DCOLOR_ARGB(255, 255, 69, 0);
	pp.cOverLifetime[1] = D3DCOLOR_ARGB(0, 255, 69, 0);
	pp.bSizeOverLifetime = true;
	pp.fOverLifetime[0] = 1.0f;
	pp.fOverLifetime[1] = 1.0f;
	lstrcpy(pp.szTexFName, Archives::Texture("Particle/Magic").c_str());
	SetParam(pp);
	PostInit();
	Stop();
}

SlashEffect::~SlashEffect()
{
}