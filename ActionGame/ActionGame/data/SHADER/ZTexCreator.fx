// -------------------------------------------------------------
// �V���h�E�}�b�v
// 
// -------------------------------------------------------------

// -------------------------------------------------------------
// �O���[�o���ϐ�
// -------------------------------------------------------------
float4x4 matWorld[4]	:	WORLD;		// ���[���h�ϊ��s��z��
float4x4 matView		:	VIEW;		// �r���[�ϊ��s��
float4x4 matProj		:	PROJECTION;	// �ˉe�ϊ��s��

int iBlendNum;							// �u�����h����z��̐�


// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT
{
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
	Out.Pos = mul(Pos, matWorld[0]);
	Out.Pos = mul(Out.Pos, matView);
	Out.Pos = mul(Out.Pos, matProj);

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
	float		Weight[4] = (float[4])W;	// �d�݂�float�ɕ���
	float		LastBlendWeight = 0.0f;		// �Ō�̍s��Ɋ|������d��
	float4x4	matCombWorld = 0.0f;		// �������[���h�ϊ��s��

											// ���[���h�ϊ��s����u�����h
	for (int i = 0; i < iBlendNum - 1; i++) {
		LastBlendWeight += Weight[i];				// �Ō�̏d�݂������Ōv�Z���Ă���
		matCombWorld += matWorld[i] * Weight[i];
	}

	// �Ō�̏d�݂𑫂��Z
	matCombWorld += matWorld[iBlendNum - 1] * (1.0f - LastBlendWeight);

	float4 P_after;
	P_after = mul(Pos, matCombWorld);	// ���[���h�ϊ�
	P_after = mul(P_after, matView);	// �r���[�ϊ�
	P_after = mul(P_after, matProj);	// �ˉe�ϊ�
	Out.Pos = P_after;

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
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS();
	}
}