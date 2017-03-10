// �����O�N���X
// 2016. 1.24	�v���O�����쐬
// Author	:	SyunMizuno


#include	"Ring.h"
#include	"../../GameFramework/framework/Utility/Math/MathUtility.h"
#include	"../../GameFramework/framework/Graphic/Graphics.h"


typedef struct {
	Point3		vtx;
	D3DCOLOR	diffuse;
	Point2		tex;
} VERTEX_RING;

#define	FVF_RING	( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )



/*									//
//			�R���X�g���N�^			//
//									*/
Ring::Ring() :
	m_fScrollSpeed(1.0f),
	m_fRadius(1.0f),
	m_fWeight(3.0f),
	m_pTex(NULL)
{
	SetName("Ring");
	PreSet();
}


/*									//
//			�f�X�g���N�^			//
//									*/
Ring::~Ring() {

}


/*									//
//				�X�V				//
//									*/
void Ring::Update() {
	if (m_tRing[POLYGON_NUM].Alpha >= 1.0f)
		return;

	float alpha = m_tRing[0].Alpha;

	bool flg = true;
	for (int i = 0; i < POLYGON_NUM + 1; ++i) {
		if (flg) {
			if (m_tRing[i].Alpha < 1.0f)
				m_tRing[i].Alpha += (m_fScrollSpeed * Time::GetDeltaTime());
		}
		float limit = 1.0f / POLYGON_NUM * (i + 1);

		// ���̒��_���X�V�m�F
		if (alpha >= limit) {
			flg = true;
		} else {
			flg = false;
		}
	}
}


/*									//
//				�`��				//
//									*/
void Ring::LateDraw() {
	DWORD Cull, Light, dest;

	//----- ���_
	VERTEX_RING Vertex[(POLYGON_NUM + 1) * 2];
	for (int i = 0, p = 0; i < POLYGON_NUM + 1; ++ i, p += 2) {
		// �F
		Vertex[p].diffuse		=
		Vertex[p + 1].diffuse	= Color(1.0f, 1.0f, 1.0f, m_tRing[i].Alpha);
		// ���_
		Vertex[p].vtx.x			=
		Vertex[p + 1].vtx.x		= m_tRing[i].Pos.x;
		Vertex[p].vtx.y			= m_fWeight / 2.0f;
		Vertex[p + 1].vtx.y		= Vertex[p].vtx.y * -1.0f;
		Vertex[p].vtx.z			=
		Vertex[p + 1].vtx.z		= m_tRing[i].Pos.z;
		// �e�N�X�`��
		Vertex[p].tex			= Point2(m_tRing[i].Tex.x, 0.0f);
		Vertex[p+1].tex			= Point2(m_tRing[i].Tex.x, 1.0f);
	}

	// �C���f�b�N�X
	unsigned short Index[(POLYGON_NUM + 1) * 2];
	for (int i = 0; i < (POLYGON_NUM + 1) * 2; ++ i) {
		Index[i] = i;
	}

	//----- �`��
	// �ݒ�̕ۑ�
	DWORD zwrite, texv;
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	pDevice->GetRenderState(D3DRS_CULLMODE, &Cull);
	pDevice->GetRenderState(D3DRS_LIGHTING, &Light);
	pDevice->GetRenderState(D3DRS_DESTBLEND, &dest);
	pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &zwrite);
	pDevice->GetSamplerState(0, D3DSAMP_ADDRESSV, &texv);

	// �ݒ�̕ύX
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// �J�����O������
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);

	pDevice->SetTransform(D3DTS_WORLD, &GetTransform()->GetWorld());
	pDevice->SetFVF(FVF_RING);
	pDevice->SetTexture(0, m_pTex);
	pDevice->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		0,
		(POLYGON_NUM + 1) * 2,	// ���_��
		POLYGON_NUM * 2,			// �ʐ�
		Index,					// �C���f�b�N�X
		D3DFMT_INDEX16,
		Vertex,					// ���_
		sizeof(VERTEX_RING));

	// �ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, Cull);
	pDevice->SetRenderState(D3DRS_LIGHTING, Light);
	pDevice->SetRenderState(D3DRS_DESTBLEND, dest);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, zwrite);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, texv);
}


/*									//
//				���a�ݒ�			//
//									*/
void Ring::SetRadius(float r) {
	m_fRadius = r;
	PreSet();
}


/*									//
//				�Đݒ�				//
//									*/
void Ring::PreSet() {
	float deg = 360.0f / POLYGON_NUM;
	float buf = 1.0f / POLYGON_NUM;
	for (int i = 0; i < POLYGON_NUM; i ++) {
		m_tRing[i].Pos.x = m_fRadius * sinf(D3DXToRadian(i*deg));
		m_tRing[i].Pos.y = 0.0f;
		m_tRing[i].Pos.z = m_fRadius * cosf(D3DXToRadian(i*deg));
		m_tRing[i].Tex = Point2(buf * (POLYGON_NUM - i), 0.0f);
		m_tRing[i].Alpha = 0.0f;
	}
	m_tRing[POLYGON_NUM].Pos.x = m_fRadius * sinf(D3DXToRadian(0.0f));
	m_tRing[POLYGON_NUM].Pos.y = 0.0f;
	m_tRing[POLYGON_NUM].Pos.z = m_fRadius * cosf(D3DXToRadian(0.0f));
	m_tRing[POLYGON_NUM].Tex = Point2(0.0f, 0.0f);
	m_tRing[POLYGON_NUM].Alpha = 0.0f;
}


/*									//
//		�X�N���[�����x�ݒ�			//
//									*/
void Ring::SetScrollSpeed(float v) {
	if (v <= 0.0f)	return;
	m_fScrollSpeed = v;
}


/*									//
//				���Z�b�g			//
//									*/
void Ring::Reset() {
	for (int i = 0; i < POLYGON_NUM + 1; i ++) {
		m_tRing[i].Alpha = 0.0f;
	}
}