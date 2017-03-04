// �V���h�E�}�b�v
// 2017. 2.25	�v���O�����쐬

#include	"PosCreator.fh"


// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT {
	float4 Pos			: POSITION;
	float4 ShadowMapUV	: TEXCOORD0;
};

// -------------------------------------------------------------
// ���_�V�F�[�_�v���O����
// -------------------------------------------------------------
VS_OUTPUT VS(
	float4 Pos	: POSITION		// ���f���̒��_
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;		// �o�̓f�[�^

	// ���W�ϊ�
	Out.Pos = PosCreator(Pos, matWorld[0]);

	// �J�������W�n�ł̐[�x���e�N�X�`���ɓ����
	Out.ShadowMapUV = Out.Pos;

	return Out;
}


// �X�L�����b�V��
VS_OUTPUT VS_Skin(
	float4 Pos	: POSITION,		// ���f���̒��_
	float4 W	: BLENDWEIGHT
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;		// �o�̓f�[�^

	//----- ���W�ϊ�
	float4x4 World = SkinWorldCreator(W);
	Out.Pos = PosCreator(Pos, World);

	// �J�������W�n�ł̐[�x���e�N�X�`���ɓ����
	Out.ShadowMapUV = Out.Pos;

	return Out;
}
// -------------------------------------------------------------
// �s�N�Z���V�F�[�_�v���O����
// -------------------------------------------------------------
float4 PS(VS_OUTPUT In) : COLOR0 {
	return In.ShadowMapUV.z / In.ShadowMapUV.w;
}

// -------------------------------------------------------------
// �e�N�j�b�N
// -------------------------------------------------------------
technique TShader {
	// �V���h�E�}�b�v�̍쐬
	pass P0 {
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS();
	}

	// �V���h�E�}�b�v�̍쐬(Ver.�X�L��
	pass P1 {
		VertexShader = compile vs_1_1 VS_Skin();
		PixelShader = compile ps_2_0 PS();
	}
}