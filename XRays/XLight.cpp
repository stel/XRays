#include "stdafx.h"
#include "XLight.h"

XLight::XLight()
{
	Intensity = 0.2;
}

double XLight::GetIntensity() const
{
	return Intensity;
}

void XLight::SetIntensity(double nIntensity)
{
	Intensity = nIntensity;
}