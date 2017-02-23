//////////////////////////////////////////////////////////////
// パーティクル
// 2016. 7. 7	プログラム作成
// 変更者		SyunMizuno
//////////////////////////////////////////////////////////////

#include	"Particle.h"
#include	"../Texture/TexManager.h"
#include	"../Camera/CameraManager.h"
#include	"../Utility/System/SystemUtility.h"
#include	<tchar.h>
#include	"../../../Archives/Archives.h"
#include	"../Graphic/Graphics.h"


#define	frand()					(rand() / (float)RAND_MAX)

#define	FVF_PVERTEX				(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


/*									//
//		パーティクルの初期設定		//
//									*/
void Particle::GetDefaultParam(TParticleParam& pp) {
	pp.fDuration = 10.0f;
	pp.uMaxParticles = 10000;
	pp.coord = eCartesian;
//	pp.synthetic = eAlphaBlend;
	pp.bLooping = true;
	pp.fGravity = 9.8f;
	pp.fStartSpeed = 25.0f;
	pp.fStartSpeed2 = 35.0f;
	pp.fStartSize = 1.0f;
	pp.fStartSize2 = 1.0f;
	pp.fStartRotation = 0.0f;
	pp.fStartRotation2 = pp.fStartRotation;
	pp.cStartColor = D3DCOLOR_ARGB(127, 191, 255, 255);
	pp.cStartColor2 = pp.cStartColor;
	pp.fStartLifetime = 5.0f;
	pp.fStartLifetime2 = pp.fStartLifetime;
	pp.uRate = 600;
	pp.fCornAngle = 20.0f;
	pp.fRadius = 0.0f;
	pp.bColorOverLifetime = true;
	pp.cOverLifetime[0] = D3DCOLOR_ARGB(127, 191, 255, 255);
	pp.cOverLifetime[1] = D3DCOLOR_ARGB(0, 255, 255, 255);
	pp.bSizeOverLifetime = true;
	pp.fOverLifetime[0] = 1.0f;
	pp.fOverLifetime[1] = 10.0f;
	lstrcpy(pp.szTexFName, Archives::Texture("Particle/Default").c_str());
}


/*									//
//			コンストラクタ			//
//									*/
Particle::Particle() {
	m_pTexture = NULL;
	m_szTexFName[0] = _T('\0');

	GetDefaultParam(m_pp);
//	m_synthetic = m_pp.synthetic;
	m_fTime = 0.0f;
	m_fRate = 0.0f;
	m_uParticles = m_pp.uMaxParticles;

	m_pGrain = NULL;
	m_pUse = NULL;
	m_uUse = 0;
	m_pUnused = NULL;

	m_pVertex = NULL;
	m_pIndex = NULL;

	SetName("Particle");
}


/*									//
//			デストラクタ			//
//									*/
Particle::~Particle() {
	Release();
}


/*									//
//				解放処理			//
//									*/
void Particle::Release() {
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
	SAFE_DELETE_ARRAY(m_pGrain);
	m_pUse = m_pUnused = NULL;
}


/*									//
//			テクスチャの設定		//
//									*/
void Particle::UpdateTexture() {
	m_pTexture = GetTexManager()->Get(m_pp.szTexFName);
}


/*									//
//			パラメータの更新		//
//									*/
void Particle::SetParam(TParticleParam& pp) {
	m_pp = pp;

	//----- 初期数値が逆の場合交換
	// 速度
	if (m_pp.fStartSpeed > m_pp.fStartSpeed2) {
		float fSpeed = m_pp.fStartSpeed;
		m_pp.fStartSpeed = m_pp.fStartSpeed2;
		m_pp.fStartSpeed2 = fSpeed;
	}

	// サイズ
	if (m_pp.fStartSize > m_pp.fStartSize2) {
		float fSize = m_pp.fStartSize;
		m_pp.fStartSize = m_pp.fStartSize2;
		m_pp.fStartSize2 = fSize;
	}

	// 角度
	if (m_pp.fStartRotation > m_pp.fStartRotation2) {
		float fRotation = m_pp.fStartRotation;
		m_pp.fStartRotation = m_pp.fStartRotation2;
		m_pp.fStartRotation2 = fRotation;
	}

	// 寿命
	if (m_pp.fStartLifetime > m_pp.fStartLifetime2) {
		float fLifetime = m_pp.fStartLifetime;
		m_pp.fStartLifetime = m_pp.fStartLifetime2;
		m_pp.fStartLifetime2 = fLifetime;
	}

	// 色
	UINT uA = HIBYTE(HIWORD(m_pp.cStartColor));
	UINT uR = LOBYTE(HIWORD(m_pp.cStartColor));
	UINT uG = HIBYTE(LOWORD(m_pp.cStartColor));
	UINT uB = LOBYTE(m_pp.cStartColor);
	UINT uA2 = HIBYTE(HIWORD(m_pp.cStartColor2));
	UINT uR2 = LOBYTE(HIWORD(m_pp.cStartColor2));
	UINT uG2 = HIBYTE(LOWORD(m_pp.cStartColor2));
	UINT uB2 = LOBYTE(m_pp.cStartColor2);
	bool bChange = false;
	if (uA > uA2) {UINT u = uA; uA = uA2; uA2 = u;bChange = true;}
	if (uR > uR2) {UINT u = uR; uR = uR2; uR2 = u;bChange = true;}
	if (uG > uG2) {UINT u = uG; uG = uG2; uG2 = u;bChange = true;}
	if (uB > uB2) {UINT u = uB; uB = uB2; uB2 = u;bChange = true;}
	if (bChange) {
		m_pp.cStartColor = D3DCOLOR_ARGB(uA, uR, uG, uB);
		m_pp.cStartColor2 = D3DCOLOR_ARGB(uA2, uR2, uG2, uB2);
	}
	m_fTime = 0.0f;		// 生成時間
	m_fRate = 0.0f;		// 生成数ワーク
	m_uParticles = m_pp.uMaxParticles;	// 最大数のセット
//	m_synthetic = m_pp.synthetic;

	// テクスチャの更新
	UpdateTexture();
}


/*									//
//			設定のやり直し			//
//									*/
void Particle::PostInit() {
	// とりあえず解放
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
	SAFE_DELETE_ARRAY(m_pGrain);
	m_pUse = m_pUnused = NULL;

	// テクスチャの更新
	UpdateTexture();

	// リスト繋ぎなおし
	m_pGrain = new TGrain[m_pp.uMaxParticles];
	m_pGrain[0].m_pBack = NULL;
	m_pGrain[0].m_pNext = &m_pGrain[1];
	for (UINT i = 1; i < m_pp.uMaxParticles - 1; ++i) {
		m_pGrain[i].m_pNext = &m_pGrain[i + 1];
		m_pGrain[i].m_pBack = &m_pGrain[i - 1];
	}
	m_pGrain[m_pp.uMaxParticles - 1].m_pBack = &m_pGrain[m_pp.uMaxParticles - 2];
	m_pGrain[m_pp.uMaxParticles - 1].m_pNext = NULL;
	m_pUse = NULL;
	m_uUse = 0;
	m_pUnused = &m_pGrain[0];

	m_pVertex = new TVertex[m_pp.uMaxParticles * 4];
	m_pIndex = new WORD[m_pp.uMaxParticles * 6];
}


/*									//
//				要素の削除			//
//									*/
void Particle::Delete(TGrain* p) {
	// リスト再設定
	if (p->m_pBack) {
		p->m_pBack->m_pNext = p->m_pNext;
	} else {
		m_pUse = p->m_pNext;
	}
	if (p->m_pNext) {
		p->m_pNext->m_pBack = p->m_pBack;
	}

	p->m_pBack = NULL;
	p->m_pNext = m_pUnused;
	if (m_pUnused) {
		m_pUnused->m_pBack = p;
	}
	m_pUnused = p;

	--m_uUse;
}


/*									//
//				要素の作成			//
//									*/
Particle::TGrain* Particle::New() {
	if( m_pUnused ) {
		//----- 繋ぎ直し
		TGrain* p = m_pUnused;
		m_pUnused = m_pUnused->m_pNext;
		if (m_pUnused) {
			m_pUnused->m_pBack = NULL;
		}
		p->m_pNext = m_pUse;
		if (m_pUse) {
			m_pUse->m_pBack = p;
		}
		m_pUse = p;

		// 初期値設定
		p->m_pos = m_pTransform->GetPos();
		p->m_pos.y += m_pp.fStartSize2 * 0.5f;
		if (m_pp.coord == eSpherical) {
			p->m_accel = Vector3(0.0f, 0.0f, 0.0f);
		} else {
			p->m_accel = Vector3(0.0f, -m_pp.fGravity, 0.0f);
		}
		float a = 0.0f;									// 開始角
		float b = D3DXToRadian(m_pp.fCornAngle / 2.0f);	// 終了角
		float theta = acosf(cosf(a) - frand() * (cosf(a) - cosf(b)));
		float r = sinf(theta);
		float eta = frand() * 2.0f * D3DX_PI;
		p->m_velocity.x = r * cosf(eta);
		p->m_velocity.y = cosf(theta);
		p->m_velocity.z = r * sinf(eta);
		D3DXVec3Normalize(&p->m_velocity, &p->m_velocity);
		p->m_pos += p->m_velocity * m_pp.fRadius;
		p->m_startPos = p->m_pos;
		switch (m_pp.coord) {
		case eSpherical:
			p->m_velocity.x = r;
			p->m_velocity.y = D3DXToDegree(eta);
			p->m_velocity.z = D3DXToDegree(theta);
			break;
		case eCylindrical:
			p->m_velocity.x = r;
			p->m_velocity.y = cosf(theta);
			p->m_velocity.z = D3DXToDegree(theta);
			break;
		case eCartesian:
		default:
			break;
		}
		p->m_velocity *= m_pp.fStartSpeed + (m_pp.fStartSpeed2 - m_pp.fStartSpeed) * frand();
		p->m_size = m_pp.fStartSize + (m_pp.fStartSize2 - m_pp.fStartSize) * frand();
		p->m_startSize = p->m_size;
		p->m_angle = m_pp.fStartRotation + (m_pp.fStartRotation2 - m_pp.fStartRotation) * frand();

		// 色
		UINT uA = HIBYTE(HIWORD(m_pp.cStartColor));
		UINT uR = LOBYTE(HIWORD(m_pp.cStartColor));
		UINT uG = HIBYTE(LOWORD(m_pp.cStartColor));
		UINT uB = LOBYTE(m_pp.cStartColor);
		UINT uA2 = HIBYTE(HIWORD(m_pp.cStartColor2));
		UINT uR2 = LOBYTE(HIWORD(m_pp.cStartColor2));
		UINT uG2 = HIBYTE(LOWORD(m_pp.cStartColor2));
		UINT uB2 = LOBYTE(m_pp.cStartColor2);
		uA += (uA2 - uA) * rand() / RAND_MAX;
		uR += (uR2 - uR) * rand() / RAND_MAX;
		uG += (uG2 - uG) * rand() / RAND_MAX;
		uB += (uB2 - uB) * rand() / RAND_MAX;
		p->m_color = D3DCOLOR_ARGB(uA, uR, uG, uB);
		p->m_startColor = p->m_color;
		p->m_lifetime = m_pp.fStartLifetime + (m_pp.fStartLifetime2 - m_pp.fStartLifetime) * frand();
		p->m_startLifetime = p->m_lifetime;

		++m_uUse;
		return p;
	}
	return NULL;
}


/*									//
//				更新				//
//									*/
void Particle::Update() {
	Obj3D::Update();

	// 経過時間
	float fDeltaTime = Time::GetDeltaTime();

	// 生成
	if (m_uParticles) {
		m_fRate += m_pp.uRate * fDeltaTime;
		while (m_fRate >= 1.0f) {
			m_fRate -= 1.0f;
			--m_uParticles;
			New();
		}
	}

	Matrix world;
	for (int i = 0; i < 3; ++i) {
		world.m[0][i] = m_pTransform->GetRight()[i];
		world.m[1][i] = m_pTransform->GetUp()[i];
		world.m[2][i] = m_pTransform->GetForward()[i];
		world.m[3][i] = 0.0f;
	}

	// 転置行列を作成
	Matrix& mView = GetCameraManager()->Get("Main")->GetView();
//	mView = mView * world;
	Matrix mBB(	mView._11, mView._21, mView._31, 0.0f,
				mView._12, mView._22, mView._32, 0.0f,
				mView._13, mView._23, mView._33, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f );

	Matrix mRZ, mtx;
	TGrain* pNext = NULL;
	TVertex* pV = m_pVertex;
	WORD* pI = m_pIndex;
	WORD wIndex = 0;

	// 全要素の更新
	for (TGrain* p = m_pUse; p; p = pNext) {
		pNext = p->m_pNext;
		// 更新
		p->m_lifetime -= fDeltaTime;
		if (p->m_lifetime <= 0.0f) {
			Delete(p);
			continue;
		}
		p->m_velocity += p->m_accel * fDeltaTime;
		p->m_pos += p->m_velocity * fDeltaTime;
		if (p->m_pos.y <= -p->m_size * 2.0f) {
			Delete(p);
			continue;
		}
		float delta;
		if (m_pp.bColorOverLifetime || m_pp.bSizeOverLifetime) {
			delta = p->m_lifetime / p->m_startLifetime;
		}
		if (m_pp.bColorOverLifetime) {
			int nA = HIBYTE(HIWORD(m_pp.cOverLifetime[0]));
			int nR = LOBYTE(HIWORD(m_pp.cOverLifetime[0]));
			int nG = HIBYTE(LOWORD(m_pp.cOverLifetime[0]));
			int nB = LOBYTE(m_pp.cOverLifetime[0]);
			int nA2 = HIBYTE(HIWORD(m_pp.cOverLifetime[1]));
			int nR2 = LOBYTE(HIWORD(m_pp.cOverLifetime[1]));
			int nG2 = HIBYTE(LOWORD(m_pp.cOverLifetime[1]));
			int nB2 = LOBYTE(m_pp.cOverLifetime[1]);
			nA = (int)(nA2 + (nA - nA2) * delta);
			nR = (int)(nR2 + (nR - nR2) * delta);
			nG = (int)(nG2 + (nG - nG2) * delta);
			nB = (int)(nB2 + (nB - nB2) * delta);
			p->m_color = D3DCOLOR_ARGB(nA, nR, nG, nB);
		}
		if (m_pp.bSizeOverLifetime) {
			p->m_size = m_pp.fOverLifetime[1] + (m_pp.fOverLifetime[0] - m_pp.fOverLifetime[1]) * delta;
		}

		// 頂点構築
		D3DXMatrixRotationZ(&mRZ, D3DXToRadian(p->m_angle));
		//Vector3 vX, vY, vZ, position;
		//vX = m_pTransform->GetRight();
		//vY = m_pTransform->GetUp();
		//vZ = m_pTransform->GetForward();
		//position = m_pTransform->GetPos();
		//vX = Vector3(vX.x * p->m_pos.x, vY.x * p->m_pos.y, vZ.x * p->m_pos.z);
		//vY = Vector3(vX.y * p->m_pos.x, vY.y * p->m_pos.y, vZ.y * p->m_pos.z);
		//vZ = Vector3(vX.z * p->m_pos.x, vY.z * p->m_pos.y, vZ.z * p->m_pos.z);
		//Vector3 vec = vX + vY + vZ;

		switch (m_pp.coord) {
		case eCartesian:
		default:
			mBB._41 = p->m_pos.x;
			mBB._42 = p->m_pos.y;
			mBB._43 = p->m_pos.z;
			//vX *= p->m_pos.x;
			//vY *= p->m_pos.y;
			//vZ *= p->m_pos.z;
			//mBB._41 = vec.x;
			//mBB._42 = vec.y;
			//mBB._43 = vec.z;
			break;
		case eSpherical:
			mBB._41 = p->m_pos.x * cosf(D3DXToRadian(p->m_pos.z)) * cosf(D3DXToRadian(p->m_pos.y));
			mBB._42 = p->m_pos.x * cosf(D3DXToRadian(p->m_pos.y));
			mBB._43 = p->m_pos.x * sinf(D3DXToRadian(p->m_pos.z)) * cosf(D3DXToRadian(p->m_pos.y));
			//vX *= (p->m_pos.x * cosf(D3DXToRadian(p->m_pos.z)) * cosf(D3DXToRadian(p->m_pos.y)));
			//vY *= (p->m_pos.x * cosf(D3DXToRadian(p->m_pos.y)));
			//vZ *= (p->m_pos.x * sinf(D3DXToRadian(p->m_pos.z)) * cosf(D3DXToRadian(p->m_pos.y)));
			break;
		case eCylindrical:
			mBB._41 = p->m_pos.x * cosf(D3DXToRadian(p->m_pos.z));
			mBB._42 = p->m_pos.y;
			mBB._43 = p->m_pos.x * sinf(D3DXToRadian(p->m_pos.z));
			//vX *= p->m_pos.x * cosf(D3DXToRadian(p->m_pos.z));
			//vY *= p->m_pos.y;
			//vZ *= p->m_pos.x * sinf(D3DXToRadian(p->m_pos.z));
			break;
		}
		//mBB._41 = vec.x;
		//mBB._42 = vec.y;
		//mBB._43 = vec.z;
		//mBB._41 = vX.x + vX.y + vX.z;
		//mBB._42 = vY.x + vY.y + vY.z;
		//mBB._43 = vZ.x + vZ.y + vZ.z;

		mtx = mRZ * mBB;
		//mtx._41 = position.x + vec.x;
		//mtx._42 = position.y + vec.y;
		//mtx._43 = position.z + vec.z;
		//mtx._41 += position.x;
		//mtx._42 += position.y;
		//mtx._43 += position.z;

		Vector3 pos;
		D3DXVec3TransformCoord(&pos, &Vector3(p->m_size * -0.5f, p->m_size * 0.5f, 0.0f), &mtx);
		pV[0].m_pos = pos;
		D3DXVec3TransformNormal(&pos, &Vector3(0.0f, 0.0f, -1.0f), &mtx);
		pV[0].m_normal = pos;
		pV[0].m_diffuse = p->m_color;
		pV[0].m_u = 0.0f, pV[0].m_v = 0.0f;
		D3DXVec3TransformCoord(&pos, &Vector3(p->m_size * 0.5f, p->m_size * 0.5f, 0.0f), &mtx);
		pV[1].m_pos = pos;
		pV[1].m_normal = pV[0].m_normal;
		pV[1].m_diffuse = p->m_color;
		pV[1].m_u = 1.0f, pV[1].m_v = 0.0f;
		D3DXVec3TransformCoord(&pos, &Vector3(p->m_size * -0.5f, p->m_size * -0.5f, 0.0f), &mtx);
		pV[2].m_pos = pos;
		pV[2].m_normal = pV[0].m_normal;
		pV[2].m_diffuse = p->m_color;
		pV[2].m_u = 0.0f, pV[2].m_v = 1.0f;
		D3DXVec3TransformCoord(&pos, &Vector3(p->m_size * 0.5f, p->m_size * -0.5f, 0.0f), &mtx);
		pV[3].m_pos = pos;
		pV[3].m_normal = pV[0].m_normal;
		pV[3].m_diffuse = p->m_color;
		pV[3].m_u = 1.0f, pV[3].m_v = 1.0f;
		pV += 4;

		// インデックス構築
		pI[0] = wIndex + 0;
		pI[1] = wIndex + 1;
		pI[2] = wIndex + 2;
		pI[3] = wIndex + 2;
		pI[4] = wIndex + 1;
		pI[5] = wIndex + 3;
		pI += 6;
		wIndex += 4;
	}

	if (!m_uParticles)	return;
	// 時間を進める
	m_fTime += fDeltaTime;
	if (m_fTime >= m_pp.fDuration) {
		m_fRate = 0.0f;
		if (m_pp.bLooping) {
			RePlay();
		} else {
			Stop();
		}
	}
}


/*									//
//				描画				//
//									*/
void Particle::LateDraw() {
	if (!m_uUse) {
		return;
	}
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	Matrix mWorld;
	D3DXMatrixIdentity(&mWorld);
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);

	//switch (m_pp.synthetic) {
	//case eSubtractive:		// 減算合成
	//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		// THRU
	//case eAdditive:			// 加算合成
	//case eAlphaBlend:		// 半透明合成
	DWORD light, zwrite, dest;
//		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンド可
//		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//switch (m_pp.synthetic) {
		//case eAdditive:		// 加算合成
		//case eSubtractive:	// 減算合成
	pDevice->GetRenderState(D3DRS_DESTBLEND, &dest);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		//	break;
		//default:
		//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//	break;
		//}
		pDevice->GetRenderState(D3DRS_LIGHTING, &light);					// ライティングしない
		pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &zwrite);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);					// ライティングしない
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//	break;
	//default:
	//	break;
	//}

	pDevice->SetFVF(FVF_PVERTEX);
	const D3DMATERIAL9 material = {
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{0.1f, 0.1f, 0.1f, 1.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
		0.0f,
	};
	pDevice->SetMaterial(&material);
	pDevice->SetTexture(0, m_pTexture);									// テクスチャを登録

	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
		0, m_uUse * 6, m_uUse * 2, m_pIndex, D3DFMT_INDEX16,
		m_pVertex, sizeof(*m_pVertex));

	//switch (m_pp.synthetic) {
	//case eSubtractive:		// 減算合成
	//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		// THRU
	//case eAdditive:			// 加算合成
	//case eAlphaBlend:		// 半透明合成
//		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);					// ライティングする
//		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);			// αブレンド 不可
	pDevice->SetRenderState(D3DRS_DESTBLEND, dest);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, light);
	pDevice->SetRenderState(D3DRS_LIGHTING, zwrite);					// ライティングする
//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);			// αブレンド 不可
	//	break;
	//}
}