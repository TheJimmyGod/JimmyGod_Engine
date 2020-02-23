// Description : Post processing shader.

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

static float Offsets[11] =
{
  -5,
  -4,
  -3,
  -2,
  -1,
  0,
  1,
  2,
  3,
  4,
  5,
};

static float Weights[11][11] =
{
  {0.007959,0.008049,0.00812,0.008171,0.008202,0.008212,0.008202,0.008171,0.00812,0.008049,0.007959},
  {0.008049,0.00814,0.008212,0.008263,0.008294,0.008305,0.008294,0.008263,0.008212,0.00814,0.008049},
  {0.00812,0.008212,0.008284,0.008336,0.008367,0.008378,0.008367,0.008336,0.008284,0.008212,0.00812},
  {0.008171,0.008263,0.008336,0.008388,0.00842,0.00843,0.00842,0.008388,0.008336,0.008263,0.008171},
  {0.008202,0.008294,0.008367,0.00842,0.008451,0.008462,0.008451,0.00842,0.008367,0.008294,0.008202},
  {0.008212,0.008305,0.008378,0.00843,0.008462,0.008473,0.008462,0.00843,0.008378,0.008305,0.008212},
  {0.008202,0.008294,0.008367,0.00842,0.008451,0.008462,0.008451,0.00842,0.008367,0.008294,0.008202},
  {0.008171,0.008263,0.008336,0.008388,0.00842,0.00843,0.00842,0.008388,0.008336,0.008263,0.008171},
  {0.00812,0.008212,0.008284,0.008336,0.008367,0.008378,0.008367,0.008336,0.008284,0.008212,0.00812},
  {0.008049,0.00814,0.008212,0.008263,0.008294,0.008305,0.008294,0.008263,0.008212,0.00814,0.008049},
  {0.007959,0.008049,0.00812,0.008171,0.008202,0.008212,0.008202,0.008171,0.00812,0.008049,0.007959},
};


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

float4 PSGaussian(VSOutput input) : SV_Target
{
	float PixelWidth = 1.0f / 1280;
	float PixelHeight = 1.0f / 720;

	float4 Color = { 0, 0, 0, 0 };

	float2 Blur;

	for (int x = 0; x < 11; x++)
	{
		Blur.x = input.texCoord.x + Offsets[x] * PixelWidth;
		for (int y = 0; y < 11; y++)
		{
			Blur.y = input.texCoord.y + Offsets[y] * PixelHeight;
			Color += textureMap.Sample(textureSampler, Blur) * Weights[x][y];
		}
	}

	return Color;
}

float4 PSNoProcessing(VSOutput input) : SV_Target
{
	float4 color = textureMap.Sample(textureSampler, input.texCoord);
	return color;
}