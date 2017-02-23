// スクリーン
// 2017. 2.19	プログラム作成
// author		SyunMizuno


#pragma once


class Screen {
private:
	static int	m_nWidth;
	static int	m_nHeight;
	static bool	m_bFullScreen;

public:
	Screen() {};
	virtual ~Screen() {};

	// 横幅取得
	static int GetWidth();
	// 縦幅取得
	static int GetHeight();
	// サイズ設定
	static void SetSize(int width, int height);
	// フルスクリーン確認
	static bool GetFullScreen();
	// フルスクリーン設定
	static void SetFullScreen(bool fullScreen);
};