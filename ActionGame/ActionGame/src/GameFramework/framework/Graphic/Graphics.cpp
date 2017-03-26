// �O���t�B�b�N
// 2017. 2. 23	�v���O�����쐬
// author		SyunMizuno


#include	"Graphics.h"
#include	"../Debug/Debug.h"


/*									//
//			�R���X�g���N�^			//
//									*/
Graphics::Graphics() :
	m_pDevice(NULL)
{
}


/*									//
//			�f�X�g���N�^			//
//									*/
Graphics::~Graphics() {
}


/*									//
//			�f�o�C�X�̎擾			//
//									*/
LPDIRECT3DDEVICE9 Graphics::GetDevice() {
	return m_pDevice;
}


/*									//
//			�f�o�C�X�̐ݒ�			//
//									*/
void Graphics::SetDevice(LPDIRECT3DDEVICE9 _dev) {
	m_pDevice = _dev;
}


#pragma region View
/*									//
//			�r���[�s��̐ݒ�		//
//									*/
void Graphics::SetView(const Matrix& view) {
	m_pDevice->SetTransform(D3DTS_VIEW, &view);
}


/*									//
//			�r���[�s��̎擾		//
//									*/
Matrix Graphics::GetView() {
	Matrix mat;
	HRESULT hr = S_OK;

	hr = m_pDevice->GetTransform(D3DTS_VIEW, &mat);
	if (FAILED(hr)) {
		Debug::LogError("View�̎擾�Ɏ��s���܂���");
	}
	return mat;
}
#pragma endregion


#pragma region Projection
/*									//
//		�v���W�F�N�V�����s��̐ݒ�	//
//									*/
void Graphics::SetProjection(const Matrix& proj) {
	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);
}


/*									//
//		�v���W�F�N�V�����s��̎擾	//
//									*/
Matrix Graphics::GetProjection() {
	Matrix mat;
	HRESULT hr = S_OK;
	hr = m_pDevice->GetTransform(D3DTS_PROJECTION, &mat);
	if (FAILED(hr)) {
		Debug::LogError("Projection�̎擾�Ɏ��s���܂���");
	}
	return mat;
}
#pragma endregion


#pragma region Alpha Blend
/*									//
//	���u�����h���g�p���邩�ݒ�		//
//									*/
void Graphics::SetAlphaBlendEnable(bool flg) {
	HRESULT hr = S_OK;
	hr = m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, flg);
	if (hr != S_OK) {
		Debug::LogError("SetAlphaBlendEnable : �ݒ�Ɏ��s���܂���");
	}
}


/*									//
//		���u�����h�̐ݒ���擾		//
//									*/
// true. �g�p�� : false. ���g�p
bool Graphics::GetAlphaBlendEnable() {
	DWORD use;
	HRESULT hr = S_OK;
	hr = m_pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &use);
	if (hr != S_OK) {
		Debug::LogError("GetAlphaBlendEnable : �擾�Ɏ��s���܂���");
	}

	return use != 0 ? true : false;
}
#pragma endregion


#pragma region Z Enable
/*									//
//		Z�o�b�t�@���g�p���邩�ݒ�	//
//									*/
void Graphics::SetZEnable(bool flg) {
	HRESULT hr = S_OK;
	hr = m_pDevice->SetRenderState(D3DRS_ZENABLE, flg);
	if (hr != S_OK) {
		Debug::LogError("SetZEnable : �ݒ�Ɏ��s���܂���");
	}
}


/*									//
//		Z�o�b�t�@�̐ݒ���擾		//
//									*/
// true. �g�p�� : false. ���g�p
bool Graphics::GetZEnable() {
	DWORD use;
	HRESULT hr = S_OK;
	hr = m_pDevice->GetRenderState(D3DRS_ZENABLE, &use);
	if (hr != S_OK) {
		Debug::LogError("GetZEnable : �擾�Ɏ��s���܂���");
	}

	return use != 0 ? true : false;
}
#pragma endregion


#pragma region Wrap Mode
/*									//
//			Wrap�̐ݒ�				//
//									*/
void Graphics::SetWrapMode(_eWrapMode mode, _eWrapVec vec) {
	DWORD Mode = WrapModeToNum(mode);
	HRESULT hr = S_OK;

	switch (vec) {
	case _eWrapVec::WRAPVEC_U:
		hr = m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, Mode);
		break;

	case _eWrapVec::WRAPVEC_V:
		hr = m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, Mode);
		break;

	case _eWrapVec::WRAPVEC_UV:
		hr = m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, Mode);
		if (hr != S_OK)	break;
		hr = m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, Mode);
		break;
	};

	if (hr != S_OK) {
		Debug::LogError("SetWrapMode : �ݒ�Ɏ��s���܂���");
	}
}


/*									//
//			Wrap�̎擾				//
//									*/
_eWrapMode Graphics::GetWrapMode(_eWrapVec vec) {
	DWORD Mode = 0;
	HRESULT hr = S_OK;

	switch (vec) {
	case _eWrapVec::WRAPVEC_U:
		hr = m_pDevice->GetSamplerState(0, D3DSAMP_ADDRESSU, &Mode);
		break;

	case _eWrapVec::WRAPVEC_V:
		hr = m_pDevice->GetSamplerState(0, D3DSAMP_ADDRESSV, &Mode);
		break;

	case _eWrapVec::WRAPVEC_UV:
		hr = m_pDevice->GetSamplerState(0, D3DSAMP_ADDRESSU, &Mode);
		Debug::Log("GetWrapMode : UV���ݒ肳�ꂽ�̂�U�̂ݗD��ŕԂ��܂�");
		break;
	};

	if (hr != S_OK) {
		Debug::LogError("GetWrapMode : �擾�Ɏ��s���܂���");
	}

	return NumToWrapMode(Mode);
}


/*									//
//		Wrap���琔���֕ύX			//
//									*/
DWORD Graphics::WrapModeToNum(_eWrapMode mode) {
	DWORD num;
	switch (mode) {
	case _eWrapMode::WRAPMODE_WRAP:
		num = D3DTADDRESS_WRAP;
		break;

	case _eWrapMode::WRAPMODE_CLAMP:
		num = D3DTADDRESS_CLAMP;
		break;
	};
	return num;
}


/*									//
//		��������Wrap�֕ύX			//
//									*/
_eWrapMode Graphics::NumToWrapMode(DWORD num) {
	_eWrapMode mode;
	switch (num) {
	case D3DTADDRESS_WRAP:
		mode = _eWrapMode::WRAPMODE_WRAP;
		break;

	case D3DTADDRESS_CLAMP:
		mode = _eWrapMode::WRAPMODE_CLAMP;
		break;
	};
	return mode;
}
#pragma endregion


#pragma region Cull Mode
/*									//
//			Culling�̐ݒ�			//
//									*/
void Graphics::SetCullMode(_eCullMode mode) {
	HRESULT hr = S_OK;

	hr = m_pDevice->SetRenderState(D3DRS_CULLMODE, mode + 1);

	if (hr != S_OK) {
		Debug::LogError("SetCullMode : �ݒ�Ɏ��s���܂���");
	}
}


/*									//
//			Culling�̎擾			//
//									*/
_eCullMode Graphics::GetCullMode() {
	DWORD mode;
	HRESULT hr = S_OK;

	hr = m_pDevice->GetRenderState(D3DRS_CULLMODE, &mode);

	if (hr != S_OK) {
		Debug::LogError("GetCullMode : �擾�Ɏ��s���܂���");
	}

	return (_eCullMode)(mode - 1);
}
#pragma endregion