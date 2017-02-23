// 2D用　位置、回転、スケールを扱う
// 2017. 1.27	プログラム作成
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"../framework/Convert.h"
#include	<vector>
#include	"../Obj/Obj.h"



class RectTransform {
private:
#pragma region variable

	IObj*	m_pObject;

	Vector3	m_vPosition;
	float	m_fAngle;
	Vector3	m_vScale;

	Vector2	m_vSize;
	Vector2	m_vPivot;

	Vector3	m_Vertex[4];

#pragma endregion


public:
#pragma region method

	RectTransform(IObj* object);
	virtual ~RectTransform() {};

#pragma region Position
	// 座標取得
	Point3 GetPos();
	// 座標設定
	void SetPos(const Point3& pos);
	void SetPos(float x, float y, float z);
	// 軸移動
	void Translate(const Vector3& vec, RectTransform* relativeTo = NULL);
	void Translate(float x, float y, float z, RectTransform* relativeTo = NULL);
#pragma endregion

#pragma region Rotation
	// 回転角度の取得
	float GetAngle();
	// 回転
	void Rotate(float angle);
	// オイラー角
	void SetEularAngles(float angle);
	// 角度補正
	void ReSetAngle();
#pragma endregion

#pragma region Scale
	// 拡大率取得（ワールド
	Vector3 GetScale();
	// 拡大率設定
	void SetScale(const Vector3& scale);
	// 拡大率設定
	void SetScale(float x, float y, float z);
#pragma endregion

#pragma region Size
	Vector2 GetSize();
	void SetSize(Vector2& size);
	void SetSize(float x, float y);
#pragma endregion

#pragma region Pivot
	Point2 GetPivot();
	void SetPivot(Point2& pivot);
	void SetPivot(float x, float y);
#pragma endregion

	Vector3* GetVertex();

	// オブジェクトの取得
	IObj* GetGameObject() { return m_pObject; }

	// 削除
	void Destroy();

#pragma endregion

private:
#pragma region conceal method

#pragma region Position
	// ローカル軸移動
	void TranslateLocal(const Vector3& vec, RectTransform* relativeTo);
	void TranslateLocal(float x, float y, float z, RectTransform* relativeTo);
	// ワールド軸移動
	void TranslateWorld(const Vector3& vec);
	void TranslateWorld(float x, float y, float z);
#pragma endregion

	void ReSetVertex();

#pragma endregion
};