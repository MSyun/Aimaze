//	HLSL�x�[�X�N���X
// 2016. 5. 8	�v���O�����쐬
// author		SyunMizuno

#include	"HlslBase.h"
#include	<tchar.h>

/*
		memo :
				�֐�����m_pEffect�̃C���X�^���X�m�F�����Ă���̂�
				�p�t�H�[�}���X�������ł��ق����ꍇ��邱��
*/


/*									//
//			�R���X�g���N�^			//
//									*/
IHlslBase::IHlslBase() :
	m_pEffect(NULL),
	m_hTechnique(NULL)
{}


/*									//
//			�f�X�g���N�^			//
//									*/
IHlslBase::~IHlslBase() {
	if (m_pEffect) {
		m_pEffect->Release();
		m_pEffect = NULL;
	}
}


/*									//
//			�f�o�C�X���X�g			//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT IHlslBase::Invalidate() {
	HRESULT hr = S_OK;

	if (m_pEffect)
		hr = m_pEffect->OnLostDevice();

	return hr;
}


/*									//
//			�f�o�C�X���Z�b�g		//
//									*/
// <return> S_OK. ���� : E_FAIL. ���s
HRESULT IHlslBase::Restore() {
	HRESULT hr = S_OK;

	if (m_pEffect)
		hr = m_pEffect->OnResetDevice();

	return hr;
}


/*									//
//		�V�F�[�_�̗��p�ۊm�F		//
//									*/
// <return> true. �\ : false. �s��
bool IHlslBase::IsOK() {
	if (m_pEffect)
		return true;

	return false;
}


/*									//
//		�V�F�[�_�[�����J�n			//
//									*/
void IHlslBase::Begin() {
	if (m_pEffect) {
		m_pEffect->Begin(NULL, 0);
	}
}


/*									//
//			�p�X���J�n				//
//									*/
void IHlslBase::BeginPass(int pass) {
	if (m_pEffect) {
		m_pEffect->BeginPass(pass);
	}
}


/*									//
//			�p�X���I��				//
//									*/
void IHlslBase::EndPass() {
	if (m_pEffect) {
		m_pEffect->EndPass();
	}
}


/*									//
//		�V�F�[�_�[�����I��			//
//									*/
void IHlslBase::End() {
	if (m_pEffect) {
		m_pEffect->End();
	}
}


/*									//
//		�X�e�[�g�ύX�̒ʒm			//
//									*/
void IHlslBase::CommitChanges() {
	if (m_pEffect) {
		m_pEffect->CommitChanges();
	}
}