#pragma once
#include "XCoordSystem.h"
#include "XVector3d.h"

class XSceneObject
{
public:
	XSceneObject();
	XSceneObject(const XSceneObject& SceneObject);

	XSceneObject& operator= (const XSceneObject &SceneObject);

	void SetPosition(const XVector3d& nPosition); 
	XVector3d GetPosition() const;

	void SetRotation(const XVector3d& nPosition); 
	XVector3d GetRotation() const;

	bool selected;
	
//protected:
	XCoordSystem CoordSystem;
};