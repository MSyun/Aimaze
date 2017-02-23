// 2D�p�@�ʒu�A��]�A�X�P�[��������
// 2017. 1.27	�v���O�����쐬
// author		SyunMizuno


#include	"RectTransform.h"
#include	"../framework/Utility/Math/MathUtility.h"


/*
	Memo	:
			Pivot�̐ݒ肪�܂��I����ĂȂ���
*/



/*									//
//			�R���X�g���N�^			//
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
//				���W�擾			//
//									*/
Point3 RectTransform::GetPos() {
	return m_vPosition;
}


/*									//
//				���W�ݒ�			//
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
//			���[�J�����ړ�			//
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
//			���[���h���ړ�			//
//									*/
void RectTransform::TranslateWorld(const Vector3& vec) {
	SetPos(GetPos() + vec);
}
void RectTransform::TranslateWorld(float x, float y, float z) {
	SetPos(GetPos() + Vector3(x, y, z));
}
#pragma endregion

/*									//
//				���ړ�				//
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
//			�I�C���[�p�擾			//
//									*/
float RectTransform::GetAngle() {
	return m_fAngle;
}


/*									//
//				����]				//
//									*/
void RectTransform::Rotate(float angle) {
	m_fAngle += angle;
	ReSetAngle();
	ReSetVertex();
}


/*									//
//			�I�C���[�p?�ݒ�			//
//									*/
void RectTransform::SetEularAngles(float angle) {
	m_fAngle = angle;
	ReSetAngle();
}


/*									//
//			�p�x�̍Đݒ�			//
//									*/
void RectTransform::ReSetAngle() {
	if (m_fAngle >= 360.0f)	m_fAngle -= 360.0f;
	else
		if (m_fAngle < 0.0f)		m_fAngle += 360.0f;
}
#pragma endregion

#pragma region Scale
/*									//
//				�g�嗦�擾			//
//									*/
Vector3 RectTransform::GetScale() {
	return m_vScale;
}


/*									//
//				�g�嗦�ݒ�			//
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
//				�폜�ʒm			//
//									*/
void RectTransform::Destroy() {
		m_pObject->Destroy();
}

#pragma region Size
/*									//
//				�T�C�Y�擾			//
//									*/
Vector2 RectTransform::GetSize() {
	return m_vSize;
}


/*									//
//				�T�C�Y�ݒ�			//
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
//				�T�C�Y�擾			//
//									*/
Point2 RectTransform::GetPivot() {
	return m_vPivot;
}


/*									//
//				�T�C�Y�ݒ�			//
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
//				���_�擾			//
//									*/
Vector3* RectTransform::GetVertex() {
	return m_Vertex;
}

/*									//
//				���_���Z�b�g		//
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