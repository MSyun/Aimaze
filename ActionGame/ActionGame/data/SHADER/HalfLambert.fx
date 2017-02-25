// �n�[�t�����o�[�g
// 

#include	"PosCreator.fh"

// -------------------------------------------------------------
// �O���[�o���ϐ�
// -------------------------------------------------------------
float4 vLightDir;	// ���C�g�̕���
float4 vColor;		// ���C�g�����b�V���̐F


texture tex;
sampler TexSamp = sampler_state {
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
struct VS_OUTPUT {
	float4	Pos			: POSITION;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
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

	// �e�N�X�`��
	Out.Tex = Tex;

	// �@��
	Out.Normal = mul(Normal, (float3x3)matWorld[0]);;

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

	//----- �e�N�X�`��
	Out.Tex = Tex;

	// �@��
	Out.Normal = mul(Normal, (float3x3)matWorld);;
	
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
