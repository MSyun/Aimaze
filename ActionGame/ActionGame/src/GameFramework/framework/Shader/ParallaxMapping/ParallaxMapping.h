//	視差マッピングシェーダ
// 2016. 7.16	プログラム作成
// Author		SyunMizuno

#pragma once


#include	"../HlslDrawBase.h"


class	ParallaxMapping : public	IHlslDrawBase {
private:
#pragma region variable

	D3DXHANDLE	m_hEyePos;
	D3DXHANDLE	m_hSpecular;
	D3DXHANDLE	m_hSpecPow;
	D3DXHANDLE	m_hBumpTex;
	D3DXHANDLE	m_hHLimit;

	Vector4	m_vPosCamera;

#pragma endregion

public:
	ParallaxMapping();
	~ParallaxMapping();
	virtual HRESULT Load();

	void SetSpecular(float Specular);
	void SetSpecularPower(float SpecPow);
	void SetMatrix();

	void SetHeightLimit(float limit);
	void SetCamera(const Vector4* pCameraPos);
	void SetCamera(const Vector3* pCameraPos);

	virtual void SetLight(const Vector4* pLightDir);

	// ハンドルの接続
	virtual void ConnectHandle();
};