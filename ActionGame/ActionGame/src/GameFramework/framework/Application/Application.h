// �A�v���P�[�V�����p�x�[�X
// 2016. 2. 7	�v���O�����쐬
// author		SyunMizuno


#pragma once

#include	<Windows.h>


class	Application {
public:
#pragma region method

	Application(int width = 600, int height = 480, bool bwnd = true);
	virtual ~Application();

	// ���s
	int Run(HINSTANCE _hInstance);

	// ���[�J���p�v���V�[�W��
	virtual LRESULT LocalWindowProc(
		HWND	hWnd,
		UINT	msg,
		WPARAM	wParam,
		LPARAM	lParam);

	// �v���V�[�W��
	static LRESULT CALLBACK WndProc(
		HWND	hWnd,
		UINT	uMsg,
		WPARAM	wParam,
		LPARAM	lParam);

	// �A�v���P�[�V�����̏I��
	static void End() { DestroyWindow(m_hWnd); }

#pragma region Getter

	HINSTANCE GetHInstance()			{ return m_hInstance; }
	void GetWndClass(WNDCLASSEX* wc)	{ *wc = m_WndClassEx; }
	//----- Window�֘A
	CREATESTRUCT GetWndCS()				{ return m_cs; }
	static HWND GetHWND()				{ return m_hWnd; }
	static void SetHWND(HWND _hwnd)		{ m_hWnd = _hwnd; }

#pragma endregion

#pragma endregion

#pragma region variable

protected:
	HINSTANCE		m_hInstance;	// �A�v���P�[�V�����n���h��
	WNDCLASSEX		m_WndClassEx;	// �E�C���h�E�N���X�\����
	//----- Window�֘A
	CREATESTRUCT	m_cs;
	static HWND m_hWnd;

#pragma endregion

#pragma region conceal method

private:
	// �E�C���h�E�N���X�̓o�^
	virtual bool RegistWndClass();
	// ���C���E�C���h�E�̍쐬
	virtual bool CreateMainWnd();

protected:
	// �E�C���h�E�N���X�̕ύX
	virtual void PreWndClass();
	// ���C���E�C���h�E�̕ύX
	virtual void PreMainWnd();
	// �A�v���P�[�V�����̏�����
	virtual bool Initialize();
	// ���b�Z�[�W���[�v
	virtual void Loop();
	// �A�v���P�[�V�����I������
	virtual void ReleaseApp();

#pragma endregion
};