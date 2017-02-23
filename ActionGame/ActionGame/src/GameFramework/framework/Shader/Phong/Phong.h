//	フォンクラス
// 2016. 3.13	プログラム作成
// 2016. 4.28	ブルームをメンバに追加
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"../HlslDrawBase.h"


class	Phong : public	IHlslDrawBase {
private:
#pragma region variable

	D3DXHANDLE	m_hEyePos;
	D3DXHANDLE	m_hSpecular;
	D3DXHANDLE	m_hSpecPow;
	D3DXHANDLE	m_hLightBloomPow;

	Vector4	m_vPosCamera;

#pragma endregion

public:
#pragma region method

	Phong();
	virtual ~Phong();
	virtual HRESULT Load();
	virtual void SetMatrix();
	virtual void SetLight(const Vector4* pLightDir);

	virtual void SetSpecular(float Specular);
	virtual void SetSpecularPower(float SpecPow);
	virtual void SetLightBloomPower(float LightBloomPow);
	virtual void SetCamera(const Vector4* pCameraPos);
	virtual void SetCamera(const Vector3* pCameraPos);

#pragma endregion
};