// �^�C�g���V�[��
// 2017. 1. 1	�v���O�����쐬
// author		SyunMizuno


#pragma once


#include	"../../GameFramework/framework/Scene/SceneBase.h"
#include	"../../GameFramework/Obj/2D/Sprite.h"
#include	<vector>
using namespace std;


class TitleScene	:	public	SceneBase {
private:
#pragma region variable

	// ���͊m�F
	bool	m_bInput;

	// �I�𒆂̃��[�h
	int		m_nSelectMode;

	Sprite*	m_pPress;
	RectTransform*	m_pSelecter;
	vector<Sprite*>	m_aMode;

#pragma endregion

public:
#pragma region method

	TitleScene();
	virtual ~TitleScene();

	virtual void Update();
	virtual void Draw();
	virtual bool ReStart() { return true; };

#pragma endregion

protected:
#pragma region conceal method

	virtual bool Init();
	virtual void Release();

	bool ModeSelect();
	void ChangeScene();

#pragma endregion
};