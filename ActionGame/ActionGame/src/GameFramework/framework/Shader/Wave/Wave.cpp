//	波シェーダ
// 2016. 7.15	プログラム作成
// Author		SyunMizuno


#include	"Wave.h"
#include	<tchar.h>
#include	"../../Utility/System/SystemUtility.h"
#include	"../../Debug/Debug.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
Wave::Wave() :
	m_pBumpMapTexture(NULL),
	m_pBumpMapSurface(NULL),
	m_nRenderTargetIndex(0)
{
	for (int i = 0; i < 2; i ++) {
		m_pWaveMapTexture[i] = NULL;
		m_pWaveMapSurface[i] = NULL;
	}
}


/*									//
//			デストラクタ			//
//									*/
Wave::~Wave() {
	this->Invalidate();
}


/*									//
//			デバイスロスト			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT Wave::Invalidate() {
	HRESULT hr = S_OK;
	hr = IHlslBase::Invalidate();

	if (FAILED(hr)) {
		Debug::LogError("Invalidateに失敗しました");
		return hr;
	}

	// 波マップ
	for (int i = 0; i < 2; i ++) {
		SAFE_RELEASE(m_pWaveMapTexture[i]);
		SAFE_RELEASE(m_pWaveMapSurface[i]);
	}

	// 法線マップ
	SAFE_RELEASE(m_pBumpMapTexture);
	SAFE_RELEASE(m_pBumpMapSurface);

	m_nRenderTargetIndex = 0;

	return hr;
}


/*									//
//			デバイスリセット		//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT Wave::Restore() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	if (FAILED(IHlslBase::Restore())) {
		Debug::LogError("Restoreに失敗しました");
		return E_FAIL;
	}

	//----- 波マップ作成
	for (int i = 0; i < 2; i ++) {
		if (FAILED(D3DXCreateTexture(
			pDevice,
			(int)MAP_WIDTH,
			(int)MAP_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_G16R16F,
			D3DPOOL_DEFAULT,
			&m_pWaveMapTexture[i])))
			return E_FAIL;

		if (FAILED(m_pWaveMapTexture[i]->GetSurfaceLevel(0, &m_pWaveMapSurface[i])))
			return E_FAIL;
	}

	// 波マップを黒で初期化
	LPDIRECT3DSURFACE9 OldSurface = NULL;
	pDevice->GetRenderTarget(0, &OldSurface);

	LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
	pDevice->GetDepthStencilSurface(&OldDepthStencilSurface);

	// デプスバッファは使用しないので無効にする
	pDevice->SetDepthStencilSurface(NULL);

	for (int i = 0; i < 2; i ++) {
		pDevice->SetRenderTarget(0, m_pWaveMapSurface[i]);
		pDevice->Clear(
			0L,
			NULL,
			D3DCLEAR_TARGET,
			0x0,
			1.0f,
			0L);
	}

	pDevice->SetRenderTarget(0, OldSurface);
	SAFE_RELEASE(OldSurface);

	pDevice->SetDepthStencilSurface(OldDepthStencilSurface);
	SAFE_RELEASE(OldDepthStencilSurface);

	//----- バンプマップ作製
	if (FAILED(D3DXCreateTexture(
		pDevice,
		(DWORD)MAP_WIDTH,
		(DWORD)MAP_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pBumpMapTexture)))
		return E_FAIL;

	if (FAILED(m_pBumpMapTexture->GetSurfaceLevel(0, &m_pBumpMapSurface)))
		return E_FAIL;

	return S_OK;
}


/*									//
//				初期化				//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT Wave::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- サポートのバージョン確認
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("このPCのグラフィックボードは対応していません"), _T("Wave"), MB_OK);
	}

	// ファイル読み込み
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("Wave").c_str(),
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

	// ハンドルを取得
	m_hTechnique = m_pEffect->GetTechniqueByName("TShader");
	m_hTexOffset = m_pEffect->GetParameterByName(NULL, "TexOffset");
	m_hSpringPower = m_pEffect->GetParameterByName(NULL, "SpringPower");
	m_hAddWavePos = m_pEffect->GetParameterByName(NULL, "AddWavePos");
	m_hAddWaveHeight = m_pEffect->GetParameterByName(NULL, "AddWaveHeight");
	m_hWaveTex = m_pEffect->GetParameterByName(NULL, "tex");

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//			サイズの設定			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT Wave::SetSize(float x, float y) {
	if (!IsOK())	return S_OK;

	MAP_WIDTH = x;
	MAP_HEIGHT = y;

	//オフセット値を設定
	D3DXVECTOR2 Size;
	Size.x = 1.0f / MAP_WIDTH;
	Size.y = 1.0f / MAP_HEIGHT;
	m_pEffect->SetFloatArray(m_hTexOffset, Size, sizeof(D3DXVECTOR2));

	// レンダーターゲット等の作成
	if (FAILED(this->Invalidate())) {
		Debug::LogError("Invalidateに失敗しました");
		return E_FAIL;
	}
	if (FAILED(this->Restore())) {
		Debug::LogError("Invalidateに失敗しました");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//			ばね強度の設定			//
//									*/
void Wave::SetSpringPower(float SpringPower) {
	if (IsOK()) {
		m_pEffect->SetFloat(m_hSpringPower, SpringPower);
	}
}


/*									//
//				波の追加			//
//									*/
void Wave::AddWave(float posx, float posy, float height) {
	if (IsOK()) {
		// 波を追加する位置を設定
		D3DXVECTOR2 AddWavePos = D3DXVECTOR2(posx, posy);
		m_pEffect->SetValue(m_hAddWavePos, &AddWavePos, sizeof(D3DXVECTOR2));

		// 追加する波の高さを設定する
		m_pEffect->SetFloat(m_hAddWaveHeight, height);
	}
}


/*									//
//				波の追加			//
//									*/
LPDIRECT3DTEXTURE9 Wave::Render() {
	if (IsOK()) {
		D3DVIEWPORT9 OldViewport, NewViewport;
		LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

		// ビューポートを波マップのサイズに合わせる
		pDevice->GetViewport(&OldViewport);
		CopyMemory(&NewViewport, &OldViewport, sizeof(D3DVIEWPORT9));
		NewViewport.Width = (DWORD)MAP_WIDTH;
		NewViewport.Height = (DWORD)MAP_HEIGHT;
		pDevice->SetViewport(&NewViewport);


		typedef struct { FLOAT p[3]; FLOAT tu, tv; } VERTEX;

		VERTEX Vertex1[4] = {
			//   x      y     z      tu tv
			{{  1.0f, -1.0f, 0.1f},   1, 1,},
			{{ -1.0f, -1.0f, 0.1f},   0, 1,},
			{{ -1.0f,  1.0f, 0.1f},   0, 0,},
			{{  1.0f,  1.0f, 0.1f},   1, 0,},
		};
		pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);


		// 現在のレンダーターゲットサーフェイスを取得
		LPDIRECT3DSURFACE9 OldSurface = NULL;
		pDevice->GetRenderTarget(0, &OldSurface);

		m_nRenderTargetIndex = 1 - m_nRenderTargetIndex;

		// レンダーターゲットをセットする
		pDevice->SetRenderTarget(0, m_pWaveMapSurface[1 - m_nRenderTargetIndex]);

		LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
		pDevice->GetDepthStencilSurface(&OldDepthStencilSurface);

		// デプスバッファを使用しないので無効にする
		pDevice->SetDepthStencilSurface(NULL);

		// 波マップをテクスチャ―ステージ０にセットする
		m_pEffect->SetTexture(m_hWaveTex, m_pWaveMapTexture[m_nRenderTargetIndex]);

		// 波マップの更新
		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(0);

		// 2Dスプライトの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Vertex1, sizeof(VERTEX));

		m_pEffect->EndPass();

		// 更新した波マップを参照し水面を凸凹にしてレンダリングする
		pDevice->SetRenderTarget(0, m_pBumpMapSurface);

		m_pEffect->SetTexture(m_hWaveTex, m_pWaveMapTexture[1 - m_nRenderTargetIndex]);

		m_pEffect->BeginPass(1);

		// 2Dスプライトの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Vertex1, sizeof(VERTEX));

		m_pEffect->EndPass();
		m_pEffect->End();

		// 戻す
		pDevice->SetRenderTarget(0, OldSurface);
		SAFE_RELEASE(OldSurface);

		pDevice->SetDepthStencilSurface(OldDepthStencilSurface);
		SAFE_RELEASE(OldDepthStencilSurface);

		pDevice->SetViewport(&OldViewport);
	}

	return m_pBumpMapTexture;
}