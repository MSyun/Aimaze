//////////////////////////////////////////////////////////////
// ゲームコントローラークラス(Inputのゲーム用ラッパー
// 2016. 8.12	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


/*
	memo	:
			列挙ではゲームで使用するボタンを宣言すること
*/


// このクラスを使う場合絶対必要なため
#include	"../framework/Input/Input.h"
#include	"../framework/Save/SaveObjBase.h"


typedef enum {
	BUTTON_SIKAKU = 0,	// □
	BUTTON_BATU,		// ×
	BUTTON_MARU,		// ○
	BUTTON_SANKAKU,		// △

	BUTTON_L1,			// L1
	BUTTON_R1,			// L2
	BUTTON_L2,			// R1
	BUTTON_R2,			// R2

	BUTTON_START,		// START

	BUTTON_UP,			// ↑
	BUTTON_RIGHT,		// →
	BUTTON_DOWN,		// ↓
	BUTTON_LEFT,		// ←

	BUTTON_MAX
} _GameButton;


class	GameController : public	ISaveObjBase {
private:
#pragma region struct

	// 登録キー番号
	struct _button {
		BYTE	KeyNum;	// キーボード
		char	JoyNum;	// パッド
	};

#pragma endregion

#pragma region variable

	_button m_tKeyInfo[BUTTON_MAX];

	// デフォルト設定
	void SetDefaultKey();
	void SetDefaultJoy();
	_button GetKeyInfo(int _key);

	// セーブオブジェクト
	static DATARECORD	m_DataRecord[];

#pragma endregion

public:
#pragma region method

	GameController();
	virtual ~GameController();

	// 初期状態へ
	void Reset();

	// キー番号を取得
	int GetKeyNum(int _button);
	int GetJoyNum(int _button);

	// キー番号を設定
	void SetKeyNum(int _button, int _key);
	void SetJoyNum(int _button, int _key);

	//----- 保存データ
	virtual DATARECORD* GetDataRecord() { return m_DataRecord; }
	virtual int GetClassID();

#pragma endregion
};