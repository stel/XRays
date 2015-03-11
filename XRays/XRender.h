#pragma once

#include "XVector3d.h"
#include "XMesh.h"
#include "XZBuffer.h"
#include "XScene.h"
#include "XCamera.h"

class XRender
{
public:
	XRender() {};
	XRender(CDC *DC, CRect Rect);

	virtual void SetTargetDC(CDC *nDC);
	virtual void SetTargetRect(CRect nRect);

	virtual void DrawPixel(int x, int y, COLORREF Color) = 0;
	virtual void DrawZPixel(int x, int y, int z, COLORREF Color) = 0;
	virtual void DrawPoint(const XVector3d &A, COLORREF Color) = 0;
	virtual void DrawLine(const XVector3d &A, const XVector3d &B) = 0;
	virtual void DrawTriangle(XVector3d A, XVector3d B, XVector3d C, COLORREF Color) = 0;
	virtual void DrawMesh(XMesh &Mesh) = 0;

	virtual void RenderScene(const XScene& Scene, const XCamera* Camera) = 0;

	virtual void OutputText(const CString &Text, int x, int y) const = 0;
	virtual double GetZValue(int x, int y) = 0;

protected:
	CDC *TargetDC;
	CRect TargetRect;
};