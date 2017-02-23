//////////////////////////////////////////////////////////////
// ïœä∑
// 2016. 8.3	ÉvÉçÉOÉâÉÄçÏê¨
// Author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#define	DX


#ifdef DX
#include	<d3dx9.h>

typedef	D3DXMATRIX		Matrix;
typedef	D3DXVECTOR2		Point2;
typedef	D3DXVECTOR3		Point3;
typedef	D3DXVECTOR4		Point4;
typedef	D3DXVECTOR2		Vector2;
typedef	D3DXVECTOR3		Vector3;
typedef	D3DXVECTOR4		Vector4;
typedef D3DXQUATERNION	Quaternion;
typedef	D3DXCOLOR		Color;

#else
#include	"Vector/Vector2/Vector2.h"
#include	"Vector/Vector3/Vector3.h"

#endif