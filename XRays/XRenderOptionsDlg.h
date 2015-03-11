#pragma once

#include "XRenderOutputWnd.h"

class XRenderOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(XRenderOptionsDlg)

public:
	XRenderOptionsDlg(CWnd* pParent = NULL);
	virtual ~XRenderOptionsDlg();

	enum { IDD = IDD_RENDEROPTIONSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	void OnCommandRange(UINT id);

	DECLARE_MESSAGE_MAP()

private:
	COLORREF BGColor;
	CColorDialog ColorDLG;
	XRenderOutputWnd *RenderOutputWnd;
};
