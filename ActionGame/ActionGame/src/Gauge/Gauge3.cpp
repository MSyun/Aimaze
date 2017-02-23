// 3Dゲージクラス
// 2017. 1.31	プログラム作成
// Author		SyunMizuno



#include	"Gauge3.h"
#include	"../GameFramework/framework/Graphic/3dgraphic.h"


/*									//
//			コンストラクタ			//
//									*/
Gauge3::Gauge3(int _max, LPDIRECT3DTEXTURE9 _tex) :
	Quad(_tex),
	m_nMax(_max),
	m_nCur(_max)
{
	SetName("Gauge3");
}


/*									//
//				初期化				//
//									*/
HRESULT Gauge3::Init(float _SizeX, float _SizeZ)
{
	return Quad::SetUp(_SizeX, _SizeZ);
}


/*									//
//			現在の数値を設定		//
//									*/
void Gauge3::SetCurrentNum(int _num) {
	// 範囲チェック
	if (_num < 0 || _num > m_nMax)	return;

	m_nCur = _num;

	// テクスチャ座標
	float texX = 1.0f / m_nMax * m_nCur;
	// 座標
	float posX = m_fSizeX / m_nMax * m_nCur;

	//頂点バッファの中身を埋める
	VERTEX_3D	*pVtx;

	// 頂点データの範囲をロックし頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < m_nNumVtx; i ++) {
		if (i % 2 == 0)	continue;
		// テクスチャ座標
		pVtx[i].tex.x = texX;
		// 座標
		pVtx[i].vtx.x = pVtx[i - 1].vtx.x + posX;
	}

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}