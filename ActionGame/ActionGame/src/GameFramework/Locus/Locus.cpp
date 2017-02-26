//////////////////////////////////////////////////////////////
// �O�ՃN���X
// 2016. 7.26	�v���O�����쐬
// Author		SyunMizuno
//////////////////////////////////////////////////////////////



#include	"Locus.h"

#include	"../framework/Time/Time.h"
#include	"../framework/Utility/System/SystemUtility.h"
#include	"../framework/Utility/Math/MathUtility.h"
#include	"../framework/Graphic/Graphics.h"


typedef struct {
	Point3	vtx;
	D3DCOLOR	diffuse;
	Vector2	tex;
} VERTEX_LOCUS;

#define	FVF_LOCUS	( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )


/*									//
//			�R���X�g���N�^			//
//									*/
Locus::Locus() :
	m_nNew(0),
	m_pTex(NULL),
	m_nVertexNum(0),
	m_bUse(false),
	m_fUp(1.0f),
	m_fDown(1.0f),
	m_bRevision(true),
	m_fGhostSpeed(1.0f)
{
	for (int i = 0; i < SAVE_NUM; i ++) {
		m_vStart[i].Pos = Vector3(0.0f, 0.0f, 0.0f);
		m_vEnd[i].Pos = Vector3(0.0f, 0.0f, 0.0f);
	}

	// ���_��
	m_nVertexNum = (SAVE_NUM - 1) * DIVIDE_NUM + SAVE_NUM;

	// �\����
	m_pStartVertex = new TLocus[m_nVertexNum];
	m_pEndVertex = new TLocus[m_nVertexNum];
	for (int i = 0; i < m_nVertexNum; i ++) {
		m_pStartVertex[i].Pos	=
		m_pEndVertex[i].Pos		= Vector3(0.0f, 0.0f, 0.0f);
		m_pStartVertex[i].Tex	= Vector2(1.0f, 1.0f);
		m_pEndVertex[i].Tex		= Vector2(0.0f, 1.0f);
	}
}


/*									//
//			�f�X�g���N�^			//
//									*/
Locus::~Locus() {
	SAFE_DELETE_ARRAY(m_pStartVertex);
	SAFE_DELETE_ARRAY(m_pEndVertex);
}


/*									//
//				�X�V				//
//									*/
void Locus::Update() {
	//----- �ϐ��錾
	int p1, p2, p3, p4;

	//----- ������
	float delta = Time::GetDeltaTime();
	int NewNum = m_nNew;

	//----- ���_�P�ʂ̐ݒ�
	for( int i = 0; i < SAVE_NUM-1; i ++ ) {
		// �z��ԍ��̐ݒ�
		if( i == 0 )			p1 = NewNum;
		else if( NewNum == 0 )	p1 = SAVE_NUM-1;
		else					p1 = NewNum-1;
		p2 = NewNum;
		p3 = (NewNum+1) % SAVE_NUM;
		if( i == SAVE_NUM-2 )	p4 = p3;
		else					p4 = (NewNum+2) % SAVE_NUM;

		// �␳
		for( int n = 0; n < DIVIDE_NUM + 1; n ++ ) {
			// �z��ԍ�
			int num = i * (DIVIDE_NUM + 1) + n;

			// �⊮�m�F
			if (m_bRevision) {
				// �����ꏊ�i0.0f �` 1.0f)
				float s = 1.0f / (DIVIDE_NUM + 1) * n;
				// ���W�⊮
				m_pStartVertex[num].Pos	= CatmullRom(m_vStart[p1].Pos, m_vStart[p2].Pos, m_vStart[p3].Pos, m_vStart[p4].Pos, s);
				m_pEndVertex[num].Pos	= CatmullRom(m_vEnd[p1].Pos, m_vEnd[p2].Pos, m_vEnd[p3].Pos, m_vEnd[p4].Pos, s);
				// �e�N�X�`�����W�⊮
				m_pStartVertex[num].Tex	= m_vStart[p2].Tex + (m_vStart[p3].Tex - m_vStart[p2].Tex) * s;
				m_pEndVertex[num].Tex	= m_vEnd[p2].Tex + (m_vEnd[p3].Tex - m_vEnd[p2].Tex) * s;
			} else {
				m_pStartVertex[num] = m_vStart[p2];
				m_pEndVertex[num] = m_vEnd[p2];
			}
		}

		// ���̔ԍ���
		NewNum = (NewNum+1) % SAVE_NUM;
	}

	// �Ō�̒��_
	m_pStartVertex[m_nVertexNum-1].Pos	= m_vStart[NewNum].Pos;
	m_pEndVertex[m_nVertexNum-1].Pos	= m_vEnd[NewNum].Pos;
	m_pStartVertex[m_nVertexNum-1].Tex	= m_vStart[NewNum].Tex;
	m_pEndVertex[m_nVertexNum-1].Tex	= m_vEnd[NewNum].Tex;

	// �e�N�X�`�����W�X�V
	for (int i = 0; i < SAVE_NUM; i ++) {
		if (m_vStart[i].Tex.y > 0.0f)	m_vStart[i].Tex.y -= (1.0f / m_fGhostSpeed * delta);
		if (m_vEnd[i].Tex.y > 0.0f)		m_vEnd[i].Tex.y -= (1.0f / m_fGhostSpeed * delta);
	}
}


/*									//
//				�`��				//
//									*/
void Locus::Draw() {
	//----- �ϐ��錾
	DWORD Cull, Light;
	Matrix mat;
	D3DXMatrixIdentity(&mat);
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();

	//----- ���_
	VERTEX_LOCUS Vertex[ ((SAVE_NUM-1) * DIVIDE_NUM + SAVE_NUM) * 2 ];
	// ���
	for( int i = 0, p = 0; i < m_nVertexNum; i ++, p += 2 ) {
		// �F
		Vertex[p].diffuse	=
		Vertex[p+1].diffuse	= Color(1.0f, 1.0f, 1.0f, 1.0f);
		// ���_
		Vertex[p].vtx		= m_pStartVertex[i].Pos;
		Vertex[p+1].vtx		= m_pEndVertex[i].Pos;
		// �e�N�X�`��
		Vertex[p].tex		= m_pStartVertex[i].Tex;
		Vertex[p+1].tex		= m_pEndVertex[i].Tex;
	}

	// �C���f�b�N�X
	unsigned short Index[((SAVE_NUM-1) * DIVIDE_NUM + SAVE_NUM)*2];
	for (int i = 0; i < m_nVertexNum * 2; i ++) {
		Index[i] = i;
	}

	//----- �`��
	// �ݒ�̕ۑ�
	DWORD zwrite, alpha, src, dest, texv;
	pDevice->GetRenderState(D3DRS_CULLMODE, &Cull);
	pDevice->GetRenderState(D3DRS_LIGHTING, &Light);
	pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &zwrite);
	pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &alpha);
	pDevice->GetRenderState(D3DRS_SRCBLEND, &src);
	pDevice->GetRenderState(D3DRS_DESTBLEND, &dest);
	pDevice->GetSamplerState(0, D3DSAMP_ADDRESSV, &texv);

	pDevice->SetTransform(D3DTS_WORLD, &mat);
	pDevice->SetFVF(FVF_LOCUS);
	pDevice->SetTexture(0, m_pTex);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// �J�����O������
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	pDevice->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		0,
		m_nVertexNum * 2,		// ���_��
		(m_nVertexNum - 1) * 2,		// �ʐ�
		Index,					// �C���f�b�N�X
		D3DFMT_INDEX16,
		Vertex,					// ���_
		sizeof(VERTEX_LOCUS));

	// �ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, Cull);
	pDevice->SetRenderState(D3DRS_LIGHTING, Light);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, zwrite);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, alpha);			// ���u�����h��
	pDevice->SetRenderState(D3DRS_SRCBLEND, src);
	pDevice->SetRenderState(D3DRS_DESTBLEND, dest);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, texv);
}


/*									//
//				�J�n				//
//									*/
void Locus::Begin(Matrix& mat) {
	Vector3 pos = Vector3(mat._41, mat._42, mat._43);
	Vector3 up = Vector3(mat._21, mat._22, mat._23);
	D3DXVec3Normalize(&up, &up);

	m_bUse = true;
	m_nNew = 0;

	for( int i = 0; i < SAVE_NUM; i ++ ) {
		m_vStart[i].Pos	= pos + up * m_fUp;
		m_vEnd[i].Pos	= pos + up * m_fDown;
		m_vStart[i].Tex	= Vector2(1.0f, 1.0f);
		m_vEnd[i].Tex	= Vector2(0.0f, 1.0f);
	}
}


/*									//
//				�I��				//
//									*/
void Locus::End() {
	m_bUse = false;
}


/*									//
//				���W�Z�b�g			//
//									*/
void Locus::SetNewPos(Matrix& mat) {
	if (!m_bUse) {
		Begin(mat);

		return;
	}

	Vector3 pos = Vector3(mat._41, mat._42, mat._43);
	Vector3 up = Vector3(mat._21, mat._22, mat._23);
	D3DXVec3Normalize(&up, &up);

	m_vStart[m_nNew].Pos	= pos + up * m_fUp;
	m_vEnd[m_nNew].Pos		= pos + up * m_fDown;
	m_vStart[m_nNew].Tex	= Vector2(1.0f, 1.0f);
	m_vEnd[m_nNew].Tex		= Vector2(0.0f, 1.0f);

	m_nNew = (m_nNew + 1) % SAVE_NUM;
}