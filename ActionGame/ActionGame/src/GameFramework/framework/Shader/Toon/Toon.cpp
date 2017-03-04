//	トゥーンシェーダ
// 2016. 7.13	プログラム作成
// Author		SyunMizuno


#include	"Toon.h"
#include	<tchar.h>
#include	"../../Utility/System/SystemUtility.h"
#include	"../../Debug/Debug.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
Toon::Toon() :
	m_pTex(NULL)
{}


/*									//
//			デストラクタ			//
//									*/
Toon::~Toon() {
}


/*									//
//				初期化				//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT Toon::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- サポートのバージョン確認
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("このPCのグラフィックボードは対応していません"), _T("Toon"), MB_OK);
	}

	//----- ファイル読み込み
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("Toon").c_str(),
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		&pErr);

	if (FAILED(hr)) {
		MessageBox(NULL, (LPCTSTR)pErr->GetBufferPointer(), _T("ERROR"), MB_OK);
		SAFE_RELEASE(pErr);
		return E_FAIL;
	}
	SAFE_RELEASE(pErr);

	//fxファイル内で宣言している変数のハンドルを取得する
	m_hTechnique = m_pEffect->GetTechniqueByName("TShader");
	m_hWorld = m_pEffect->GetParameterByName(NULL, "matWorld");
	m_hCameraView = m_pEffect->GetParameterByName(NULL, "matView");
	m_hCameraProj = m_pEffect->GetParameterByName(NULL, "matProj");
	m_hBlendNum = m_pEffect->GetParameterByName(NULL, "iBlendNum");
	m_hLightDir = m_pEffect->GetParameterByName(NULL, "vLightDir");
	m_hAmbient = m_pEffect->GetParameterByName(NULL, "vColor");
	m_hTexture = m_pEffect->GetParameterByName(NULL, "tex");
	m_hToonTex = m_pEffect->GetParameterByName(NULL, "toontex");

	m_hLightView = m_pEffect->GetParameterByName(NULL, "matLightView");
	m_hLightProj = m_pEffect->GetParameterByName(NULL, "matLightProj");
	m_hBias = m_pEffect->GetParameterByName(NULL, "fBias");
	m_hScaleBias = m_pEffect->GetParameterByName(NULL, "matScaleBias");
	m_hShadowMap = m_pEffect->GetParameterByName(NULL, "ShadowMap");

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//		トゥーンテクスチャを設定	//
//									*/
void Toon::SetToonMap(LPDIRECT3DTEXTURE9 pTex) {
	if (IsOK()) {
		m_pEffect->SetTexture(m_hToonTex, pTex);
		m_pTex = pTex;
	}
}


/*									//
//				開始				//
//									*/
void Toon::Begin() {
	IHlslDrawBase::Begin();

	if (!m_pTex)
		Debug::LogError("Toon用のテクスチャが設定されていません");
}