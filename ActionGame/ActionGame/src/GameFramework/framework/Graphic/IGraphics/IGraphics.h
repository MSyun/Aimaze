// �O���t�B�b�N�C���^�[�t�F�[�X
// 2017. 2. 23	�v���O�����쐬
// author		SyunMizuno


#pragma once

#include	"../../Convert.h"

/*
	Memo	:
			���̂܂܂ł�Get/SetDevice���悭�Ȃ��̂ŗv�C��
*/



__interface IGraphics {
public:
	virtual void SetView(Matrix& view);
	virtual Matrix GetView();
	virtual void SetProjection(Matrix& proj);
	virtual Matrix GetProjection();
};