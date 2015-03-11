#pragma once

#include "XVector3d.h"

class XRay
{
public:
	XRay();
	XRay(const XRay& Ray);
	XRay(const XVector3d nOrigin, XVector3d nDirection);

	void SetDirection(const XVector3d nDirection);
	XVector3d GetDirection() const;

	void SetOrigin(const XVector3d nOrigin);
	XVector3d GetOrigin() const;

private:
	XVector3d Origin;
	XVector3d Direction;
};