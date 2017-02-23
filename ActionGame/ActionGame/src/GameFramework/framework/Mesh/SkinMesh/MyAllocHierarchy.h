// ========================================================================
// Author		:	SyunMizuno
// 2015.10		:	作成
// 2016. 1. 6	:	OBB追加
// 2016. 1.30	:	コンテナのメンバをLPD3DXBUFFERからD3DXBONECOMBINATIONへ
// 2016. 4. 2	:	OBBTreeを排除
// ========================================================================

#pragma once

#include	<d3dx9.h>
#include	"../../Convert.h"


/*
	memo:
		フレーム内のオフセット行列は生成されているが
		使用されていない
		メモリの無駄、もったいない
*/


/*									//
//			フレーム構造体			//
//									*/
struct MYFRAME : public	D3DXFRAME {
	Matrix	OffsetMat;						// ボーンオフセット行列
	DWORD	OffsetID;						// ボーンオフセット行列ID
};


/*									//
//			コンテナ構造体			//
//									*/
struct MYMESHCONTAINER : public	D3DXMESHCONTAINER {
	LPDIRECT3DTEXTURE9*		ppTexture;		// テクスチャ
	DWORD					dwBoneWeight;	// 重みの個数（重みとは頂点への影響）
	DWORD					dwBoneNum;		// ボーンの数
	LPD3DXBONECOMBINATION	pBoneComb;		// ボーンコンビネーション構造体配列へのポインタ
	Matrix*					pMtxBoneOffset;	// オフセット行列

	// コンストラクタ
	MYMESHCONTAINER() {
		ppTexture = NULL;
		pBoneComb = NULL;
		pMtxBoneOffset = NULL;
	}
};


/*									//
//		ヒエラルキークラス			//
//									*/
class MyAllocHierarchy : public	ID3DXAllocateHierarchy {
private:
	TCHAR m_szDir[_MAX_PATH];	// Xファイルのあるディレクトリ

public:
	MyAllocHierarchy() { m_szDir[0] = 0; }
	~MyAllocHierarchy() {}
	// フレームを生成する
	STDMETHOD(CreateFrame)(THIS_
		LPCSTR Name,
		LPD3DXFRAME *ppNewFrame
		);

	// コンテナを生成する
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer
		);

	// フレームを削除する
	STDMETHOD(DestroyFrame)(THIS_
		LPD3DXFRAME pFrameToFree
		);

	// コンテナを削除する
	STDMETHOD(DestroyMeshContainer)(THIS_
		LPD3DXMESHCONTAINER pMeshContainerToFree
		);

	// フレーム検索
	HRESULT FindSkin(LPD3DXFRAME	_pFrameRoot, LPD3DXFRAME	_pFrame);

	// ディレクトリの設定
	void SetDirectory(LPCTSTR pszDir);

protected:
	// 文字列コピー
	virtual LPSTR CopyStr(LPCSTR name);

	// メッシュデータ登録
	virtual HRESULT RegistMeshData(const D3DXMESHDATA *pSrc, D3DXMESHDATA *pDest);

	// マテリアル登録
	virtual HRESULT RegistMaterial(const D3DXMATERIAL *pSrc, DWORD num, MYMESHCONTAINER **pDest);

	// エフェクト登録
	virtual HRESULT RegistEffect(const D3DXEFFECTINSTANCE *pSrc, D3DXEFFECTINSTANCE **ppDest);

	// 隣接ポリゴン登録
	virtual HRESULT RegistAdjacency(const DWORD *pSrc, DWORD polynum, DWORD **Dest);

	// スキン登録
	virtual HRESULT RegistSkin(const LPD3DXSKININFO Src, MYMESHCONTAINER **Dest, LPD3DXMESH Mesh);

	// お残りIDセット
	int FindSkinLeft(LPD3DXFRAME _pFrameRoot, DWORD _nCnt);
	// フレームIDセット
	int SetFrameId(LPD3DXFRAME _pFrame, LPD3DXMESHCONTAINER _pMeshContainer);

	// TangentとBinormalを追加
	HRESULT CloneMeshWithTangentAndBinormal(
		IDirect3DDevice9* pDevice,		// 描画デバイス
		ID3DXMesh*	pSrcMesh,			// 元のメッシュ
		ID3DXMesh**	pOutCloneMesh);	// クローンメッシュ
};