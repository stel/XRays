#include "stdafx.h"
#include "XSceneObject.h"

XSceneObject::XSceneObject()
{
	selected = false;
}

XSceneObject::XSceneObject(const XSceneObject& SceneObject)
{
	*this = SceneObject;
}

XSceneObject& XSceneObject::operator= (const XSceneObject &SceneObject)
{
	CoordSystem = SceneObject.CoordSystem;
	selected = SceneObject.selected;
	return *this;
}

void XSceneObject::SetPosition(const XVector3d& nPosition)
{
	CoordSystem.SetOrigin(nPosition);
}

XVector3d XSceneObject::GetPosition() const
{
	return CoordSystem.GetOrigin();
}

void XSceneObject::SetRotation(const XVector3d& nRotation)
{
	CoordSystem.SetRotation(nRotation);
}

XVector3d XSceneObject::GetRotation() const
{
	return CoordSystem.GetRotation();
}