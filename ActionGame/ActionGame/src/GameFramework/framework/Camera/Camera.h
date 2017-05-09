// カメラクラス
// 2017. 1. 4	プログラム作成
// Author		SyunMizuno

#pragma once


#include	"../Convert.h"
#include	"../../Skydome/Skydome.h"
#include	"../../Obj/3D/Obj3D.h"


class	Camera : public	Obj3D {
protected:
#pragma region variable

	// ビュー関係
	Matrix	m_matView;		// ビュー行列
	float	m_fLookLength;	// 注視点までの距離

	// プロジェクション関係
	Matrix		m_matProj;	// プロジェクション行列
	float		m_fFovy;	// 視野角
	float		m_fAspect;	// アスペクト比
	float		m_fNear;	// 前方クリップ
	float		m_fFar;		// 後方クリップ

	Skydome*	m_pSky;	// スカイボックス

#pragma endregion

public:
#pragma region method

	/* コンストラクタ */
	Camera();

	/* デストラクタ */
	virtual ~Camera();

	/* カメラの設定 */
	void Set();

	/* 注視点の設定
	// len	:	設定する長さ
	*/
	void SetLookLength(float len) { m_fLookLength = len; }
	void AddLookLength(float len) { m_fLookLength += len; }

	/* ビュー行列の取得 */
	Matrix GetView() { return m_matView; }

	/* プロジェクション行列の取得 */
	Matrix GetProj() { return m_matProj; }

	/* 視野角の設定
	// Fovy	:	設定したい視野角
	*/
	void SetFovy(float Fovy) { m_fFovy = Fovy; }

	/* アスペクト比の設定
	// Aspect	:	設定したいアスペクト比の設定
	*/
	void SetAspect(float Aspect) { m_fAspect = Aspect; }

	/* ニアクリップの設定
	// Near	:	設定したいニアクリップ
	*/
	void SetNear(float Near) { m_fNear = Near; }

	/* ファークリップの設定
	// Far	:	設定したいファークリップ
	*/
	void SetFar(float Far) { m_fFar = Far; }

	/* スカイドームの作成 */
	void CreateSky();

	/* スカイドームの削除 */
	void DeleteSky();

#pragma endregion

protected:
#pragma region conceal method

	/* 更新 */
	virtual void Update();

#pragma endregion
};