////////////////////////////////////////////////////
// 2015.12.10	プログラム作成
// 2016. 7. 8	ビルボード機能追加
////////////////////////////////////////////////////


#include	"Quad.h"
#include	"../../Graphic/3dgraphic.h"
#include	"../../Graphic/Graphics.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Quad::Quad(LPDIRECT3DTEXTURE9 _Tex) :
	m_pTex(_Tex),
	m_pVtxBuff(NULL), m_pIdxBuff(NULL),
	m_nNumVtx(0), m_nNumIdx(0), m_nNumPolygon(0),
	m_fSizeX(3), m_fSizeZ(3),
	m_bBillboard(false)
{
	SetName("QuadMesh");
}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Quad::SetUp(
	float _SizeX, float _SizeZ,	// サイズ
	int _NumX, int _NumZ)		// ブロック数
{
	Release();

	m_fSizeX = _SizeX;
	m_fSizeZ = _SizeZ;
	// 頂点情報の作成
	return MakeVertex(_NumX, _NumZ, _SizeX, _SizeZ);
}


//=============================================================================
// 終了処理
//=============================================================================
void Quad::Release() {
	if (m_pVtxBuff != NULL) {
		// 頂点バッファ
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	if (m_pIdxBuff != NULL) {
		// インデックスバッファ
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
}


//=============================================================================
// 更新処理
//=============================================================================
void Quad::Update() {
}


//=============================================================================
// 描画処理
//=============================================================================
void Quad::LateDraw() {
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	Matrix mat = m_pTransform->GetWorld(), view;
	DWORD lightflg, cull;
	pDevice->GetRenderState(D3DRS_LIGHTING, &lightflg);

	if (m_bBillboard) {
		// ビュー行列の取得
		Matrix view;
		view = GetGraphics()->GetView();

		// ビルボードとして使用
		if (view != NULL) {
			// 法線ベクトルがY軸なので
			// Y,Zベクトルを入れ替える
			mat._11 = view._11;
			mat._12 = view._21;
			mat._13 = view._31;
			mat._21 = view._13;
			mat._22 = view._23;
			mat._23 = view._33;
			mat._31 = view._12;
			mat._32 = view._22;
			mat._33 = view._32;
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			m_pTransform->SetWorld(mat);
		}
	}
	pDevice->SetTransform(D3DTS_WORLD, &mat);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff, 0,
		sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// カリング設定
	pDevice->GetRenderState(D3DRS_CULLMODE, &cull);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTex);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,	// 頂点バッファのベース位置
		0,	// 頂点バッファの使用開始位置
		m_nNumVtx,
		0,	// 頂点インデックスバッファの開始位置
		m_nNumPolygon);

	pDevice->SetRenderState(D3DRS_LIGHTING, lightflg);
	pDevice->SetRenderState(D3DRS_CULLMODE, cull);
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT Quad::MakeVertex(
	int _NumX, int _NumZ,		// ブロック数
	float _SizeX, float _SizeZ)// サイズ
{
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	// 総頂点数の計算
	m_nNumVtx = (_NumX + 1) * (_NumZ + 1);
	m_nNumIdx = (_NumX + 1) * _NumZ * 2 + (_NumZ - 1) * 2;

	// 総ポリゴン数の計算
	m_nNumPolygon = _NumX * _NumZ * 2 + (_NumZ - 1) * 4;

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumVtx,	// 頂点データ用に確保するバッファサイズ
		D3DUSAGE_WRITEONLY,				// 頂点バッファの使用法
		FVF_VERTEX_3D,					// 使用する頂点フォーマット
		D3DPOOL_MANAGED,				// リソースのバッファを保持するメモリクラスを指定
		&m_pVtxBuff,					// 頂点バッファインタフェースへのポインタ
		NULL))) {
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nNumIdx,	// 確保するサイズ
		D3DUSAGE_WRITEONLY,			// 使用法
		D3DFMT_INDEX16,				// フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&m_pIdxBuff,				// 
		NULL))) {
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D	*pVtx;

#if	0
		const float fSizeTexX = 1.0f / _NumX;
		const float fSizeTexY = 1.0f / _NumZ;
#else
		const float fSizeTexX = 1.0f;
		const float fSizeTexY = 1.0f;
#endif

		// 頂点データの範囲をロックし頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntZ = 0; nCntZ < _NumZ + 1; nCntZ ++) {
			for (int nCntX = 0; nCntX < _NumX + 1; nCntX ++) {
				// 頂点座標
				pVtx[nCntZ*(_NumX + 1) + nCntX].vtx.x = -_NumX / 2.0f * _SizeX + nCntX * _SizeX;
				pVtx[nCntZ*(_NumX + 1) + nCntX].vtx.y = 0.0f;
				pVtx[nCntZ*(_NumX + 1) + nCntX].vtx.z = _NumZ / 2.0f * _SizeZ - nCntZ  * _SizeZ;

				// 法線
				pVtx[nCntZ*(_NumX + 1) + nCntX].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				// 反射光
				pVtx[nCntZ*(_NumX + 1) + nCntX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標
				pVtx[nCntZ*(_NumX + 1) + nCntX].tex.x = fSizeTexX * nCntX;
				pVtx[nCntZ*(_NumX + 1) + nCntX].tex.y = fSizeTexY * nCntZ;
			}
		}

		// 頂点データをアンロックする
		m_pVtxBuff->Unlock();

		{	// インデックスバッファの中身を埋める
			WORD*	pIdx;

			// インデックスデータの範囲をロックし頂点バッファへのポインタを取得
			m_pIdxBuff->Lock(
				0, 0, (void**)&pIdx, 0);

			int nCntIdx = 0;
			for (int nCntZ = 0; nCntZ < _NumZ; nCntZ ++) {
				if (nCntZ > 0) {
					// 縮退ポリゴンのためのダブりを設定
					pIdx[nCntIdx] = (nCntZ + 1) * (_NumX + 1);
					nCntIdx++;
				}
				for (int nCntX = 0; nCntX < _NumX + 1; nCntX ++) {
					pIdx[nCntIdx] = (nCntZ + 1) * (_NumX + 1) + nCntX;
					nCntIdx++;
					pIdx[nCntIdx] = nCntZ * (_NumX + 1) + nCntX;
					nCntIdx++;
				}

				if (nCntZ < _NumZ - 1) {
					// 縮退ポリゴンのためのダブりを設定
					pIdx[nCntIdx] = nCntZ * (_NumX + 1) + _NumX;
					nCntIdx++;
				}
			}
		}
		// インデックスデータをアンロックする
		m_pIdxBuff->Unlock();
	}

	return S_OK;
}


void Quad::SetColor(Color _color) {
	{//頂点バッファの中身を埋める
		VERTEX_3D	*pVtx;

		// 頂点データの範囲をロックし頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < m_nNumVtx; nCnt ++)
			// 反射光
			pVtx[nCnt].diffuse = _color;

		// 頂点データをアンロックする
		m_pVtxBuff->Unlock();
	}
}