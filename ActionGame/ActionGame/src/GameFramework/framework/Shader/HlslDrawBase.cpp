//	HLSL描画系統クラス
// 2016. 7.19	プログラム作成
// author		SyunMizuno


#include	"HlslDrawBase.h"
#include	<tchar.h>
#include	"../Graphic/Graphics.h"


/*									//
//			コンストラクタ			//
//									*/
IHlslDrawBase::IHlslDrawBase() {
}


/*									//
//			デストラクタ			//
//									*/
IHlslDrawBase::~IHlslDrawBase() {
}


/*									//
//		シェーダー処理開始			//
//									*/
void IHlslDrawBase::Begin() {
	if (IsOK()) {
		LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
		pDevice->GetTransform(D3DTS_VIEW, &m_mtxView);
		pDevice->GetTransform(D3DTS_PROJECTION, &m_mtxProj);

		m_pEffect->Begin(NULL, 0);
	}
}


/*									//
//		テクスチャを貼り付け		//
//									*/
void IHlslDrawBase::SetDecalMap(LPDIRECT3DTEXTURE9 &pTex) {
	if (IsOK()) {
		m_pEffect->SetTexture(m_hTexture, pTex);
	} else {
		GetGraphics()->GetDevice()->SetTexture(0, pTex);
	}
}


/*									//
//			環境光を設定			//
//									*/
void IHlslDrawBase::SetAmbient(float Ambient) {
	if (IsOK()) {
		D3DXVECTOR4	A;
		A = Vector4(Ambient, Ambient, Ambient, Ambient);
		m_pEffect->SetVector(m_hAmbient, &A);
	}
	// シェーダーが使用できないときは、固定機能パイプラインのマテリアルを設定
	else {
		D3DMATERIAL9 old_material;
		LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
		pDevice->GetMaterial(&old_material);
		old_material.Ambient.r = Ambient;
		old_material.Ambient.g = Ambient;
		old_material.Ambient.b = Ambient;
		old_material.Ambient.a = 1.0f;
		pDevice->SetMaterial(&old_material);
	}
}


/*									//
//			環境光を設定			//
//									*/
void IHlslDrawBase::SetAmbient(const Vector4* pAmbient) {
	if (IsOK()) {
		m_pEffect->SetVector(m_hAmbient, pAmbient);
	}
	//シェーダーが使用できないときは、固定機能パイプラインのマテリアルを設定する
	else {
		D3DMATERIAL9 old_material;
		LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
		pDevice->GetMaterial(&old_material);
		old_material.Ambient.r = pAmbient->x;
		old_material.Ambient.g = pAmbient->y;
		old_material.Ambient.b = pAmbient->z;
		old_material.Ambient.a = pAmbient->w;
		pDevice->SetMaterial(&old_material);
	}
}


/*									//
//			ブレンド数を設定		//
//									*/
void IHlslDrawBase::SetBlendNum(DWORD BlendNum) {
	if (IsOK()) {
		m_pEffect->SetInt(m_hBlendNum, BlendNum);
	} else
		GetGraphics()->GetDevice()->SetRenderState(D3DRS_VERTEXBLEND, BlendNum - 1);
}


/*									//
//			ワールド行列を設定		//
//									*/
void IHlslDrawBase::SetWorldMatrix(const Matrix* world, int n) {
	if (IsOK()) {
		m_mtxWorld[n] = (*world);
	} else
		GetGraphics()->GetDevice()->SetTransform(D3DTS_WORLDMATRIX(n), world);
}


/*									//
//			行列の設定				//
//									*/
void IHlslDrawBase::SetMatrix() {
	if (IsOK()) {
		// ワールド × ビュー × 射影
		m_pEffect->SetMatrixArray(m_hWorld, m_mtxWorld, 4);
		m_pEffect->SetMatrix(m_hView, &m_mtxView);
		m_pEffect->SetMatrix(m_hProj, &m_mtxProj);
	}

	// シェーダーが使用できないときは、固定機能パイプラインのマトリックスを設定する
	else
		GetGraphics()->GetDevice()->SetTransform(D3DTS_WORLD, &m_mtxWorld[0]);
}

/*									//
//			光源の設定				//
//									*/
void IHlslDrawBase::SetLight(const Vector4* pLightDir) {
	if (!IsOK())	return;

	Vector4 LightDir = *pLightDir;
	D3DXVec3Normalize((Vector3*)&LightDir, (Vector3*)&LightDir);
	m_pEffect->SetVector(m_hLightDir, &LightDir);
}