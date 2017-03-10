//////////////////////////////////////////////////////////////
// �O�ՃN���X
// 2016. 7.26	�v���O�����쐬
// 2016. 7.29	����
// 2016. 9.10	�␳On,Off�ǉ�
// Author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	<d3dx9.h>
#include	"../framework/Convert.h"


class	Locus {
private:
#pragma region struct

	struct TLocus {
		Vector3	Pos;
		Vector2	Tex;
	};

#pragma endregion

#pragma region variable

#define		SAVE_NUM	400	// �ۑ��t���[��
#define		DIVIDE_NUM	3	// �������̐�

	bool	m_bUse;	// �L�^�t���O

	bool	m_bRevision;	// �␳��On,Off

	// ���W����̋���
	float	m_fUp;
	float	m_fDown;

	//----- ���_���
	// �ۑ��p
	TLocus	m_vStart[SAVE_NUM];
	TLocus	m_vEnd[SAVE_NUM];
	// �Ȑ��⊮
	TLocus* m_pStartVertex;
	TLocus* m_pEndVertex;

	// ���_�ۑ���
	int	m_nVertexNum;

	// �ŐV�v�f
	int	m_nNew;

	// �����鑬�x
	float m_fGhostSpeed;

	//----- �e�N�X�`��
	LPDIRECT3DTEXTURE9	m_pTex;

#pragma endregion

public:
#pragma region method

	Locus();
	virtual ~Locus();

	// �X�V
	virtual void Update();
	// �`��
	virtual void Draw();

	// ���ΓI�ȏ�̍��W�ݒ�
	void SetUp(const float up) { m_fUp = up; }

	// ���ΓI�ȉ��̍��W�ݒ�
	void SetDown(const float down) { m_fDown = down; }

	// �J�n
	void Begin(Matrix& mat);

	// �I��
	void End();

	// ���W�̐ݒ�
	void SetNewPos(Matrix& mat);

	// �ۂ�����␳�̐ݒ�
	void SetRevision(const bool _revision) { m_bRevision = _revision; }

	// �e�N�X�`���ݒ�
	void SetTex(LPDIRECT3DTEXTURE9 tex) { m_pTex = tex; }

	// �����鑬�x�ݒ�
	void SetGhostSpeed(float speed) { m_fGhostSpeed = speed; }

#pragma endregion
};