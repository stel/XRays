// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "XViewport.h"
#include "XViewportsContainer.h"
#include "XAddCubeDlg.h"
#include "XRenderOutputWnd.h"
#include "XRenderOptionsDlg.h"

#define TOOLBAR_DRAW_BUTTON_WIDTH 24
#include "TrueColorToolBar.h"

class CMainFrame : public CFrameWnd
{

public:
	CMainFrame();
protected:
	DECLARE_DYNAMIC(CMainFrame)

	// Attributes
public:

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CTrueColorToolBar m_wndToolBar, ViewportControlToolBar;
	XViewport	Viewport[4];
	XViewportsContainer m_wndViewportsContainer;


	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnNewScene();
	afx_msg void OnAddCube();
	afx_msg void OnAddFace();
	afx_msg void OnRender();
	afx_msg void OnViewportControlCommand(UINT nID);

	afx_msg void OnUpdateAddFaceModeBtn(CCmdUI* pCmdUI);

	afx_msg void OnUpdateSelectObjectModeBtn(CCmdUI* pCmdUI);

	afx_msg void OnUpdateSelectModeBtn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveModeBtn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRotateModeBtn(CCmdUI* pCmdUI);

	afx_msg void OnUpdateMoveCameraModeBtn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRotateCameraModeBtn(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};


