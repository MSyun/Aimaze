//////////////////////////////////////////////////////////////
// 軌跡クラス
// 2016. 7.26	プログラム作成
// 2016. 7.29	完成
// 2016. 9.10	補正On,Off追加
// Author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	<d3dx9.h>
#include	"../framework/Convert.h"


class	Locus {
private:
#pragma region struct

	struct TLocus {
		Vector3	Pos;
		Vector2	Tex;
	};

#pragma endregion

#pragma region variable

#define		SAVE_NUM	400	// 保存フレーム
#define		DIVIDE_NUM	3	// 分割線の数

	bool	m_bUse;	// 記録フラグ

	bool	m_bRevision;	// 補正のOn,Off

	// 座標からの距離
	float	m_fUp;
	float	m_fDown;

	//----- 頂点情報
	// 保存用
	TLocus	m_vStart[SAVE_NUM];
	TLocus	m_vEnd[SAVE_NUM];
	// 曲線補完
	TLocus* m_pStartVertex;
	TLocus* m_pEndVertex;

	// 頂点保存数
	int	m_nVertexNum;

	// 最新要素
	int	m_nNew;

	// 消える速度
	float m_fGhostSpeed;

	//----- テクスチャ
	LPDIRECT3DTEXTURE9	m_pTex;

#pragma endregion

public:
#pragma region method

	Locus();
	virtual ~Locus();

	// 更新
	virtual void Update();
	// 描画
	virtual void Draw();

	// 相対的な上の座標設定
	void SetUp(const float up) { m_fUp = up; }

	// 相対的な下の座標設定
	void SetDown(const float down) { m_fDown = down; }

	// 開始
	void Begin(Matrix& mat);

	// 終了
	void End();

	// 座標の設定
	void SetNewPos(Matrix& mat);

	// 丸くする補正の設定
	void SetRevision(const bool _revision) { m_bRevision = _revision; }

	// テクスチャ設定
	void SetTex(LPDIRECT3DTEXTURE9 tex) { m_pTex = tex; }

	// 消える速度設定
	void SetGhostSpeed(float speed) { m_fGhostSpeed = speed; }

#pragma endregion
};