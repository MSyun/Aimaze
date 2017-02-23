//	コライダー
// 2016. 5.17	プログラム作成
// Author		SyunMizuno



#include	"Collision.h"
#include	<math.h>
#include	"../Utility/Math/MathUtility.h"
#include	"../Graphic/Graphics.h"

/*
	memo	:
			
*/

static bool bDraw = false;


struct VERTEX_XYZ_COL{
	FLOAT x, y, z;
	D3DCOLOR color;
};


#define FVF_XYZ_COLOR		( D3DFVF_XYZ | D3DFVF_DIFFUSE )


void ChangeDrawCollider() {
	bDraw ^= true;
}



/*									//
//					更新			//
//									*/
// 球
void UpdateCollision(Sphere& sphere, Matrix& world) {
	sphere.p = Point3(world._41, world._42, world._43);
	for (int i = 0; i < 3; i ++) {
		Vector3 axis = world.m[i];
		D3DXVec3Normalize(&axis, &axis);
		sphere.p += axis * sphere.p_d[i];
	}
}
// AABB
void UpdateCollision(Aabb& aabb, Matrix& world) {
	aabb.p = Point3(world._41, world._42, world._43);
	for (int i = 0; i < 3; i ++) {
		Vector3 axis = world.m[i];
		D3DXVec3Normalize(&axis, &axis);
		aabb.p += axis * aabb.p_d[i];
	}
}
// OBB
void UpdateCollision(Obb& obb, Matrix& world) {
	obb.axis[0] = Vector3(world._11, world._12, world._13);
	obb.axis[1] = Vector3(world._21, world._22, world._23);
	obb.axis[2] = Vector3(world._31, world._32, world._33);
	obb.p = Point3(world._41, world._42, world._43);

	for (int i = 0; i < 3; i ++) {
		D3DXVec3Normalize(&obb.axis[i], &obb.axis[i]);
		obb.p += obb.axis[i] * obb.p_d[i];
	}
}
// カプセル
void UpdateCollision(Capsule& capsule, Matrix& world) {
	capsule.p = Point3(world._41, world._42, world._43);

	for (int i = 0; i < 3; i ++) {
		capsule.axis[i] = Vector3(world.m[i][0], world.m[i][1], world.m[i][2]);
		D3DXVec3Normalize(&capsule.axis[i], &capsule.axis[i]);
	}

	// 線分の端を作成
	capsule.a = capsule.axis[capsule.Direction] * (capsule.h / 2.0f);
	capsule.b = -capsule.axis[capsule.Direction] * (capsule.h / 2.0f);

	capsule.a += capsule.p + capsule.p_d;
	capsule.b += capsule.p + capsule.p_d;
}




/*									//
//				あたり判定			//
//									*/
// 球 vs 球
bool CheckCollision( Sphere &sphere1, Sphere &sphere2 ) {
	// 中心間の距離の平方を計算
	Vector3 d = sphere1.p - sphere2.p;
	float dist2 = D3DXVec3Dot(&d,&d);
	// 平方した距離が平方した半径の合計よりも小さい場合に球は交差している
	float radiusSum = sphere1.r + sphere2.r;
	return dist2 <= radiusSum * radiusSum;
}
// AABB vs AABB
bool CheckCollision(Aabb& aabb1, Aabb& aabb2 ) {
	if( fabsf(aabb1.p[0] - aabb2.p[0]) > (aabb1.r[0] + aabb2.r[0]) ) return false;
	if( fabsf(aabb1.p[1] - aabb2.p[1]) > (aabb1.r[1] + aabb2.r[1]) ) return false;
	if( fabsf(aabb1.p[2] - aabb2.p[2]) > (aabb1.r[2] + aabb2.r[2]) ) return false;
	return true;
}
// OBB vs OBB
bool CheckCollision( Obb& obb1, Obb& obb2 ) {
	// 各方向ベクトルの確保
	// (N***:標準化方向ベクトル)
	Vector3 NAe1 = obb1.axis[0], Ae1 = NAe1 * obb1.r[0];
	Vector3 NAe2 = obb1.axis[1], Ae2 = NAe2 * obb1.r[1];
	Vector3 NAe3 = obb1.axis[2], Ae3 = NAe3 * obb1.r[2];
	Vector3 NBe1 = obb2.axis[0], Be1 = NBe1 * obb2.r[0];
	Vector3 NBe2 = obb2.axis[1], Be2 = NBe2 * obb2.r[1];
	Vector3 NBe3 = obb2.axis[2], Be3 = NBe3 * obb2.r[2];
	Vector3 Interval = obb1.p - obb2.p;	// 座標の差

	// 分離軸：Ae1
	FLOAT rA = D3DXVec3Length( &Ae1 );
	FLOAT rB = LenSegOnSeparateAxis( &NAe1, &Be1, &Be2, &Be3 );
	FLOAT L = fabsf(D3DXVec3Dot( &Interval, &NAe1 ));
	if( L > rA + rB )	return false;	// 衝突していない

	// 分離軸：Ae2
	rA = D3DXVec3Length( &Ae2 );
	rB = LenSegOnSeparateAxis( &NAe2, &Be1, &Be2, &Be3 );
	L = fabsf(D3DXVec3Dot( &Interval, &NAe2 ));
	if( L > rA + rB )	return false;	// 衝突していない

	// 分離軸：Ae3
	rA = D3DXVec3Length( &Ae3 );
	rB = LenSegOnSeparateAxis( &NAe3, &Be1, &Be2, &Be3 );
	L = fabsf(D3DXVec3Dot( &Interval, &NAe3 ));
	if( L > rA + rB )	return false;	// 衝突していない

	// 分離軸：Be1
	rA = LenSegOnSeparateAxis( &NBe1, &Ae1, &Ae2, &Ae3 );
	rB = D3DXVec3Length( &Be1 );
	L = fabsf(D3DXVec3Dot( &Interval, &NBe1 ));
	if( L > rA + rB )	return false;	// 衝突していない

	// 分離軸：Be2
	rA = LenSegOnSeparateAxis( &NBe2, &Ae1, &Ae2, &Ae3 );
	rB = D3DXVec3Length( &Be2 );
	L = fabsf(D3DXVec3Dot( &Interval, &NBe2 ));
	if( L > rA + rB )	return false;	// 衝突していない

	// 分離軸：Be3
	rA = LenSegOnSeparateAxis( &NBe3, &Ae1, &Ae2, &Ae3 );
	rB = D3DXVec3Length( &Be3 );
	L = fabsf(D3DXVec3Dot( &Interval, &NBe3 ));
	if( L > rA + rB )	return false;	// 衝突していない

	// 分離軸：C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross( &Cross, &NAe1, &NBe1 );
	rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
	rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
	L = fabsf(D3DXVec3Dot( &Interval, &Cross ));
	if( L > rA + rB )	return false;

	// 分離軸：C12
	D3DXVec3Cross( &Cross, &NAe1, &NBe2 );
	rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
	rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
	L = fabsf(D3DXVec3Dot( &Interval, &Cross ));
	if( L > rA + rB )	return false;

	// 分離軸：C13
	D3DXVec3Cross( &Cross, &NAe1, &NBe3 );
	rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
	rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
	L = fabsf(D3DXVec3Dot( &Interval, &Cross ));
	if( L > rA + rB )	return false;

	// 分離軸：C21
	D3DXVec3Cross( &Cross, &NAe2, &NBe1 );
	rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
	rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
	L = fabsf(D3DXVec3Dot( &Interval, &Cross ));
	if( L > rA + rB )	return false;

	// 分離軸：C22
	D3DXVec3Cross( &Cross, &NAe2, &NBe2 );
	rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
	rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
	L = fabsf(D3DXVec3Dot( &Interval, &Cross ));
	if( L > rA + rB )	return false;

	// 分離軸：C23
	D3DXVec3Cross( &Cross, &NAe2, &NBe3 );
	rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
	rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
	L = fabsf(D3DXVec3Dot( &Interval, &Cross ));
	if( L > rA + rB )	return false;

	// 分離軸：C31
	D3DXVec3Cross( &Cross, &NAe3, &NBe1 );
	rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
	rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
	L = fabsf(D3DXVec3Dot( &Interval, &Cross ));
	if( L > rA + rB )	return false;

	// 分離軸：C32
	D3DXVec3Cross( &Cross, &NAe3, &NBe2 );
	rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
	rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
	L = fabsf(D3DXVec3Dot( &Interval, &Cross ));
	if( L > rA + rB )	return false;

	// 分離軸：C33
	D3DXVec3Cross( &Cross, &NAe3, &NBe3 );
	rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
	rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
	L = fabsf(D3DXVec3Dot( &Interval, &Cross ));
	if( L > rA + rB )	return false;

	// 分離平面が存在しないので「衝突している」
	return true;
}
// 球 vs カプセル
bool CheckCollision( Sphere& sphere, Capsule& capsule ) {
	// 球の中心とカプセルの線分の間の（平行した）距離を計算
	float dist2 = SqDistPointSegment( capsule.a, capsule.b, sphere.p );
	// （平行した）距離が（平行した）半径の総和よりも小さい場合は、衝突
	float radius = sphere.r + capsule.r;
	return dist2 <= radius * radius;
}
bool CheckCollision( Capsule& capsule, Sphere& sphere ) {
	return CheckCollision( sphere, capsule );
}
// 球 vs AABB
bool CheckCollision( Sphere& sphere, Aabb& aabb, Point3& p ) {
	// 球の中心に対する最近接点であるAABB上にある点pを見つける
	ClosestPtPointAABB(sphere.p, aabb, p);

	// 球とAABBが交差するのは、球の中心から点pまでの（平方した）距離が
	// （平方した）球の半径よりも小さい場合
	Vector3 v = p - sphere.p;

	return D3DXVec3Dot(&v, &v) <= sphere.r * sphere.r;
}
bool CheckCollision( Sphere& sphere, Aabb& aabb ) {
	Point3 p;
	// 球の中心に対する最近接点であるAABB上にある点pを見つける
	ClosestPtPointAABB(sphere.p, aabb, p);

	// 球とAABBが交差するのは、球の中心から点pまでの（平方した）距離が
	// （平方した）球の半径よりも小さい場合
	Vector3 v = p - sphere.p;

	return D3DXVec3Dot(&v, &v) <= sphere.r * sphere.r;
}
bool CheckCollision(Aabb& aabb, Sphere& sphere ) {
	return CheckCollision( sphere, aabb );
}
// 球 vs OBB
bool CheckCollision( Sphere& sphere, Obb& obb, Point3& p ) {
	// 球の中心に対する最近接点であるAABB上にある点pを見つける
	ClosestPtPointOBB(sphere.p, obb, p);

	// 球とAABBが交差するのは、球の中心から点pまでの（平方した）距離が
	// （平方した）球の半径よりも小さい場合
	Vector3 v = p - sphere.p;

	return D3DXVec3Dot(&v, &v) <= sphere.r * sphere.r;
}
bool CheckCollision( Sphere& sphere, Obb& obb ) {
	Point3 p;
	// 球の中心に対する最近接点であるAABB上にある点pを見つける
	ClosestPtPointOBB(sphere.p, obb, p);

	// 球とAABBが交差するのは、球の中心から点pまでの（平方した）距離が
	// （平方した）球の半径よりも小さい場合
	Vector3 v = p - sphere.p;

	return D3DXVec3Dot(&v, &v) <= sphere.r * sphere.r;
}
bool CheckCollision( Obb& obb, Sphere& sphere ) {
	return CheckCollision( sphere, obb );
}
// カプセル vs カプセル
bool CheckCollision( Capsule& capsule1, Capsule& capsule2 ) {
	// カプセルの内部の構造の間の（平行した）距離を計算
	float s, t;
	Point3 c1, c2;
	float dist2 = ClosestPtSegmentSegment( capsule1.a, capsule1.b, capsule2.a, capsule2.b, s, t, c1, c2 );

	// （平方した）距離が（平行した）半径の総和よりも小さい場合は、衝突
	float radius = capsule1.r + capsule2.r;
	return dist2 <= radius * radius;
}


/*									//
//				描画				//
//									*/
// 球
void DrawCollision(Sphere& sphere, Color& Color ) {
	if( !bDraw ) return;

	DWORD Old;
	Matrix mat;
	D3DXMatrixIdentity(&mat);

	VERTEX_XYZ_COL VertexX[36];
	VERTEX_XYZ_COL VertexY[36];
	VERTEX_XYZ_COL VertexZ[36];
	for( int n = 0; n < 36; n++ ) {
		VertexX[n].x = sphere.p.x;
		VertexX[n].y = sphere.p.y+(sphere.r*cosf(D3DXToRadian(n*10)));
		VertexX[n].z = sphere.p.z+(sphere.r*sinf(D3DXToRadian(n*10)));
		VertexX[n].color = Color;
		VertexY[n].x = sphere.p.x+(sphere.r*sinf(D3DXToRadian(n*10)));
		VertexY[n].y = sphere.p.y;
		VertexY[n].z = sphere.p.z+(sphere.r*cosf(D3DXToRadian(n*10)));
		VertexY[n].color = Color;
		VertexZ[n].x = sphere.p.x+(sphere.r*sinf(D3DXToRadian(n*10)));
		VertexZ[n].y = sphere.p.y+(sphere.r*cosf(D3DXToRadian(n*10)));
		VertexZ[n].z = sphere.p.z;
		VertexZ[n].color = Color;
	};

	unsigned short Index[72];
	for( int n = 0, i = 0; n < 36; n++ ) {
		Index[i] = n;
		Index[i+1] = n+1;
		i+=2;
	};
	Index[71] = 0;


	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	pDevice->SetTransform( D3DTS_WORLD, &mat);

	pDevice->SetFVF( FVF_XYZ_COLOR );
	pDevice->SetTexture( 0, NULL );
	pDevice->GetRenderState( D3DRS_LIGHTING, &Old );
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 36, 36, Index, D3DFMT_INDEX16, VertexX, sizeof( VERTEX_XYZ_COL ) );
	pDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 36, 36, Index, D3DFMT_INDEX16, VertexY, sizeof( VERTEX_XYZ_COL ) );
	pDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 36, 36, Index, D3DFMT_INDEX16, VertexZ, sizeof( VERTEX_XYZ_COL ) );
	pDevice->SetRenderState( D3DRS_LIGHTING, Old );
}
// AABB
void DrawCollision(Aabb& aabb, D3DXCOLOR& Color ) {
	if( !bDraw ) return;

	DWORD Old;
	Matrix mat;
	D3DXMatrixIdentity(&mat);

	Point3 p = Point3( aabb.p[0], aabb.p[1], aabb.p[2] );
	VERTEX_XYZ_COL Vertex[8] = {
		{ p.x - aabb.r[0], p.y + aabb.r[1], p.z - aabb.r[2], Color },
		{ p.x - aabb.r[0], p.y + aabb.r[1], p.z + aabb.r[2], Color },
		{ p.x + aabb.r[0], p.y + aabb.r[1], p.z + aabb.r[2], Color },
		{ p.x + aabb.r[0], p.y + aabb.r[1], p.z - aabb.r[2], Color },
		{ p.x - aabb.r[0], p.y - aabb.r[1], p.z - aabb.r[2], Color },
		{ p.x - aabb.r[0], p.y - aabb.r[1], p.z + aabb.r[2], Color },
		{ p.x + aabb.r[0], p.y - aabb.r[1], p.z + aabb.r[2], Color },
		{ p.x + aabb.r[0], p.y - aabb.r[1], p.z - aabb.r[2], Color },
	};

	unsigned short Index[24] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	pDevice->SetTransform( D3DTS_WORLD, &mat );

	pDevice->SetFVF( FVF_XYZ_COLOR );
	pDevice->SetTexture( 0, NULL );
	pDevice->GetRenderState( D3DRS_LIGHTING, &Old );
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pDevice->DrawIndexedPrimitiveUP(
						D3DPT_LINELIST,
						0,
						8,
						12,
						Index,
						D3DFMT_INDEX16,
						Vertex,
						sizeof( VERTEX_XYZ_COL ) );
	pDevice->SetRenderState( D3DRS_LIGHTING, Old );
}
// OBB
void DrawCollision(Obb& obb, D3DXCOLOR& Color ) {
	if( !bDraw ) return;

	DWORD Old;
	Matrix mat;
	D3DXMatrixIdentity(&mat);

	Vector3 G = obb.p
		- (	obb.axis[0] * obb.r[0] * 2.0f
		+	obb.axis[1] * obb.r[1] * 2.0f
		+	obb.axis[2] * obb.r[2] * 2.0f ) / 2;

	Vector3 Axis[3] = {	obb.axis[0] * obb.r[0] * 2.0f,
						obb.axis[1] * obb.r[1] * 2.0f,
						obb.axis[2] * obb.r[2] * 2.0f };

	VERTEX_XYZ_COL Vertex[8] = {
		{ G.x, G.y, G.z, Color },
		{ G.x + Axis[1].x, G.y + Axis[1].y, G.z + Axis[1].z, Color },
		{ G.x + Axis[1].x + Axis[2].x, G.y + Axis[1].y + Axis[2].y, G.z + Axis[1].z + Axis[2].z, Color },
		{ G.x + Axis[2].x, G.y + Axis[2].y, G.z + Axis[2].z, Color },

		{ G.x + Axis[0].x, G.y + Axis[0].y, G.z + Axis[0].z, Color },
		{ G.x + Axis[0].x + Axis[1].x, G.y + Axis[0].y + Axis[1].y, G.z + Axis[0].z + Axis[1].z, Color },
		{ G.x + Axis[0].x + Axis[1].x + Axis[2].x, G.y + Axis[0].y + Axis[1].y + Axis[2].y, G.z + Axis[0].z + Axis[1].z + Axis[2].z, Color },
		{ G.x + Axis[0].x + Axis[2].x, G.y + Axis[0].y + Axis[2].y, G.z + Axis[0].z + Axis[2].z, Color },
	};

	unsigned short Index[24] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	pDevice->SetTransform( D3DTS_WORLD, &mat );

	pDevice->SetFVF( FVF_XYZ_COLOR );
	pDevice->SetTexture( 0, NULL );
	pDevice->GetRenderState( D3DRS_LIGHTING, &Old );
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 8, 12, Index, D3DFMT_INDEX16, Vertex, sizeof( VERTEX_XYZ_COL ) );
	pDevice->SetRenderState( D3DRS_LIGHTING, Old );
}
// カプセル
void DrawCollision(Capsule& capsule, D3DXCOLOR& Color ) {
	if( !bDraw ) return;

	DWORD Old;
	Matrix mat;
	D3DXMatrixIdentity(&mat);

	VERTEX_XYZ_COL VertexX1[36];
	VERTEX_XYZ_COL VertexX2[36];
	VERTEX_XYZ_COL VertexY1[36];
	VERTEX_XYZ_COL VertexY2[36];
	//VERTEX_XYZ_COL VertexY[36];
	//VERTEX_XYZ_COL VertexZ[36];
	for( int n = 0; n < 36; n++ ) {
		VertexX1[n].x = capsule.a.x+(capsule.r*cosf(D3DXToRadian(n*-10)));
		VertexX1[n].y = capsule.a.y;
		VertexX1[n].z = capsule.a.z+(capsule.r*sinf(D3DXToRadian(n*-10)));
		VertexX1[n].color = Color;
		VertexX2[n].x = capsule.a.x+(capsule.r*sinf(D3DXToRadian(n*10)));
		VertexX2[n].y = capsule.a.y+(capsule.r*cosf(D3DXToRadian(n*10)));
		VertexX2[n].z = capsule.a.z;
		VertexX2[n].color = Color;
		VertexY1[n].x = capsule.b.x+(capsule.r*cosf(D3DXToRadian(n*-10)));
		VertexY1[n].y = capsule.b.y;
		VertexY1[n].z = capsule.b.z+(capsule.r*sinf(D3DXToRadian(n*-10)));
		VertexY1[n].color = Color;
		VertexY2[n].x = capsule.b.x+(capsule.r*sinf(D3DXToRadian(n*10)));
		VertexY2[n].y = capsule.b.y+(capsule.r*cosf(D3DXToRadian(n*10)));
		VertexY2[n].z = capsule.b.z;
		VertexY2[n].color = Color;
	//	VertexZ[n].x = sphere.p.x+(sphere.r*sinf(D3DXToRadian(n*10)));
	//	VertexZ[n].y = sphere.p.y+(sphere.r*cosf(D3DXToRadian(n*10)));
	//	VertexZ[n].z = sphere.p.z;
	//	VertexZ[n].color = Color;
	};

	unsigned short Index[72];
	for( int n = 0, i = 0; n < 36; n++ ) {
		Index[i] = n;
		Index[i+1] = n+1;
		i+=2;
	};
	Index[71] = 0;

	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	pDevice->SetTransform( D3DTS_WORLD, &mat);

	pDevice->SetFVF( FVF_XYZ_COLOR );
	pDevice->SetTexture( 0, NULL );
	pDevice->GetRenderState( D3DRS_LIGHTING, &Old );
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 36, 36, Index, D3DFMT_INDEX16, VertexX1, sizeof( VERTEX_XYZ_COL ) );
	pDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 36, 36, Index, D3DFMT_INDEX16, VertexX2, sizeof( VERTEX_XYZ_COL ) );
	pDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 36, 36, Index, D3DFMT_INDEX16, VertexY1, sizeof( VERTEX_XYZ_COL ) );
	pDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 36, 36, Index, D3DFMT_INDEX16, VertexY2, sizeof( VERTEX_XYZ_COL ) );
	//pDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 36, 36, Index, D3DFMT_INDEX16, VertexY, sizeof( VERTEX_XYZ_COL ) );
	//pDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 36, 36, Index, D3DFMT_INDEX16, VertexZ, sizeof( VERTEX_XYZ_COL ) );
	pDevice->SetRenderState( D3DRS_LIGHTING, Old );
}



/*									//
//	分離軸の成分から投影線分長を算出//
//									*/
FLOAT LenSegOnSeparateAxis(
	Vector3 *Sep,
	Vector3 *e1,
	Vector3 *e2,
	Vector3 *e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	FLOAT r1 = fabsf(D3DXVec3Dot(Sep, e1));
	FLOAT r2 = fabsf(D3DXVec3Dot(Sep, e2));
	FLOAT r3 = e3 ? (fabsf(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}


/*									//
// 点cと線分abの間の距離の平方を返す//
//									*/
float SqDistPointSegment(Point3& a, Point3& b, Point3& c) {
	Vector3 ab = b - a, ac = c - a, bc = c - b;
	float e = D3DXVec3Dot(&ac, &ab);

	// cがabの外側に射影される場合を扱う
	if (e <= 0.0f) return D3DXVec3Dot(&ac, &ac);
	float f = D3DXVec3Dot(&ab, &ab);
	if (e >= f) return D3DXVec3Dot(&bc, &bc);

	// cがab上に射影される場合を扱う
	return D3DXVec3Dot(&ac, &ac) - e * e / f;
}


/*									//
// 点pとAABB bの間の距離の平方を返す//
//									*/
float SqDistPointAABB(Point3& p, Aabb& b) {
	float sqDist = 0.0f;
	for (int i = 0; i < 3; i ++) {
		// 各軸に対してボックスの範囲を超える余分な距離を計算
		float v = p[i];
		float min = b.p[i] - b.r[i];
		float max = b.p[i] + b.r[i];
		float len = min - v;
		if (v < min) sqDist += len * len;
		len = v - max;
		if (v > max) sqDist += len * len;
	}
	return sqDist;
}


/*									//
// 点pとOBB bの間の距離の平方を返す	//
//									*/
float SqDistPointOBB(Point3& p, Obb& b) {
	Vector3 v = p - b.p;
	float sqDist = 0.0f;
	for (int i = 0; i < 3; i ++) {
		// ボックスの中心からpまでのベクトルを各軸上に射影し、その軸に沿ったpの距離を得る
		// ボックスの範囲を超える余分な距離を計算
		float d = D3DXVec3Dot(&v, &b.axis[i]), excess = 0.0f;
		if (d < -b.r[i])
			excess = d + b.r[i];
		else if (d > b.r[i])
			excess = d - b.r[i];
		sqDist += excess * excess;
	}
	return sqDist;
}


/*									//
//		2つの線分の最近接点			//
//									*/
float ClosestPtSegmentSegment(Point3& p1, Point3& q1, Point3& p2, Point3& q2, float& s, float& t, Point3& c1, Point3& c2) {
	Vector3 d1 = q1 - p1;	// 線分S1の方向ベクトル
	Vector3 d2 = q2 - p2;	// 線分S2の方向ベクトル
	Vector3 r = p1 - p2;
	float a = D3DXVec3Dot(&d1, &d1);	// 線分S1の距離の平方、常に非負
	float e = D3DXVec3Dot(&d2, &d2);	// 線分S2の距離の平方、常に非負
	float f = D3DXVec3Dot(&d2, &r);

	// 片方あるいは両方の線分が点に縮退しているかどうかチェック
	if (a <= FLT_EPSILON && e <= FLT_EPSILON) {
		// 両方の線分が点に縮退
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;
		return D3DXVec3Dot(&(c1 - c2), &(c1 - c2));
	}
	if (a <= FLT_EPSILON) {
		// 最初の線分が点に縮退
		s = 0.0f;
		t = Clamp(f / e, 0.0f, 1.0f);	// s = 0 => t = (b * s + f) / e = f / e
	} else {
		float c = D3DXVec3Dot(&d1, &r);
		if (e <= FLT_EPSILON) {
			// 2番目の線分が点に縮退
			t = 0.0f;
			s = Clamp(-c / a, 0.0f, 1.0f);	// t = 0 => s = (b * t - c) / a = -c / a
		} else {
			// ここから一般的な縮退の場合を開始
			float b = D3DXVec3Dot(&d1, &d2);
			float denom = a * e - b * b;	// 常に非負
			// 線分が平行でない場合、L1上のL2に対する最近接点を計算、そして
			// 線分S1に対してクランプ。そうでない場合は任意s(ここでは0)を選択
			if (denom != 0.0f) {
				s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			} else s = 0.0f;
			// L2上のS1(s)に対する最近接点を以下を用いて計算
			// t = Dot((P1 + D1 * s) - P2, D2) / Dot(D2, D2) = (b * s + f) / e
			float tnom = b * s + f;
			// tが[0,1]の中にあれば終了。そうでなければtをクランプ、sをtの新しい値に対して以下を用いて再計算
			// s = Dot((P2 + D2 * t) - P1, D1) / Dot(D1, D1) = (t * b - c) / a
			// そしてsを[0,1]に対してクランプ
			if (tnom < 0.0f) {
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
				//			} else if( t > 1.0f ) {
			} else if (tnom > e) {
				t = 1.0f;
				s = Clamp((b - c) / a, 0.0f, 1.0f);
			} else {
				t = tnom / e;
			}
		}
	}

	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;
	return D3DXVec3Dot(&(c1 - c2), &(c1 - c2));
}


/*									//
//	線分abおよび点cに対して、		//
//	ab上の最近接点dを算出			//
//									*/
void ClosestPtPointSegment(Point3 c, Point3 a, Point3 b, float& t, Point3& d) {
	Vector3 ab = b - a;

	// ab上にcを射影、しかしDot(ab, ab)による除算は延期
	t = D3DXVec3Dot(&(c - a), &ab);
	if (t <= 0.0f) {
		// cは範囲[a,b]の外側、aの側に射影され、aまでクランプ
		t = 0.0f;
		d = a;
	} else {
		float denom = D3DXVec3Dot(&ab, &ab);	// denom = |ab|^2 なので常に非負
		if (t >= denom) {
			// cは範囲[a,b]の外側、bの側に射影され、bまでクランプ
			t = 1.0f;
			d = b;
		} else {
			// cは範囲[a,b]の内側に射影され、この時まで除算は延期
			t = t / denom;
			d = a + t * ab;
		}
	}
}


/*									//
//	点pに対して、AABB bの上もしくは	//
//	中にあるpの最近接点qを算出		//
//									*/
void ClosestPtPointAABB(Point3& p, Aabb& b, Point3& q) {
	// 各座標軸に対して、点の座標値がボックスの外側にある場合に
	// ボックスまで点をクランプ、そうでなければそのまま
	for (int i = 0; i < 3; i ++) {
		float v = p[i];
		float min = b.p[i] - b.r[i];
		float max = b.p[i] + b.r[i];
		if (v < min) v = min;	// v = max( v, b.min[i] )
		if (v > max) v = max;	// v = min( v, b.max[i] )
		q[i] = v;
	}
}


/*									//
//	点pに対して、OBB bの上もしくは	//
//	中にあるpの最近接点qを算出		//
//									*/
void ClosestPtPointOBB(Point3& p, Obb& b, Point3& q) {
	Vector3 d = p - b.p;
	// ボックスの中心における結果から開始、そこから段階的に進める
	q = b.p;
	// 各OBBの軸に対して
	for (int i = 0; i < 3; i ++) {
		// ...dをその軸に射影して
		// ボックスの中心からdの軸に沿った距離を得る
		float dist = D3DXVec3Dot(&d, &b.axis[i]);
		// ボックスの範囲よりも距離が大きい場合、ボックスまでクランプ
		if (dist > b.r[i]) dist = b.r[i];
		if (dist < -b.r[i]) dist = -b.r[i];
		// ワールド座標を得るためにその距離だけ軸に沿って進める
		q += dist * b.axis[i];
	}
}