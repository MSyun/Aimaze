//	���C�g�N���X
// 2016. 3.27	�v���O�����쐬
// 2016. 4.21	�t�H���p�̃X�y�L���������o�ǉ�
// 2016. 5.10	���C�g�̐F��ǉ�
// author		SyunMizuno

#pragma once


#include	"../Convert.h"


/*
	memo	:
			���̃N���X�͂��Ԃ�DirectionalLight
*/


class	Light {
private:
#pragma region variable

	Point3	m_vPos;
	Vector3	m_vLookAt;
	Vector3	m_vUp;
	Matrix	m_mtxView;
	Matrix	m_mtxProj;
	float	m_fNear;	// �j�A�N���b�v
	float	m_fFar;		// �t�@�[�N���b�v

	Color	m_vColor;	// ���C�g�̐F

	float	m_fSpeclar;	// ���C�g�̋���

#pragma endregion

public:
#pragma region method

	Light();
	~Light();
	void Set(bool _custom = false);
	void SetPos(Vector3* _pos) { m_vPos = *_pos; }
	void SetLook(Vector3* _look) { m_vLookAt = *_look; }
	void SetNear(float _near) { m_fNear = _near; }
	void SetFar(float _far) { m_fFar = _far; }

	void SetColor(float r, float g, float b, float a) { m_vColor.r = r; m_vColor.g = g; m_vColor.b = b; m_vColor.a = a; }
	void SetColor(float col) { m_vColor.r = col; m_vColor.g = col; m_vColor.b = col; }
	void SetColor(Color col) { m_vColor = col; }

	Vector3 GetPos() { return m_vPos; }
	Vector4 GetDir() { return Vector4(m_vLookAt.x - m_vPos.x, m_vLookAt.y - m_vPos.y, m_vLookAt.z - m_vPos.z, 0.f); }
	Matrix GetView() { return m_mtxView; }
	Matrix GetProj() { return m_mtxProj; }
	void SetSpecular(float _specular) { m_fSpeclar = _specular; }
	float GetSpecular() { return m_fSpeclar; }
	D3DXCOLOR GetColor() { return m_vColor; }

#pragma endregion
};