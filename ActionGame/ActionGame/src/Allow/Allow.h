// 矢印
// 2017. 2. 10	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../GameFramework/Obj/3D/Obj3D.h"



class Allow	:	public	Obj3D {
public:
	Allow();
	virtual ~Allow();

	// 当たっている間
	virtual void OnCollisionTrigger(Obj3D* obj);

protected:
	// 初期化
	virtual void Init();
	// 更新
	virtual void Update();
};