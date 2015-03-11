// XRenderOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XRays.h"
#include "XRenderOptionsDlg.h"

IMPLEMENT_DYNAMIC(XRenderOptionsDlg, CDialog)

XRenderOptionsDlg::XRenderOptionsDlg(CWnd* pParent)
: CDialog(XRenderOptionsDlg::IDD, pParent), ColorDLG(RGB(0, 0, 0), CC_FULLOPEN)
{
}

XRenderOptionsDlg::~XRenderOptionsDlg()
{
}

void XRenderOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	SetDlgItemInt(IDC_EDT_ANTIALIASING, 1, 1);
}

BEGIN_MESSAGE_MAP(XRenderOptionsDlg, CDialog)
	ON_COMMAND_RANGE(IDC_BTN_BGCOLOR, IDC_BTN_RENDER, OnCommandRange)
END_MESSAGE_MAP()

void XRenderOptionsDlg::OnCommandRange(UINT id)
{
	double Ambient;
	int Antialiasing;

	switch (id)
	{
	case IDC_BTN_RENDER:
		Ambient = 0.5;
		Antialiasing = GetDlgItemInt(IDC_EDT_ANTIALIASING, 0, 1);
		BGColor = ColorDLG.GetColor();
		RenderOutputWnd = new XRenderOutputWnd;
		RenderOutputWnd->SetRenderParams(BGColor, Ambient, Antialiasing);
		RenderOutputWnd->Create(NULL, L"XRays Render Output Window");
		RenderOutputWnd->MoveWindow(0, 0, 500, 500);
		RenderOutputWnd->ShowWindow(SW_SHOW);
		EndDialog(ID_ADD);
		break;

	case IDC_BTN_BGCOLOR:
		ColorDLG.DoModal();
		BGColor = ColorDLG.GetColor();
		break;
	}
}
