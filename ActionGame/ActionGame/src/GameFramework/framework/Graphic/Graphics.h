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
	// �r���[�s��̐ݒ�
	virtual void SetView(const Matrix& view);
	// �r���[�s��̎擾
	virtual Matrix GetView();
#pragma endregion

#pragma region Projection
	// �v���W�F�N�V�����s��̐ݒ�
	virtual void SetProjection(const Matrix& proj);
	// �v���W�F�N�V�����s��̎擾
	virtual Matrix GetProjection();
#pragma endregion

#pragma region Alpha Blend
	// ���u�����h���g�p���邩�ݒ�
	virtual void SetAlphaBlendEnable(bool flg);
	// ���u�����h�̐ݒ���擾
	// true. �g�p�� : false. ���g�p
	virtual bool GetAlphaBlendEnable();
#pragma endregion

#pragma region Z Enable
	// Z�o�b�t�@���g�p���邩�ݒ�
	virtual void SetZEnable(bool flg);
	// Z�o�b�t�@�̐ݒ���擾
	// true. �g�p�� : false. ���g�p
	virtual bool GetZEnable();
#pragma endregion

#pragma region Wrap Mode
	// Wrap�̐ݒ�
	virtual void SetWrapMode(_eWrapMode mode, _eWrapVec vec = WRAPVEC_UV);
	// Wrap�̎擾
	virtual _eWrapMode GetWrapMode(_eWrapVec vec);
#pragma endregion

#pragma region Cull Mode
	// Culling�̐ݒ�
	virtual void SetCullMode(_eCullMode mode);
	// Culling�̎擾
	virtual _eCullMode GetCullMode();
#pragma endregion


private:
	// Wrap���琔���֕ύX
	DWORD WrapModeToNum(_eWrapMode mode);
	// ��������Wrap�֕ύX
	_eWrapMode NumToWrapMode(DWORD num);
};


inline Graphics* GetGraphics() {
	return Graphics::Instance();
}