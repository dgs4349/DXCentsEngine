#ifndef LIGHT_CALC_H
#define LIGHT_CALC_H

#include "LightTypes.h"

float3 GetLightColor(Light light)
{
	float3 color;

	// Direction Light Calculation
	float3 normDir = normalize(-light.direction.rgb);
	float NdotL = saturate(dot(norm, li))
}


#endif