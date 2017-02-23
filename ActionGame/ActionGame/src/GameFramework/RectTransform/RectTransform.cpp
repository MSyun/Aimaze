// 2D用　位置、回転、スケールを扱う
// 2017. 1.27	プログラム作成
// author		SyunMizuno


#include	"RectTransform.h"
#include	"../framework/Utility/Math/MathUtility.h"


/*
	Memo	:
			Pivotの設定がまだ終わってないよ
*/



/*									//
//			コンストラクタ			//
//									*/
RectTransform::RectTransform(IObj* object) :
	m_pObject(object),
	m_vPosition(0.0f, 0.0f, 0.0f),
	m_fAngle(0.0f),
	m_vScale(1.0f, 1.0f, 1.0f),
	m_vSize(100.0f, 100.0f),
	m_vPivot(0.5f, 0.5f)
{
	ReSetVertex();
}

#pragma region Position
/*									//
//				座標取得			//
//									*/
Point3 RectTransform::GetPos() {
	return m_vPosition;
}


/*									//
//				座標設定			//
//									*/
void RectTransform::SetPos(const Point3& pos) {
	SetPos(pos.x, pos.y, pos.z);
}
void RectTransform::SetPos(float x, float y, float z) {
	m_vPosition.x = x;
	m_vPosition.y = y;
	m_vPosition.z = z;
	ReSetVertex();
}

#pragma region conceal method
/*									//
//			ローカル軸移動			//
//									*/
void RectTransform::TranslateLocal(const Vector3& vec, RectTransform* relativeTo) {
	TranslateLocal(vec.x, vec.y, vec.z, relativeTo);
}
void RectTransform::TranslateLocal(float x, float y, float z, RectTransform* relativeTo) {
	//Vector3 move =
	//	relativeTo->GetRight() * x +
	//	relativeTo->GetUp() * y +
	//	relativeTo->GetForward() * z;
	//SetPos(GetPos() + move);
}


/*									//
//			ワールド軸移動			//
//									*/
void RectTransform::TranslateWorld(const Vector3& vec) {
	SetPos(GetPos() + vec);
}
void RectTransform::TranslateWorld(float x, float y, float z) {
	SetPos(GetPos() + Vector3(x, y, z));
}
#pragma endregion

/*									//
//				軸移動				//
//									*/
void RectTransform::Translate(const Vector3& vec, RectTransform* relativeTo) {
	Translate(vec.x, vec.y, vec.z, relativeTo);
}
void RectTransform::Translate(float x, float y, float z, RectTransform* relativeTo) {
	if (relativeTo == NULL) {
		TranslateWorld(x, y, z);
	} else {
		TranslateLocal(x, y, z, relativeTo);
	}
	ReSetVertex();
}
#pragma endregion

#pragma region Rotate
/*									//
//			オイラー角取得			//
//									*/
float RectTransform::GetAngle() {
	return m_fAngle;
}


/*									//
//				軸回転				//
//									*/
void RectTransform::Rotate(float angle) {
	m_fAngle += angle;
	ReSetAngle();
	ReSetVertex();
}


/*									//
//			オイラー角?設定			//
//									*/
void RectTransform::SetEularAngles(float angle) {
	m_fAngle = angle;
	ReSetAngle();
}


/*									//
//			角度の再設定			//
//									*/
void RectTransform::ReSetAngle() {
	if (m_fAngle >= 360.0f)	m_fAngle -= 360.0f;
	else
		if (m_fAngle < 0.0f)		m_fAngle += 360.0f;
}
#pragma endregion

#pragma region Scale
/*									//
//				拡大率取得			//
//									*/
Vector3 RectTransform::GetScale() {
	return m_vScale;
}


/*									//
//				拡大率設定			//
//									*/
void RectTransform::SetScale(const Vector3& scale) {
	SetScale(scale.x, scale.y, scale.z);
}
void RectTransform::SetScale(float x, float y, float z) {
	m_vScale.x = x;
	m_vScale.y = y;
	m_vScale.z = z;
	ReSetVertex();
}
#pragma endregion

/*									//
//				削除通知			//
//									*/
void RectTransform::Destroy() {
		m_pObject->Destroy();
}

#pragma region Size
/*									//
//				サイズ取得			//
//									*/
Vector2 RectTransform::GetSize() {
	return m_vSize;
}


/*									//
//				サイズ設定			//
//									*/
void RectTransform::SetSize(Vector2& size) {
	SetSize(size.x, size.y);
}
void RectTransform::SetSize(float x, float y) {
	m_vSize.x = x;
	m_vSize.y = y;
	ReSetVertex();
}
#pragma endregion

#pragma region Pivot
/*									//
//				サイズ取得			//
//									*/
Point2 RectTransform::GetPivot() {
	return m_vPivot;
}


/*									//
//				サイズ設定			//
//									*/
void RectTransform::SetPivot(Point2& pivot) {
	SetPivot(pivot.x, pivot.y);
}
void RectTransform::SetPivot(float x, float y) {
	m_vPivot.x = Clamp(x, 0.0f, 1.0f);
	m_vPivot.y = Clamp(y, 0.0f, 1.0f);
	ReSetVertex();
}
#pragma endregion

/*									//
//				頂点取得			//
//									*/
Vector3* RectTransform::GetVertex() {
	return m_Vertex;
}

/*									//
//				頂点リセット		//
//									*/
void RectTransform::ReSetVertex() {
	float fSin = sinf(D3DXToRadian(m_fAngle));
	float fCos = cosf(D3DXToRadian(m_fAngle));

	m_Vertex[0] = Vector3(
		m_vPosition.x + (-m_vSize.x * m_vPivot.x * fCos * m_vScale.x) - (-m_vSize.y * m_vPivot.y * fSin * m_vScale.y),
		m_vPosition.y - (-m_vSize.x * m_vPivot.x * -fSin * m_vScale.x) + (-m_vSize.y * m_vPivot.y * fCos * m_vScale.y),
		m_vPosition.z);

	m_Vertex[1] = Vector3(
		m_vPosition.x + (m_vSize.x * m_vPivot.x * fCos * m_vScale.x) - (-m_vSize.y * m_vPivot.y * fSin * m_vScale.y),
		m_vPosition.y - (m_vSize.x * m_vPivot.x * -fSin * m_vScale.x) + (-m_vSize.y * m_vPivot.y * fCos * m_vScale.y),
		m_vPosition.z);

	m_Vertex[2] = Vector3(
		m_vPosition.x + (-m_vSize.x * m_vPivot.x * fCos * m_vScale.x) - (m_vSize.y * m_vPivot.y * fSin * m_vScale.y),
		m_vPosition.y - (-m_vSize.x * m_vPivot.x * -fSin * m_vScale.x) + (m_vSize.y * m_vPivot.y * fCos * m_vScale.y),
		m_vPosition.z);

	m_Vertex[3] = Vector3(
		m_vPosition.x + (m_vSize.x * m_vPivot.x * fCos * m_vScale.x) - (m_vSize.y * m_vPivot.y * fSin * m_vScale.y),
		m_vPosition.y - (m_vSize.x * m_vPivot.x * -fSin * m_vScale.x) + (m_vSize.y * m_vPivot.y * fCos * m_vScale.y),
		m_vPosition.z);
}