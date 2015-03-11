#pragma once

#include "XVector3d.h"
#include "XCoordSystem.h"
#include "XSceneObject.h"

class XCamera : public XSceneObject
{
public:
	XCamera();
	XCamera(int _XPos, int _YPos, int _ZPos);
	~XCamera();

	void SetTarget(XVector3d &NewTarget);
	XVector3d CCS(const XVector3d &Vector3d) const;
	XVector3d WCS(const XVector3d &Vector3d) const;

	void MakeFrontProjection();
	void MakeBackProjection();
	void MakeTopProjection();
	void MakeBottomProjection();
	void MakeLeftProjection();
	void MakeRightProjection();
	void MakePerspectiveProjection();

	XVector3d Rotation, Target;

private:
	bool Perspective;

	double Scale;
};
