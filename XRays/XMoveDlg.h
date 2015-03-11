#pragma once

#include "XMesh.h"

class XMoveDlg : public CDialog
{
	DECLARE_DYNAMIC(XMoveDlg)

public:
	XMoveDlg(CWnd* pParent = NULL);
	virtual ~XMoveDlg();
	void SetSelectedVertex(XVertex* Vertex);
	XVertex* SelectedVertex;

	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
};
