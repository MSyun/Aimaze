// スクリーンショットクラス
// 2015.11.14	プロジェクト作成
// author		SyunMizuno

#pragma once

#include	<d3dx9.h>
#include	<string>
using namespace std;

class ScreenShot {
protected:
#pragma region variable

	IDirect3DDevice9*	m_pDevice;
	HWND				m_hWnd;

#pragma endregion

public:
#pragma region method

	ScreenShot(IDirect3DDevice9* pDevice, HWND hWnd);
	virtual ~ScreenShot();
	// 撮影
	void Snap(const string filename, bool window);

#pragma endregion

protected:
#pragma region conceal method

	virtual void CreateSurface(IDirect3DSurface9* pSurface);
	virtual RECT CreateRect(bool window);

#pragma endregion
};