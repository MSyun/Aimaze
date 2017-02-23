//	HLSLベースクラス
// 2016. 5. 8	プログラム作成
// author		SyunMizuno

#include	"HlslBase.h"
#include	<tchar.h>

/*
		memo :
				関数内でm_pEffectのインスタンス確認をしているので
				パフォーマンスが少しでもほしい場合削ること
*/


/*									//
//			コンストラクタ			//
//									*/
IHlslBase::IHlslBase() :
	m_pEffect(NULL),
	m_hTechnique(NULL)
{}


/*									//
//			デストラクタ			//
//									*/
IHlslBase::~IHlslBase() {
	if (m_pEffect) {
		m_pEffect->Release();
		m_pEffect = NULL;
	}
}


/*									//
//			デバイスロスト			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT IHlslBase::Invalidate() {
	HRESULT hr = S_OK;

	if (m_pEffect)
		hr = m_pEffect->OnLostDevice();

	return hr;
}


/*									//
//			デバイスリセット		//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT IHlslBase::Restore() {
	HRESULT hr = S_OK;

	if (m_pEffect)
		hr = m_pEffect->OnResetDevice();

	return hr;
}


/*									//
//		シェーダの利用可否確認		//
//									*/
// <return> true. 可能 : false. 不可
bool IHlslBase::IsOK() {
	if (m_pEffect)
		return true;

	return false;
}


/*									//
//		シェーダー処理開始			//
//									*/
void IHlslBase::Begin() {
	if (m_pEffect) {
		m_pEffect->Begin(NULL, 0);
	}
}


/*									//
//			パスを開始				//
//									*/
void IHlslBase::BeginPass(int pass) {
	if (m_pEffect) {
		m_pEffect->BeginPass(pass);
	}
}


/*									//
//			パスを終了				//
//									*/
void IHlslBase::EndPass() {
	if (m_pEffect) {
		m_pEffect->EndPass();
	}
}


/*									//
//		シェーダー処理終了			//
//									*/
void IHlslBase::End() {
	if (m_pEffect) {
		m_pEffect->End();
	}
}


/*									//
//		ステート変更の通知			//
//									*/
void IHlslBase::CommitChanges() {
	if (m_pEffect) {
		m_pEffect->CommitChanges();
	}
}