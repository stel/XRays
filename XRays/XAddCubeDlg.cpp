// XAddCubeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XRays.h"
#include "XAddCubeDlg.h"
#include "XVector3d.h"

IMPLEMENT_DYNAMIC(XAddCubeDlg, CDialog)

XAddCubeDlg::XAddCubeDlg(CWnd* pParent /*=NULL*/)
: CDialog(XAddCubeDlg::IDD, pParent), ColorDLG(RGB(200, 35, 100), CC_FULLOPEN)
{
}

XAddCubeDlg::~XAddCubeDlg()
{
}

void XAddCubeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	SetDlgItemInt(IDC_EDIT_X, 0, 1);
	SetDlgItemInt(IDC_EDIT_Y, 0, 1);
	SetDlgItemInt(IDC_EDIT_Z, 0, 1);
	SetDlgItemInt(IDC_EDIT_SIZE, 100, 1);
}

BEGIN_MESSAGE_MAP(XAddCubeDlg, CDialog)
	ON_COMMAND_RANGE(ID_ADD, IDC_COLORBTN, OnAdd)
END_MESSAGE_MAP()

void XAddCubeDlg::OnAdd(UINT id)
{
	XVector3d Pos;
	float Size;
	wchar_t str[20];

	switch (id)
	{
	case ID_ADD:
		GetDlgItemText(IDC_EDIT_X, str, 20);
		Pos.X = _wtof(str);
		GetDlgItemText(IDC_EDIT_Y, str, 20);
		Pos.Y = _wtof(str);
		GetDlgItemText(IDC_EDIT_Z, str, 20);
		Pos.Z = _wtof(str);
		GetDlgItemText(IDC_EDIT_SIZE, str, 20);
		Size = _wtof(str);
		XRaysApp.Scene.AddCube(Pos, Size, CubeColor);
		EndDialog(ID_ADD);
		break;

	case IDC_COLORBTN:
		ColorDLG.DoModal();
		CubeColor = ColorDLG.GetColor();
		break;
	}
}
