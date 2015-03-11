#include "stdafx.h"
#include "XCamera.h"

XCamera::XCamera()
{
	Perspective = false;
	Scale = 1;
}

XCamera::XCamera(int _XPos, int _YPos, int _ZPos)
{
}

XCamera::~XCamera()
{
}

void XCamera::SetTarget(XVector3d &NewTarget)
{
	Target = NewTarget;
}

XVector3d XCamera::CCS(const XVector3d &Vector3d) const
{
	XVector3d CCSVector(Vector3d), Position(CoordSystem.GetOrigin());

	CCSVector = CoordSystem.TransformVector(CCSVector);
	CCSVector += CoordSystem.GetOrigin();

	if (Perspective)
	{
		CCSVector = CCSVector*Position.Z / (Position.Z + CCSVector.Z);
	}

	CCSVector += Target;

	return CCSVector;
}

XVector3d XCamera::WCS(const XVector3d &Vector3d) const
{
	XVector3d WCSVector(Vector3d), Position(CoordSystem.GetOrigin());
	XCoordSystem CS = CoordSystem;
	double OldZ;

	WCSVector -= Target;

	if (Perspective)
	{
		OldZ = Position.Z*WCSVector.Z / (Position.Z - WCSVector.Z);
		WCSVector = WCSVector / Position.Z*(Position.Z + OldZ);
	}

	WCSVector -= CoordSystem.GetOrigin();
	WCSVector = CS.Back().TransformVector(WCSVector);

	return WCSVector;
}


void XCamera::MakeFrontProjection()
{
	Perspective = false;
	SetPosition(XVector3d(0, 0, 200));
	SetRotation(XVector3d(0, 0, 0));
}

void XCamera::MakeBackProjection()
{
	Perspective = false;
	SetPosition(XVector3d(0, 0, 200));
	SetRotation(XVector3d(180, 0, 180));
}

void XCamera::MakeTopProjection()
{
	Perspective = false;
	SetPosition(XVector3d(0, 0, 200));
	SetRotation(XVector3d(90, 0, 0));
}

void XCamera::MakeBottomProjection()
{
	Perspective = false;
	SetPosition(XVector3d(0, 0, 200));
	SetRotation(XVector3d(-90, 0, 0));
}

void XCamera::MakeLeftProjection()
{
	Perspective = false;
	SetPosition(XVector3d(0, 0, 200));
	SetRotation(XVector3d(0, 90, 0));
}

void XCamera::MakeRightProjection()
{
	Perspective = false;
	SetPosition(XVector3d(0, 0, 200));
	SetRotation(XVector3d(0, -90, 0));
}


void XCamera::MakePerspectiveProjection()
{
	Perspective = true;
	SetPosition(XVector3d(0, 0, 200));
	SetRotation(XVector3d(30, 30, 0));
}


