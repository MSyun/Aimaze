//	ガウスフィルタクラス
// 2016. 4.25	プログラム作成
// 2016. 4.27	追加
// 2016. 4.28	α版完成
// author		SyunMizuno

#include	"Gaussian.h"
#include	<tchar.h>
#include	"../../Debug/Debug.h"
#include	"../../Utility/System/SystemUtility.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Graphic/Graphics.h"


/*
		memo	:
				Invalidate内とRestore内にテクスチャの生成等を書かないと
				サイズ変更したときに対応できないはず（要修正
*/


/*									//
//			コンストラクタ			//
//									*/
Gaussian::Gaussian() :
	m_pOldBackBuffer(NULL),
	m_pOldZBuffer(NULL),
	m_pMapZ(NULL),
	m_pOriginalMap(NULL),
	m_pOriginalMapSurf(NULL),
	m_pXMap(NULL),
	m_pXMapSurf(NULL),
	m_pXYMap(NULL),
	m_pXYMapSurf(NULL),
	m_fDispersion_sq(10.0f)
{
}


/*									//
//			デストラクタ			//
//									*/
Gaussian::~Gaussian() {
	Invalidate();
}


/*									//
//			デバイスロスト			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT Gaussian::Invalidate() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslBase::Invalidate())) {
		Debug::LogError("Invalidateに失敗しました");
		return E_FAIL;
	}

	SAFE_RELEASE(m_pOldBackBuffer);
	SAFE_RELEASE(m_pOldZBuffer);
	SAFE_RELEASE(m_pMapZ);
	SAFE_RELEASE(m_pOriginalMap);
	SAFE_RELEASE(m_pOriginalMapSurf);
	SAFE_RELEASE(m_pXMap);
	SAFE_RELEASE(m_pXMapSurf);
	SAFE_RELEASE(m_pXYMap);
	SAFE_RELEASE(m_pXYMapSurf);

	return S_OK;
}


/*									//
//			デバイスリセット		//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT Gaussian::Restore() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslBase::Restore())) {
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
//				初期化				//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT Gaussian::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- サポートのバージョン確認
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("このPCのグラフィックボードは対応していません"), _T("Gaussian"), MB_OK);
	}

	//----- ファイル読み込み
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("Gaussian").c_str(),
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

	// 重みの設定
	this->UpdateWeight(m_fDispersion_sq*m_fDispersion_sq);

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//			ハンドルの接続			//
//									*/
void Gaussian::ConnectHandle() {
	m_hTechnique = m_pEffect->GetTechniqueByName("TShader");
	m_hWeight = m_pEffect->GetParameterByName(NULL, "weight");
	m_hSrcMap = m_pEffect->GetParameterByName(NULL, "SrcMap");
}


/*									//
//			サイズの設定			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT Gaussian::SetSize(float x, float y) {
	if (!IsOK())	return S_OK;

	MAP_WIDTH = x;
	MAP_HEIGHT = y;

	m_pEffect->SetFloat("MAP_WIDTH", MAP_WIDTH);	// 幅の設定
	m_pEffect->SetFloat("MAP_HEIGHT", MAP_HEIGHT);	// 高さの設定

	 //オフセット値を設定
	D3DXVECTOR4 offset;
	offset.x = 16.0f / MAP_WIDTH;  offset.y = 0.0f / MAP_HEIGHT;
	m_pEffect->SetVector("offsetX", &offset);
	offset.x = 0.0f / MAP_WIDTH;  offset.y = 16.0f / MAP_HEIGHT;
	m_pEffect->SetVector("offsetY", &offset);

	// レンダーターゲット等の作成
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
HRESULT Gaussian::CreateRenderTarget() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// 深度バッファの保存
	if (FAILED(pDevice->CreateDepthStencilSurface(
		(int)MAP_WIDTH,
		(int)MAP_HEIGHT,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pMapZ,
		NULL)))
		return E_FAIL;

	if (FAILED(pDevice->CreateTexture(
		(int)MAP_WIDTH,
		(int)MAP_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pOriginalMap,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pOriginalMap->GetSurfaceLevel(0, &m_pOriginalMapSurf)))
		return E_FAIL;

	// エッジ
	if (FAILED(pDevice->CreateTexture(
		(int)MAP_WIDTH,
		(int)MAP_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pXMap,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pXMap->GetSurfaceLevel(0, &m_pXMapSurf)))
		return E_FAIL;

	// エッジをぼかしたマップ
	if (FAILED(pDevice->CreateTexture(
		(int)MAP_WIDTH,
		(int)MAP_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pXYMap,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pXYMap->GetSurfaceLevel(0, &m_pXYMapSurf)))
		return E_FAIL;

	return S_OK;
}


/*									//
//		テクスチャ作成準備			//
//									*/
void Gaussian::Begin() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	pDevice->GetRenderTarget(0, &m_pOldBackBuffer);
	pDevice->GetDepthStencilSurface(&m_pOldZBuffer);
	pDevice->SetRenderTarget(0, m_pOriginalMapSurf);
	pDevice->SetDepthStencilSurface(m_pMapZ);

	// テクスチャをクリア
	pDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0L);
}


/*									//
//			ボカシ処理開始			//
//									*/
void Gaussian::End() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	m_pEffect->Begin(NULL, 0);

	// ぼかしx
	pDevice->SetRenderTarget(0, m_pXMapSurf);
	DWORD Z;
	pDevice->GetRenderState(D3DRS_ZENABLE, &Z);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	typedef struct { FLOAT p[3]; FLOAT tu, tv; } VERTEX;

	VERTEX Vertex1[4] = {
		//   x      y     z      tu tv
		{{  1.0f, -1.0f, 0.1f},   1, 1,},
		{{ -1.0f, -1.0f, 0.1f},   0, 1,},
		{{ -1.0f,  1.0f, 0.1f},   0, 0,},
		{{  1.0f,  1.0f, 0.1f},   1, 0,},
	};
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	m_pEffect->SetTexture(m_hSrcMap, m_pOriginalMap);
	m_pEffect->BeginPass(0);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Vertex1, sizeof(VERTEX));
	m_pEffect->EndPass();

	// ぼかしy
	pDevice->SetRenderTarget(0, m_pXYMapSurf);
	m_pEffect->SetTexture(m_hSrcMap, m_pXMap);
	m_pEffect->BeginPass(1);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Vertex1, sizeof(VERTEX));
	m_pEffect->EndPass();

	m_pEffect->End();

	// もとに戻す
	pDevice->SetRenderTarget(0, m_pOldBackBuffer);
	pDevice->SetDepthStencilSurface(m_pOldZBuffer);

	pDevice->SetRenderState(D3DRS_ZENABLE, Z);
}


/*									//
//				重みの計算			//
//									*/
void Gaussian::UpdateWeight(float dispersion) {
	DWORD i;

	float total = 0;
	for (i = 0; i < WEIGHT_MUN; i++) {
		float pos = 1.0f + 2.0f * (float)i;
		m_fWeight[i] = expf(-0.5f * (float)(pos*pos) / dispersion);
		total += 2.0f * m_fWeight[i];
	}
	// 規格化
	for (i = 0; i < WEIGHT_MUN; i++)
		m_fWeight[i] /= total;

	// シェーダに変更を通知
	if (IsOK())
		m_pEffect->SetFloatArray(m_hWeight, m_fWeight, WEIGHT_MUN);
}