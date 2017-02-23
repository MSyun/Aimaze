// クロスフィルタークラス
// 2016. 3.19	プログラム作成
// author		SyunMizuno


#include	"CrossFilter.h"
#include	<tchar.h>
#include	"../../Utility/System/SystemUtility.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Debug/Debug.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
CrossFilter::CrossFilter() :
	m_pOldBackBuffer(NULL),
	m_pOldZBuffer(NULL),
	m_pMapZ(NULL),
	m_pOriginalMap(NULL),
	m_pOriginalMapSurf(NULL),
	m_pTexSceneScaled(NULL),
	m_pSurfSceneScaled(NULL),
	m_pTexBrightPass(NULL),
	m_pSurfBrightPass(NULL),
	m_pTexStarSource(NULL),
	m_pSurfStarSource(NULL)
{
	for (int i = 0; i < NUM_STAR_TEXTURES; i ++) {
		m_apTexStar[i] = NULL;
		m_apSurfStar[i] = NULL;
	}
}


/*									//
//			デストラクタ			//
//									*/
CrossFilter::~CrossFilter() {
	Invalidate();
}


/*									//
//			デバイスロスト			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT CrossFilter::Invalidate() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslBase::Invalidate())) {
		Debug::LogError("Invalidateに失敗しました");
		return E_FAIL;
	}

	SAFE_RELEASE(m_pOldZBuffer);
	SAFE_RELEASE(m_pMapZ);
	SAFE_RELEASE(m_pOriginalMap);
	SAFE_RELEASE(m_pOriginalMapSurf);
	SAFE_RELEASE(m_pTexSceneScaled);
	SAFE_RELEASE(m_pSurfSceneScaled);
	SAFE_RELEASE(m_pTexBrightPass);
	SAFE_RELEASE(m_pSurfBrightPass);
	SAFE_RELEASE(m_pTexStarSource);
	SAFE_RELEASE(m_pSurfStarSource);
	for (int i = 0; i < NUM_STAR_TEXTURES; i ++) {
		SAFE_RELEASE(m_apTexStar[i]);
		SAFE_RELEASE(m_apSurfStar[i]);
	}

	return S_OK;
}


/*									//
//			デバイスリセット		//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT CrossFilter::Restore() {
	if (!IsOK())	return S_OK;

	if (FAILED(IHlslBase::Restore())) {
		Debug::LogError("Restoreに失敗しました");
		return E_FAIL;
	}

	if (FAILED(CreateRenderTarget())) {
		Debug::LogError("Restoreに失敗しました");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//				初期化				//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT CrossFilter::Load() {
	//----- 変数宣言
	HRESULT hr;
	D3DCAPS9 caps;

	//----- サポートのバージョン確認
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) ||
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("このPCのグラフィックボードは対応していません"), _T("CrossFilter"), MB_OK);
	}

	//----- ファイル読み込み
	LPD3DXBUFFER pErr = NULL;	// エラー用
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("CrossFilter").c_str(),
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

	// fxファイル内で宣言している変数のハンドルを取得する
	m_hTechnique = m_pEffect->GetTechniqueByName("tDownScale4x4");

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//			サイズの設定			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT CrossFilter::SetSize(float x, float y) {
	if( !IsOK() )	return S_OK;

	m_dwWidth	= (DWORD)x;
	m_dwHeight	= (DWORD)y;

	// レンダーターゲット等の作成
	if (FAILED(this->CreateRenderTarget())) {
		Debug::LogError("テクスチャ・サーフェイスの作成に失敗しました");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//	テクスチャ・サーフェスの作成	//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT CrossFilter::CreateRenderTarget() {
	if (!IsOK())	return S_OK;

	HRESULT hr;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();


	// 深度バッファの保存
	hr = pDevice->CreateDepthStencilSurface(
		m_dwWidth,
		m_dwHeight,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pMapZ,
		NULL);
	if (FAILED(hr))	return hr;

	//----- シーンを描画するHDR
	hr = pDevice->CreateTexture(
		m_dwWidth,
		m_dwHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A16B16G16R16F,
		D3DPOOL_DEFAULT,
		&m_pOriginalMap,
		NULL);
	if (FAILED(hr)) return hr;

	hr = m_pOriginalMap->GetSurfaceLevel(0, &m_pOriginalMapSurf);
	if (FAILED(hr)) return hr;

	//----- 縮小バッファ
	// 縮小バッファの基本サイズ（FBを切り捨てで4の倍数の大きさ）
	m_dwCropWidth = m_dwWidth - m_dwWidth % 4;
	m_dwCropHeight = m_dwHeight - m_dwHeight % 4;

	hr = pDevice->CreateTexture(
		m_dwCropWidth / 4,
		m_dwCropHeight / 4,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A16B16G16R16F,
		D3DPOOL_DEFAULT,
		&m_pTexSceneScaled,
		NULL);
	if (FAILED(hr)) return hr;

	hr = m_pTexSceneScaled->GetSurfaceLevel(0, &m_pSurfSceneScaled);
	if (FAILED(hr)) return hr;

	//----- 輝度の抽出
	hr = pDevice->CreateTexture(
		m_dwCropWidth / 4 + 2,
		m_dwCropHeight / 4 + 2,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexBrightPass,
		NULL);
	if (FAILED(hr)) return hr;

	hr = m_pTexBrightPass->GetSurfaceLevel(0, &m_pSurfBrightPass);
	if (FAILED(hr)) return hr;
	// ふちを黒く塗りつぶしておく
	pDevice->ColorFill(m_pSurfBrightPass, NULL, D3DCOLOR_ARGB(0, 0, 0, 0));

	//----- ちらつかないようにぼかすためのテクスチャ
	hr = pDevice->CreateTexture(
		m_dwCropWidth / 4 + 2,
		m_dwCropHeight / 4 + 2,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexStarSource,
		NULL);
	if (FAILED(hr)) return hr;

	hr = m_pTexStarSource->GetSurfaceLevel(0, &m_pSurfStarSource);
	if (FAILED(hr)) return hr;
	// ふちを黒く塗りつぶしておく
	pDevice->ColorFill(m_pSurfStarSource, NULL, D3DCOLOR_ARGB(0, 0, 0, 0));

	//----- 光芒のためのテクスチャ
	for (int i = 0; i < NUM_STAR_TEXTURES; i++) {
		hr = pDevice->CreateTexture(
			m_dwCropWidth / 4,
			m_dwCropHeight / 4,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F,
			D3DPOOL_DEFAULT,
			&m_apTexStar[i],
			NULL);
		if (FAILED(hr)) return hr;

		hr = m_apTexStar[i]->GetSurfaceLevel(0, &m_apSurfStar[i]);
		if (FAILED(hr)) return hr;
	}

	return hr;
}


/*									//
//		シェーダー処理開始			//
//									*/
void CrossFilter::Begin() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();


	// 描画先の切り替え
	pDevice->GetRenderTarget(0, &m_pOldBackBuffer);
	pDevice->GetDepthStencilSurface(&m_pOldZBuffer);
	pDevice->SetRenderTarget(0, m_pOriginalMapSurf);
	pDevice->SetDepthStencilSurface(m_pMapZ);

	// レンダリングターゲットのクリア
	pDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0L);
	// アルファブレンドを中止
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


/*									//
//		シェーダー処理終了			//
//									*/
void CrossFilter::End() {
	if (!IsOK())	return;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// 縮小テクスチャ作成
	this->Scene_To_SceneScaled();
	// アルファブレンドを行う
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetDepthStencilSurface(NULL);
	// 輝度抽出
	this->SceneScaled_To_BrightPass();
	// テクスチャ引き伸ばし
	this->BrightPass_To_StarSource();
	// 合成
	this->RenderStar();

	// レンダリングターゲットを戻す
	pDevice->SetRenderTarget(0, m_pOldBackBuffer);	// 初期サーフェス
	pDevice->SetDepthStencilSurface(m_pOldZBuffer);
}


/*									//
//		1/4にして、					//
//		m_pTexSceneScaledに入れる	*/
HRESULT CrossFilter::Scene_To_SceneScaled() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// はみ出した場合に中心部分をコピーする
	CoordRect coords;
	RECT rectSrc;
	rectSrc.left = (m_dwWidth - m_dwCropWidth) / 2;
	rectSrc.top = (m_dwHeight - m_dwCropHeight) / 2;
	rectSrc.right = rectSrc.left + m_dwCropWidth;
	rectSrc.bottom = rectSrc.top + m_dwCropHeight;
	// レンダリングターゲットにあったテクスチャ座標を計算する
	GetTextureCoords(m_pOriginalMap, &rectSrc, m_pTexSceneScaled, NULL, &coords);

	// 周辺の16テクセルをサンプリング点として
	// 計算する0.5は中心に合わせるための補正
	int index = 0;
	D3DXVECTOR2 offsets[MAX_SAMPLES];

	for (int y = 0; y < 4; y ++) {
		for (int x = 0; x < 4; x ++) {
			offsets[index].x = (x - 1.5f) / m_dwWidth;
			offsets[index].y = (y - 1.5f) / m_dwHeight;
			index ++;
		}
	}
	m_pEffect->SetValue("g_avSampleOffsets", offsets, sizeof(offsets));

	// 16テクセルをサンプリングしてその平均を縮小バッファに出力する
	pDevice->SetRenderTarget(0, m_pSurfSceneScaled);
	m_pEffect->SetTechnique("tDownScale4x4");
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);
	pDevice->SetTexture(0, m_pOriginalMap);
	DrawFullScreenQuad(coords.u0, coords.v0, coords.u1, coords.v1);
	m_pEffect->EndPass();
	m_pEffect->End();

	return S_OK;
}


/*									//
//			輝度の抽出				//
//									*/
HRESULT CrossFilter::SceneScaled_To_BrightPass() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// 出力先の情報から描画先のサイズを設定する
	D3DSURFACE_DESC desc;
	m_pTexBrightPass->GetLevelDesc(0, &desc);
	RECT rectDest = { 0, 0, (LONG)desc.Width, (LONG)desc.Height };
	InflateRect(&rectDest, -1, -1);	// 出力先の大きさを一回り小さくする
	DWORD scissor;
	pDevice->GetRenderState(D3DRS_SCISSORTESTENABLE, &scissor);
	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	pDevice->SetScissorRect(&rectDest);

	// 全画面コピー
	pDevice->SetRenderTarget(0, m_pSurfBrightPass);
	pDevice->Clear(0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0L);
	m_pEffect->SetTechnique("tBrightPassFilter");
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);
	pDevice->SetTexture(0, m_pTexSceneScaled);
	DrawFullScreenQuad(0.0f, 0.0f, 1.0f, 1.0f);
	m_pEffect->EndPass();
	m_pEffect->End();

	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, scissor);

	return S_OK;
}


/*									//
//		ガウス型のボカシをかける	//
//									*/
HRESULT CrossFilter::BrightPass_To_StarSource() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// 出力先の情報から描画先のサイズを設定する
	D3DSURFACE_DESC desc;
	m_pTexStarSource->GetLevelDesc(0, &desc);
	RECT rectDest = { 0, 0, (LONG)desc.Width, (LONG)desc.Height };
	InflateRect(&rectDest, -1, -1);
	DWORD scissor;
	pDevice->GetRenderState(D3DRS_SCISSORTESTENABLE, &scissor);
	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	pDevice->SetScissorRect(&rectDest);

	// テクスチャ座標を算出する
	CoordRect coords;
	GetTextureCoords(m_pTexBrightPass, NULL, m_pTexStarSource, &rectDest, &coords);

	// 元画像の大きさからガウス分布の係数を計算する
	D3DXVECTOR2 offsets[MAX_SAMPLES];
	D3DXVECTOR4 weights[MAX_SAMPLES];
	m_pTexBrightPass->GetLevelDesc(0, &desc);
	GetGaussBlur5x5(desc.Width, desc.Height, offsets, weights);
	m_pEffect->SetValue("g_avSampleOffsets", offsets, sizeof(offsets));
	m_pEffect->SetValue("g_avSampleWeights", weights, sizeof(weights));

	// ガウスぼかしをおこなう
	m_pEffect->SetTechnique("tGaussBlur5x5");
	pDevice->SetRenderTarget(0, m_pSurfStarSource);
	pDevice->Clear(0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0L);
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);
	pDevice->SetTexture(0, m_pTexBrightPass);
	DrawFullScreenQuad(coords.u0, coords.v0, coords.u1, coords.v1);
	m_pEffect->EndPass();
	m_pEffect->End();

	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, scissor);

	return S_OK;
}


/*									//
//			ボカシをかける			//
//									*/
HRESULT CrossFilter::GetGaussBlur5x5(
	DWORD dwD3DTexWidth,
	DWORD dwD3DTexHeight,
	D3DXVECTOR2* avTexCoordOffset,
	D3DXVECTOR4* avSampleWeight)
{
	float tu = 1.0f / (float)dwD3DTexWidth;
	float tv = 1.0f / (float)dwD3DTexHeight;

	float totalWeight = 0.0f;
	int index = 0;
	for (int x = -2; x <= 2; x++) {
		for (int y = -2; y <= 2; y++) {
			// 係数が小さくなる部分は除去
			if (2 < abs(x) + abs(y)) continue;

			avTexCoordOffset[index] = D3DXVECTOR2(x * tu, y * tv);
			float fx = (FLOAT)x;
			float fy = (FLOAT)y;
			avSampleWeight[index].x =
			avSampleWeight[index].y =
			avSampleWeight[index].z =
			avSampleWeight[index].w = expf(-(fx*fx + fy*fy) / (2 * 1.0f));
			totalWeight += avSampleWeight[index].x;

			index++;
		}
	}

	// 重みの合計を1.0fにする
	for (int i = 0; i < index; i++) avSampleWeight[i] *= 1.0f / totalWeight;

	return S_OK;
}


/*									//
//			光芒をかける			//
//		ここは上手くいっていない	*/
HRESULT CrossFilter::RenderStar() {
	if (!IsOK())	return S_OK;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// エフェクトで使う定数の設定
	static const int s_maxPasses = 1;
	static const int nSamples = 8;
	// 光芒の色
	static const D3DXCOLOR s_colorWhite(0.63f, 0.63f, 0.63f, 0.0f);
	static const D3DXCOLOR s_ChromaticAberrationColor[8] = {
		D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f),	// 白
		D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.0f),	// 赤
		D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.0f),	// 赤
		D3DXCOLOR(0.5f, 0.2f, 0.6f, 0.0f),	// 紫
		D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.0f),	// 青
		D3DXCOLOR(0.2f, 0.3f, 0.7f, 0.0f),	// 青
		D3DXCOLOR(0.2f, 0.6f, 0.2f, 0.0f),	// 緑
		D3DXCOLOR(0.3f, 0.5f, 0.3f, 0.0f),	// 緑
	};

	static D3DXVECTOR4 s_aaColor[s_maxPasses][nSamples];

	for (int p = 0; p < s_maxPasses; p++) {
		// 中心からの距離に応じて光芒の色をつける
		float ratio = (float)(p + 1) / (float)s_maxPasses;
		// それぞれのサンプリングで適当に色をつける
		for (int s = 0; s < nSamples; s++) {
			D3DXCOLOR chromaticAberrColor;
			D3DXColorLerp(&chromaticAberrColor,
				&(s_ChromaticAberrationColor[s]),
				&s_colorWhite,
				ratio);
			// 全体的な色の変化を調整する
			D3DXColorLerp((D3DXCOLOR*)&(s_aaColor[p][s]),
				&s_colorWhite,
				&chromaticAberrColor,
				0.7f);
		}
	}

	float radOffset = 10.f;	// 視点に応じて回転する
	HRESULT hr;
	// 元画像の幅と高さを調べる
	D3DSURFACE_DESC desc;
	m_pSurfStarSource->GetDesc(&desc);
	float srcW = (FLOAT)desc.Width;
	float srcH = (FLOAT)desc.Height;

	int nStarLines = 6;	// 光芒の本数
	for (int d = 0; d < nStarLines; d ++) {	// 方向に応じたループ
		LPDIRECT3DTEXTURE9 pTexSource = m_pTexStarSource;
		float rad = radOffset + 2 * d * D3DX_PI / (FLOAT)nStarLines;	// 角度
		float sn = sinf(rad);
		float cs = cosf(rad);
		D3DXVECTOR2 vtStepUV = D3DXVECTOR2(0.3f * sn / srcW,
			0.3f * cs / srcH);

		float attnPowScale = (atanf(D3DX_PI / 4) + 0.1f) * (160.0f + 120.0f) / (srcW + srcH);

		int iWorkTexture = 0;
		for (int p = 0; p < s_maxPasses; p++) {
			// 描画先の決定
			LPDIRECT3DSURFACE9 pSurfDest = NULL;
			if (p == s_maxPasses - 1) {
				// 最後のパスは、保存用のバッファに確保する
				pSurfDest = m_apSurfStar[d + 2];
			} else {
				pSurfDest = m_apSurfStar[iWorkTexture];
			}
			pDevice->SetRenderTarget(0, pSurfDest);

			// テクスチャ座標や合成するときの重みを計算する
			D3DXVECTOR4 avSampleWeights[MAX_SAMPLES];
			D3DXVECTOR2 avSampleOffSets[MAX_SAMPLES];

			for (int i = 0; i < nSamples; i ++) {
				// それぞれの重み
				float lum = powf(0.95f, attnPowScale * i);
				avSampleWeights[i] = s_aaColor[s_maxPasses - 1 - p][i] * lum * (p + 1.0f) * 0.5f;

				// テクスチャ座標をずらす量
				avSampleOffSets[i].x = vtStepUV.x * i;
				avSampleOffSets[i].y = vtStepUV.y * i;
				if (0.9f <= fabs(avSampleOffSets[i].x) ||
					0.9f <= fabs(avSampleOffSets[i].y)) {
					avSampleOffSets[i].x = 0.0f;
					avSampleOffSets[i].y = 0.0f;
					avSampleWeights[i] *= 0.0f;
				}
			}

			hr = m_pEffect->SetValue("g_avSampleOffsets", avSampleOffSets, sizeof(avSampleOffSets));
			hr = m_pEffect->SetVectorArray("g_avSampleWeights", avSampleWeights, nSamples);

			// 全画面コピー
			hr = m_pEffect->SetTechnique("tStar");
			hr = m_pEffect->Begin(NULL, 0);
			hr = m_pEffect->BeginPass(0);
			pDevice->SetTexture(0, pTexSource);
			DrawFullScreenQuad(0.0f, 0.0f, 1.0f, 1.0f);
			m_pEffect->EndPass();
			m_pEffect->End();

			// 次のパスのためにパラメータを設定する
			vtStepUV *= (float)nSamples;
			attnPowScale *= nSamples;

			// レンダリングした出力を次のテクスチャにする
			pTexSource = m_apTexStar[iWorkTexture];

			iWorkTexture ^= 1;
			//iWorkTexture += 1;
		}
	}

	// 全ての光芒を合成する
	pDevice->SetRenderTarget(0, m_apSurfStar[0]);

	hr = m_pEffect->SetTechnique("tMergeTextures");
	hr = m_pEffect->Begin(NULL, 0);
	hr = m_pEffect->BeginPass(0);
	hr = pDevice->SetTexture(0, m_apTexStar[0 + 2]);
	hr = pDevice->SetTexture(1, m_apTexStar[1 + 2]);
	hr = pDevice->SetTexture(2, m_apTexStar[2 + 2]);
	hr = pDevice->SetTexture(3, m_apTexStar[3 + 2]);
	hr = pDevice->SetTexture(4, m_apTexStar[4 + 2]);
	hr = pDevice->SetTexture(5, m_apTexStar[5 + 2]);

	DrawFullScreenQuad(0.0f, 0.0f, 1.0f, 1.0f);

	m_pEffect->EndPass();
	m_pEffect->End();


	return S_OK;
}


/*									//
//		テクスチャ座標の設定		//
//									*/
HRESULT CrossFilter::GetTextureCoords(
	LPDIRECT3DTEXTURE9 pTexSrc,
	RECT* pRectSrc,
	LPDIRECT3DTEXTURE9 pTexDest,
	RECT* pRectDest,
	CoordRect* pCoords)
{
	D3DSURFACE_DESC desc;

	// 妥当性の検証
	if (pTexSrc == NULL || pTexDest == NULL || pCoords == NULL)
		return E_INVALIDARG;

	// 基本的には、テクスチャ座標をそのまま出力する
	pCoords->u0 = 0.0f;
	pCoords->v0 = 0.0f;
	pCoords->u1 = 1.0f;
	pCoords->v1 = 1.0f;

	// 入力元のサーフェスに関して補正する
	if (pRectSrc != NULL) {
		pTexSrc->GetLevelDesc(0, &desc);	// テクスチャの情報を取得する
		// 転送元の矩形に応じてテクスチャ座標をあわせる
		pCoords->u0 += pRectSrc->left / desc.Width;
		pCoords->v0 += pRectSrc->top / desc.Height;
		pCoords->u1 -= (desc.Width - pRectSrc->right) / desc.Width;
		pCoords->v1 -= (desc.Height - pRectSrc->bottom) / desc.Height;
	}

	// 出力先のサーフェスに関して補正する
	if (pRectDest != NULL) {
		pTexDest->GetLevelDesc(0, &desc);	// テクスチャの情報を取得する
		// 出力先の矩形に応じてテクスチャ座標をあわせる
		pCoords->u0 -= pRectDest->left / desc.Width;
		pCoords->v0 -= pRectDest->top / desc.Height;
		pCoords->u1 += (desc.Width - pRectDest->right) / desc.Width;
		pCoords->v1 += (desc.Height - pRectDest->bottom) / desc.Height;
	}

	return S_OK;
}


/*									//
//		テクスチャの描画			//
//									*/
void CrossFilter::DrawFullScreenQuad(
	float fLeftU,
	float fTopV,
	float fRightU,
	float fBottomV)
{
	D3DSURFACE_DESC desc;
	PDIRECT3DSURFACE9 pSurf;
	DWORD zenable;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	// レンダリングターゲットの情報（幅と高さ）を取得する
	pDevice->GetRenderTarget(0, &pSurf);
	pSurf->GetDesc(&desc);
	pSurf->Release();
	FLOAT	w = (FLOAT)desc.Width;
	FLOAT	h = (FLOAT)desc.Height;

	typedef struct {
		float p[4];	// 位置座標
		float t[2];	// テクスチャ座標
	} ScreenVertex;
	ScreenVertex svQuad[4] = {
		// x		y		  z		w		u		v
		{0 - 0.5f, 0 - 0.5f, 0.5f, 1.0f, fLeftU,  fTopV,},
		{w - 0.5f, 0 - 0.5f, 0.5f, 1.0f, fRightU, fTopV,},
		{0 - 0.5f, h - 0.5f, 0.5f, 1.0f, fLeftU,  fBottomV,},
		{w - 0.5f, h - 0.5f, 0.5f, 1.0f, fRightU, fBottomV,},
	};

	pDevice->GetRenderState(D3DRS_ZENABLE, &zenable);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, svQuad, sizeof(ScreenVertex));
	pDevice->SetRenderState(D3DRS_ZENABLE, zenable);
}