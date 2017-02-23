// ========================================================================
// Author		:	SyunMizuno
// 2015.10		:	作成
// 2015.11.22	:	メモリリーク削除
// 2016. 4. 2	:	OBBの追加
// 2016. 5. 9	:	ハーフランバート実装
// ========================================================================

#pragma once

#include	<vector>
#include	"MyAllocHierarchy.h"
#include	"HighLevelAnimController.h"
#include	"../../Convert.h"
#include	"../../../Transform/Transform.h"
#include	"../../Shader/ShaderManager.h"

using namespace std;

/*
	MEMO_SPACE:
*/


//----- メッシュ用構造体(Managerで使用
typedef struct _tSkin {
	LPD3DXFRAME					pFrameRoot;		// フレーム
	LPD3DXANIMATIONCONTROLLER	pController;	// コントローラー
} _tSkin;


class	SkinMesh {
public:
#pragma region enum

	// 加算合成チェック
	typedef enum {
		CHECK_ALL = 0,
		CHECK_ALPHA,
		CHECK_NOALPHA,
	} _eAlphaCheck;

#pragma endregion

protected:
#pragma region variable

	vector<MYFRAME*>		m_aFrame;		// フレーム配列
	vector<MYFRAME*>		m_aInContainer;	// コンテナ配列
	vector<Transform*>		m_aTransform;	// Transform

	HighLevelAnimController	m_AnimCont;		// コントローラー
	bool					m_bLoad;		// このクラスで作成したフラグ
	_eAlphaCheck			m_eAlpha;		// 加算合成チェック

#pragma endregion

public:
#pragma region method

	SkinMesh();
	virtual ~SkinMesh();

	//----- 更新
	virtual void Update(Matrix& world);
	virtual void LateUpdate();

	//----- 描画
	virtual void Draw(
		_shadertype type = SHADER_NONE);
	virtual void LateDraw(
		_shadertype type = SHADER_NONE);

	//----- 解放
	void Release();

#pragma region Load
	// 読み込み
	HRESULT	Load(const string filename);
	// マネージャーからの読み込み
	void ManagerGet(_tSkin* pSkin);
#pragma endregion

	// ボーン行列取得
	Transform* GetBoneTransform(const string _BoneName);

	//----- コントローラ
	HighLevelAnimController& GetCont() { return m_AnimCont; }

	//----- あたり判定
	bool Intersect(
		Vector3& L0,
		Vector3& L1,
		bool bRay = false,
		Vector3* pCross = NULL,
		Vector3* pNormal = NULL);

#pragma endregion

protected:
#pragma region conceal method

	//----- 更新
	virtual void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, Matrix* pParentMatrix);

	//----- 描画
	// 加算合成チェック
	bool CheckAlpha(float alpha);

#pragma region Draw
	// フレーム
	void DrawFrame(_shadertype type);
	// コンテナ
	void DrawMeshContainer(
		MYMESHCONTAINER* pMeshContainer,
		MYFRAME* pFrame);
	// コンテナ（シェーダ
	void DrawShaderContainer(
		MYMESHCONTAINER* pMeshContainer,
		MYFRAME* pFrame,
		_shadertype type);
#pragma endregion

	// 対象のボーンを検索
	MYFRAME* SearchBoneFrame(const string _BoneName);

	// フレーム配列作成
	virtual void CreateFrameArray(LPD3DXFRAME _pFrame);

#pragma region Collision
	//----- あたり判定
	bool IntersectFrame(
		Vector3& L0,
		Vector3& W,
		bool bRay,
		Vector3* pCross,
		Vector3* pNormal);

	bool IntersectMeshContainer(
		LPD3DXMESHCONTAINER pMeshContainer,
		Vector3& L0,
		Vector3& W,
		bool bRay,
		Vector3* pCross,
		Vector3* pNormal);
#pragma endregion

#pragma endregion
};