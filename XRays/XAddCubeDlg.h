#pragma once

#include "XRays.h"

class XAddCubeDlg : public CDialog
{
	DECLARE_DYNAMIC(XAddCubeDlg)

public:
	XAddCubeDlg(CWnd* pParent = NULL);  
	virtual ~XAddCubeDlg();

	enum { IDD = IDD_ADDCUBE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnAdd(UINT id);
	DECLARE_MESSAGE_MAP()

private:
	COLORREF CubeColor;
	CColorDialog ColorDLG;
};
