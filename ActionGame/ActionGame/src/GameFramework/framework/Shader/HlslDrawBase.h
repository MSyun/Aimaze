//	HLSLï`âÊånìùÉNÉâÉX
// 2016. 7.19	ÉvÉçÉOÉâÉÄçÏê¨
// 2017. 2.28	âeópÉÅÉìÉoÇí«â¡
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"HlslBase.h"
#include	"../Convert.h"


class	IHlslDrawBase : public	IHlslBase {
protected:
#pragma region variable

	D3DXHANDLE	m_hWorld;
	D3DXHANDLE	m_hCameraView;
	D3DXHANDLE	m_hCameraProj;
	D3DXHANDLE	m_hBlendNum;
	D3DXHANDLE	m_hTexture;
	D3DXHANDLE	m_hLightDir;
	D3DXHANDLE	m_hAmbient;

	// âeóﬁ
	D3DXHANDLE	m_hLightView;
	D3DXHANDLE	m_hLightProj;
	D3DXHANDLE	m_hBias;
	D3DXHANDLE	m_hShadowMap;
	D3DXHANDLE	m_hScaleBias;

	Matrix		m_mtxWorld[4];
	Matrix		m_mtxCameraView;
	Matrix		m_mtxCameraProj;

	// âeóp
	Matrix		m_mtxLightView;
	Matrix		m_mtxLightProj;

#pragma endregion


public:
#pragma region method

	IHlslDrawBase();
	virtual ~IHlslDrawBase();
	virtual HRESULT Load() = 0;
	virtual void Begin();
	void SetDecalMap(LPDIRECT3DTEXTURE9 &pTex);
	void SetAmbient(float Ambient);
	void SetAmbient(const Vector4* pAmbient);
	void SetBlendNum(DWORD BlendNum);
	void SetWorldMatrix(const Matrix* world, int n = 0);
	virtual void SetMatrix();
	virtual void SetLight(const Vector4* pLightDir);
	void SetCameraView(Matrix& view);
	void SetCameraProj(Matrix& proj);

	void SetLightView(Matrix& view);
	void SetLightProj(Matrix& proj);
	void SetBias(float bias);
	void SetShadowMap(LPDIRECT3DTEXTURE9 shadow);
	void SetScaleBias(Matrix& mat);

	virtual void SetSpecular(float Specular) {}
	virtual void SetSpecularPower(float SpecPow) {}
	virtual void SetLightBloomPower(float LightBloomPow) {}
	virtual void SetCamera(const Vector4* pCameraPos) {}
	virtual void SetCamera(const Vector3* pCameraPos) {}

#pragma endregion
};