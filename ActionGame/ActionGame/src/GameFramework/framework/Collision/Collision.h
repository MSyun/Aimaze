//	あたり判定
// 2016. 5.17	プログラム作成
// 2016. 7. 9	AABB追加
// Author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"../Convert.h"


#define	DefaultColor	D3DXCOLOR(0, 255, 255, 255)


/*
	MEMO	:
			座標誤差	：	最初に誤差を登録することで常に誤差を反映してくれる
*/

#pragma region Type

// 球
struct Sphere {
	Point3	p;		// 座標
	Point3	p_d;	// 座標誤差
	float	r;		// 半径
	Sphere() : p(0.0f, 0.0f, 0.0f), p_d(0.0f, 0.0f, 0.0f), r(1.0f) {}
};

// AABB
struct Aabb {
	Point3	p;		// 座標
	Point3	p_d;	// 座標誤差
	float	r[3];	// 半径
	Aabb() : p(0.0f, 0.0f, 0.0f), p_d(0.0f, 0.0f, 0.0f) {}
};

// OBB
struct Obb {
	Point3	p;			// 座標
	Point3	p_d;		// 座標誤差
	Vector3	axis[3];	// ローカルベクトル
	float	r[3];		// 半径
	Obb() : p(0.0f, 0.0f, 0.0f), p_d(0.0f, 0.0f, 0.0f) {}
};

typedef enum {
	X_AXIS = 0,
	Y_AXIS,
	Z_AXIS,
} _AXIS;

// カプセル
struct Capsule {
	Point3	p;		// 座標
	Point3	p_d;	// 座標誤差
	float	r;		// 半径
	float	h;		// 長さ
	Point3	a;		// 中間部の線分の開始点
	Point3	b;		// 中間部の線分の終了点
	_AXIS	Direction;	// 向き
	Vector3	axis[3];
	Capsule() : a(0.0f, 0.0f, 0.0f), b(0.0f, 0.0f, 0.0f),
		p(0.0f, 0.0f, 0.0f), p_d(0.0f, 0.0f, 0.0f),
		r(1.0f), h(3.0f),
		Direction(Y_AXIS) {}
};

// ひし形
struct Lozenge {
	Point3	p;			// 原点
	Point3	p_d;		// 座標誤差
	Vector3	axis[2];	// 長方形の2つの軸となる辺
	float	r;			// 半径
};

#pragma endregion


#pragma region Update

void UpdateCollision(Sphere& sphere, Matrix& world);
void UpdateCollision(Aabb& aabb, Matrix& world);
void UpdateCollision(Obb& obb, Matrix& world);
void UpdateCollision(Capsule& capsule, Matrix& world);

#pragma endregion


#pragma region Check

// 球 vs 球
bool CheckCollision(Sphere& sphere1, Sphere& sphere2);
// AABB vs AABB
bool CheckCollision(Aabb& aabb1, Aabb& aabb2);
// OBB vs OBB
bool CheckCollision(Obb& obb1, Obb& obb2);
// 球 vs カプセル
bool CheckCollision(Sphere& sphere, Capsule& capsule);
bool CheckCollision(Capsule& capsule, Sphere& sphere);
// 球 vs AABB
bool CheckCollision(Sphere& sphere, Aabb& aabb, Point3& p);
bool CheckCollision(Sphere& sphere, Aabb& aabb);
bool CheckCollision(Aabb& aabb, Sphere& sphere);
// 球 vs OBB
bool CheckCollision(Sphere& sphere, Obb& obb, Point3& p);
bool CheckCollision(Sphere& sphere, Obb& obb);
bool CheckCollision(Obb& obb, Sphere& sphere);
// カプセル vs カプセル
bool CheckCollision(Capsule& capsule1, Capsule& capsule2);

#pragma endregion


#pragma region Draw

void DrawCollision(Sphere& sphere, Color& Color = DefaultColor);
void DrawCollision(Aabb& aabb, Color& Color = DefaultColor);
void DrawCollision(Obb& obb, Color& Color = DefaultColor);
void DrawCollision(Capsule& capsule, Color& Color = DefaultColor);

#pragma endregion

//	分離軸の成分から投影線分長を算出
FLOAT LenSegOnSeparateAxis( Vector3 *Sep,
							Vector3 *e1,
							Vector3 *e2,
							Vector3 *e3 = 0 );
// 点cと線分abの間の距離の平方を返す
float SqDistPointSegment( Point3& a, Point3& b, Point3& c );
// 点pとAABB bの間の距離の平方を返す
float SqDistPointAABB( Point3& p, Aabb& b );
// 点pとOBB bの間の距離の平方を返す
float SqDistPointOBB( Point3& p, Obb& b );
// 2つの線分の最近接点
float ClosestPtSegmentSegment(	Point3& p1, Point3& q1,
								Point3& p2, Point3& q2,
								float& s, float& t,
								Point3& c1, Point3& c2 );
// 線分abおよび点cに対して、ab上の最近接点dを返す
void ClosestPtPointSegment( Point3 c, Point3 a, Point3 b, float& t, Point3& d );
// 点pに対して、AABB bの上もしくは中にあるpの最近接点qを返す
void ClosestPtPointAABB( Point3& p, Aabb& b, Point3& q );
// 点pに対して、OBB bの上もしくは中にあるpの最近接点qを返す
void ClosestPtPointOBB( Point3& p, Obb& b, Point3& q );


void ChangeDrawCollider();