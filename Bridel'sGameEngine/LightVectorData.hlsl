struct LightVectorData
{
	float3 vToL;	// vector To Light
	float3 dirToL;	// direction To Light
	float distToL;	// distance To Light
};

LightVectorData CalculateLightVectorData(const in float3 lightPos, const in float3 fragPos)
{
	LightVectorData lv;
	lv.vToL = lightPos - fragPos;
	lv.distToL = length(lv.vToL);
	lv.dirToL = lv.vToL / lv.distToL;
	return lv;
}