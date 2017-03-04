//	ランバートクラス
// 2016. 3.12	プログラム作成
// author		SyunMizuno


#include	"Phong.h"
#include	<tchar.h>
#include	"../../Utility/System/SystemUtility.h"
#include	"../../../../Archives/Archives.h"
#include	"../../Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
Phong::Phong() {
}


/*									//
//			デストラクタ			//
//									*/
Phong::~Phong() {
}


/*									//
//				初期化				//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT Phong::Load() {
	HRESULT hr;
	D3DCAPS9 caps;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- サポートのバージョン確認
	pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(1, 1) &&
		caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return MessageBox(NULL, _T("グラフィックボードは対応していません"), _T("Phong"), MB_OK);
	}

	//----- ファイル読み込み
	LPD3DXBUFFER pErr = NULL;
	hr = D3DXCreateEffectFromFile(
		pDevice,
		Archives::Shader("Phong").c_str(),
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
	m_hTexture = m_pEffect->GetParameterByName(NULL, "tex");
	m_hLightDir = m_pEffect->GetParameterByName(NULL, "vLightDir");
	m_hEyePos = m_pEffect->GetParameterByName(NULL, "vEyePos");
	m_hAmbient = m_pEffect->GetParameterByName(NULL, "vColor");
	m_hSpecular = m_pEffect->GetParameterByName(NULL, "Specular");
	m_hSpecPow = m_pEffect->GetParameterByName(NULL, "SpecularPower");
	m_hLightBloomPow = m_pEffect->GetParameterByName(NULL, "LightBloomPower");

	m_hLightView = m_pEffect->GetParameterByName(NULL, "matLightView");
	m_hLightProj = m_pEffect->GetParameterByName(NULL, "matLightProj");
	m_hBias = m_pEffect->GetParameterByName(NULL, "fBias");
	m_hScaleBias = m_pEffect->GetParameterByName(NULL, "matScaleBias");
	m_hShadowMap = m_pEffect->GetParameterByName(NULL, "ShadowMap");

	m_pEffect->SetTechnique(m_hTechnique);

	return S_OK;
}


/*									//
//		スペキュラーの範囲を設定	//
//									*/
void Phong::SetSpecular(float Specular) {
	if (IsOK())
		m_pEffect->SetFloat(m_hSpecular, Specular);
}


/*									//
//		スペキュラーの強さを設定	//
//									*/
void Phong::SetSpecularPower(float SpecPow) {
	if (IsOK())
		m_pEffect->SetFloat(m_hSpecPow, SpecPow);
}


/*									//
//		スペキュラーの強さを設定	//
//									*/
void Phong::SetLightBloomPower(float LightBloomPow) {
	if (IsOK())
		m_pEffect->SetFloat(m_hLightBloomPow, LightBloomPow);
}


/*									//
//				行列の設定			//
//									*/
void Phong::SetMatrix() {
	if( IsOK() ) {
		D3DXMATRIX	m1;
		D3DXVECTOR4	v;

		// ワールド × ビュー × 射影
		m_pEffect->SetMatrixArray( m_hWorld, m_mtxWorld, 4 );
		m_pEffect->SetMatrix(m_hCameraView, &m_mtxView);
		m_pEffect->SetMatrix(m_hCameraProj, &m_mtxProj);
		m_pEffect->SetMatrix(m_hLightView, &m_mtxLightView);
		m_pEffect->SetMatrix(m_hLightProj, &m_mtxLightProj);

		//カメラ位置
		m1 = m_mtxWorld[0] * m_mtxView;
		D3DXMatrixInverse( &m1, NULL, &m1 );
		D3DXVec4Transform( &v, &m_vPosCamera, &m1 );
		m_pEffect->SetVector( m_hEyePos, &v );
	}

	// シェーダーが使用できないときは、固定機能パイプラインのマトリックスを設定する
	else
		GetGraphics()->GetDevice()->SetTransform( D3DTS_WORLD, &m_mtxWorld[0] );
}


/*									//
//			カメラの設定			//
//									*/
void Phong::SetCamera(const Vector4* pCameraPos) {
	// カメラ位置
	if (!IsOK())	return;

	m_vPosCamera = *pCameraPos;
}

void Phong::SetCamera(const Vector3* pCameraPos) {
	SetCamera(&Vector4(*pCameraPos, 1.0f));
}


/*									//
//			光源の設定				//
//									*/
void Phong::SetLight(const Vector4* pLightDir) {
	if (!IsOK())	return;

	Matrix	m1;
	Vector4	v;
	Vector4	LightDir;
	// 平行光源の方向ベクトルを設定する
	LightDir = *pLightDir;

	D3DXMatrixInverse(&m1, NULL, &m_mtxWorld[0]);
	D3DXVec4Transform(&v, &LightDir, &m1);
	// 正規化する
	D3DXVec3Normalize((Vector3*)&v, (Vector3*)&v);
	m_pEffect->SetVector(m_hLightDir, &v);
}