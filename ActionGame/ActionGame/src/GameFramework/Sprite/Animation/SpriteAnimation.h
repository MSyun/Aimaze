// author		SyunMizuno
// 2016. 2. 4	プログラム作成

#pragma once


#include	"../../Obj/2D/Sprite.h"

class	SpriteAnimation : public	Sprite {
protected:
#pragma region variable

	float	m_fCounter;	// アニメーション間隔
	int		m_nPattern;	// 参照中画像
	int		m_nDivideX;
	int		m_nDivideY;

	float	m_fAnimationSpeed;

	virtual void Animation();
	void SetTex();

#pragma endregion

public:
#pragma region method

	SpriteAnimation();
	virtual ~SpriteAnimation();
	virtual HRESULT Init(
		IDirect3DTexture9* _pTex,
		Vector2	_size,
		int		_divX,
		int		_divY,
		float	_count);

	void SetAnimationSpeed(float speed);

#pragma endregion

protected:
#pragma region conceal method

	virtual void Update();

#pragma endregion
};