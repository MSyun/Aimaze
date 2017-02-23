// �O���t�B�b�N
// 2017. 2. 23	�v���O�����쐬
// author		SyunMizuno


#pragma once

#include	<d3dx9.h>
#include	"IGraphics/IGraphics.h"
#include	"../Singleton/Singleton.h"

/*
	Memo	:
			DirectX9����ɂȂ��Ă���̂ŗv�C��
*/


class Graphics	:	public	IGraphics	,	public	Singleton<Graphics> {
private:
	LPDIRECT3DDEVICE9	m_pDevice;

public:
	Graphics();
	virtual ~Graphics();

	LPDIRECT3DDEVICE9 GetDevice();
	void SetDevice(LPDIRECT3DDEVICE9 _dev);

#pragma region View
	// �r���[�̐ݒ�
	virtual void SetView(Matrix& view);
	// �r���[�̎擾
	virtual Matrix GetView();
#pragma endregion

#pragma region Proj
	// �v���W�F�N�V�����̐ݒ�
	virtual void SetProjection(Matrix& proj);
	// �v���W�F�N�V�����̎擾
	virtual Matrix GetProjection();
#pragma endregion
};


inline Graphics* GetGraphics() {
	return Graphics::Instance();
}