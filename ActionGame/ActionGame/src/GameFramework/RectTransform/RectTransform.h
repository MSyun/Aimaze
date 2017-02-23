// 2D�p�@�ʒu�A��]�A�X�P�[��������
// 2017. 1.27	�v���O�����쐬
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
	// ���W�擾
	Point3 GetPos();
	// ���W�ݒ�
	void SetPos(const Point3& pos);
	void SetPos(float x, float y, float z);
	// ���ړ�
	void Translate(const Vector3& vec, RectTransform* relativeTo = NULL);
	void Translate(float x, float y, float z, RectTransform* relativeTo = NULL);
#pragma endregion

#pragma region Rotation
	// ��]�p�x�̎擾
	float GetAngle();
	// ��]
	void Rotate(float angle);
	// �I�C���[�p
	void SetEularAngles(float angle);
	// �p�x�␳
	void ReSetAngle();
#pragma endregion

#pragma region Scale
	// �g�嗦�擾�i���[���h
	Vector3 GetScale();
	// �g�嗦�ݒ�
	void SetScale(const Vector3& scale);
	// �g�嗦�ݒ�
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

	// �I�u�W�F�N�g�̎擾
	IObj* GetGameObject() { return m_pObject; }

	// �폜
	void Destroy();

#pragma endregion

private:
#pragma region conceal method

#pragma region Position
	// ���[�J�����ړ�
	void TranslateLocal(const Vector3& vec, RectTransform* relativeTo);
	void TranslateLocal(float x, float y, float z, RectTransform* relativeTo);
	// ���[���h���ړ�
	void TranslateWorld(const Vector3& vec);
	void TranslateWorld(float x, float y, float z);
#pragma endregion

	void ReSetVertex();

#pragma endregion
};