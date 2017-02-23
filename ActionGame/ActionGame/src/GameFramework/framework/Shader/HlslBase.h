//	HLSLベースクラス
// 2016. 5. 8	プログラム作成
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>

class IHlslBase {
protected:
#pragma region variable

	LPD3DXEFFECT		m_pEffect;
	D3DXHANDLE			m_hTechnique;

#pragma endregion

public:
#pragma region method

	IHlslBase();
	virtual ~IHlslBase();

	virtual HRESULT Invalidate();
	virtual HRESULT Restore();
	virtual HRESULT Load() = 0;
	virtual void Begin();
	void BeginPass(int pass);
	void EndPass();
	virtual void End();
	void CommitChanges();

#pragma endregion

protected:
#pragma region conceal method

	// 使用確認
	bool IsOK();

#pragma endregion
};