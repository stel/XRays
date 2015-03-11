// XMoveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XRays.h"
#include "XMoveDlg.h"

IMPLEMENT_DYNAMIC(XMoveDlg, CDialog)

XMoveDlg::XMoveDlg(CWnd* pParent)
: CDialog(XMoveDlg::IDD, pParent)
{
}

XMoveDlg::~XMoveDlg()
{
}

void XMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	wchar_t str[20];
	CDialog::DoDataExchange(pDX);

	_swprintf(str, L"%f", SelectedVertex->Pos.X);
	SetDlgItemText(IDC_EDIT_X, str);
	_swprintf(str, L"%f", SelectedVertex->Pos.Y);
	SetDlgItemText(IDC_EDIT_Y, str);
	_swprintf(str, L"%f", SelectedVertex->Pos.Z);
	SetDlgItemText(IDC_EDIT_Z, str);
}

BEGIN_MESSAGE_MAP(XMoveDlg, CDialog)
	ON_BN_CLICKED(IDOK, &XMoveDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void XMoveDlg::OnBnClickedOk()
{
	wchar_t str[20];
	XVector3d Pos;

	GetDlgItemText(IDC_EDIT_X, str, 20);
	Pos.X = _wtof(str);
	GetDlgItemText(IDC_EDIT_Y, str, 20);
	Pos.Y = _wtof(str);
	GetDlgItemText(IDC_EDIT_Z, str, 20);
	Pos.Z = _wtof(str);

	SelectedVertex->Pos = Pos;
	OnOK();
}
