#pragma once

#include "XSceneObject.h"
#include "XColor.h"

class XLight : public XSceneObject
{
public:
	XLight();

	double GetIntensity() const;
	void SetIntensity(double nIntensity);

private:
	double Intensity;
	XColor Color;
};