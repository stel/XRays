#pragma once 

#include "XRender.h"

class XRenderFlat : public XRender
{
public:
	XRenderFlat();

	void SetTargetDC(CDC *nDC);
	void SetTargetRect(CRect nRect);

	void DrawPixel(int x, int y, COLORREF Color);
	inline void DrawZPixel(int x, int y, int z, COLORREF Color);
	void DrawPoint(const XVector3d &A, COLORREF Color);
	void DrawLine(const XVector3d &A, const XVector3d &B);
	void DrawTriangle(XVector3d A, XVector3d B, XVector3d C, COLORREF Color);
	void DrawMesh(XMesh &Mesh);

	void RenderScene(const XScene& Scene, const XCamera* Camera);

	void OutputText(const CString &Text, int x, int y) const;

	double GetZValue(int x, int y)
	{
		return ZBuffer.GetZValue(x, y);
	}

private:
	inline void swapVertices(const XVector3d** v1, const XVector3d** v2)
	{
		const XVector3d* b = *v1;
		*v1 = *v2;
		*v2 = b;
	}
	XZBuffer ZBuffer;
};
