// ========================================================================
// Author		:	SyunMizuno
// 2015.10		:	作成
// 2015.11.22	:	メモリリーク削除
// 2015.11.28	:	Manager関数追加
// 2016. 2. 6	:	スキンメッシュマネージャーとの同期
// 2016. 5.10	:	ライトとマテリアルの色を合成
// ========================================================================

#include	"SkinMesh.h"

// シェーダ用
#include	"../../Light/LightManager.h"
#include	"../../Camera/CameraManager.h"
#include	<tchar.h>
#include	"../../Debug/Debug.h"
#include	"../../Utility/System/SystemUtility.h"
#include	"../../Graphic/Graphics.h"

/*
	MEMO_SPACE:
		DrawMeshContainer内で行列計算を行っているので要修正

		LambertはPhongでまかなえるので必要ないかも

		最終的にライトはほかで登録するはず
*/


//*									//
//			コンストラクタ			//
//									*/
SkinMesh::SkinMesh() :
	m_bLoad(false)
{
	m_aFrame.clear();
	m_aInContainer.clear();
	m_aTransform.clear();
}


/*									//
//			デストラクタ			//
//									*/
SkinMesh::~SkinMesh() {
	Release();
}


/*									//
//				解放				//
//									*/
void SkinMesh::Release() {
	if (m_bLoad) {
		MyAllocHierarchy Hierarchy;
		if(m_aFrame.size() > 0)
			Hierarchy.DestroyFrame(m_aFrame[0]);

		m_bLoad = false;
	}
	for (auto it = m_aTransform.begin(); it != m_aTransform.end(); ++it) {
		SAFE_DELETE(*it);
	}
}

#pragma region Load
/*									//
//		スキンメッシュ読み込み		//
//									*/
HRESULT SkinMesh::Load(const string filename) {
	HRESULT hr;
	D3DXFRAME* pFrameRoot;
	MyAllocHierarchy	Hierarchy;
	LPD3DXANIMATIONCONTROLLER	Controller = NULL;

	Release();

	// ディレクトリを抽出
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(filename.c_str(), szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	Hierarchy.SetDirectory(szDir);

	// Xファイルからアニメーションメッシュを読み込み作成する
	hr = D3DXLoadMeshHierarchyFromX(
		filename.c_str(),
		D3DXMESH_MANAGED,
		GetGraphics()->GetDevice(),
		&Hierarchy,
		NULL,
		(D3DXFRAME**)&pFrameRoot,
		&Controller);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("スキンメッシュの読み込みに失敗しました"), filename.c_str(), MB_OK);
		return hr;
	}

	// フレーム内の生成
	Hierarchy.FindSkin(pFrameRoot, pFrameRoot);

	CreateFrameArray(pFrameRoot);

	// コントローラーのコピー
	m_AnimCont.SetAnimCont(Controller);
	Controller->Release();

	m_bLoad = true;

	return hr;
}

/*									//
//		マネージャーより読み込み	//
//									*/
void SkinMesh::ManagerGet(_tSkin* pSkin) {
	if (!pSkin) {
		Debug::LogError("渡されたメッシュがありません");
		return;
	}

	CreateFrameArray(pSkin->pFrameRoot);
	if(pSkin->pController)
		m_AnimCont.SetAnimCont(pSkin->pController);
}
#pragma endregion

/*									//
//				更新				//
//									*/
void SkinMesh::Update(Matrix& world) {
	if (m_aFrame.empty())	return;

	UpdateFrameMatrices(m_aFrame[0], &world);

	for (auto it = m_aTransform.begin(); it != m_aTransform.end(); ++it) {
		vector<Transform*>* child = (*it)->GetChilds();
		for (auto itr = child->begin(); itr != child->end(); ++itr) {
			(*itr)->GetGameObject()->UpdateAll();
		}
	}
}
void SkinMesh::LateUpdate() {
	for (auto it = m_aTransform.begin(); it != m_aTransform.end(); ++it) {
		vector<Transform*>* child = (*it)->GetChilds();
		for (auto itr = child->begin(); itr != child->end(); ++itr) {
			(*itr)->GetGameObject()->LateUpdateAll();
		}
	}
}


/*									//
//			合成行列更新			//
//									*/
void SkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, Matrix* pParentMatrix) {
	MYFRAME*	pFrame = (MYFRAME*)pFrameBase;
	if (pParentMatrix) {
		// CombinedTransformationMatrixに最終行列を格納
		Matrix world;
		D3DXMatrixMultiply(&world, &pFrame->TransformationMatrix, pParentMatrix);
		m_aTransform[pFrame->OffsetID]->SetWorld(world);
	} else {
		// CombinedTransformationMatrixに最終行列を格納
		m_aTransform[pFrame->OffsetID]->SetWorld(pFrame->TransformationMatrix);
	}

	// 兄弟フレーム
	if (pFrame->pFrameSibling)
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);

	// 子フレーム
	if (pFrame->pFrameFirstChild)
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &m_aTransform[pFrame->OffsetID]->GetWorld());
}

#pragma region Draw
/*									//
//				描画				//
//									*/
void SkinMesh::Draw(_shadertype type) {
	m_eAlpha = _eAlphaCheck::CHECK_NOALPHA;
	DrawFrame(type);
	for (auto it = m_aTransform.begin(); it != m_aTransform.end(); ++it) {
		vector<Transform*>* child = (*it)->GetChilds();
		for (auto itr = child->begin(); itr != child->end(); ++itr) {
			(*itr)->GetGameObject()->DrawAll();
		}
	}
}
void SkinMesh::LateDraw(_shadertype type) {
	m_eAlpha = _eAlphaCheck::CHECK_ALPHA;
	DrawFrame(type);
	for (auto it = m_aTransform.begin(); it != m_aTransform.end(); ++it) {
		vector<Transform*>* child = (*it)->GetChilds();
		for (auto itr = child->begin(); itr != child->end(); ++itr) {
			(*itr)->GetGameObject()->LateDrawAll();
		}
	}
}
void SkinMesh::DrawShadow() {
	for (auto it = m_aInContainer.begin(); it != m_aInContainer.end(); ++it) {
		MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)(*it)->pMeshContainer;
		while (pMeshContainer) {
			DrawShadowContainer(pMeshContainer, (*it));

			pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
		}
	}
}


/*									//
//			フレームの描画			//
//									*/
void SkinMesh::DrawFrame(_shadertype type) {
	for (auto it = m_aInContainer.begin(); it != m_aInContainer.end(); ++it) {
		MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)(*it)->pMeshContainer;
		while (pMeshContainer) {
			//----- 描画タイプの選択
			if (type == SHADER_NONE)
				DrawMeshContainer(pMeshContainer, (*it));
			else
				DrawShaderContainer(pMeshContainer, (*it), type);

			pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
		}
	}
}


/*									//
//			加算合成チェック		//
//									*/
bool SkinMesh::CheckAlpha(float alpha) {
	//----- 加算合成
	switch (m_eAlpha) {
	case CHECK_ALL:
		return true;

	case CHECK_ALPHA:
		if (alpha < 1.0f)
			return true;
		break;

	case CHECK_NOALPHA:
		if (alpha >= 1.0f)
			return true;
		break;
	};

	return false;
}

#pragma region DrawContainer
/*									//
//			コンテナ内描画			//
//									*/
void SkinMesh::DrawMeshContainer(
	MYMESHCONTAINER* pMeshContainer,
	MYFRAME* pFrame)
{
	// スキンメッシュ
	if (pMeshContainer->pSkinInfo != NULL) {
		// スキニング計算
		for (DWORD i = 0; i < pMeshContainer->dwBoneNum; ++i) {
			DWORD AttribID = pMeshContainer->pBoneComb[i].AttribId;
			//----- 条件チェック
			if (!this->CheckAlpha(pMeshContainer->pMaterials[AttribID].MatD3D.Diffuse.a)) {
				continue;
			}

			DWORD dwBlendMatrixNum = 0;
			// 影響している行列数取得
			for (DWORD k = 0; k < pMeshContainer->dwBoneWeight; ++k) {
				UINT mtxIndex = pMeshContainer->pBoneComb[i].BoneId[k];
				if (mtxIndex != UINT_MAX) {
					// mStackにオフセット行列*ボーン合成行列を格納
					Matrix mStack = pMeshContainer->pMtxBoneOffset[mtxIndex] * m_aTransform[mtxIndex]->GetWorld();
					// 行列スタックに格納
					GetGraphics()->GetDevice()->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
					// 現在影響を受けているボーンの数
					++dwBlendMatrixNum;
				}
			}

			// ジオメトリブレンディングを使用するために行列の個数を指定
			GetGraphics()->GetDevice()->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixNum - 1);

			// メッシュの描画
			GetGraphics()->GetDevice()->SetMaterial(&pMeshContainer->pMaterials[AttribID].MatD3D);
			GetGraphics()->GetDevice()->SetTexture(0, pMeshContainer->ppTexture[AttribID]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	} else {
		// 通常メッシュ
		GetGraphics()->GetDevice()->SetTransform(D3DTS_WORLD, &m_aTransform[pFrame->OffsetID]->GetWorld());
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; ++i) {
			//----- 条件チェック
			if (!this->CheckAlpha(pMeshContainer->pMaterials[i].MatD3D.Diffuse.a)) {
				continue;
			}
			GetGraphics()->GetDevice()->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);	// マテリアルセット
			GetGraphics()->GetDevice()->SetTexture(0, pMeshContainer->ppTexture[i]);			// テクスチャセット
			pMeshContainer->MeshData.pMesh->DrawSubset(i);					// 描画
		}
	}
}


/*									//
//		コンテナ内描画（シェーダ	//
//									*/
void SkinMesh::DrawShaderContainer(
	MYMESHCONTAINER* pMeshContainer,
	MYFRAME* pFrame,
	_shadertype type)
{
	//----- シェーダ
	IHlslDrawBase* pShader = (IHlslDrawBase*)GetShaderManager()->Get(type);
	pShader->Begin();

	//////////////////////////////////////
	// ここは後で修正
	Light* light = GetLightManager()->Get("Directional");
	pShader->SetLight(&light->GetDirection4());
	//pShader->SetLightView(light->GetView());
	//pShader->SetLightProj(light->GetProj());
	//ShadowMap* shadow = (ShadowMap*)GetShaderManager()->Get(SHADER_SHADOW);
	//pShader->SetShadowMap(shadow->GetShadowTex());
	//////////////////////////////////////

	pShader->SetCamera(&GetCameraManager()->CurrentCamera()->GetTransform()->GetPos());
	pShader->SetSpecular(light->GetSpecular());

	// スキンメッシュ
	if (pMeshContainer->pSkinInfo != NULL) {
		// スキニング計算
		for (DWORD i = 0; i < pMeshContainer->dwBoneNum; ++i) {
			DWORD AttribID = pMeshContainer->pBoneComb[i].AttribId;

			//----- 条件チェック
			if (!this->CheckAlpha(pMeshContainer->pMaterials[AttribID].MatD3D.Diffuse.a)) {
				continue;
			}

			DWORD dwBlendMatrixNum = 0;
			// 影響している行列数取得
			for (DWORD k = 0; k < pMeshContainer->dwBoneWeight; ++k) {
				UINT mtxIndex = pMeshContainer->pBoneComb[i].BoneId[k];
				if (mtxIndex != UINT_MAX) {
					// mStackにオフセット行列*ボーン合成行列を格納
					Matrix mStack = pMeshContainer->pMtxBoneOffset[mtxIndex] * m_aTransform[mtxIndex]->GetWorld();
					// 行列スタックに格納
					pShader->SetWorldMatrix(&mStack, k);
					// 現在影響を受けているボーンの数
					++dwBlendMatrixNum;
				}
			}
			pShader->SetMatrix();
			// ジオメトリブレンディングを使用するために行列の個数を指定
			pShader->SetBlendNum(dwBlendMatrixNum);

			// メッシュの描画
			//----- マテリアル
			// ライトの色
			Color light = GetLightManager()->Get("Directional")->GetColor();
			// メッシュの色
			D3DCOLORVALUE Col = pMeshContainer->pMaterials[AttribID].MatD3D.Ambient;
			// 合成
			Vector4 A(Col.r*light.r, Col.g*light.g, Col.b*light.b, Col.a*light.a);
			pShader->SetAmbient(&A);
			// テクスチャ
			pShader->SetDecalMap(pMeshContainer->ppTexture[AttribID]);
			// パスの開始
			if (pMeshContainer->ppTexture[AttribID])
				pShader->BeginPass(2);
			else
				pShader->BeginPass(3);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
			pShader->EndPass();	// パスの終了
		}
	} else {
		// 通常メッシュ
		pShader->SetWorldMatrix(&m_aTransform[pFrame->OffsetID]->GetWorld());
		pShader->SetMatrix();
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; ++i) {
			//----- 条件チェック
			if (!this->CheckAlpha(pMeshContainer->pMaterials[i].MatD3D.Diffuse.a)) {
				continue;
			}

			//----- マテリアル
			// ライトの色
			Color light = GetLightManager()->Get("Directional")->GetColor();
			// メッシュの色
			D3DCOLORVALUE Col = pMeshContainer->pMaterials[i].MatD3D.Ambient;
			// 合成
			Vector4 A(Col.r*light.r, Col.g*light.g, Col.b*light.b, Col.a*light.a);
			pShader->SetAmbient(&A);
			// テクスチャ
			pShader->SetDecalMap(pMeshContainer->ppTexture[i]);
			// パスの開始
			if (pMeshContainer->ppTexture[i] != NULL)
				pShader->BeginPass(0);
			else
				pShader->BeginPass(1);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);					// 描画
			pShader->EndPass();	// パスの終了
		}
	}
	pShader->End();
}


/*									//
//			コンテナ内描画（影		//
//									*/
void SkinMesh::DrawShadowContainer(
	MYMESHCONTAINER* pMeshContainer,
	MYFRAME* pFrame)
{
	//----- シェーダ
	IHlslDrawBase* pShader = (IHlslDrawBase*)GetShaderManager()->Get(SHADER_SHADOW);

	// スキンメッシュ
	if (pMeshContainer->pSkinInfo != NULL) {
		// スキニング計算
		for (DWORD i = 0; i < pMeshContainer->dwBoneNum; ++i) {
			DWORD AttribID = pMeshContainer->pBoneComb[i].AttribId;

			DWORD dwBlendMatrixNum = 0;
			// 影響している行列数取得
			for (DWORD k = 0; k < pMeshContainer->dwBoneWeight; ++k) {
				UINT mtxIndex = pMeshContainer->pBoneComb[i].BoneId[k];
				if (mtxIndex != UINT_MAX) {
					// mStackにオフセット行列*ボーン合成行列を格納
					Matrix mStack = pMeshContainer->pMtxBoneOffset[mtxIndex] * m_aTransform[mtxIndex]->GetWorld();
					// 行列スタックに格納
					pShader->SetWorldMatrix(&mStack, k);
					// 現在影響を受けているボーンの数
					++dwBlendMatrixNum;
				}
			}
			pShader->SetMatrix();
			// ジオメトリブレンディングを使用するために行列の個数を指定
			pShader->SetBlendNum(dwBlendMatrixNum);

			// パスの開始
			pShader->BeginPass(1);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
			pShader->EndPass();	// パスの終了
		}
	} else {
		// 通常メッシュ
		pShader->SetWorldMatrix(&m_aTransform[pFrame->OffsetID]->GetWorld());
		pShader->SetMatrix();
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; ++i) {
			// パスの開始
			pShader->BeginPass(0);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);					// 描画
			pShader->EndPass();	// パスの終了
		}
	}
}
#pragma endregion
#pragma endregion

#pragma region Bone
/*									//
//		ボーンの合成行列取得		//
//									*/
Transform* SkinMesh::GetBoneTransform(const string _BoneName) {
	MYFRAME* pFrame = SearchBoneFrame(_BoneName);

	if (pFrame)
		return m_aTransform[pFrame->OffsetID];
	else {
		return NULL;
	}
}


/*									//
//			対象のボーン検索		//
//									*/
MYFRAME* SkinMesh::SearchBoneFrame(const string _BoneName) {
	for (auto it = m_aFrame.begin(); it != m_aFrame.end(); ++it) {
		if (strcmp((*it)->Name, _BoneName.c_str()) == 0)
			return (*it);
	}

	Debug::LogError("検索したボーン : " + _BoneName + "が見つかりませんでした");
	return NULL;
}
#pragma endregion

/*									//
//			フレーム配列作成		//
//									*/
void SkinMesh::CreateFrameArray(LPD3DXFRAME _pFrame) {
	MYFRAME* pFrame = (MYFRAME*)_pFrame;

	m_aFrame.push_back(pFrame);
	m_aTransform.push_back(new Transform(NULL));
	if (pFrame->pMeshContainer)	m_aInContainer.push_back(pFrame);

	if (pFrame->pFrameSibling)		CreateFrameArray(pFrame->pFrameSibling);
	if (pFrame->pFrameFirstChild)	CreateFrameArray(pFrame->pFrameFirstChild);
}

#pragma region Collision
/*									//
//			あたり判定				//
//									*/
bool SkinMesh::Intersect(
	Vector3& L0,		// 始点
	Vector3& L1,		// 終点
	bool bRay,			// レイフラグ
	Vector3* pCross,	// 交差点
	Vector3* pNormal)	// 接触物の法線ベクトル
{
	// 線分の方向ベクトルを求める
	Vector3 W;
	if (bRay) {
		W = L1;
	} else {
		W = L1 - L0;
	}
	return IntersectFrame(L0, W, bRay, pCross, pNormal);
}
// フレーム
bool SkinMesh::IntersectFrame(
	Vector3& L0,
	Vector3& W,
	bool bRay,
	Vector3* pCross,
	Vector3* pNormal)
{
	bool bResult = false;

	for (auto it = m_aInContainer.begin(); it != m_aInContainer.end(); ++it) {
		bResult = IntersectMeshContainer(
			(*it)->pMeshContainer,
			L0,
			W,
			bRay,
			pCross,
			pNormal);
		if (bResult)
			return bResult;
	}

	return bResult;
}
// コンテナ
bool SkinMesh::IntersectMeshContainer(
	LPD3DXMESHCONTAINER pMeshContainer,
	Vector3& L0,
	Vector3& W,
	bool bRay,
	Vector3* pCross,
	Vector3* pNormal)
{
	bool bResult = false;

	// メッシュの取得
	LPD3DXMESH pMesh = pMeshContainer->MeshData.pMesh;
	LPBYTE pVtx;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
	WORD* pIdx;
	pMesh->LockIndexBuffer(0, (LPVOID*)&pIdx);
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwIdx = pMesh->GetNumFaces();
	for (DWORD i = 0; i < dwIdx; i++) {
		// 三角形の頂点取得
		Vector3 P0 = *(LPD3DXVECTOR3)(pVtx + dwStride * *pIdx++);
		Vector3 P1 = *(LPD3DXVECTOR3)(pVtx + dwStride * *pIdx++);
		Vector3 P2 = *(LPD3DXVECTOR3)(pVtx + dwStride * *pIdx++);
		// 三角形の辺のベクトル取得
		Vector3 P0P1 = P1 - P0;
		Vector3 P1P2 = P2 - P1;
		Vector3 P2P0 = P0 - P2;
		// 三角形の辺と線分の作る面の法線ベクトル取得
		Vector3 N;
		D3DXVec3Cross(&N, &P0P1, &W);
		if (D3DXVec3Dot(&N, &(L0 - P0)) < 0.0f) {
			continue;
		}
		D3DXVec3Cross(&N, &P1P2, &W);
		if (D3DXVec3Dot(&N, &(L0 - P1)) < 0.0f) {
			continue;
		}
		D3DXVec3Cross(&N, &P2P0, &W);
		if (D3DXVec3Dot(&N, &(L0 - P2)) < 0.0f) {
			continue;
		}
		// 三角形の法線ベクトル取得、正規化
		D3DXVec3Cross(&N, &P0P1, &P1P2);
		D3DXVec3Normalize(&N, &N);

		// 媒介変数tを求める前の分母を計算
		float base = D3DXVec3Dot(&N, &W);
		if (base == 0.0f) {
			continue;	// 線分と面が平行
		}
		// tを求める
		float t = D3DXVec3Dot(&N, &(P0 - L0)) / base;
		if (t < 0.0f) {
			continue;	// 交点が始点より後方
		}
		if (!bRay && t > 1.0f) {
			continue;	// 交点が終点より前方
		}
		// 交点を求める
		if (pCross) {
			*pCross = L0 + t * W;
		}
		// 法線ベクトルを返す
		if (pNormal) {
			*pNormal = N;
		}
		// 交点が見つかった
		bResult = true;
		break;
	}
	pMesh->UnlockIndexBuffer();
	pMesh->UnlockVertexBuffer();

	return bResult;
}
#pragma endregion