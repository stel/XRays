#pragma once

#include "XRays.h"
#include "XCamera.h"
#include "XZBuffer.h"
#include "XRenderFlat.h"
#include "XRenderWired.h"
#include "XPropertiesDlg.h"
#include "XMoveDlg.h"

enum XViewportProjection
{
	VP_FRONT = 0,
	VP_BACK,
	VP_TOP,
	VP_BOTTOM,
	VP_LEFT,
	VP_RIGHT,
	VP_PERSPECTIVE
};

enum XViewportCameraMode
{
	VPM_SELECT = 0,
	VPM_MOVECAMERA,
	VPM_ROTATECAMERA,
	VPM_SCALECAMERA,
	VPM_MOVEOBJECT,
	VPM_ROTATEOBJECT,
	VPM_SCALEOBJECT,
	VPM_ADDVERTEX,
	VPM_ADDFACE,
	VPM_DESABLED
};

enum XViewportSelectMode
{
	VPSM_OBJECT = 0,
	VPSM_FACE,
	VPSM_VERTEX
};

class XViewport : public CWnd
{
public:
	XViewport();
	~XViewport();

	void SetViewportMode(const XViewportCameraMode nMode);
	XViewportCameraMode GetViewportMode();
	void SetViewportSelectMode(const XViewportSelectMode nSelectMode);
	XViewportSelectMode GetViewportSelectMode();
	virtual BOOL Create(const CRect rcPane, CWnd* Container, UINT uID, int nIndex);
	void SetProjection(XViewportProjection nProjection);
	void Update();

	XCamera Camera;

protected:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnViewportMaximize();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnMenuProjection(UINT nID);
	void OnMenuRender(UINT nID);

	void OnMenuAddEditDelete(UINT nID);

	DECLARE_MESSAGE_MAP()

private:
	XPropertiesDlg PropertiesDlg;
	XMoveDlg MoveDlg;
	XFace FaceToAdd;
	int AddFaceStep;

	void UpdateBuffer();

	XViewportProjection Projection;

	XRender* Render;

	double OldCameraScale;
	XVector3d OldCameraPos, OldRotation;
	XArray<XVector3d> OldObjectsPositions, OldObjectsRotations;

	CString Title;
	XViewportCameraMode Mode;
	XViewportSelectMode SelectMode;
	CRect rcClient;
	CPoint MousePos;
	int Index;
	bool DrawAxises;

	CDC BufferDC;
	CBitmap Buffer;

	CMenu ViewportMenu;
};
