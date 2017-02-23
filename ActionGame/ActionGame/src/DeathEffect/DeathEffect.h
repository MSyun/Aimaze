// ���S���o�N���X
// 2017. 2.15	�v���O�����쐬
// Author		SyunMizuno


#pragma once


#include	"../GameFramework/framework/Particle/Particle.h"
#include	"../GameFramework/framework/Mesh/Quad/Quad.h"



class DeathEffect	:	public	Quad {
private:
	Particle*	m_pSmoke;

	float	m_fScale;
	bool	m_bPlay;

public:
	DeathEffect();
	virtual ~DeathEffect();

	void Play();

protected:
	void CreateSmoke();
	virtual void Update();
};