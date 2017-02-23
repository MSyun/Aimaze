// �����O�N���X
// 2016. 1.24	�v���O�����쐬
// Author	:	SyunMizuno

#pragma once


#include	"../GameFramework/Obj/3D/Obj3D.h"


class Ring	:	public	Obj3D {
private:
#pragma region variable

#define		POLYGON_NUM		36	// �|���S����

	struct TRing {
		Point3	Pos;
		Point2	Tex;
		float	Alpha;
	};

	TRing	m_tRing[POLYGON_NUM + 1];

	float	m_fScrollSpeed;	// �X�N���[�����x
	float	m_fRadius;		// ���a
	float	m_fWeight;		// ����
	LPDIRECT3DTEXTURE9	m_pTex;

#pragma endregion

public:
#pragma region method

	Ring();
	virtual ~Ring();

	// �e�N�X�`���ݒ�
	void SetTex(LPDIRECT3DTEXTURE9 tex) { m_pTex = tex; }

	// ���a�ݒ�
	void SetRadius(float r);

	// �����ݒ�
	void SetWeight(float w) { m_fWeight = w; }

	// �X�N���[�����x�ݒ�
	void SetScrollSpeed(float v);

	// ���Z�b�g
	void Reset();

#pragma endregion

private:
	virtual void Update();
	virtual void LateDraw();

	void PreSet();
};