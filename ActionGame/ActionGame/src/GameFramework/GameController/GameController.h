//////////////////////////////////////////////////////////////
// �Q�[���R���g���[���[�N���X(Input�̃Q�[���p���b�p�[
// 2016. 8.12	�v���O�����쐬
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


/*
	memo	:
			�񋓂ł̓Q�[���Ŏg�p����{�^����錾���邱��
*/


// ���̃N���X���g���ꍇ��ΕK�v�Ȃ���
#include	"../framework/Input/Input.h"
#include	"../framework/Save/SaveObjBase.h"


typedef enum {
	BUTTON_SIKAKU = 0,	// ��
	BUTTON_BATU,		// �~
	BUTTON_MARU,		// ��
	BUTTON_SANKAKU,		// ��

	BUTTON_L1,			// L1
	BUTTON_R1,			// L2
	BUTTON_L2,			// R1
	BUTTON_R2,			// R2

	BUTTON_START,		// START

	BUTTON_UP,			// ��
	BUTTON_RIGHT,		// ��
	BUTTON_DOWN,		// ��
	BUTTON_LEFT,		// ��

	BUTTON_MAX
} _GameButton;


class	GameController : public	ISaveObjBase {
private:
#pragma region struct

	// �o�^�L�[�ԍ�
	struct _button {
		BYTE	KeyNum;	// �L�[�{�[�h
		char	JoyNum;	// �p�b�h
	};

#pragma endregion

#pragma region variable

	_button m_tKeyInfo[BUTTON_MAX];

	// �f�t�H���g�ݒ�
	void SetDefaultKey();
	void SetDefaultJoy();
	_button GetKeyInfo(int _key);

	// �Z�[�u�I�u�W�F�N�g
	static DATARECORD	m_DataRecord[];

#pragma endregion

public:
#pragma region method

	GameController();
	virtual ~GameController();

	// ������Ԃ�
	void Reset();

	// �L�[�ԍ����擾
	int GetKeyNum(int _button);
	int GetJoyNum(int _button);

	// �L�[�ԍ���ݒ�
	void SetKeyNum(int _button, int _key);
	void SetJoyNum(int _button, int _key);

	//----- �ۑ��f�[�^
	virtual DATARECORD* GetDataRecord() { return m_DataRecord; }
	virtual int GetClassID();

#pragma endregion
};