// ========================================================================
// Author		:	SyunMizuno
// 2015.10		:	作成
// 2015.11.22	:	整理
// 2016. 1. 6	:	OBB追加
// 2016. 2. 6	:	CreateContainer内を細分割化、テクスチャマネージャーと同期
// 2016. 4. 2	:	OBBTreeを排除
// ========================================================================

#include	"MyAllocHierarchy.h"
#include	"../../Texture/TexManager.h"
#include	"../../Utility/System/SystemUtility.h"

/*
	memo	:
				メンバをCOMポインタで管理を目指せ
*/


/*									//
//			文字列コピー			//
//									*/
LPSTR MyAllocHierarchy::CopyStr(LPCSTR name) {
	if (!name) return NULL;
	LPSTR Str = new char[strlen(name) + 1];
	strcpy_s(Str, strlen(name) + 1, name);
	return Str;
}


/*									//
//		メッシュデータ登録			//
//									*/
HRESULT MyAllocHierarchy::RegistMeshData(const D3DXMESHDATA *pSrc, D3DXMESHDATA *pDest) {
	pDest->Type = pSrc->Type;

	// メッシュデータに法線追加
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pSrc->pMesh->GetDevice(&pDevice);	//DirectXデバイス取得
	if (!(pSrc->pMesh->GetFVF() & D3DFVF_NORMAL)) {
		pDest->Type = D3DXMESHTYPE_MESH;
		if (FAILED(pSrc->pMesh->CloneMeshFVF(
			pSrc->pMesh->GetOptions(),
			pSrc->pMesh->GetFVF() | D3DFVF_NORMAL,
			pDevice,
			&pDest->pMesh))) {
			return E_FAIL;
		}

		// 各頂点の法線設定
		D3DXComputeNormals(pDest->pMesh, NULL);
	} else
	// タイプ別格納
	// 通常メッシュ
	if (pSrc->Type == D3DXMESHTYPE_MESH) {
		pDest->pMesh = pSrc->pMesh;
		pDest->pMesh->AddRef();
	} else
	// プログレッシブメッシュ
	if (pSrc->Type == D3DXMESHTYPE_PMESH) {
		pDest->pPMesh = pSrc->pPMesh;
		pDest->pPMesh->AddRef();
	}

	// バンプマップ用処理
	CloneMeshWithTangentAndBinormal(pDevice, pDest->pMesh, &pDest->pMesh);

	SAFE_RELEASE(pDevice);

	return S_OK;
}


/*									//
//			マテリアル登録			//
//									*/
HRESULT MyAllocHierarchy::RegistMaterial(const D3DXMATERIAL *pSrc, DWORD num, MYMESHCONTAINER **pDest) {
	// マテリアルデータ格納領域を動的確保
	(*pDest)->pMaterials = new D3DXMATERIAL[num];
	if ((*pDest)->pMaterials == NULL) return E_OUTOFMEMORY;
	memcpy((*pDest)->pMaterials, pSrc, sizeof(D3DXMATERIAL) * num);

	// テクスチャデータ格納領域を動的確保
	(*pDest)->ppTexture = new LPDIRECT3DTEXTURE9[num];
	if ((*pDest)->ppTexture == NULL) return E_OUTOFMEMORY;
	memset((*pDest)->ppTexture, 0, sizeof(LPDIRECT3DTEXTURE9) * num);

	if (num > 0) {
		// カレントディレクトリを変更
		TCHAR szCurrentDir[_MAX_PATH];
		if (m_szDir[0]) {
			GetCurrentDirectory(_MAX_PATH, szCurrentDir);
			SetCurrentDirectory(m_szDir);
		}

		for (DWORD i = 0; i < num; i++) {
			// マテリアル情報
			(*pDest)->pMaterials[i].MatD3D = pSrc[i].MatD3D;
			(*pDest)->pMaterials[i].MatD3D.Ambient = pSrc[i].MatD3D.Diffuse;
			// テクスチャファイル名
			(*pDest)->pMaterials[i].pTextureFilename = CopyStr(pSrc[i].pTextureFilename);
			// テクスチャ生成確認
			if ((*pDest)->pMaterials[i].pTextureFilename != NULL) {
				// テクスチャ読み込み
				(*pDest)->ppTexture[i] = GetTexManager()->Get((*pDest)->pMaterials[i].pTextureFilename);
			}
		}

		// カレントディレクトリを元に戻す
		if (m_szDir[0])
			SetCurrentDirectory(szCurrentDir);
	} else {
		//- マテリアルなしの場合 -//
		(*pDest)->pMaterials[0].pTextureFilename = NULL;
		memset(&(*pDest)->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		(*pDest)->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		(*pDest)->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		(*pDest)->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		(*pDest)->pMaterials[0].MatD3D.Specular = (*pDest)->pMaterials[0].MatD3D.Diffuse;
	}

	return S_OK;
}


/*									//
//			エフェクト登録			//
//									*/
HRESULT MyAllocHierarchy::RegistEffect(const D3DXEFFECTINSTANCE *pSrc, D3DXEFFECTINSTANCE **ppDest) {
	// 格納先配列の動的確保
	*ppDest = new D3DXEFFECTINSTANCE;
	if (*ppDest == NULL) return E_OUTOFMEMORY;

	// エフェクトファイル名
	(*ppDest)->pEffectFilename = CopyStr(pSrc->pEffectFilename);

	// デフォルト数
	(*ppDest)->NumDefaults = pSrc->NumDefaults;

	// D3DXEFFECTDEFAULTの動的確保
	(*ppDest)->pDefaults = new D3DXEFFECTDEFAULT[pSrc->NumDefaults];
	if ((*ppDest)->pDefaults == NULL) return E_OUTOFMEMORY;

	// 一時的に変数変更
	D3DXEFFECTDEFAULT *pEDSrc = pSrc->pDefaults;		// コピー元
	D3DXEFFECTDEFAULT *pEDDst = (*ppDest)->pDefaults;	// コピー先

	for (DWORD i = 0; i < pSrc->NumDefaults; i++) {
		pEDDst[i].pParamName = CopyStr(pEDSrc[i].pParamName);		// パラメータ名
		DWORD NumBytes = pEDDst[i].NumBytes = pEDSrc[i].NumBytes;	// パラメータサイズ
		pEDDst[i].Type = pEDSrc[i].Type;							// パラメータタイプ
		if (pEDSrc[i].Type <= D3DXEDT_DWORD) {
			pEDDst[i].pValue = new DWORD[NumBytes];
			if (pEDDst[i].pValue == NULL) return E_OUTOFMEMORY;
			memcpy(pEDDst[i].pValue, pEDSrc[i].pValue, NumBytes);
		}
	}
	return S_OK;
}


/*									//
//		隣接ポリゴン登録			//
//									*/
HRESULT MyAllocHierarchy::RegistAdjacency(const DWORD *pSrc, DWORD polynum, DWORD **Dest) {
	// 面ごとに持つ3つの隣接性情報が格納されたDWORD型のアドレス格納用(ポインタ)変数
	*Dest = new DWORD[polynum * 3];
	if (*Dest == NULL) return E_OUTOFMEMORY;
	memcpy(*Dest, pSrc, sizeof(DWORD) * polynum * 3);
	return S_OK;
}


/*									//
//			スキン登録				//
//									*/
HRESULT MyAllocHierarchy::RegistSkin(const LPD3DXSKININFO Src, MYMESHCONTAINER **Dest, LPD3DXMESH Mesh) {
	// スキンメッシュ有り
	(*Dest)->pSkinInfo = Src;
	(*Dest)->pSkinInfo->AddRef();

	// ボーンの数を取得
	DWORD dwBoneNum = Src->GetNumBones();

	// ボーンオフセット行列
	(*Dest)->pMtxBoneOffset = new D3DXMATRIX[dwBoneNum];
	if ((*Dest)->pMtxBoneOffset == NULL) return E_OUTOFMEMORY;
	for (DWORD i = 0; i < dwBoneNum; i++) {
		//角フレーム(ボーン)のオフセット行列を取得して格納
		memcpy(&(*Dest)->pMtxBoneOffset[i], (*Dest)->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
	}

	//- 固定パイプライン描画用に変換 -//
	// シェーダで描画する場合は別途変換が必要

	// 頂点単位でのブレンドの重みとボーンの組み合わせテーブルを適応した新しいメッシュを返す。
	// つまりは最適化よ、これ
	LPD3DXBUFFER	bf;
	LPD3DXBUFFER	pBoneBuffer;
	if (FAILED((*Dest)->pSkinInfo->ConvertToBlendedMesh(
		Mesh,						//元のメッシュデータアドレス
		NULL,						//オプション(現在は使われていないためNULLでいい)
		(*Dest)->pAdjacency,		//元のメッシュの隣接性情報
		NULL,						//出力メッシュの隣接性情報
		NULL,						//各面の新しいインデックス値格納用変数のアドレス
		&bf,						//角頂点の新しいインデックス値格納用変数のアドレス
		&(*Dest)->dwBoneWeight,		//ボーンの影響の一面当たりの最大数格納用変数のアドレス
		&(*Dest)->dwBoneNum,		//ボーンの組み合わせテーブルに含まれるボーン数格納用変数のアドレス
		&pBoneBuffer,				//ボーンの組み合わせテーブルへのポインタ
		&(*Dest)->MeshData.pMesh	//出力されるメッシュアドレス格納用変数のアドレス(固定パイプライン用)
	))) {
		return E_FAIL;
	}
	// 順番変更後の頂点に合わせてボーンの影響情報を更新
	DWORD *d = (DWORD*)bf->GetBufferPointer();
	(*Dest)->pSkinInfo->Remap((*Dest)->MeshData.pMesh->GetNumVertices(), d);
	bf->Release();

	// ボーンテーブルからバッファの先頭アドレスを取得
	(*Dest)->pBoneComb = (LPD3DXBONECOMBINATION)pBoneBuffer->GetBufferPointer();

	return S_OK;
}


/*									//
//			フレーム生成関数		//
//									*/
HRESULT MyAllocHierarchy::CreateFrame(THIS_
	LPCSTR Name,				// 生成するフレーム名
	LPD3DXFRAME *ppNewFrame)	// 生成したフレームオブジェクトを返す
{
	//----- オブジェクトの生成
	MYFRAME*	pFrame = new MYFRAME;
	if (pFrame == NULL) return E_OUTOFMEMORY;	// 領域確保の失敗時の処理
	ZeroMemory(pFrame, sizeof(MYFRAME));		// 初期化

	//----- 名前
	pFrame->Name = CopyStr(Name);

	// 行列の初期化
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);

	// 追加：オフセット関係初期化
	pFrame->OffsetID = 0xffffff;
	D3DXMatrixIdentity(&(pFrame->OffsetMat));

	// 作成したフレームのアドレスを格納
	*ppNewFrame = pFrame;

	return S_OK;
}


/*									//
// Xファイルのあるディレクトリを設定//
//									*/
void MyAllocHierarchy::SetDirectory(LPCTSTR pszDir) {
	if (pszDir)
		lstrcpy(m_szDir, pszDir);
	else
		m_szDir[0] = 0;
}


/*									//
//		メッシュコンテナ生成		//
//									*/
HRESULT MyAllocHierarchy::CreateMeshContainer(THIS_
	LPCSTR Name,								// フレーム名
	CONST D3DXMESHDATA *pMeshData,				// メッシュデータ
	CONST D3DXMATERIAL *pMaterials,				// マテリアルデータ
	CONST D3DXEFFECTINSTANCE *pEffectInstances,	// エフェクトインスタンスデータ
	DWORD NumMaterials,							// マテリアル配列数
	CONST DWORD *pAdjacency,					// メッシュの隣接性データ
	LPD3DXSKININFO pSkinInfo,					// スキンメッシュへのポインタ
	LPD3DXMESHCONTAINER *ppNewMeshContainer)	// 生成したメッシュコンテナを返す
{
	// メッシュコンテナの生成
	MYMESHCONTAINER *pMeshContainer = new MYMESHCONTAINER;
	if (pMeshContainer == NULL)	return E_OUTOFMEMORY;
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));


	//===============================================
	//----- Name
	pMeshContainer->Name = CopyStr(Name);


	//===============================================
	//----- pMeshData
	// タイプ格納
	if (S_OK != RegistMeshData(pMeshData, &pMeshContainer->MeshData))
		return E_FAIL;


	//===============================================
	//----- pMaterials
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	if (S_OK != RegistMaterial(pMaterials, NumMaterials, &pMeshContainer))
		return E_OUTOFMEMORY;


	//===============================================
	//----- pEffectInstances
	if (S_OK != RegistEffect(pEffectInstances, &pMeshContainer->pEffects))
		return E_OUTOFMEMORY;


	//===============================================
	//----- pAdjacency
	if (S_OK != RegistAdjacency(pAdjacency, pMeshData->pMesh->GetNumFaces(), &pMeshContainer->pAdjacency))
		return E_OUTOFMEMORY;


	//===============================================
	//----- pSkinInfo
	if (pSkinInfo != NULL) {
		if (S_OK != RegistSkin(pSkinInfo, &pMeshContainer, pMeshData->pMesh))
			return E_OUTOFMEMORY;
	} else {
		// スキンメッシュ無し
		pMeshContainer->pSkinInfo = NULL;
	}

	*ppNewMeshContainer = pMeshContainer;

	return S_OK;
}


/*									//
//			フレームを削除			//
//									*/
HRESULT MyAllocHierarchy::DestroyFrame(THIS_
	LPD3DXFRAME pFrameToFree)
{
	// 名前の削除
	SAFE_DELETE_ARRAY(pFrameToFree->Name);

	// コンテナの削除
	if (pFrameToFree->pMeshContainer != NULL)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	// 兄弟フレーム
	if (pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	// 子フレーム
	if (pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	SAFE_DELETE(pFrameToFree);

	return S_OK;
}


/*									//
//			コンテナを削除			//
//									*/
HRESULT MyAllocHierarchy::DestroyMeshContainer(THIS_
	LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerToFree;


	//===============================================
	//----- pSkinInfo
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->pMtxBoneOffset);


	//===============================================
	//----- pAdjacency
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);


	//===============================================
	//----- pEffect
	for (DWORD i = 0; i < pMeshContainer->pEffects->NumDefaults; i++) {
		SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pDefaults[i].pParamName);
		SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pDefaults[i].pValue);
	}
	SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pDefaults);
	SAFE_DELETE_ARRAY(pMeshContainer->pEffects->pEffectFilename);
	SAFE_DELETE_ARRAY(pMeshContainer->pEffects);


	//===============================================
	//----- pMaterials
	for (DWORD i = 0; i < pMeshContainer->NumMaterials; i++) {
		if (!pMeshContainer->pMaterials[i].pTextureFilename)	continue;
		GetTexManager()->Delete(pMeshContainer->pMaterials[i].pTextureFilename);
		SAFE_DELETE_ARRAY(pMeshContainer->pMaterials[i].pTextureFilename);
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTexture);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);


	//===============================================
	//----- pMeshData（unionなので1つでいいかも）
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->MeshData.pPMesh);
	SAFE_RELEASE(pMeshContainer->MeshData.pPatchMesh);


	//===============================================
	//----- Name
	SAFE_DELETE_ARRAY(pMeshContainer->Name);

	SAFE_DELETE(pMeshContainer);
	pMeshContainerToFree = NULL;

	return S_OK;
}


/*									//
//			フレーム検索			//
//									*/
HRESULT MyAllocHierarchy::FindSkin(LPD3DXFRAME	_pFrameRoot, LPD3DXFRAME	_pFrame) {
	if (_pFrame->pMeshContainer != NULL) {
		// オフセット行列設定
		if (_pFrame->pMeshContainer->pSkinInfo != NULL) {
			// フレームIDの設定
			DWORD nCnt = SetFrameId(_pFrameRoot, _pFrame->pMeshContainer);
			if (nCnt == -1) {
				MYMESHCONTAINER*	pMeshContainer = (MYMESHCONTAINER*)_pFrame->pMeshContainer;
				nCnt = pMeshContainer->pSkinInfo->GetNumBones();
			}
			// セットされてないID達
			FindSkinLeft(_pFrameRoot, nCnt);

			return S_OK;
		}

		FindSkinLeft(_pFrameRoot, 0);
	}

	// 兄弟フレーム検索
	if (_pFrame->pFrameSibling != NULL) {
		FindSkin(_pFrameRoot, _pFrame->pFrameSibling);
	}

	// 子フレーム検索
	if (_pFrame->pFrameFirstChild != NULL) {
		FindSkin(_pFrameRoot, _pFrame->pFrameFirstChild);
	}
	return S_OK;
}


/*									//
//			お残りIDセット			//
//									*/
int MyAllocHierarchy::FindSkinLeft(LPD3DXFRAME	_pFrameRoot, DWORD _nCnt) {
	MYFRAME*	pFrame = (MYFRAME*)_pFrameRoot;
	// お残りさん
	if (pFrame->OffsetID == 0xffffff) {
		pFrame->OffsetID = _nCnt++;
	}

	// 兄弟フレーム検索
	if (pFrame->pFrameSibling != NULL) {
		_nCnt = FindSkinLeft(pFrame->pFrameSibling, _nCnt);
	}

	// 子フレーム検索
	if (pFrame->pFrameFirstChild != NULL) {
		_nCnt = FindSkinLeft(pFrame->pFrameFirstChild, _nCnt);
	}
	return _nCnt;
}


/*									//
//		フレームIDセット			//
//									*/
int MyAllocHierarchy::SetFrameId(LPD3DXFRAME _pFrame, LPD3DXMESHCONTAINER _pMeshContainer) {
	// キャスト～
	MYMESHCONTAINER*	pMeshContainer = (MYMESHCONTAINER*)_pMeshContainer;

	DWORD dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();

	// フレーム検索
	for (DWORD _Cnt = 0; _Cnt < dwBoneNum; _Cnt++) {
		MYFRAME* pFrame = (MYFRAME*)D3DXFrameFind(_pFrame, pMeshContainer->pSkinInfo->GetBoneName(_Cnt));

		// 終了
		if (pFrame == NULL)
			return _Cnt;

		pFrame->OffsetID = _Cnt;
		memcpy(&pFrame->OffsetMat, pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(_Cnt), sizeof(D3DXMATRIX));
	}

	return -1;
}

/*									//
//		TangentとBinormalを追加		//
//									*/
HRESULT MyAllocHierarchy::CloneMeshWithTangentAndBinormal(
	IDirect3DDevice9* pDevice,	// 描画デバイス
	ID3DXMesh*	pSrcMesh,		// 元のメッシュ
	ID3DXMesh**	pOutCloneMesh)	// クローンメッシュ
{
	// 頂点宣言
	const D3DVERTEXELEMENT9 vertexDecl[] =
	{
		{ 0,   0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0,  12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0,  24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0,  32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0,  44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		D3DDECL_END()
	};

	LPD3DXMESH pTempMesh = NULL;

	// クローンメッシュ生成
	if (FAILED(pSrcMesh->CloneMesh(pSrcMesh->GetOptions(),
		vertexDecl,
		pDevice,
		pOutCloneMesh))) {
		return E_FAIL;
	}

	return D3D_OK;
}