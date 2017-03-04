//	影用テクスチャクラス
// 2017. 2.26	プログラム作成
// author		SyunMizuno



#include	"ShadowMap.h"
#include	"../../Graphic/Graphics.h"
#include	<tchar.h>
#include	"../../../../Archives/Archives.h"
#include	"../../Utility/System/SystemUtility.h"
#include	"../../Debug/Debug.h"
#include	"../../Screen/Screen.h"
#include	"../../Light/LightManager.h"


ShadowMap::ShadowMap() :
	m_unSizeX(Screen::GetWidth()),
	m_unSizeY(Screen::GetHeight()),
	m_pZTex(NULL),
	m_pZSurf(NULL),
	m_pZ(NULL),
	m_pOldBackBuffer(NULL),
	m_pOldZ(NULL)
{
}

ShadowMap::~ShadowMap() {
	Invalidate();
}


/*									//
//				初期化				//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT ShadowMap::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- サポートのバージョン確認
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("このPCのグラフィックボードは対応していません"), _T("ShadowMap"), MB_OK);
	}

	//----- ファイル読み込み
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("ShadowMap").c_str(),
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

	ConnectHandle();
	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//			ハンドルの接続			//
//									*/
void ShadowMap::ConnectHandle() {
	m_hTechnique = m_pEffect->GetTechniqueByName("TShader");
	m_hWorld = m_pEffect->GetParameterByName(NULL, "matWorld");
	m_hCameraProj = m_pEffect->GetParameterByName(NULL, "matCameraProj");
	m_hCameraView = m_pEffect->GetParameterByName(NULL, "matCameraView");
	m_hBlendNum = m_pEffect->GetParameterByName(NULL, "iBlendNum");
}


/*									//
//			デバイスロスト			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT ShadowMap::Invalidate() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslDrawBase::Invalidate())) {
		Debug::LogError("Invalidateに失敗しました");
		return E_FAIL;
	}

	SAFE_RELEASE(m_pZTex);
	SAFE_RELEASE(m_pZSurf);
	SAFE_RELEASE(m_pZ);
	SAFE_RELEASE(m_pOldBackBuffer);
	SAFE_RELEASE(m_pOldZ);

	return S_OK;
}


/*									//
//			デバイスリセット		//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT ShadowMap::Restore() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslDrawBase::Restore())) {
		Debug::LogError("Restoreに失敗しました");
		return E_FAIL;
	}

	if (FAILED(CreateRenderTarget())) {
		Debug::LogError("CreateRenderTargetに失敗しました");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//		テクスチャサイズの設定		//
//									*/
HRESULT ShadowMap::SetSize(UINT x, UINT y) {
	if (x <= 0 || y <= 0) {
		Debug::LogError("虚数のテクスチャサイズは生成できません");
		return E_FAIL;
	}

	m_unSizeX = x;
	m_unSizeY = y;

	if (FAILED(this->CreateRenderTarget())) {
		Debug::LogError("CreateRenderTargetに失敗しました");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//	テクスチャ・サーフェスの作成	//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT ShadowMap::CreateRenderTarget() {
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// 指定のZ値テクスチャを生成
	if (FAILED(D3DXCreateTexture(
		pDevice,
		m_unSizeX,
		m_unSizeY,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pZTex)))
		return E_FAIL;

	m_pZTex->GetSurfaceLevel(0, &m_pZSurf);	// サーフェイス取得

											// 描画デバイスに定義されているバッファの能力を取得
	IDirect3DSurface9* pSurf;
	pDevice->GetDepthStencilSurface(&pSurf);
	D3DSURFACE_DESC Desc;
	pSurf->GetDesc(&Desc);
	pSurf->Release();

	// 独自深度バッファを作成
	if (FAILED(pDevice->CreateDepthStencilSurface(
		m_unSizeX,
		m_unSizeY,
		Desc.Format,
		Desc.MultiSampleType,
		Desc.MultiSampleQuality,
		FALSE,
		&m_pZ,
		NULL
	)))
		return E_FAIL;

	return S_OK;
}


/*									//
//		シェーダー処理開始			//
//									*/
void ShadowMap::Begin() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// デバイスが持っているバッファを一時保存
	pDevice->GetRenderTarget(0, &m_pOldBackBuffer);
	pDevice->GetDepthStencilSurface(&m_pOldZ);

	// デバイスにZ値テクスチャサーフェイスと深度バッファを設定
	pDevice->SetRenderTarget(0, m_pZSurf);
	pDevice->SetDepthStencilSurface(m_pZ);

	// 各サーフェイスを初期化
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);

	Light* pLight = GetLightManager()->Get("Directional");
	SetCameraView(pLight->GetView());
	SetCameraProj(pLight->GetProj());
	m_pEffect->Begin(NULL, 0);
}


/*									//
//		シェーダー処理終了			//
//									*/
void ShadowMap::End() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	IHlslDrawBase::End();
	// デバイスに元のサーフェイスを戻す
	pDevice->SetRenderTarget(0, m_pOldBackBuffer);
	pDevice->SetDepthStencilSurface(m_pOldZ);

	m_pOldBackBuffer = NULL;
	m_pOldZ = NULL;
}


/*									//
//		影用テクスチャの取得		//
//									*/
LPDIRECT3DTEXTURE9 ShadowMap::GetShadowTex() {
	return m_pZTex;
}