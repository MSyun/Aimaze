// グラフィック
// 2017. 2. 23	プログラム作成
// author		SyunMizuno


#include	"Graphics.h"
#include	"../Debug/Debug.h"


/*									//
//			コンストラクタ			//
//									*/
Graphics::Graphics() :
	m_pDevice(NULL)
{
}


/*									//
//			デストラクタ			//
//									*/
Graphics::~Graphics() {
}


/*									//
//			デバイスの取得			//
//									*/
LPDIRECT3DDEVICE9 Graphics::GetDevice() {
	return m_pDevice;
}


/*									//
//			デバイスの設定			//
//									*/
void Graphics::SetDevice(LPDIRECT3DDEVICE9 _dev) {
	m_pDevice = _dev;
}


#pragma region View
/*									//
//			ビュー行列の設定		//
//									*/
void Graphics::SetView(const Matrix& view) {
	m_pDevice->SetTransform(D3DTS_VIEW, &view);
}


/*									//
//			ビュー行列の取得		//
//									*/
Matrix Graphics::GetView() {
	Matrix mat;
	HRESULT hr = S_OK;

	hr = m_pDevice->GetTransform(D3DTS_VIEW, &mat);
	if (FAILED(hr)) {
		Debug::LogError("Viewの取得に失敗しました");
	}
	return mat;
}
#pragma endregion


#pragma region Projection
/*									//
//		プロジェクション行列の設定	//
//									*/
void Graphics::SetProjection(const Matrix& proj) {
	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);
}


/*									//
//		プロジェクション行列の取得	//
//									*/
Matrix Graphics::GetProjection() {
	Matrix mat;
	HRESULT hr = S_OK;
	hr = m_pDevice->GetTransform(D3DTS_PROJECTION, &mat);
	if (FAILED(hr)) {
		Debug::LogError("Projectionの取得に失敗しました");
	}
	return mat;
}
#pragma endregion


#pragma region Alpha Blend
/*									//
//	αブレンドを使用するか設定		//
//									*/
void Graphics::SetAlphaBlendEnable(bool flg) {
	HRESULT hr = S_OK;
	hr = m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, flg);
	if (hr != S_OK) {
		Debug::LogError("SetAlphaBlendEnable : 設定に失敗しました");
	}
}


/*									//
//		αブレンドの設定を取得		//
//									*/
// true. 使用中 : false. 未使用
bool Graphics::GetAlphaBlendEnable() {
	DWORD use;
	HRESULT hr = S_OK;
	hr = m_pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &use);
	if (hr != S_OK) {
		Debug::LogError("GetAlphaBlendEnable : 取得に失敗しました");
	}

	return use != 0 ? true : false;
}
#pragma endregion


#pragma region Z Enable
/*									//
//		Zバッファを使用するか設定	//
//									*/
void Graphics::SetZEnable(bool flg) {
	HRESULT hr = S_OK;
	hr = m_pDevice->SetRenderState(D3DRS_ZENABLE, flg);
	if (hr != S_OK) {
		Debug::LogError("SetZEnable : 設定に失敗しました");
	}
}


/*									//
//		Zバッファの設定を取得		//
//									*/
// true. 使用中 : false. 未使用
bool Graphics::GetZEnable() {
	DWORD use;
	HRESULT hr = S_OK;
	hr = m_pDevice->GetRenderState(D3DRS_ZENABLE, &use);
	if (hr != S_OK) {
		Debug::LogError("GetZEnable : 取得に失敗しました");
	}

	return use != 0 ? true : false;
}
#pragma endregion


#pragma region Wrap Mode
/*									//
//			Wrapの設定				//
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
		Debug::LogError("SetWrapMode : 設定に失敗しました");
	}
}


/*									//
//			Wrapの取得				//
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
		Debug::Log("GetWrapMode : UVが設定されたのでUのみ優先で返します");
		break;
	};

	if (hr != S_OK) {
		Debug::LogError("GetWrapMode : 取得に失敗しました");
	}

	return NumToWrapMode(Mode);
}


/*									//
//		Wrapから数字へ変更			//
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
//		数字からWrapへ変更			//
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
//			Cullingの設定			//
//									*/
void Graphics::SetCullMode(_eCullMode mode) {
	HRESULT hr = S_OK;

	hr = m_pDevice->SetRenderState(D3DRS_CULLMODE, mode + 1);

	if (hr != S_OK) {
		Debug::LogError("SetCullMode : 設定に失敗しました");
	}
}


/*									//
//			Cullingの取得			//
//									*/
_eCullMode Graphics::GetCullMode() {
	DWORD mode;
	HRESULT hr = S_OK;

	hr = m_pDevice->GetRenderState(D3DRS_CULLMODE, &mode);

	if (hr != S_OK) {
		Debug::LogError("GetCullMode : 取得に失敗しました");
	}

	return (_eCullMode)(mode - 1);
}
#pragma endregion