// Description : Custom Shader for the earth

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D nightLightMap : register(t4);
Texture2D cloudMap : register(t5);
SamplerState textureSampler : register(s0);

cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix WVP; // World view projection
	float3 ViewPosition;
}

cbuffer LightBuffer : register(b1)
{
	float3 LightDirection;
	float4 LightAmbient;
	float4 LightDiffuse;
	float4 LightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
	float4 MaterialAmbient;
	float4 MaterialDiffuse;
	float4 MaterialSpecular;
	float MaterialPower;
}

cbuffer SettingsBuffer : register(b3)
{
	float specularMapWeight : packoffset(c0.x);
	float bumpMapWeight : packoffset(c0.y);
	float normalMapWeight : packoffset(c0.z);
}

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_Position;
	float3 worldNormal : NORMAL;
	float3 worldTangent : TEXCOORD0;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float2 texCoord : TEXCOORD3;
};

struct VSCloudOutput
{
	float4 position : SV_Position;
	float3 worldNormal : NORMAL;
	float3 dirToLight : TEXCOORD0;
	float2 texCoord : TEXCOORD1;
};

VSOutput VSPlanet(VSInput input)
{
	VSOutput output;
	float3 localPosition = input.position + (input.normal * bumpMapWeight /** displacement * bumpMapWeight*/);
	float3 worldPosition = mul(float4(localPosition, 1.0f), World).xyz;
	float3 worldNormal = mul(input.normal, (float3x3)World);
	float3 worldTangent = mul(input.tangent, (float3x3)World);
    
	output.position = mul(float4(localPosition, 1.0f), WVP);
	output.worldNormal = worldNormal;
	output.worldTangent = worldTangent;
	output.dirToLight = -LightDirection;
	output.dirToView = normalize(ViewPosition - worldPosition);
	output.texCoord = input.texCoord;
	return output;
}
//		|
//		V
//	Rasterizer
//		|
//		V
float4 PSPlanet(VSOutput input) : SV_TARGET
{
	// Renormalize normals from vertex shader

	float3 worldNormal = normalize(input.worldNormal);
	float3 worldTangent = normalize(input.worldTangent);
	float3 worldBinormal = normalize(cross(worldNormal, worldTangent));

	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float3 normal = worldNormal;
	float4 ambient = LightAmbient * MaterialAmbient;
	
	float diffuseRaw = dot(dirToLight, normal);
	float diffuseIntensity = saturate(dot(dirToLight, normal)); // Saturate is comparison of size of number
    float4 diffuse = diffuseIntensity * LightDiffuse * MaterialDiffuse;

	float3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = saturate(dot(halfAngle, normal));
	float specularIntensity = pow(specularBase, MaterialPower);
    float4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	float4 textureColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float specularFactor = specularMap.Sample(textureSampler, input.texCoord).r;

    float4 color = (ambient + diffuse) * textureColor + specular * 1.0f/** (specularMapWeight != 0.0f ? specularFactor : 1.0f)*/;
    return color;
}