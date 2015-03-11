// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "XRays.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_NEWSCENE, OnNewScene)
	ON_COMMAND(ID_ADDCUBE, OnAddCube)
	ON_COMMAND(ID_ADDFACE, OnAddFace)
	ON_COMMAND(ID_RENDER, OnRender)
	ON_COMMAND_RANGE(ID_VPM_SELECT, ID_VPM_ROTATECAMERA, OnViewportControlCommand)
	ON_UPDATE_COMMAND_UI(ID_ADDFACE, OnUpdateAddFaceModeBtn)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VPM_SELECTOBJECT, ID_VPM_SELECTVERTEX, OnUpdateSelectObjectModeBtn)

	ON_UPDATE_COMMAND_UI(ID_VPM_SELECT, OnUpdateSelectModeBtn)
	ON_UPDATE_COMMAND_UI(ID_VPM_MOVEOBJECT, OnUpdateMoveModeBtn)
	ON_UPDATE_COMMAND_UI(ID_VPM_ROTATEOBJECT, OnUpdateRotateModeBtn)
	ON_UPDATE_COMMAND_UI(ID_VPM_MOVECAMERA, OnUpdateMoveCameraModeBtn)
	ON_UPDATE_COMMAND_UI(ID_VPM_ROTATECAMERA, OnUpdateRotateCameraModeBtn)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_wndViewportsContainer.Create(this);
	
	for (int i = 0; i < 4; i++)
	{
		m_wndViewportsContainer.CreateViewport(i, &Viewport[i]);
	}

	Viewport[0].SetProjection(VP_TOP);
	Viewport[1].SetProjection(VP_FRONT);
	Viewport[2].SetProjection(VP_LEFT);
	Viewport[3].SetProjection(VP_PERSPECTIVE);
	m_wndViewportsContainer.SetActiveViewport(3);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(ID_MAINTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.LoadTrueColorToolBar(18, IDB_MAINTOOLBAR, IDB_MAINTOOLBAR_HOT, IDB_MAINTOOLBAR_GRAY);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}

	if (!ViewportControlToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM
		| CBRS_TOOLTIPS) ||
		!ViewportControlToolBar.LoadToolBar(IDR_VIEWPORTCONTROLTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	ViewportControlToolBar.LoadTrueColorToolBar(16, IDB_VIEWPORTCONTROLTOOLBAR);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
	{
		return FALSE;
	}

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.dwExStyle &= ~CS_VREDRAW;
	cs.dwExStyle &= ~CS_HREDRAW;

	cs.lpszClass = AfxRegisterWndClass(0, 0, 0, AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	
	return TRUE;
}

#ifdef _DEBUG

void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	m_wndViewportsContainer.GetActiveViewport()->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnNewScene()
{
	XRaysApp.Scene.Meshs.Clear();
	m_wndViewportsContainer.UpdateViewports();
}

void CMainFrame::OnAddCube()
{
	XAddCubeDlg AddCubeDlg;
	AddCubeDlg.DoModal();
	m_wndViewportsContainer.UpdateViewports();
}

void CMainFrame::OnAddFace()
{
	for (int i = 0; i < 4; i++)
	{
		Viewport[i].SetViewportMode(VPM_ADDFACE);
	}
}

void CMainFrame::OnViewportControlCommand(UINT nID)
{
	for (int i = 0; i < 4; i++)
	{
		switch (nID)
		{
		case ID_VPM_SELECT:
			Viewport[i].SetViewportMode(VPM_SELECT);
			break;

		case ID_VPM_MOVEOBJECT:
			Viewport[i].SetViewportMode(VPM_MOVEOBJECT);
			break;

		case ID_VPM_ROTATEOBJECT:
			Viewport[i].SetViewportMode(VPM_ROTATEOBJECT);
			break;

		case ID_VPM_MOVECAMERA:
			Viewport[i].SetViewportMode(VPM_MOVECAMERA);
			break;

		case ID_VPM_ROTATECAMERA:
			Viewport[i].SetViewportMode(VPM_ROTATECAMERA);
			break;

		case ID_VPM_SELECTOBJECT:
			Viewport[i].SetViewportSelectMode(VPSM_OBJECT);
			Viewport[i].SetViewportMode(VPM_SELECT);
			break;

		case ID_VPM_SELECTFASE:
			Viewport[i].SetViewportSelectMode(VPSM_FACE);
			Viewport[i].SetViewportMode(VPM_SELECT);
			break;

		case ID_VPM_SELECTVERTEX:
			Viewport[i].SetViewportSelectMode(VPSM_VERTEX);
			Viewport[i].SetViewportMode(VPM_SELECT);
			break;
		}
	}
}

void CMainFrame::OnRender()
{
	XRenderOptionsDlg RenderOptionsDlg;

	RenderOptionsDlg.DoModal();
}

void CMainFrame::OnUpdateAddFaceModeBtn(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(Viewport[0].GetViewportMode() == VPM_ADDFACE);
}

void CMainFrame::OnUpdateSelectObjectModeBtn(CCmdUI* pCmdUI)
{
	switch (pCmdUI->m_nID)
	{
	case ID_VPM_SELECTOBJECT:
		pCmdUI->SetRadio(Viewport[0].GetViewportSelectMode() == VPSM_OBJECT);
		break;

	case ID_VPM_SELECTFASE:
		pCmdUI->SetRadio(Viewport[0].GetViewportSelectMode() == VPSM_FACE);
		break;

	case ID_VPM_SELECTVERTEX:
		pCmdUI->SetRadio(Viewport[0].GetViewportSelectMode() == VPSM_VERTEX);
		break;
	}
}

void CMainFrame::OnUpdateSelectModeBtn(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Viewport[0].GetViewportMode() == VPM_SELECT);
}

void CMainFrame::OnUpdateMoveModeBtn(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Viewport[0].GetViewportMode() == VPM_MOVEOBJECT);
}

void CMainFrame::OnUpdateRotateModeBtn(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Viewport[0].GetViewportMode() == VPM_ROTATEOBJECT);
	pCmdUI->Enable(Viewport[0].GetViewportSelectMode() == VPSM_OBJECT);
}

void CMainFrame::OnUpdateMoveCameraModeBtn(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Viewport[0].GetViewportMode() == VPM_MOVECAMERA);
}

void CMainFrame::OnUpdateRotateCameraModeBtn(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Viewport[0].GetViewportMode() == VPM_ROTATECAMERA);
}
