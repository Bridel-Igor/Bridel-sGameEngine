#include "ShaderOps.hlsl"
#include "LightVectorData.hlsl"

#include "PointLight.hlsl"

cbuffer ObjectCBuf
{
	float4 materialColor;
	float4 specularColor;
	float specularPower;
};


float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal) : SV_Target
{
	// renormalize interpolated normal
	viewNormal = normalize(viewNormal);
	// fragment to light vector data
	const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);
	// attenuation
	const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse 
	const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);
	// specular
	const float3 specular = Speculate(
		diffuseColor, diffuseIntensity, viewNormal,
		lv.vToL, viewFragPos, att, specularPower
	);
	// final color
	return float4(saturate((diffuse + ambient, 1.0f) * materialColor.rgb + specular), 1.0f);
}