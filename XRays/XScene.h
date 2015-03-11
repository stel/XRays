#pragma once

#include "XMesh.h"
#include "XLight.h"
#include "XArray.h"

class XScene
{
public:
	XScene();

	void AddMesh(const XMesh &Mesh);

	void AddCube(const XVector3d &Pos, const float a, COLORREF Color);

	void UnselectAll();
	void UnselectAllFaces(XMesh *Mesh);
	void UnselectAllVertices(XMesh *Mesh);
	void UnMarkAllVertices();

	XArray<XMesh*> XScene::GetSelectedMeshs();
	XArray<XFace*> XScene::GetSelectedFace();
	XArray<XVertex*> XScene::GetSelectedVertex();

	XArray<XMesh> Meshs;
	XArray<XLight> Lights;
};