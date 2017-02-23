// カメラ制御クラス
// 2017. 1.30	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../GameFramework/Obj/3D/Obj3D.h"
#include	"../GameFramework/framework/Camera/Camera.h"


class CameraControll	:	public	Obj3D {
private:
	Transform*	m_Player;
	Camera*		m_Camera;

	//----- sin要素
	Vector3	m_vShake;
	bool	m_bShake;		// 揺れ確認
	float	m_fCounter;		// 計測用
	float	m_fDeviation;	// 振れ幅
	float	m_fCyCle;		// 周期（1サイクルする間隔

public:
	CameraControll();
	virtual ~CameraControll() {};

	// 揺れ
	void Shake(float time, float deviat);

private:
	virtual void Init();
	virtual void LateUpdate();
	virtual void Input();

	void ShakeCount();
};