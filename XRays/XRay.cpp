#include "stdafx.h"
#include "XRay.h"

XRay::XRay()
{
}

XRay::XRay(const XRay& Ray)
{
	Origin = Ray.Origin;
	Direction = Ray.Direction;
}

XRay::XRay(const XVector3d nOrigin, XVector3d nDirection)
{
	Origin = nOrigin;
	Direction = nDirection;
}

void XRay::SetDirection(const XVector3d nDirection)
{
	Direction = nDirection;
}

XVector3d XRay::GetDirection() const
{
	return Direction;
}

void XRay::SetOrigin(const XVector3d nOrigin)
{
	Origin = nOrigin;
}

XVector3d XRay::GetOrigin() const
{
	return Origin;
}