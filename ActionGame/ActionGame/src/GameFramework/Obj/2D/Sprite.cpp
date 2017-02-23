///////////////////////////////////////////////////////////////
// �X�v���C�g�N���X
// author		SyunMizuno
// 2016. 1. 3	�v���O�����쐬
///////////////////////////////////////////////////////////////

#include	"Sprite.h"
#include	"Obj2DManager.h"
#include	"../../framework/Debug/Debug.h"
#include	"../../framework/Utility/System/SystemUtility.h"
#include	"../../framework/Graphic/Graphics.h"

/*									//
//			�R���X�g���N�^			//
//									*/
Sprite::Sprite() :
	m_pTex(NULL),
	m_Color(255, 255, 255, 255),
	m_vTexPos(0.0f, 0.0f),
	m_vTexSize(1.0f, 1.0f)
{
	GetObj2DManager()->Add(this);

	m_pRectTransform = new RectTransform(this);

	MakeVtx();
}


/*									//
//			�f�X�g���N�^			//
//									*/
Sprite::~Sprite() {
	GetObj2DManager()->Delete(this);
	SAFE_DELETE(m_pRectTransform);
}


/*									//
//				�X�V				//
//									*/
void Sprite::Update() {
	SetVtx();
	SetTex();
	SetColor();
}


/*									//
//				�`��				//
//									*/
void Sprite::Draw() {
	// ���C�e�B���O�𖳌��ɂ���
	DWORD lightflg;
	GetGraphics()->GetDevice()->GetRenderState(D3DRS_LIGHTING, &lightflg);
	GetGraphics()->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	GetGraphics()->GetDevice()->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̃Z�b�g
	GetGraphics()->GetDevice()->SetTexture(0, m_pTex);

	// �|���S���̕`��
	GetGraphics()->GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,	// 1:�v���~�e�B�u�̎��
		NUM_POLYGON,			// 2:�|���S����
		m_aVtx,					// 3:�z��̐擪�A�h���X
		sizeof(VERTEX_2D));	// 4:���_�̃f�[�^�̑傫��

	// ���C���e�B���O��߂�
	GetGraphics()->GetDevice()->SetRenderState(D3DRS_LIGHTING, lightflg);
}


/*									//
//			���_������				//
//									*/
void Sprite::MakeVtx() {
	// rhw�̐ݒ�( 1.0�Œ� )
	m_aVtx[0].rhw = 
	m_aVtx[1].rhw = 
	m_aVtx[2].rhw = 
	m_aVtx[3].rhw = 1.0f;

	// �J���[�̐ݒ�( 0 �` 255 )
	Sprite::SetColor();

	// ���W
	Sprite::SetVtx();

	// �e�N�X�`��
	Sprite::SetTex();
}


/*									//
//			���_���W�̐ݒ�			//
//									*/
void Sprite::SetVtx() {
	Vector3* Vtx = m_pRectTransform->GetVertex();

	for (int i = 0; i < 4; ++ i) {
		m_aVtx[i].pos = Vtx[i];
	}
}


/*									//
//		�e�N�X�`�����W�̏�����		//
//									*/
void Sprite::SetTex() {
	// �e�N�X�`�����W�̐ݒ�
	m_aVtx[0].tex = m_vTexPos;
	m_aVtx[1].tex = Vector2(m_vTexPos.x + m_vTexSize.x, m_vTexPos.y);
	m_aVtx[2].tex = Vector2(m_vTexPos.x, m_vTexPos.y + m_vTexSize.y);
	m_aVtx[3].tex = Vector2(m_vTexPos.x + m_vTexSize.x, m_vTexPos.y + m_vTexSize.y);
}


/*									//
//			���_�J���[�̐ݒ�		//
//									*/
void Sprite::SetColor() {
	// ( 0 �` 255 )
	m_aVtx[0].col = m_Color;
	m_aVtx[1].col = m_Color;
	m_aVtx[2].col = m_Color;
	m_aVtx[3].col = m_Color;
}


/*									//
//		�����e�N�X�`���T�C�Y�̎擾	//
// 2�ׂ̂���łȂ��Ǝ����ŕ␳�����*/
void Sprite::SetSizeDefault() {
	if (!m_pTex) {
		Debug::Log("�e�N�X�`�����ݒ�̂��ߏ��̎擾�͂ł��܂���");
		return;
	}

	// �e�N�X�`���T�C�Y�̎擾
	D3DSURFACE_DESC desc;

	if (FAILED(m_pTex->GetLevelDesc(0, &desc))) {
		Debug::LogError("�e�N�X�`�����̎擾�Ɏ��s���܂���");
		return;
	}

	m_pRectTransform->SetSize((float)desc.Width, (float)desc.Height);
}