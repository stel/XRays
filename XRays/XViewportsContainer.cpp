#include "stdafx.h"
#include "XViewportsContainer.h"

#define FULL_SIZE 32768

BEGIN_MESSAGE_MAP(XViewportsContainer, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_NCCREATE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CREATE()
END_MESSAGE_MAP()

XViewportsContainer::XViewportsContainer()
{
	m_nPanes = 4;
	m_nMinSize = 30;
	TrackerThickness = 3;
	int total = 0;

	Viewport = new XViewport*[m_nPanes];
	::ZeroMemory(Viewport, m_nPanes * sizeof(XViewport*));

	fMoveHorizontal = false;
	fMoveVertical = false;
}

XViewportsContainer::~XViewportsContainer()
{
	delete[] Viewport;
}

BOOL XViewportsContainer::Create(CWnd* pParent, UINT nID)
{
	HCURSOR crsResize = NULL;
	CRect rcOuter;

	ASSERT(pParent);
	pParent->GetClientRect(&rcOuter);

	if (!CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, crsResize, 0, NULL),
		NULL, WS_CHILD | WS_VISIBLE, rcOuter, pParent, nID, NULL))
	{
		return false;
	}

	GetClientRect(&rcClient);
	HPos = rcClient.right / 2;
	VPos = rcClient.bottom / 2;

	return true;
}

BOOL XViewportsContainer::CreateViewport(int nIndex, XViewport* pViewport)
{
	CRect rcViewport;

	ASSERT((nIndex >= 0) && (nIndex < m_nPanes));
	Viewport[nIndex] = pViewport;
	GetViewportRect(nIndex, rcViewport);

	return pViewport->Create(rcViewport, this, AFX_IDW_PANE_FIRST + nIndex, nIndex);
}

XViewport* XViewportsContainer::GetViewport(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < m_nPanes));

	return Viewport[nIndex];
}

void XViewportsContainer::SetActiveViewport(int nIndex)
{
	if (ActiveViewport == nIndex)
	{
		return;
	}

	ActiveViewport = nIndex;
	Viewport[ActiveViewport]->SetFocus();
	Invalidate(false);
}

XViewport* XViewportsContainer::GetActiveViewport() const
{
	return Viewport[ActiveViewport];
}

int XViewportsContainer::GetActiveViewportNum() const
{
	return ActiveViewport;
}

void XViewportsContainer::GetViewportRect(int nIndex, CRect& rcViewport) const
{
	int i = (2 & nIndex) > 0,
		j = 1 & nIndex;

	rcViewport.left = (HPos + TrackerThickness)*j;
	rcViewport.top = (VPos + TrackerThickness)*i;
	rcViewport.right = rcViewport.left + (rcClient.right - TrackerThickness)*j + (1 - 2 * j)*HPos;
	rcViewport.bottom = rcViewport.top + (rcClient.bottom - TrackerThickness)*i + (1 - 2 * i)*VPos;
}

void XViewportsContainer::ResizePanes()
{
	int n = 0;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (Viewport[n])
			{
				Viewport[n++]->MoveWindow((HPos + TrackerThickness)*j + 2, (VPos + TrackerThickness)*i + 2,
					(rcClient.Width() - TrackerThickness)*j + (1 - 2 * j)*HPos - 4,
					(rcClient.Height() - TrackerThickness)*i + (1 - 2 * i)*VPos - 4, false);
			}
			else
			{
				return;
			}
		}
	}
}

void XViewportsContainer::InvertTracker()
{
	CDC* pDC = GetDC();
	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush;

	hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);

	pDC->PatBlt(HPos, 0, TrackerThickness, rcClient.bottom, PATINVERT);
	pDC->PatBlt(0, VPos, rcClient.right, TrackerThickness, PATINVERT);

	if (hOldBrush != NULL)
		SelectObject(pDC->m_hDC, hOldBrush);
	ReleaseDC(pDC);
}

void XViewportsContainer::OnPaint()
{
	CPaintDC dc(this);
	CDC t_dc;
	CBitmap t_b;
	CRect rcActiveViewport;
	COLORREF clrBar = ::GetSysColor(COLOR_BTNFACE);

	t_dc.CreateCompatibleDC(&dc);
	t_b.CreateCompatibleBitmap(&dc, rcClient.right, rcClient.bottom);
	SelectObject(t_dc, t_b);

	GetViewportRect(ActiveViewport, rcActiveViewport);
	t_dc.FillSolidRect(rcActiveViewport, 0x0000DDFF);
	t_dc.FillSolidRect(rcActiveViewport.left + 2, rcActiveViewport.top + 2,
		rcActiveViewport.right - rcActiveViewport.left - 4, rcActiveViewport.bottom - rcActiveViewport.top - 4, 0x00000000);
	t_dc.FillSolidRect(HPos, 0, TrackerThickness, rcClient.bottom, clrBar);
	t_dc.FillSolidRect(0, VPos, rcClient.right, TrackerThickness, clrBar);

	dc.BitBlt(0, 0, rcClient.right, rcClient.bottom, &t_dc, 0, 0, SRCCOPY);
	t_dc.DeleteDC();
	t_b.DeleteObject();
}

void XViewportsContainer::OnSize(UINT nType, int cx, int cy)
{
	HPos += (cx - rcClient.right) / 2;
	VPos += (cy - rcClient.bottom) / 2;
	GetClientRect(&rcClient);

	ResizePanes();
}

void XViewportsContainer::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x >= HPos - 10 && point.x <= HPos + TrackerThickness + 10)
	{
		fMoveHorizontal = true;
	}

	if (point.y >= VPos - 10 && point.y <= VPos + TrackerThickness + 10)
	{
		fMoveVertical = true;
	}

	if (fMoveHorizontal || fMoveVertical)
	{
		SetCapture();
		InvertTracker();
	}
}

void XViewportsContainer::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (GetCapture() != this)
	{
		return;
	}

	fMoveHorizontal = false;
	fMoveVertical = false;

	InvertTracker();
	ReleaseCapture();
	ResizePanes();
	Invalidate(false);
}

void XViewportsContainer::OnMouseMove(UINT nFlags, CPoint point)
{
	bool VTrackerHit, HTrackerHit;

	HTrackerHit = (point.x >= HPos - 10 && point.x <= HPos + TrackerThickness + 10);
	VTrackerHit = (point.y >= VPos - 10 && point.y <= VPos + TrackerThickness + 10);

	if (HTrackerHit && VTrackerHit)
	{
		SetCursor(LoadCursor(0, IDC_SIZEALL));
	}
	else
	{
		if (HTrackerHit)
		{
			SetCursor(LoadCursor(0, IDC_SIZEWE));
		}
		else
		{
			SetCursor(LoadCursor(0, IDC_SIZENS));
		}
	}

	if (GetCapture() != this)
	{
		return;
	}

	InvertTracker();

	if (fMoveHorizontal)
	{
		HPos = point.x;
	}

	if (fMoveVertical)
	{
		VPos = point.y;
	}

	InvertTracker();
}

BOOL XViewportsContainer::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CWnd::OnNcCreate(lpCreateStruct))
	{
		return FALSE;
	}

	CWnd* pParent = GetParent();
	ASSERT_VALID(pParent);
	pParent->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);
	return TRUE;
}

void XViewportsContainer::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	lpwndpos->flags |= SWP_NOCOPYBITS;
	CWnd::OnWindowPosChanging(lpwndpos);
}

void XViewportsContainer::UpdateViewports()
{
	for (int i = 0; i < 4; i++)
	{
		Viewport[i]->Update();
	}
}
