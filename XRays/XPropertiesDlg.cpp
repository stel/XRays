// XPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XRays.h"
#include "XPropertiesDlg.h"

IMPLEMENT_DYNAMIC(XPropertiesDlg, CDialog)

XPropertiesDlg::XPropertiesDlg(CWnd* pParent)
: CDialog(XPropertiesDlg::IDD, pParent)
{
}

XPropertiesDlg::~XPropertiesDlg()
{
}

void XPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	wchar_t str[20];
	CDialog::DoDataExchange(pDX);

	SetDlgItemInt(IDC_EDT_REFLECTIVITY, SelectedMesh->Reflectivity * 100, 0);

	_swprintf(str, L"%f", SelectedMesh->GetPosition().X);
	SetDlgItemText(IDC_EDIT_X, str);

	_swprintf(str, L"%f", SelectedMesh->GetPosition().Y);
	SetDlgItemText(IDC_EDIT_Y, str);

	_swprintf(str, L"%f", SelectedMesh->GetPosition().Z);
	SetDlgItemText(IDC_EDIT_Z, str);
}

BEGIN_MESSAGE_MAP(XPropertiesDlg, CDialog)
	ON_BN_CLICKED(IDC_COLORBTN, &XPropertiesDlg::OnBnClickedColorbtn)
	ON_BN_CLICKED(IDOK, &XPropertiesDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void XPropertiesDlg::OnBnClickedColorbtn()
{
	ColorDLG.SetCurrentColor(SelectedMesh->Color);
	ColorDLG.DoModal();
	SelectedMesh->Color = ColorDLG.GetColor();
}

void XPropertiesDlg::SetSelectedMesh(XMesh *Mesh)
{
	SelectedMesh = Mesh;
}

void XPropertiesDlg::OnBnClickedOk()
{
	wchar_t str[20];
	XVector3d Pos;
	int refl = GetDlgItemInt(IDC_EDT_REFLECTIVITY, 0, 0);
	SelectedMesh->Reflectivity = refl / 100.0;

	GetDlgItemText(IDC_EDIT_X, str, 20);
	Pos.X = _wtof(str);
	GetDlgItemText(IDC_EDIT_Y, str, 20);
	Pos.Y = _wtof(str);
	GetDlgItemText(IDC_EDIT_Z, str, 20);
	Pos.Z = _wtof(str);

	SelectedMesh->SetPosition(Pos);

	OnOK();
}
