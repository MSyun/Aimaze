// 3D�Q�[�W�N���X
// 2017. 1.31	�v���O�����쐬
// Author		SyunMizuno



#include	"Gauge3.h"
#include	"../GameFramework/framework/Graphic/3dgraphic.h"


/*									//
//			�R���X�g���N�^			//
//									*/
Gauge3::Gauge3(int _max, LPDIRECT3DTEXTURE9 _tex) :
	Quad(_tex),
	m_nMax(_max),
	m_nCur(_max)
{
	SetName("Gauge3");
}


/*									//
//				������				//
//									*/
HRESULT Gauge3::Init(float _SizeX, float _SizeZ)
{
	return Quad::SetUp(_SizeX, _SizeZ);
}


/*									//
//			���݂̐��l��ݒ�		//
//									*/
void Gauge3::SetCurrentNum(int _num) {
	// �͈̓`�F�b�N
	if (_num < 0 || _num > m_nMax)	return;

	m_nCur = _num;

	// �e�N�X�`�����W
	float texX = 1.0f / m_nMax * m_nCur;
	// ���W
	float posX = m_fSizeX / m_nMax * m_nCur;

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D	*pVtx;

	// ���_�f�[�^�͈̔͂����b�N�����_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < m_nNumVtx; i ++) {
		if (i % 2 == 0)	continue;
		// �e�N�X�`�����W
		pVtx[i].tex.x = texX;
		// ���W
		pVtx[i].vtx.x = pVtx[i - 1].vtx.x + posX;
	}

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}