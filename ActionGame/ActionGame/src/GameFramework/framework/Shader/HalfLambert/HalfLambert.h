//	�����o�[�g�N���X
// 2016. 3.12	�v���O�����쐬
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"../HlslDrawBase.h"

/*
	memo:
		�t�H���ƕ��p����ꍇ���̃N���X�̓t�H���ő�p�ł��邽�ߕK�v�Ȃ�
		(�t�H���ŗ��p���Ă����)
*/

class	HalfLambert : public	IHlslDrawBase {
public:
#pragma region method

	HalfLambert();
	virtual ~HalfLambert();
	virtual HRESULT Load();

#pragma endregion

protected:
	// �n���h���̐ڑ�
	virtual void ConnectHandle();
};