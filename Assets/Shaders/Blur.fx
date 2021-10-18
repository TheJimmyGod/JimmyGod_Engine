// Description: Simple shader that applies transformation and texturing.

cbuffer TransformBuffer : register(b0)
{
    matrix WVP;
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);
float BlurDistance = 0.012f;

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

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 Color;
    Color = textureMap.Sample(textureSampler, float2(input.texCoord.x + BlurDistance, input.texCoord.y + BlurDistance));
    Color += textureMap.Sample(textureSampler, float2(input.texCoord.x - BlurDistance, input.texCoord.y - BlurDistance));
    Color += textureMap.Sample(textureSampler, float2(input.texCoord.x + BlurDistance, input.texCoord.y - BlurDistance));
    Color += textureMap.Sample(textureSampler, float2(input.texCoord.x - BlurDistance, input.texCoord.y + BlurDistance));
    Color = Color / 4;
    return Color;

}