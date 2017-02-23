//////////////////////////////////////////////////////////////
// パーティクル
// 2016. 7. 7	プログラム作成
// 変更者		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"../../Obj/3D/Obj3D.h"

/*
	MEMO	:
			構造体内のメンバを違う構造体にする可能性あり
*/


enum EParticleCoord {
	eCartesian = 0,				// 直交座標系(x,y,z)
	eSpherical,					// 球座標系(r,φ,θ)
	eCylindrical,				// 円筒座標系(r,h,θ)

	eMaxParticleCoord
};


struct TParticleParam {
	EParticleCoord	coord;				// 座標系
	float			fDuration;			// 生成周期
	UINT			uMaxParticles;		// 最大パーティクル数
	bool			bLooping;			// 繰り返し
	float			fGravity;			// 重力加速度
	float			fStartSpeed;		// 初速
	float			fStartSpeed2;		// 初速2
	float			fStartSize;			// 初期サイズ
	float			fStartSize2;		// 初期サイズ2
	float			fStartRotation;		// 初期角度
	float			fStartRotation2;	// 初期角度2
	D3DCOLOR		cStartColor;		// 初期カラー
	D3DCOLOR		cStartColor2;		// 初期カラー2
	float			fStartLifetime;		// 寿命
	float			fStartLifetime2;	// 寿命2

	UINT			uRate;				// 1秒あたりのパーティクル生成数
	float			fCornAngle;			// コーン角度
	float			fRadius;			// 半径(180:半球、360:球)
	bool			bColorOverLifetime;	// 生存時間で色を変更(初期カラーが無効になる)
	D3DCOLOR		cOverLifetime[2];	// [0]:開始色、[1]:終了色
	bool			bSizeOverLifetime;	// 生存時間でサイズを変更(初期サイズが無効になる)
	float			fOverLifetime[2];	// [0]:開始サイズ、[1]:終了サイズ
	TCHAR			szTexFName[_MAX_PATH];	// テクスチャファイル名
};


class	Particle : public	Obj3D {
private:
#pragma region struct

	// リスト構造
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

	// 頂点情報
	struct TVertex {
		D3DVECTOR	m_pos;
		D3DVECTOR	m_normal;
		D3DCOLOR	m_diffuse;
		float		m_u, m_v;
	};

#pragma endregion

#pragma region variable

	LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャ
	TCHAR				m_szTexFName[_MAX_PATH];	// テクスチャファイル名

	TParticleParam		m_pp;			// パラメータ
	float				m_fTime;		// パーティクル生成時間
	float				m_fRate;		// パーティクル生成数ワーク
	UINT				m_uParticles;	// 残りパーティクル数

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