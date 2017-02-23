// スクリーン
// 2017. 2.19	プログラム作成
// author		SyunMizuno


#include	"Screen.h"


int		Screen::m_nWidth = -1;
int		Screen::m_nHeight = -1;
bool	Screen::m_bFullScreen = false;



/*									//
//				横幅取得			//
//									*/
int Screen::GetWidth() {
	return m_nWidth;
}


/*									//
//				縦幅取得			//
//									*/
int Screen::GetHeight() {
	return m_nHeight;
}


/*									//
//				サイズ設定			//
//									*/
void Screen::SetSize(int width, int height) {
	m_nWidth = width;
	m_nHeight = height;
}


/*									//
//			フルスクリーン確認		//
//									*/
bool Screen::GetFullScreen() {
	return m_bFullScreen;
}


/*									//
//			フルスクリーン設定		//
//									*/
void Screen::SetFullScreen(bool fullScreen) {
	m_bFullScreen = fullScreen;
}