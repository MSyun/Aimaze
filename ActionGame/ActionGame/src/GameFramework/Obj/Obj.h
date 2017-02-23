//////////////////////////////////////////////////////////////
// �I�u�W�F�N�g�N���X
// 2016. 5.19	�v���O�����쐬
// 2016. 5.21	Input�֐��ǉ�
// 2017. 1. 1	OnActive�֐��ǉ�
// Author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once

#include	"../framework/CPPEvent/CPPEvent.h"
#include	"../framework/Utility/System/SystemUtility.h"
#include	<string>
using namespace std;


/*
	memo	:
			Input�֐��͌�X�}�l�[�W��������Ăяo����������Ȃ��̂�
			�ꉞpublic��
*/



class	IObj {
private:
#pragma region variable

	string	m_Name;			// ���O
	BYTE	m_uTag;			// �I�u�W�F�N�gID
	bool	m_bOperate;		// ����
	bool	m_bIsActive;	// �g�p�t���O
	bool	m_bDontDestroy;	// �V�[���J�ڎ��폜����
	bool	m_bDestroy;		// �폜

	bool	m_bInit;		// ������

	CPPEvent<>*	m_onDestroyEvent;

#pragma endregion

public:
#pragma region method

	IObj() :
		m_Name("GameObject"),
		m_uTag(0),
		m_bOperate(false),
		m_bIsActive(true),
		m_bDontDestroy(false),
		m_bDestroy(false),
		m_bInit(false)
	{
		m_onDestroyEvent = new CPPEvent<>;
	}
	virtual ~IObj() {
		SAFE_DELETE(m_onDestroyEvent);
	};

	virtual void UpdateAll() { if (!m_bInit) { Init(); m_bInit = true; } }
	virtual void LateUpdateAll() = 0;
	virtual void DrawAll() {}
	virtual void LateDrawAll() {}

#pragma region Operate
	void ChangeOperate() {
		m_bOperate ^= true;
		if (m_bOperate)
			OnOperate();
	}
	void SetOperate(bool flg) {
		m_bOperate = flg;
		if (m_bOperate)
			OnOperate();
	}
	bool GetOperate() { return m_bOperate; }
#pragma endregion

#pragma region Name
	void SetName(const string name) { m_Name = name; }
	string GetName() { return m_Name; }
#pragma endregion

#pragma region Tag
	void SetTag(BYTE tag) { m_uTag = tag; }
	BYTE GetTag() { return m_uTag; }
#pragma endregion

#pragma region Active
	virtual void SetActive(bool active) {
		if (!m_bIsActive && active)
			OnActive();
		m_bIsActive = active;
	}
	bool GetActive() { return m_bIsActive; }
#pragma endregion

#pragma region DontDestroyOnLoad
	void DontDestroyOnLoad() { m_bDontDestroy = true; }
	bool GetDontDestroy() { return m_bDontDestroy; }
#pragma endregion

#pragma region Destroy
	virtual void Destroy() { m_bDestroy = true; }
	bool GetDestroy() { return m_bDestroy; }
	virtual void OnDestroy() { m_onDestroyEvent->Invoke(); }
	CPPEvent<>* GetOnDestroy() { return m_onDestroyEvent; }
#pragma endregion

#pragma endregion

protected:
#pragma region conceal method

	virtual void Input() {}

	virtual void Init() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void Draw() {}
	virtual void LateDraw() {}

	virtual void OnActive() {}
	virtual void OnOperate() {}

#pragma endregion
};