////////////////////////////////////////////////////
// 2015.12.10	�v���O�����쐬
// 2016. 7. 8	�r���{�[�h�@�\�ǉ�
////////////////////////////////////////////////////


#include	"Quad.h"
#include	"../../Graphic/3dgraphic.h"
#include	"../../Graphic/Graphics.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Quad::Quad(LPDIRECT3DTEXTURE9 _Tex) :
	m_pTex(_Tex),
	m_pVtxBuff(NULL), m_pIdxBuff(NULL),
	m_nNumVtx(0), m_nNumIdx(0), m_nNumPolygon(0),
	m_fSizeX(3), m_fSizeZ(3),
	m_bBillboard(false)
{
	SetName("QuadMesh");
}


//=============================================================================
// ����������
//=============================================================================
HRESULT Quad::SetUp(
	float _SizeX, float _SizeZ,	// �T�C�Y
	int _NumX, int _NumZ)		// �u���b�N��
{
	Release();

	m_fSizeX = _SizeX;
	m_fSizeZ = _SizeZ;
	// ���_���̍쐬
	return MakeVertex(_NumX, _NumZ, _SizeX, _SizeZ);
}


//=============================================================================
// �I������
//=============================================================================
void Quad::Release() {
	if (m_pVtxBuff != NULL) {
		// ���_�o�b�t�@
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	if (m_pIdxBuff != NULL) {
		// �C���f�b�N�X�o�b�t�@
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
}


//=============================================================================
// �X�V����
//=============================================================================
void Quad::Update() {
}


//=============================================================================
// �`�揈��
//=============================================================================
void Quad::LateDraw() {
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	Matrix mat = m_pTransform->GetWorld(), view;
	DWORD lightflg, cull;
	pDevice->GetRenderState(D3DRS_LIGHTING, &lightflg);

	if (m_bBillboard) {
		// �r���[�s��̎擾
		Matrix view;
		view = GetGraphics()->GetView();

		// �r���{�[�h�Ƃ��Ďg�p
		if (view != NULL) {
			// �@���x�N�g����Y���Ȃ̂�
			// Y,Z�x�N�g�������ւ���
			mat._11 = view._11;
			mat._12 = view._21;
			mat._13 = view._31;
			mat._21 = view._13;
			mat._22 = view._23;
			mat._23 = view._33;
			mat._31 = view._12;
			mat._32 = view._22;
			mat._33 = view._32;
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			m_pTransform->SetWorld(mat);
		}
	}
	pDevice->SetTransform(D3DTS_WORLD, &mat);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff, 0,
		sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �J�����O�ݒ�
	pDevice->GetRenderState(D3DRS_CULLMODE, &cull);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTex);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,	// ���_�o�b�t�@�̃x�[�X�ʒu
		0,	// ���_�o�b�t�@�̎g�p�J�n�ʒu
		m_nNumVtx,
		0,	// ���_�C���f�b�N�X�o�b�t�@�̊J�n�ʒu
		m_nNumPolygon);

	pDevice->SetRenderState(D3DRS_LIGHTING, lightflg);
	pDevice->SetRenderState(D3DRS_CULLMODE, cull);
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT Quad::MakeVertex(
	int _NumX, int _NumZ,		// �u���b�N��
	float _SizeX, float _SizeZ)// �T�C�Y
{
	LPDIRECT3DDEVICE9 pDevice = GetGraphics()->GetDevice();
	// �����_���̌v�Z
	m_nNumVtx = (_NumX + 1) * (_NumZ + 1);
	m_nNumIdx = (_NumX + 1) * _NumZ * 2 + (_NumZ - 1) * 2;

	// ���|���S�����̌v�Z
	m_nNumPolygon = _NumX * _NumZ * 2 + (_NumZ - 1) * 4;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumVtx,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p�@
		FVF_VERTEX_3D,					// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,				// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&m_pVtxBuff,					// ���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
		NULL))) {
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nNumIdx,	// �m�ۂ���T�C�Y
		D3DUSAGE_WRITEONLY,			// �g�p�@
		D3DFMT_INDEX16,				// �t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&m_pIdxBuff,				// 
		NULL))) {
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D	*pVtx;

#if	0
		const float fSizeTexX = 1.0f / _NumX;
		const float fSizeTexY = 1.0f / _NumZ;
#else
		const float fSizeTexX = 1.0f;
		const float fSizeTexY = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N�����_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntZ = 0; nCntZ < _NumZ + 1; nCntZ ++) {
			for (int nCntX = 0; nCntX < _NumX + 1; nCntX ++) {
				// ���_���W
				pVtx[nCntZ*(_NumX + 1) + nCntX].vtx.x = -_NumX / 2.0f * _SizeX + nCntX * _SizeX;
				pVtx[nCntZ*(_NumX + 1) + nCntX].vtx.y = 0.0f;
				pVtx[nCntZ*(_NumX + 1) + nCntX].vtx.z = _NumZ / 2.0f * _SizeZ - nCntZ  * _SizeZ;

				// �@��
				pVtx[nCntZ*(_NumX + 1) + nCntX].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				// ���ˌ�
				pVtx[nCntZ*(_NumX + 1) + nCntX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W
				pVtx[nCntZ*(_NumX + 1) + nCntX].tex.x = fSizeTexX * nCntX;
				pVtx[nCntZ*(_NumX + 1) + nCntX].tex.y = fSizeTexY * nCntZ;
			}
		}

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuff->Unlock();

		{	// �C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
			WORD*	pIdx;

			// �C���f�b�N�X�f�[�^�͈̔͂����b�N�����_�o�b�t�@�ւ̃|�C���^���擾
			m_pIdxBuff->Lock(
				0, 0, (void**)&pIdx, 0);

			int nCntIdx = 0;
			for (int nCntZ = 0; nCntZ < _NumZ; nCntZ ++) {
				if (nCntZ > 0) {
					// �k�ރ|���S���̂��߂̃_�u���ݒ�
					pIdx[nCntIdx] = (nCntZ + 1) * (_NumX + 1);
					nCntIdx++;
				}
				for (int nCntX = 0; nCntX < _NumX + 1; nCntX ++) {
					pIdx[nCntIdx] = (nCntZ + 1) * (_NumX + 1) + nCntX;
					nCntIdx++;
					pIdx[nCntIdx] = nCntZ * (_NumX + 1) + nCntX;
					nCntIdx++;
				}

				if (nCntZ < _NumZ - 1) {
					// �k�ރ|���S���̂��߂̃_�u���ݒ�
					pIdx[nCntIdx] = nCntZ * (_NumX + 1) + _NumX;
					nCntIdx++;
				}
			}
		}
		// �C���f�b�N�X�f�[�^���A�����b�N����
		m_pIdxBuff->Unlock();
	}

	return S_OK;
}


void Quad::SetColor(Color _color) {
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D	*pVtx;

		// ���_�f�[�^�͈̔͂����b�N�����_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < m_nNumVtx; nCnt ++)
			// ���ˌ�
			pVtx[nCnt].diffuse = _color;

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}