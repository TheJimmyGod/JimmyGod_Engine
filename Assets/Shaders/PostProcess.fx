// Description : Post processing shader.

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

struct VSInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

VSOutput VS(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

//....|
//....V
// Rasterizer
//....|
//....V

float4 PS(VSOutput input) : SV_Target
{
	
	//float2 pos = float2((input.texCoord.x *2.0f) - 1.0f, (input.texCoord.x*-2.0f)+1.0f);
	//float2 offset = float2(cos(pos.x *100.0f),cos(pos.y*100.0f));
	// + (offset * 0.05f)
	float4 color = textureMap.Sample(textureSampler, input.texCoord);
	//color *= abs(cos(input.texCoord.y * 100.0f));
	return color;
}