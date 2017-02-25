// ���ʔ��ˌ�
// 
// �X�L�����b�V�������Ƀo�O����

#include	"PosCreator.fh"

// -------------------------------------------------------------
// �O���[�o���ϐ�
// -------------------------------------------------------------
float4	vLightDir;					// ���C�g�̕���
float4	vColor;						// ���C�g�����b�V���̐F
float3	vEyePos;					// �J�����̈ʒu�i���[�J�����W�n�j
float	Specular = 100.f;			// �X�y�L�����[�͈̔�
float	SpecularPower = 1.0f;		// �X�y�L�����[�̋���(0�ɂ���ƃ����o�[�g�ɂȂ�)
float	LightBloomPower = 1.0f;		// ���C�g�u���[���̋���

texture tex;
sampler TexSamp = sampler_state {
	texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};


// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT {
	float4	Pos			: POSITION;
	float4	Color		: COLOR0;
	float2	Tex			: TEXCOORD0;
	float3	N			: TEXCOORD1;
	float3	Eye			: TEXCOORD2;
};
// -------------------------------------------------------------
// �V�[���̕`��
// -------------------------------------------------------------
VS_OUTPUT VS(
		float4 Pos		: POSITION,			// ���[�J���ʒu���W
		float3 Normal	: NORMAL,			// �@���x�N�g��
		float2 Tex		: TEXCOORD0			// �e�N�X�`��
){
	VS_OUTPUT Out = (VS_OUTPUT)0;		// �o�̓f�[�^
	
	// ���W�ϊ�
	Out.Pos = PosCreator(Pos, matWorld[0]);

//	float4 position;
//	position = mul( float4(Pos, 1.0f), matWorld[0] );
//	Normal = mul( Normal, (float3x3)matWorld[0] );

	// �e�N�X�`��
	Out.Tex = Tex;
	
	// ���_�F
	float3 L = -vLightDir.xyz;
	float amb = vLightDir.w;
	Out.Color = vColor * max(amb, dot(L, Normal));
//	Out.Color.w = 1.0f;

	// ���ʔ��˗p�̃x�N�g��
	Out.N	= Normal.xyz;
//	Out.Eye	= vEyePos - position.xyz;
	Out.Eye	= vEyePos - Pos;

	return Out;
}
// �X�L�����b�V���o�[�W����
VS_OUTPUT VS_SKIN(
		float4 Pos		: POSITION,			// ���[�J���ʒu���W
		float3 Normal	: NORMAL,			// �@���x�N�g��
		float2 Tex		: TEXCOORD0,		// �e�N�X�`��
		float4 W		: BLENDWEIGHT		// �d��
){
	VS_OUTPUT Out = (VS_OUTPUT)0;		// �o�̓f�[�^

	//----- ���W�ϊ�
	float4x4 matWorld = SkinWorldCreator(W);
	Out.Pos = PosCreator(Pos, matWorld);

	float4 position;
	position = mul(Pos, matWorld);
	Normal = mul(Normal, (float3x3)matWorld);

	//----- �e�N�X�`��
	Out.Tex = Tex;

	//----- ���_�F
	float3 L = -vLightDir.xyz;
	float amb = vLightDir.w;
	Out.Color = vColor * max(amb, dot(L, Normal));
//	Out.Color.w = 1.0f;

	// ���ʔ��˗p�̃x�N�g��
	Out.N	= Normal.xyz;
	Out.Eye	= vEyePos - position.xyz;
	
	return Out;
}

// -------------------------------------------------------------
struct PS_OUTPUT {
	float4 Col		:	COLOR0;
	float4 Luminous	:	COLOR1;
};

PS_OUTPUT PS_pass0(VS_OUTPUT In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;

	// ���C�g�x�N�g��
	float3 L = -vLightDir.xyz;
	// �n�[�t�x�N�g��
	float3 H = normalize(L + normalize(In.Eye));
	// �@���𐳋K���iVS����PS�̊ԂŒl���ς�邽�߁j
	float3 N = normalize(In.N);
	// ���ʔ��ˌ�
	float S = pow(max(0,dot(N,H)), Specular) * SpecularPower;

	Out.Col = In.Color * tex2D( TexSamp, In.Tex )	// �g�U���{����
			+ S;

	// �P�x�����Z�b�g
	float p = S * LightBloomPower;
	Out.Luminous = float4(p,p,p,1.0f);

	return Out;
}

PS_OUTPUT PS_pass1(VS_OUTPUT In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;

	float3 L = -vLightDir.xyz;
	float3 H = normalize(L + normalize(In.Eye));	// �n�[�t�x�N�g��
	float3 N = normalize(In.N);
	float S = pow(max(0,dot(N,H)), Specular) * SpecularPower;

	Out.Col = In.Color								// �g�U���{����
			+ S;

	// �P�x�����Z�b�g
	float p = S * LightBloomPower;
	Out.Luminous = float4(p,p,p,1.0f);

	return Out;
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
