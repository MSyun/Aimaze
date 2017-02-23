// –îˆó
// 2017. 2. 10	ƒvƒƒOƒ‰ƒ€ì¬
// Author		SyunMizuno


#pragma once


#include	"../GameFramework/Obj/3D/Obj3D.h"



class Allow	:	public	Obj3D {
public:
	Allow();
	virtual ~Allow();

	// “–‚½‚Á‚Ä‚¢‚éŠÔ
	virtual void OnCollisionTrigger(Obj3D* obj);

protected:
	virtual void Init();
	virtual void Update();
};