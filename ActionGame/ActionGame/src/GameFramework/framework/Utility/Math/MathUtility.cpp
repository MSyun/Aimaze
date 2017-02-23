#include	"MathUtility.h"



// [min, max]の範囲内までnをクランプ
float Clamp(float val, float min, float max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}
int Clamp(int val, int min, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}


// 2つのベクトルからABのなす角度θを求める
float AngleOf2Vector(Vector3 A, Vector3 B) {
	// ※ベクトルの長さが0だと答えが出ないので注意

	// ベクトルAとBの長さを計算する
	float length_A = D3DXVec3Length(&A);
	float length_B = D3DXVec3Length(&B);

	// 内積とベクトル長さを使ってcosθを求める
	float cos_sita = D3DXVec3Dot(&A, &B) / (length_A * length_B);

	// cosθからθを求める
	float sita = acosf(cos_sita);

	// ラジアンから 0 ～ 180 の角度でほしい場合はコメントを外す
	sita = sita * 180.0f / D3DX_PI;

	return sita;
}


//////////////////////////////////
 // 壁ずりベクトル
 //
 // out : 正規化壁ずりベクトル（戻り値）
 // front : 進行ベクトル
 // normal: 衝突点での法線ベクトル
 //
Vector3* WallScratchVector(
	const Vector3& front,
	const Vector3& normal)
{
	Vector3 normal_n;
	Vector3 out;
	D3DXVec3Normalize(&normal_n, &normal);
	return D3DXVec3Normalize(&out, &(front - D3DXVec3Dot(&front, &normal_n) * normal_n));
}

// ベジエ曲線
Vector2 Bezier(
	Vector2& p0,
	Vector2& p1,
	Vector2& p2,
	Vector2& p3,
	float t)
{
	Vector2 point;
	float rt1 = (1 - t);
	float rt2 = rt1 * rt1;
	float t2 = t * t;
	float a, b, c, d;
	a = rt1 * rt2;
	b = 3.0f * rt2 * t;
	c = 3.0f * rt1 * t2;
	d = t * t2;
	point.x =
		a * p0.x +
		b * p1.x +
		c * p2.x +
		d * p3.x;
	point.y =
		a * p0.y +
		b * p1.y +
		c * p2.y +
		d * p3.y;
	return point;
}

// Catmull-Romスプライン曲線
Vector3 CatmullRom(
	Vector3& p0,
	Vector3& p1,
	Vector3& p2,
	Vector3& p3,
	float t)		// 0.0f ～ 1.0f
{
	// 公式
	Vector3	v0 = 0.5f * (p2 - p0);
	Vector3	v1 = 0.5f * (p3 - p1);
	float	t2 = t * t;
	float	t3 = t2 * t;
	return	((p1 - p2)*2.0f + v0 + v1) * t3 +
		((p2 - p1)*3.0f - 2.0f*v0 - v1) * t2 +
		v0 * t + p1;
}

// OBBと平面の衝突判定関数
bool OBBvsPlane(
	Obb& obb,
	Vector3& Normal,
	Vector3& Cross,
	float* Len) {
	// 平面の法線に対するOBBの射影線の長さを算出
	float r = 0.0f;					// 近接距離
	Vector3 PlaneNormal = Normal;	// 平面の法線ベクトル

	for (int i = 0; i < 3; i ++) {
		Vector3 Direct = obb.axis[i];	// OBBの1つの軸ベクトル
		r += fabsf(D3DXVec3Dot(&(Direct * obb.r[i]), &PlaneNormal));
	}

	// 平面とOBBの距離を算出
	Vector3 ObbPos = obb.p;
	Vector3 PlanePos = Cross;
	float s = D3DXVec3Dot(&(ObbPos - PlanePos), &PlaneNormal);

	// 戻し距離を算出
	if (Len) {
		if (s > 0)	*Len = r - fabsf(s);
		else		*Len = r + fabsf(s);
	}

	// 衝突判定
	if (fabs(s) - r < 0.0f + FLT_EPSILON)
		return true;	// 衝突している

	return false;		// 衝突していない
}