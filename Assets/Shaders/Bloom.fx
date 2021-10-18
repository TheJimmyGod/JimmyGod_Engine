// Description: Simple shader that applies transformation and texturing.

cbuffer TransformBuffer : register(b0)
{
    matrix WVP;
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);
SamplerState bloomSampler : register(s1);
float BloomIntensity = 1.3f;
float OriginalIntensity = 1.0f;
float BloomSaturation = 1.0f;
float OriginalSaturation = 1.0f;

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

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), WVP);
    output.texCoord = input.texCoord;
    return output;
}

float4 AdjustSaturation(float4 Color, float Saturation)
{
    float Grey = dot(Color, float4(0.3f, 0.59f, 0.11f,0.0f));
    return lerp(Grey, Color, Saturation);
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 bloomColor = textureMap.Sample(bloomSampler, input.texCoord);
    float4 originalColor = textureMap.Sample(textureSampler, input.texCoord);
    bloomColor = AdjustSaturation(bloomColor, BloomSaturation) * BloomIntensity;
    originalColor = AdjustSaturation(originalColor, OriginalSaturation) * OriginalIntensity;
    originalColor *= (1.0f - saturate(bloomColor));
    float4 color = originalColor + bloomColor;
    color.a = 0.2f;
    return color;

}