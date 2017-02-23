//	視差マッピングシェーダ
// 2016. 7.16	プログラム作成
// Author		SyunMizuno


#include	"ParallaxMapping.h"
#include	<tchar.h>
#include	"../../Utility/System/SystemUtility.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
ParallaxMapping::ParallaxMapping() {
}


/*									//
//			デストラクタ			//
//									*/
ParallaxMapping::~ParallaxMapping() {
}


/*									//
//				初期化				//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT ParallaxMapping::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- サポートのバージョン確認
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("このPCのグラフィックボードは対応していません"), _T("ParallaxMapping"), MB_OK);
	}

	//----- ファイル読み込み
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("ParallaxMapping").c_str(),
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
	m_hView = m_pEffect->GetParameterByName(NULL, "matView");
	m_hProj = m_pEffect->GetParameterByName(NULL, "matProj");
	m_hBlendNum = m_pEffect->GetParameterByName(NULL, "iBlendNum");
	m_hTexture = m_pEffect->GetParameterByName(NULL, "tex");
	m_hLightDir = m_pEffect->GetParameterByName(NULL, "vLightDir");
	m_hEyePos = m_pEffect->GetParameterByName(NULL, "vEyePos");
	m_hAmbient = m_pEffect->GetParameterByName(NULL, "vColor");
	m_hSpecular = m_pEffect->GetParameterByName(NULL, "Specular");
	m_hSpecPow = m_pEffect->GetParameterByName(NULL, "SpecularPower");
	m_hBumpTex = m_pEffect->GetParameterByName(NULL, "Bumptex");
	m_hHLimit = m_pEffect->GetParameterByName(NULL, "fHLimit");

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//		スペキュラーの範囲を設定	//
//									*/
void ParallaxMapping::SetSpecular(float Specular) {
	if (!IsOK())	return;

	m_pEffect->SetFloat(m_hSpecular, Specular);
}


/*									//
//		スペキュラーの強さを設定	//
//									*/
void ParallaxMapping::SetSpecularPower(float SpecPow) {
	if (!IsOK())	return;

	m_pEffect->SetFloat(m_hSpecPow, SpecPow);
}


/*									//
//				行列の設定			//
//									*/
void ParallaxMapping::SetMatrix() {
	if (IsOK()) {
		// ワールド × ビュー × 射影
		m_pEffect->SetMatrixArray(m_hWorld, m_mtxWorld, 4);
		m_pEffect->SetMatrix(m_hView, &m_mtxView);
		m_pEffect->SetMatrix(m_hProj, &m_mtxProj);

		//カメラ位置
		m_pEffect->SetVector(m_hEyePos, &m_vPosCamera);
	}

	// シェーダーが使用できないときは、固定機能パイプラインのマトリックスを設定する
	else
		GetGraphics()->GetDevice()->SetTransform(D3DTS_WORLD, &m_mtxWorld[0]);
}


/*									//
//			高さの最大値の設定		//
//									*/
void ParallaxMapping::SetHeightLimit(float limit) {
	if (!IsOK())	return;

	m_pEffect->SetFloat(m_hHLimit, limit);
}


/*									//
//			カメラの設定			//
//									*/
void ParallaxMapping::SetCamera(const D3DXVECTOR4* pCameraPos) {
	if (!IsOK())	return;

	m_vPosCamera = *pCameraPos;
}

void ParallaxMapping::SetCamera(const D3DXVECTOR3* pCameraPos) {
	SetCamera(&D3DXVECTOR4(*pCameraPos, 1.0f));
}

/*									//
//			光源の設定				//
//									*/
void ParallaxMapping::SetLight(const Vector4* pLightDir) {
	if (!IsOK())	return;

	// 平行光源の方向ベクトルを設定する
	Vector4 LightDir = *pLightDir;
	// 正規化する
	D3DXVec3Normalize((D3DXVECTOR3*)&LightDir, (D3DXVECTOR3*)&LightDir);
	m_pEffect->SetVector(m_hLightDir, &LightDir);
}