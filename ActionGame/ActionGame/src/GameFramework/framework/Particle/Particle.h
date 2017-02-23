//////////////////////////////////////////////////////////////
// �p�[�e�B�N��
// 2016. 7. 7	�v���O�����쐬
// �ύX��		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"../../Obj/3D/Obj3D.h"

/*
	MEMO	:
			�\���̓��̃����o���Ⴄ�\���̂ɂ���\������
*/


enum EParticleCoord {
	eCartesian = 0,				// �������W�n(x,y,z)
	eSpherical,					// �����W�n(r,��,��)
	eCylindrical,				// �~�����W�n(r,h,��)

	eMaxParticleCoord
};


struct TParticleParam {
	EParticleCoord	coord;				// ���W�n
	float			fDuration;			// ��������
	UINT			uMaxParticles;		// �ő�p�[�e�B�N����
	bool			bLooping;			// �J��Ԃ�
	float			fGravity;			// �d�͉����x
	float			fStartSpeed;		// ����
	float			fStartSpeed2;		// ����2
	float			fStartSize;			// �����T�C�Y
	float			fStartSize2;		// �����T�C�Y2
	float			fStartRotation;		// �����p�x
	float			fStartRotation2;	// �����p�x2
	D3DCOLOR		cStartColor;		// �����J���[
	D3DCOLOR		cStartColor2;		// �����J���[2
	float			fStartLifetime;		// ����
	float			fStartLifetime2;	// ����2

	UINT			uRate;				// 1�b������̃p�[�e�B�N��������
	float			fCornAngle;			// �R�[���p�x
	float			fRadius;			// ���a(180:�����A360:��)
	bool			bColorOverLifetime;	// �������ԂŐF��ύX(�����J���[�������ɂȂ�)
	D3DCOLOR		cOverLifetime[2];	// [0]:�J�n�F�A[1]:�I���F
	bool			bSizeOverLifetime;	// �������ԂŃT�C�Y��ύX(�����T�C�Y�������ɂȂ�)
	float			fOverLifetime[2];	// [0]:�J�n�T�C�Y�A[1]:�I���T�C�Y
	TCHAR			szTexFName[_MAX_PATH];	// �e�N�X�`���t�@�C����
};


class	Particle : public	Obj3D {
private:
#pragma region struct

	// ���X�g�\��
	struct TGrain {
		TGrain*		m_pBack;
		TGrain*		m_pNext;
		Vector3		m_pos;
		Vector3		m_startPos;
		Vector3		m_velocity;
		Vector3		m_accel;
		float		m_size;
		float		m_startSize;
		float		m_angle;
		D3DCOLOR	m_color;
		D3DCOLOR	m_startColor;
		float		m_lifetime;
		float		m_startLifetime;
	};

	// ���_���
	struct TVertex {
		D3DVECTOR	m_pos;
		D3DVECTOR	m_normal;
		D3DCOLOR	m_diffuse;
		float		m_u, m_v;
	};

#pragma endregion

#pragma region variable

	LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`��
	TCHAR				m_szTexFName[_MAX_PATH];	// �e�N�X�`���t�@�C����

	TParticleParam		m_pp;			// �p�����[�^
	float				m_fTime;		// �p�[�e�B�N����������
	float				m_fRate;		// �p�[�e�B�N�����������[�N
	UINT				m_uParticles;	// �c��p�[�e�B�N����

	TGrain*		m_pGrain;
	TGrain*		m_pUse;
	TGrain*		m_pUnused;
	UINT		m_uUse;

	TVertex*	m_pVertex;
	WORD*		m_pIndex;

#pragma endregion

public:
#pragma region method

	Particle();
	virtual ~Particle();

	void GetDefaultParam(TParticleParam& pp);
	void SetParam(TParticleParam& pp);
	void PostInit();
	void Delete(TGrain* p);
	TGrain* New();

	bool GetEnd() { return m_fTime > m_pp.fDuration; }

	void RePlay() {
		m_fTime = 0.0f;
		m_uParticles = m_pp.uMaxParticles;
	}

	void Stop() {
		m_fTime = m_pp.fDuration;
		m_uParticles = 0;
	}

#pragma endregion

private:
#pragma region conceal method

	virtual void Update();
	virtual void LateDraw();

	void UpdateTexture();
	void Release();

#pragma endregion
};