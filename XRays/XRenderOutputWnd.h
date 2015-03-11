#pragma once

#include "XRenderRayTrace.h"
#include "XRayTraceScene.h"

class XRenderOutputWnd : public CFrameWnd
{
public:
	XRenderOutputWnd();
	~XRenderOutputWnd();
	void SetRenderParams(COLORREF BGColor, double Ambient, int Antialiasing);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	XRenderRayTrace Render;
	XRayTraceScene Scene;
};
