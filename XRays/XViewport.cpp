#include "stdafx.h"
#include "XViewport.h"
#include "XViewportsContainer.h"
#include "Resource.h"

XViewport::XViewport()
{
	Mode = VPM_SELECT;
	SelectMode = VPSM_OBJECT;
	ViewportMenu.LoadMenu(IDR_VIEWPORT_MENU);
	Render = new XRenderFlat;
	DrawAxises = false;
	AddFaceStep = 0;
}

XViewport::~XViewport()
{
	ViewportMenu.DestroyMenu();
}

BEGIN_MESSAGE_MAP(XViewport, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_COMMAND_RANGE(ID_PROJECTION_FRONT, ID_PROJECTION_PERSPECTIVE, OnMenuProjection)
	ON_COMMAND_RANGE(ID_RENDER_WIRED, ID_RENDER_FLAT, OnMenuRender)
	ON_COMMAND_RANGE(ID_VIEWPORT_ADD, ID_VIEWPORT_EDIT, OnMenuAddEditDelete)
	ON_WM_KEYDOWN(OnKeyDown)
END_MESSAGE_MAP()

void XViewport::SetViewportMode(const XViewportCameraMode nMode)
{
	Mode = nMode;
}

XViewportCameraMode XViewport::GetViewportMode()
{
	return Mode;
}

void XViewport::SetViewportSelectMode(const XViewportSelectMode nSelectMode)
{
	SelectMode = nSelectMode;
}

XViewportSelectMode XViewport::GetViewportSelectMode()
{
	return SelectMode;
}

BOOL XViewport::Create(const CRect rcPane, CWnd* Container, UINT uID, int nIndex)
{
	Index = nIndex;
	rcClient = rcPane;
	if (!CreateEx(0,
		AfxRegisterWndClass(CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)::GetStockObject(BLACK_BRUSH),
		NULL),
		NULL,
		WS_CHILD | WS_VISIBLE,
		rcPane,
		Container,
		uID))
	{
		return false;
	}

	CPaintDC d(this);
	BufferDC.CreateCompatibleDC(this->GetDC());
	Buffer.CreateCompatibleBitmap(this->GetDC(), rcClient.right, rcClient.bottom);
	SelectObject(BufferDC, Buffer);
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(0x0000DDFF);

	return true;
}

void XViewport::SetProjection(XViewportProjection nProjection)
{
	CMenu *tracker;
	UINT ID;
	tracker = ViewportMenu.GetSubMenu(0);
	Projection = nProjection;

	switch (Projection)
	{
	case VP_FRONT:
		Camera.MakeFrontProjection();
		Title = "Front";
		ID = ID_PROJECTION_FRONT;
		break;

	case VP_BACK:
		Camera.MakeBackProjection();
		Title = "Back";
		ID = ID_PROJECTION_BACK;
		break;

	case VP_TOP:
		Camera.MakeTopProjection();
		Title = "Top";
		ID = ID_PROJECTION_TOP;
		break;

	case VP_BOTTOM:
		Camera.MakeBottomProjection();
		Title = "Bottom";
		ID = ID_PROJECTION_BOTTOM;
		break;

	case VP_LEFT:
		Camera.MakeLeftProjection();
		Title = "Left";
		ID = ID_PROJECTION_LEFT;
		break;

	case VP_RIGHT:
		Camera.MakeRightProjection();
		Title = "Right";
		ID = ID_PROJECTION_RIGHT;
		break;

	case VP_PERSPECTIVE:
		Camera.MakePerspectiveProjection();
		Title = "Perspective";
		ID = ID_PROJECTION_PERSPECTIVE;
		break;
	}

	tracker->CheckMenuRadioItem(ID_PROJECTION_FRONT, ID_PROJECTION_PERSPECTIVE, ID, MF_BYCOMMAND);

	Invalidate(false);
}

void XViewport::UpdateBuffer()
{
}

void XViewport::Update()
{
	Invalidate(false);
}

void XViewport::OnPaint()
{
	CPaintDC pDC(this);
	CPen Xpen(0, 1, 0x00FF0000);
	CPen Ypen(0, 1, 0x0000FF00);
	CPen Zpen(0, 1, 0x000000FF);
	CPen *Axis1, *Axis2;

	XCoordSystem cs;
	cs = Camera.CoordSystem;
	cs.SetOrigin(XVector3d(22, rcClient.bottom - 22, 0));

	BufferDC.FillSolidRect(rcClient, 0x00000000);

	Render->SetTargetDC(&BufferDC);
	Render->RenderScene(XRaysApp.Scene, &Camera);

	BufferDC.SelectObject(&Xpen);
	Render->DrawLine(cs.GetOrigin(), cs.TransformVector(XVector3d(20, 0, 0)) + cs.GetOrigin());
	BufferDC.SelectObject(&Ypen);
	Render->DrawLine(cs.GetOrigin(), cs.TransformVector(XVector3d(0, 20, 0)) + cs.GetOrigin());
	BufferDC.SelectObject(&Zpen);
	Render->DrawLine(cs.GetOrigin(), cs.TransformVector(XVector3d(0, 0, 20)) + cs.GetOrigin());

	Render->OutputText(Title, 0, 0);

	if (DrawAxises)
	{
		switch (Projection)
		{
		case VP_FRONT:
		case VP_BACK:
		case VP_PERSPECTIVE:
			Axis1 = &Xpen;
			Axis2 = &Ypen;
			break;

		case VP_TOP:
		case VP_BOTTOM:
			Axis1 = &Xpen;
			Axis2 = &Zpen;
			break;

		case VP_LEFT:
		case VP_RIGHT:
			Axis1 = &Zpen;
			Axis2 = &Ypen;
			break;
		}

		BufferDC.SelectObject(Axis1);
		Render->DrawLine(XVector3d(MousePos.x, MousePos.y, 0), XVector3d(MousePos.x + 100, MousePos.y, 0));
		BufferDC.SelectObject(Axis2);
		Render->DrawLine(XVector3d(MousePos.x, MousePos.y, 0), XVector3d(MousePos.x, MousePos.y + 100, 0));
	}

	pDC.BitBlt(0, 0, rcClient.right, rcClient.bottom, &BufferDC, 0, 0, SRCCOPY);
}

void XViewport::OnLButtonDown(UINT nFlags, CPoint point)
{
	XArray<XMesh*> SelectedMeshs;
	XArray<XVertex*> SelectedVertex;
	XArray<XFace*> SelectedFaces;
	XVector3d v;
	XMesh *Mesh;
	XFace *Face;

	bool found;
	bool multiselect;

	((XViewportsContainer*)GetParent())->SetActiveViewport(Index);

	SetCapture();
	MousePos = point;

	OldCameraPos = Camera.GetPosition();
	OldRotation = Camera.GetRotation();

	OldObjectsPositions.Clear();
	OldObjectsRotations.Clear();

	found = false;
	multiselect = (nFlags == 9);

	if (Mode == VPM_SELECT || Mode == VPM_MOVEOBJECT || Mode == VPM_ROTATEOBJECT)
	{
		for (int i = 0; i < XRaysApp.Scene.Meshs.Count() && !found; i++)
		{
			Mesh = &(XRaysApp.Scene.Meshs[i]);
			v = Camera.WCS(XVector3d(point.x, point.y, Render->GetZValue(point.x, point.y)));
			v -= Mesh->CoordSystem.GetOrigin();
			v = Mesh->CoordSystem.TransformVector(v);

			switch (SelectMode)
			{
			case VPSM_OBJECT:
				if (!multiselect)
				{
					XRaysApp.Scene.UnselectAll();
				}

				for (int j = 0; j < Mesh->Faces.Count(); j++)
				{
					Face = &Mesh->Faces[j];

					if (Face->isPointInside(v))
					{
						Mesh->selected = true;
						found = true;
					}
				}
				break;

			case VPSM_FACE:
				if (!multiselect)
				{
					XRaysApp.Scene.UnselectAllFaces(Mesh);
				}

				for (int j = 0; j < Mesh->Faces.Count(); j++)
				{
					Face = &Mesh->Faces[j];

					if (Face->isPointInside(v))
					{
						if (SelectMode == VPSM_FACE)
						{
							Face->selected = !Face->selected;
						}

						Mesh->selected = true;
						found = true;
					}
				}
				break;

			case VPSM_VERTEX:
				XRaysApp.Scene.UnselectAllFaces(Mesh);

				if (!multiselect)
				{
					XRaysApp.Scene.UnselectAllVertices(Mesh);
				}

				for (int j = 0; j < Mesh->Vertices.Count(); j++)
				{
					if (Mesh->Vertices[j].Near(v))
					{
						Mesh->Vertices[j].selected = !Mesh->Vertices[j].selected;
						Mesh->selected = true;

						found = true;
					}
				}
				break;
			}
		}
	}
	else if (Mode == VPM_ADDVERTEX)
	{
		SelectedMeshs = XRaysApp.Scene.GetSelectedMeshs();

		if (SelectedMeshs.Count()>0)
		{
			v = Camera.WCS(XVector3d(point.x, point.y, 100));
			v -= SelectedMeshs[0]->CoordSystem.GetOrigin();
			v = SelectedMeshs[0]->CoordSystem.TransformVector(v);
			SelectedMeshs[0]->AddVertex(v);
		}
	}
	else if (Mode == VPM_ADDFACE)
	{
		SelectedMeshs = XRaysApp.Scene.GetSelectedMeshs();

		if (SelectedMeshs.Count() > 0)
		{
			v = Camera.WCS(XVector3d(point.x, point.y, Render->GetZValue(point.x, point.y)));
			v -= SelectedMeshs[0]->CoordSystem.GetOrigin();
			v = SelectedMeshs[0]->CoordSystem.TransformVector(v);

			for (int j = 0; j < SelectedMeshs[0]->Vertices.Count(); j++)
			{
				if (SelectedMeshs[0]->Vertices[j].Near(v))
				{
					FaceToAdd.Vertex[AddFaceStep++] = &(SelectedMeshs[0]->Vertices[j]);
					SelectedMeshs[0]->Vertices[j].marked = true;

					if (AddFaceStep == 3)
					{
						AddFaceStep = 0;
						SelectedMeshs[0]->AddFace(FaceToAdd.Vertex[0]->Num, FaceToAdd.Vertex[1]->Num, FaceToAdd.Vertex[2]->Num);
						XRaysApp.Scene.UnMarkAllVertices();
					}
				}
			}
		}
	}

	DrawAxises = ((found || multiselect) && (Mode == VPM_MOVEOBJECT || Mode == VPM_ROTATEOBJECT));

	switch (SelectMode)
	{
	case VPSM_OBJECT:
		SelectedMeshs = XRaysApp.Scene.GetSelectedMeshs();

		for (int i = 0; i < SelectedMeshs.Count(); i++)
		{
			OldObjectsPositions.Add(SelectedMeshs[i]->GetPosition());
			OldObjectsRotations.Add(SelectedMeshs[i]->GetRotation());
		}
		break;

	case VPSM_FACE:
		SelectedFaces = XRaysApp.Scene.GetSelectedFace();

		for (int i = 0; i < SelectedFaces.Count(); i++)
		{
			OldObjectsPositions.Add(SelectedFaces[i]->Vertex[0]->Pos);
			OldObjectsPositions.Add(SelectedFaces[i]->Vertex[1]->Pos);
			OldObjectsPositions.Add(SelectedFaces[i]->Vertex[2]->Pos);
		}
		break;

	case VPSM_VERTEX:
		SelectedVertex = XRaysApp.Scene.GetSelectedVertex();

		for (int i = 0; i < SelectedVertex.Count(); i++)
		{
			OldObjectsPositions.Add(SelectedVertex[i]->Pos);
		}
		break;
	}

	((XViewportsContainer*)GetParent())->UpdateViewports();
}

void XViewport::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	DrawAxises = false;
	Invalidate(false);
}

void XViewport::OnMouseMove(UINT nFlags, CPoint point)
{
	XVector3d TransformMove, TransformRotate;
	XArray<XMesh*> SelectedMeshs;
	XArray<XVertex*> SelectedVertex;
	XArray<XFace*> SelectedFaces;

	if (GetCapture() != this)
	{
		return;
	}

	switch (Projection)
	{
	case VP_FRONT:
	case VP_BACK:
	case VP_PERSPECTIVE:
		TransformMove = XVector3d(point.x - MousePos.x, point.y - MousePos.y, 0);
		TransformRotate = XVector3d(point.y - MousePos.y, -point.x + MousePos.x, 0);
		break;

	case VP_TOP:
	case VP_BOTTOM:
		TransformMove = XVector3d(point.x - MousePos.x, 0, point.y - MousePos.y);
		TransformRotate = XVector3d(point.y - MousePos.y, 0, -point.x + MousePos.x);
		break;

	case VP_LEFT:
	case VP_RIGHT:
		TransformMove = XVector3d(0, point.y - MousePos.y, -point.x + MousePos.x);
		TransformRotate = XVector3d(0, -point.x + MousePos.x, -point.y + MousePos.y);
		break;
	}

	switch (Mode)
	{
	case VPM_MOVECAMERA:
		Camera.SetPosition(XVector3d(point.x - MousePos.x, point.y - MousePos.y, 0) + OldCameraPos);
		break;

	case VPM_ROTATECAMERA:
		Camera.SetRotation(-TransformRotate + OldRotation);
		break;

	case VPM_MOVEOBJECT:
		switch (SelectMode)
		{
		case VPSM_OBJECT:
			SelectedMeshs = XRaysApp.Scene.GetSelectedMeshs();

			for (int i = 0; i < SelectedMeshs.Count(); i++)
			{
				SelectedMeshs[i]->SetPosition(TransformMove + OldObjectsPositions[i]);
			}

			((XViewportsContainer*)GetParent())->UpdateViewports();
			break;

		case VPSM_FACE:
			SelectedFaces = XRaysApp.Scene.GetSelectedFace();

			for (int i = 0; i < SelectedFaces.Count(); i++)
			{
				SelectedFaces[i]->Vertex[0]->Pos = TransformMove + OldObjectsPositions[3 * i];
				SelectedFaces[i]->Vertex[1]->Pos = TransformMove + OldObjectsPositions[3 * i + 1];
				SelectedFaces[i]->Vertex[2]->Pos = TransformMove + OldObjectsPositions[3 * i + 2];
			}

			((XViewportsContainer*)GetParent())->UpdateViewports();
			break;

		case VPSM_VERTEX:
			SelectedVertex = XRaysApp.Scene.GetSelectedVertex();
			for (int i = 0; i < SelectedVertex.Count(); i++)
			{
				SelectedVertex[i]->Pos = TransformMove + OldObjectsPositions[i];
			}

			((XViewportsContainer*)GetParent())->UpdateViewports();
			break;
		}
		break;

	case VPM_ROTATEOBJECT:
		SelectedMeshs = XRaysApp.Scene.GetSelectedMeshs();

		for (int i = 0; i < SelectedMeshs.Count(); i++)
		{
			SelectedMeshs[i]->SetRotation(TransformRotate + OldObjectsRotations[i]);
		}

		((XViewportsContainer*)GetParent())->UpdateViewports();
		break;

	case VPM_SCALECAMERA:
		break;
	}

	Update();
}

BOOL XViewport::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return 1;
}

void XViewport::OnSize(UINT nType, int cx, int cy)
{
	GetClientRect(&rcClient);
	Camera.SetTarget(XVector3d(rcClient.right / 2, rcClient.bottom / 2, 0));
	Render->SetTargetRect(rcClient);

	if (BufferDC)
	{
		Buffer.DeleteObject();
		Buffer.CreateCompatibleBitmap(GetDC(), rcClient.right, rcClient.bottom);
		SelectObject(BufferDC, Buffer);
	}
}

void XViewport::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu* tracker;
	bool EditDeleteEnabled;
	MousePos = point;
	ScreenToClient(&MousePos);

	if (AddFaceStep)
	{
		AddFaceStep = 0;
		XRaysApp.Scene.UnMarkAllVertices();
		OnLButtonDown(0, MousePos);
		return;
	}

	OnLButtonDown(0, MousePos);

	tracker = ViewportMenu.GetSubMenu(0);
	switch (SelectMode)
	{
	case VPSM_OBJECT:
		tracker->EnableMenuItem(ID_VIEWPORT_ADD, MF_DISABLED);
		EditDeleteEnabled = XRaysApp.Scene.GetSelectedMeshs().Count() < 1;
		break;

	case VPSM_FACE:
		tracker->EnableMenuItem(ID_VIEWPORT_ADD, MF_ENABLED);
		EditDeleteEnabled = XRaysApp.Scene.GetSelectedFace().Count() < 1;
		break;

	case VPSM_VERTEX:
		tracker->EnableMenuItem(ID_VIEWPORT_ADD, MF_ENABLED);
		EditDeleteEnabled = XRaysApp.Scene.GetSelectedVertex().Count() < 1;
		break;
	}

	if (EditDeleteEnabled)
	{
		tracker->EnableMenuItem(ID_VIEWPORT_EDIT, MF_DISABLED);
		tracker->EnableMenuItem(ID_VIEWPORT_DELETE, MF_DISABLED);
	}
	else
	{
		tracker->EnableMenuItem(ID_VIEWPORT_EDIT, MF_ENABLED);
		tracker->EnableMenuItem(ID_VIEWPORT_DELETE, MF_ENABLED);
	}

	tracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, pWnd);
}

void XViewport::OnViewportMaximize()
{
	CMenu* tracker;
	tracker = ViewportMenu.GetSubMenu(0);
	tracker->CheckMenuItem(ID_VIEWPORT_MAXIMIZE, MF_CHECKED);
}

void XViewport::OnMenuProjection(UINT nID)
{
	switch (nID)
	{
	case ID_PROJECTION_FRONT: SetProjection(VP_FRONT); break;
	case ID_PROJECTION_BACK: SetProjection(VP_BACK); break;
	case ID_PROJECTION_TOP: SetProjection(VP_TOP); break;
	case ID_PROJECTION_BOTTOM: SetProjection(VP_BOTTOM); break;
	case ID_PROJECTION_LEFT: SetProjection(VP_LEFT); break;
	case ID_PROJECTION_RIGHT: SetProjection(VP_RIGHT); break;
	case ID_PROJECTION_PERSPECTIVE: SetProjection(VP_PERSPECTIVE); break;
	}
}

void XViewport::OnMenuRender(UINT nID)
{
	delete[] Render;
	switch (nID)
	{
	case ID_RENDER_WIRED: Render = new XRenderWired; break;
	case ID_RENDER_FLAT: Render = new XRenderFlat; break;
	}
	Render->SetTargetRect(rcClient);
	Invalidate(false);
}

void XViewport::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	XMesh *Mesh;

	if (nChar == VK_DELETE)
	{
		switch (SelectMode)
		{
		case VPSM_OBJECT:
			for (int i = 0; i < XRaysApp.Scene.Meshs.Count(); i++)
			{
				if (XRaysApp.Scene.Meshs[i].selected)
				{
					XRaysApp.Scene.Meshs.Delete(i);
				}

				((XViewportsContainer*)GetParent())->UpdateViewports();
			}
			break;

		case VPSM_FACE:
			for (int i = 0; i < XRaysApp.Scene.Meshs.Count(); i++)
			{
				Mesh = &XRaysApp.Scene.Meshs[i];

				for (int j = 0; j < Mesh->Faces.Count(); j++)
				{
					if (Mesh->Faces[j].selected)
					{
						Mesh->DeleteFace(j);
						j--;

						((XViewportsContainer*)GetParent())->UpdateViewports();
					}
				}
			}
			break;

		case VPSM_VERTEX:
			for (int i = 0; i < XRaysApp.Scene.Meshs.Count(); i++)
			{
				Mesh = &XRaysApp.Scene.Meshs[i];

				for (int j = 0; j < Mesh->Vertices.Count(); j++)
				{
					if (Mesh->Vertices[j].selected)
					{
						Mesh->DeleteVertex(j);
						j--;

						((XViewportsContainer*)GetParent())->UpdateViewports();
					}
				}
			}
			break;
		}
	}
	else if (nChar == VK_F1)
	{
		Mode = VPM_ADDFACE;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void XViewport::OnMenuAddEditDelete(UINT nID)
{
	XVector3d v;
	XArray<XMesh*> SelectedMeshs;
	XArray<XVertex*> SelectdeVertices;
	XVertex Vertex;

	switch (nID)
	{
	case ID_VIEWPORT_ADD:
		switch (SelectMode)
		{
		case VPSM_OBJECT:
			break;

		case VPSM_FACE:

			break;

		case VPSM_VERTEX:
			SelectedMeshs = XRaysApp.Scene.GetSelectedMeshs();

			if (SelectedMeshs.Count() > 0)
			{
				v = Camera.WCS(XVector3d(MousePos.x, MousePos.y, 100));
				v -= SelectedMeshs[0]->CoordSystem.GetOrigin();
				v = SelectedMeshs[0]->CoordSystem.TransformVector(v);
				SelectedMeshs[0]->AddVertex(v);

				((XViewportsContainer*)GetParent())->UpdateViewports();
			}
			break;
		}
		break;

	case ID_VIEWPORT_EDIT:
		switch (SelectMode)
		{
		case VPSM_OBJECT:
			SelectedMeshs = XRaysApp.Scene.GetSelectedMeshs();
			PropertiesDlg.SetSelectedMesh(SelectedMeshs[0]);
			PropertiesDlg.DoModal();
			break;

		case VPSM_VERTEX:
			SelectdeVertices = XRaysApp.Scene.GetSelectedVertex();
			Vertex = *(SelectdeVertices[0]);
			MoveDlg.SelectedVertex = &Vertex;
			MoveDlg.DoModal();
			SelectdeVertices[0]->Pos = Vertex.Pos;
			break;
		}

		((XViewportsContainer*)GetParent())->UpdateViewports();
		break;

	case ID_VIEWPORT_DELETE:
		OnKeyDown(VK_DELETE, 0, 0);
		break;
	}
}
