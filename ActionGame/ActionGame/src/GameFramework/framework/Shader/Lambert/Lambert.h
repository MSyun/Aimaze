//	�����o�[�g�N���X
// 2016. 3.12	�v���O�����쐬
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"../HlslDrawBase.h"

/*
	memo:
		�t�H���ƕ��p����ꍇ���̃N���X�̓t�H���ő�p�ł��邽�ߕK�v�Ȃ�
*/

class	Lambert : public	IHlslDrawBase {
public:
	Lambert();
	virtual ~Lambert();
	virtual HRESULT Load();

protected:
	// �n���h���̐ڑ�
	virtual void ConnectHandle();
};