//	HLSL描画系統クラス
// 2016. 7.19	プログラム作成
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"HlslBase.h"
#include	"../Convert.h"


class	IHlslDrawBase : public	IHlslBase {
protected:
#pragma region variable

	D3DXHANDLE	m_hWorld;
	D3DXHANDLE	m_hView;
	D3DXHANDLE	m_hProj;
	D3DXHANDLE	m_hBlendNum;
	D3DXHANDLE	m_hTexture;
	D3DXHANDLE	m_hLightDir;
	D3DXHANDLE	m_hAmbient;

	Matrix		m_mtxWorld[4];
	Matrix		m_mtxView;
	Matrix		m_mtxProj;

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

	virtual void SetSpecular(float Specular) {}
	virtual void SetSpecularPower(float SpecPow) {}
	virtual void SetLightBloomPower(float LightBloomPow) {}
	virtual void SetCamera(const Vector4* pCameraPos) {}
	virtual void SetCamera(const Vector3* pCameraPos) {}

#pragma endregion
};