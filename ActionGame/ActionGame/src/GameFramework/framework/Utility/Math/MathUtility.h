//	計算系の便利
// 2016. 9. 5	プログラム作成
// 2016. 9.15	壁潜り位置補正追加
// 2017. 2.20	ベジエ曲線追加
// Author		SyunMizuno

#pragma once

#include	"../../Convert.h"
#include	"../../Collision/Collision.h"


// [min, max]の範囲内までnをクランプ
float Clamp(float val, float min, float max);
int Clamp(int val, int min, int max);


// 2つのベクトルからABのなす角度θを求める
float AngleOf2Vector(Vector3 A, Vector3 B);


// 壁釣りベクトル算出
Vector3* WallScratchVector(
	const Vector3& front,
	const Vector3& normal);

/*	ベジエ曲線
//	p0	: point1
//	p1	: point2
//	p2	: point3
//	p3	: point4
//	t	: 0.0f ～ 1.0f
*/
Vector2 Bezier(
	Vector2& p0,
	Vector2& p1,
	Vector2& p2,
	Vector2& p3,
	float t);

/*	Catmull-Romスプライン曲線
//	p0	: point1
//	p1	: point2
//	p2	: point3
//	p3	: point4
//	t	: 0.0f ～ 1.0f
*/
Vector3 CatmullRom(
	Vector3& p0,
	Vector3& p1,
	Vector3& p2,
	Vector3& p3,
	float t);

// OBBと平面の衝突判定関数
bool OBBvsPlane(Obb& obb, Vector3& Normal, Vector3& Cross, float* Len = NULL);
