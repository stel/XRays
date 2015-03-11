#include "stdafx.h"
#include "XRenderOutputWnd.h"
#include "XRays.h"
#include "XTriangle.h"
#include "MainFrm.h"


BEGIN_MESSAGE_MAP(XRenderOutputWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

XRenderOutputWnd::XRenderOutputWnd()
{
}

XRenderOutputWnd::~XRenderOutputWnd()
{
}

int XRenderOutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	Scene.LoadFromXScene(&XRaysApp.Scene);

	Render.SetTargetRect(CRect(0, 0, 500, 500));
	XLight Light;

	Light.SetPosition(XVector3d(-100, 100, 100));
	Light.SetIntensity(0.2);
	Scene.Lights.Add(Light);

	XCamera Camera;
	Camera = ((CMainFrame*)XRaysApp.m_pMainWnd)->m_wndViewportsContainer.GetActiveViewport()->Camera;
	Scene.LookThroughCamera(&Camera);

	return CFrameWnd::OnCreate(lpCreateStruct);
}

void XRenderOutputWnd::OnPaint()
{
	CPaintDC dc(this);
	XCamera Camera;

	Camera.SetPosition(XVector3d(0, 0, 0));

	Render.SetTargetDC(&dc);

	Render.RenderScene(&Scene, &Camera);
}

void XRenderOutputWnd::SetRenderParams(COLORREF BGColor, double Ambient, int Antialiasing)
{
	Render.BackgroundColor(BGColor);
	Render.mAntiAliasDetail = Antialiasing;
}
