#pragma once

#include	"../Convert.h"


//=================================================================
// ライブラリのリンク
//=================================================================
#if 0	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加のファイル"に対象ライブラリを設定する ]
#pragma comment ( lib, "d3d9.lib" )		// 描画処理に必要
#pragma comment ( lib, "d3dx9.lib" )	// [d3d9.lib]の拡張ライブラリ
#pragma comment ( lib, "dxguid.lib" )	// DirectXコンポーネント使用に必要
#endif


//*****************************************************************
// マクロ定義
//*****************************************************************
#define	NUM_VERTEX			( 4 )				// 頂点数
#define	NUM_POLYGON			( 2 )				// ポリゴン数


// 頂点フォーマット（頂点座標[2D] / 頂点カラー）
#define	FVF_VERTEX_2D		( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define	FVF_VERTEX_3D		( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

// 上記頂点フォーマットに合わせた構造体を定義
typedef struct {
	D3DXVECTOR3	pos;	// 頂点座標
	float		rhw;	// テクスチャのパースペクティブコレクト用
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2	tex;	// テクスチャ座標
} VERTEX_2D;

typedef struct {
	D3DXVECTOR3	vtx;
	D3DXVECTOR3	normal;
	D3DCOLOR	diffuse;
	D3DXVECTOR2	tex;
} VERTEX_3D;