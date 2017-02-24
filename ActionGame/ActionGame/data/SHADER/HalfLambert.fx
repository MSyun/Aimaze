// -------------------------------------------------------------
// �n�[�t�����o�[�g
// 
// Copyright (c) 2003 IMAGIRE Takashi. All rights reserved.
// -------------------------------------------------------------

// -------------------------------------------------------------
// �O���[�o���ϐ�
// -------------------------------------------------------------

float4x4 matWorld[4]	:	WORLD;		// ���[���h�ϊ��s��z��
float4x4 matView		:	VIEW;		// �r���[�ϊ��s��
float4x4 matProj		:	PROJECTION;	// �ˉe�ϊ��s��

int iBlendNum;							// �u�����h����z��̐�

float4 vLightDir;	// ���C�g�̕���
float4 vColor;		// ���C�g�����b�V���̐F


texture tex;
sampler TexSamp = sampler_state
{
	texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};


// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT
{
	float4	Pos			: POSITION;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
};
// -------------------------------------------------------------
// �V�[���̕`��
// -------------------------------------------------------------
VS_OUTPUT VS(
		float3 Pos		: POSITION,			// ���[�J���ʒu���W
		float3 Normal	: NORMAL,			// �@���x�N�g��
		float2 Tex		: TEXCOORD0			// �e�N�X�`��
){
	VS_OUTPUT Out = (VS_OUTPUT)0;		// �o�̓f�[�^
	
	// ���W�ϊ�
	Out.Pos = mul( float4(Pos, 1.0f), matWorld[0] );
	Normal = mul( Normal, (float3x3)matWorld[0] );
	Out.Pos = mul( Out.Pos, matView );
	Out.Pos = mul( Out.Pos, matProj );

	// �e�N�X�`��
	Out.Tex = Tex;

	// �@��
	Out.Normal = Normal;

	return Out;
}

// �X�L�����b�V���o�[�W����
VS_OUTPUT VS_SKIN(
		float3 Pos		: POSITION,			// ���[�J���ʒu���W
		float3 Normal	: NORMAL,			// �@���x�N�g��
		float2 Tex		: TEXCOORD0,		// �e�N�X�`��
		float4 W		: BLENDWEIGHT		// �d��
){
	VS_OUTPUT Out = (VS_OUTPUT)0;		// �o�̓f�[�^

	//----- ���W�ϊ�
	float		Weight[4] = (float[4])W;	// �d�݂�float�ɕ���
	float		LastBlendWeight = 0.0f;		// �Ō�̍s��Ɋ|������d��
	float4x4	matCombWorld = 0.0f;		// �������[���h�ϊ��s��

	// ���[���h�ϊ��s����u�����h
	for( int i = 0; i < iBlendNum-1; i++ ) {
		LastBlendWeight += Weight[i];				// �Ō�̏d�݂������Ōv�Z���Ă���
		matCombWorld += matWorld[i] * Weight[i];
	}

	// �Ō�̏d�݂𑫂��Z
	matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);

	Out.Pos = mul( float4(Pos, 1.0f), matCombWorld );	// ���[���h�ϊ�
	Normal = mul( Normal, (float3x3)matCombWorld );
	Out.Pos = mul( Out.Pos, matView );	// �r���[�ϊ�
	Out.Pos = mul( Out.Pos, matProj );	// �ˉe�ϊ�

	//----- �e�N�X�`��
	Out.Tex = Tex;

	// �@��
	Out.Normal = Normal;
	
	return Out;
}

// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
	// ���_�F
	float3 L = -vLightDir.xyz;
//	float amb = vLightDir.w;
	float3 N = normalize(In.Normal);

	float3 p = dot(N, L);
	p = (p + 1.0f) * 0.5f;
	p *= p;
	// �e�N�X�`��������ꍇ�}�e���A���J���[�͂���Ȃ�����
	float4 Col;
//	float4 Col = p;
	Col.xyz = vColor.xyz * p;

	Col.w = 1.0f;

	return Col * tex2D( TexSamp, In.Tex );
}

float4 PS_pass1(VS_OUTPUT In) : COLOR
{
	// ���_�F
	float3 L = -vLightDir.xyz;
	float amb = vLightDir.w;
	float3 N = normalize(In.Normal);

	float3 p = dot(N, L);
	p = p * 0.5f + 0.5f;
	p *= p;
	float4 Col;
	Col.xyz = vColor.xyz * p;

	Col.w = 1.0f;

	return Col;
}

// -------------------------------------------------------------
// �e�N�j�b�N
// -------------------------------------------------------------
technique TShader
{
	pass P0	// �e�N�X�`������
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS_pass0();
	}

	pass P1	// �e�N�X�`���Ȃ�
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS_pass1();
	}

	// �X�L�����b�V��
	pass P2	// �e�N�X�`������
	{
		VertexShader = compile vs_1_1 VS_SKIN();
		PixelShader  = compile ps_2_0 PS_pass0();
	}

	pass P3	// �e�N�X�`���Ȃ�
	{
		VertexShader = compile vs_1_1 VS_SKIN();
		PixelShader  = compile ps_2_0 PS_pass1();
	}
}
