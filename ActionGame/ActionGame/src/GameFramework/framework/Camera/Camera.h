// カメラクラス
// 2017. 1. 4	プログラム作成
// Author		SyunMizuno

#pragma once


#include	"../Convert.h"
#include	"../../Skydome/Skydome.h"
#include	"../../Obj/3D/Obj3D.h"


class	Camera : public	Obj3D {
protected:
#pragma region variable

	// ビュー関係
	Matrix	m_matView;
	float	m_fLookLength;	// 注視点までの距離

	// プロジェクション関係
	Matrix		m_matProj;
	float		m_fFovy;	// 視野角
	float		m_fAspect;	// アスペクト比
	float		m_fNear;	// 前方クリップ
	float		m_fFar;		// 後方クリップ

	Skydome*	m_pSky;	// スカイボックス

#pragma endregion

public:
#pragma region method

	Camera();
	virtual ~Camera();

	void Set();

	void SetLookLength(float len) { m_fLookLength = len; }
	void AddLookLength(float len) { m_fLookLength += len; }

	Matrix GetView() { return m_matView; }
	Matrix GetProj() { return m_matProj; }

	// プロジェクション
	void SetFovy(float Fovy) { m_fFovy = Fovy; }
	void SetAspect(float Aspect) { m_fAspect = Aspect; }
	void SetNear(float Near) { m_fNear = Near; }
	void SetFar(float Far) { m_fFar = Far; }

	void CreateSky();
	void DeleteSky();

#pragma endregion

protected:
#pragma region conceal method

	virtual void Update();

#pragma endregion
};