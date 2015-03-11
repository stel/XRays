#pragma once

#include "XViewport.h"
#include "XScene.h"

#define SSP_HORZ		1
#define SSP_VERT		2
#define VPC				4

class XViewportsContainer : public CWnd
{
public:
	XViewportsContainer();
	~XViewportsContainer();

	BOOL Create(CWnd* pParent, UINT nID = AFX_IDW_PANE_FIRST);
	BOOL CreateViewport(int nIndex, XViewport* pViewport);

	XViewport* GetViewport(int nIndex) const;
	virtual void SetActiveViewport(int nIndex);
	XViewport* GetActiveViewport() const;
	int GetActiveViewportNum() const;
	void GetViewportRect(int nIndex, CRect& rcViewport) const;

	void UpdateViewports();

protected:
	void ResizePanes();
	void InvertTracker();

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);

	DECLARE_MESSAGE_MAP()

private:
	CRect rcClient;
	int m_nPanes;
	int m_nMinSize, TrackerThickness;
	XViewport** Viewport;
	int ActiveViewport;
	int VPos, HPos;
	bool fMoveHorizontal;
	bool fMoveVertical;
};
