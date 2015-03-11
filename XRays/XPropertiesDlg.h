#pragma once

#include "XMesh.h"

class XPropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(XPropertiesDlg)

public:
	XPropertiesDlg(CWnd* pParent = NULL);
	virtual ~XPropertiesDlg();
	void SetSelectedMesh(XMesh *Mesh);

	enum { IDD = IDD_OBJECTPROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

private:
	XMesh *SelectedMesh;
	COLORREF CubeColor;
	CColorDialog ColorDLG;

public:
	afx_msg void OnBnClickedColorbtn();
	afx_msg void OnBnClickedOk();
};
