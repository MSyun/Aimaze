// グラフィック
// 2017. 2. 23	プログラム作成
// author		SyunMizuno


#include	"Graphics.h"
#include	"../Debug/Debug.h"



Graphics::Graphics() :
	m_pDevice(NULL)
{
}

Graphics::~Graphics()
{
}


LPDIRECT3DDEVICE9 Graphics::GetDevice() {
	return m_pDevice;
}


void Graphics::SetDevice(LPDIRECT3DDEVICE9 _dev) {
	m_pDevice = _dev;
}


void Graphics::SetView(Matrix& view) {
	m_pDevice->SetTransform(D3DTS_VIEW, &view);
}


Matrix Graphics::GetView() {
	Matrix mat;
	HRESULT hr = S_OK;

	hr = m_pDevice->GetTransform(D3DTS_VIEW, &mat);
	if (FAILED(hr)) {
		Debug::LogError("Viewの取得に失敗しました");
	}
	return mat;
}


void Graphics::SetProjection(Matrix& proj) {
	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);
}


Matrix Graphics::GetProjection() {
	Matrix mat;
	HRESULT hr = S_OK;
	hr = m_pDevice->GetTransform(D3DTS_PROJECTION, &mat);
	if (FAILED(hr)) {
		Debug::LogError("Projectionの取得に失敗しました");
	}
	return mat;
}