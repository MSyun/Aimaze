//	ライトクラス
// 2016. 3.27	プログラム作成
// 2016. 4.21	フォン用のスペキュラメンバ追加
// 2016. 5.10	ライトの色を追加
// 2017. 3. 1	Obj3Dを継承するように変更
// author		SyunMizuno

#pragma once


#include	"../Convert.h"
#include	"../../Obj/3D/Obj3D.h"


/*
	memo	:
			このクラスはたぶんDirectionalLight
*/


class	Light	:	public	Obj3D {
private:
#pragma region variable

	float	m_fLookLength;
	Matrix	m_mtxView;
	Matrix	m_mtxProj;
	float	m_fNear;	// ニアクリップ
	float	m_fFar;		// ファークリップ

	Color	m_vColor;	// ライトの色

	float	m_fSpeclar;	// ライトの強さ

#pragma endregion

public:
#pragma region method

	Light();
	virtual ~Light();
	void Set(bool _custom = false);

	void CreateView();
	void CreateProj();

	void SetLookLength(float len) { m_fLookLength = len; }
	void AddLookLength(float len) { m_fLookLength += len; }
	void SetNear(float _near) { m_fNear = _near; }
	void SetFar(float _far) { m_fFar = _far; }

	void SetColor(float r, float g, float b, float a) { m_vColor.r = r; m_vColor.g = g; m_vColor.b = b; m_vColor.a = a; }
	void SetColor(float col) { m_vColor.r = col; m_vColor.g = col; m_vColor.b = col; }
	void SetColor(Color col) { m_vColor = col; }

	Vector3 GetDirection3();
	Vector4 GetDirection4();

	Matrix GetView() { return m_mtxView; }
	Matrix GetProj() { return m_mtxProj; }
	void SetSpecular(float _specular) { m_fSpeclar = _specular; }
	float GetSpecular() { return m_fSpeclar; }
	Color GetColor() { return m_vColor; }

#pragma endregion
};