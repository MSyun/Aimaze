// 3D�Q�[�W�N���X
// 2017. 1.31	�v���O�����쐬
// Author		SyunMizuno

#pragma once



#include	"../../../GameFramework/framework/Mesh/Quad/Quad.h"



class Gauge3	:	public	Quad {
protected:
	int		m_nMax;		// �ő�l
	int		m_nCur;		// ���ݒl

public:
	Gauge3(int _max, LPDIRECT3DTEXTURE9 _tex = NULL);
	virtual ~Gauge3() {};

	virtual HRESULT Init(float _SizeX, float _SizeZ);

	void SetCurrentNum(int _num);
};