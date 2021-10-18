// Description: Bloom shader.

cbuffer TransformBuffer : register(b0)
{
	matrix WVP;
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

float pSize;
struct pixel
{
    float4 color : COLOR;
};
struct GLOW_OUTPUT
{
	float4 position : POSITION;
	float2 texCoord0 : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};

GLOW_OUTPUT glowVSHorizontal1(VS_INPUT input)
{
	GLOW_OUTPUT OUT;
	OUT.position = mul(float4(input.position, 1.0f), WVP);
	OUT.texCoord0 = input.texCoord + float2(-pSize * 3.0f, 0.0f);
	OUT.texCoord1 = input.texCoord + float2(-pSize * 2.0f, 0.0f);
	OUT.texCoord2 = input.texCoord + float2(-pSize * 1.0f, 0.0f);
	OUT.texCoord3 = input.texCoord + float2(0.0f, 0.0f);
	return OUT;
}

GLOW_OUTPUT glowVSHorizontal2(VS_INPUT input)
{
	GLOW_OUTPUT OUT;
	OUT.position = mul(float4(input.position, 1.0f), WVP);
	OUT.texCoord0 = input.texCoord + float2(pSize * 3.0f, 0.0f);
	OUT.texCoord1 = input.texCoord + float2(pSize * 2.0f, 0.0f);
	OUT.texCoord2 = input.texCoord + float2(pSize * 1.0f, 0.0f);
	OUT.texCoord3 = input.texCoord + float2(0.0f, 0.0f);
	return OUT;
}

GLOW_OUTPUT glowVSVertical1(VS_INPUT input)
{
	GLOW_OUTPUT OUT;
	OUT.position = mul(float4(input.position, 1.0f), WVP);
	OUT.texCoord0 = input.texCoord + float2(0.0f, -pSize * 3.0f);
	OUT.texCoord1 = input.texCoord + float2(0.0f, -pSize * 2.0f);
	OUT.texCoord2 = input.texCoord + float2(0.0f, -pSize * 1.0f);
	OUT.texCoord3 = input.texCoord + float2(0.0f, 0.0f);
	return OUT;
}

GLOW_OUTPUT glowVSVertical2(VS_INPUT input)
{
	GLOW_OUTPUT OUT;
	OUT.position = mul(float4(input.position, 1.0f), WVP);
	OUT.texCoord0 = input.texCoord + float2(0.0f, pSize * 3.0f);
	OUT.texCoord1 = input.texCoord + float2(0.0f, pSize * 2.0f);
	OUT.texCoord2 = input.texCoord + float2(0.0f, pSize * 1.0f);
	OUT.texCoord3 = input.texCoord + float2(0.0f, 0.0f);
	return OUT;
}

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = mul(float4(input.position, 1.0f), WVP);
	output.texCoord = input.texCoord;
	return output;
}

pixel GLOW_PS(GLOW_OUTPUT input) : SV_Target
{
    pixel OUT;
	float4 color = textureMap.Sample(textureSampler, input.texCoord0) * 0.1f;
	color += textureMap.Sample(textureSampler, input.texCoord1) * 0.3f;
	color += textureMap.Sample(textureSampler, input.texCoord2) * 0.4f;
	color += textureMap.Sample(textureSampler, input.texCoord3) * 0.25f;
    color.a = 1.0f;
    OUT.color = color;
	return OUT;
}

pixel PS(VS_OUTPUT input) : SV_Target
{
    pixel OUT;
	OUT.color = textureMap.Sample(textureSampler, input.texCoord);
	return OUT;
}