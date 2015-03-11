#include "stdafx.h"
#include "XRenderWired.h"

void XRenderWired::SetTargetRect(CRect nRect)
{
	ZBuffer.Resize(nRect.Width(), nRect.Height());
	XRender::SetTargetRect(nRect);
}

void XRenderWired::SetTargetDC(CDC *nDC)
{
	ZBuffer.Restore();
	XRender::SetTargetDC(nDC);
}

void XRenderWired::DrawPixel(int x, int y, COLORREF Color)
{
	TargetDC->SetPixel(x, y, Color);
}

void XRenderWired::DrawZPixel(int x, int y, int z, COLORREF Color)
{
	if (z < ZBuffer.GetZValue(x, y))
	{
		ZBuffer.SetValue(x, y, z);
		TargetDC->SetPixel(x, y, Color);
	}
}


void XRenderWired::DrawPoint(const XVector3d &A, COLORREF Color)
{
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			DrawZPixel((int)A.X + dx, (int)A.Y + dy, (int)A.Z, Color);
		}
	}
}

void XRenderWired::DrawLine(const XVector3d &A, const XVector3d &B)
{
	TargetDC->MoveTo(A.X, A.Y);
	TargetDC->LineTo(B.X, B.Y);
}

void XRenderWired::DrawZLine(XVector3d v1, XVector3d v2, COLORREF Color)
{
	int i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc,
		err_1, err_2, dx2, dy2, dz2;
	int pixel[3];

	pixel[0] = v1.X;
	pixel[1] = v1.Y;
	pixel[2] = v1.Z;

	dx = v2.X - v1.X;
	dy = v2.Y - v1.Y;
	dz = v2.Z - v1.Z;
	
	x_inc = (dx < 0) ? -1 : 1;
	l = abs(dx);
	y_inc = (dy < 0) ? -1 : 1;
	m = abs(dy);
	z_inc = (dz < 0) ? -1 : 1;
	n = abs(dz);
	
	dx2 = l << 1;
	dy2 = m << 1;
	dz2 = n << 1;

	if ((l >= m) && (l >= n))
	{
		err_1 = dy2 - l;
		err_2 = dz2 - l;

		for (i = 0; i < l; i++)
		{
			DrawZPixel(pixel[0], pixel[1], pixel[2], Color);

			if (err_1 > 0) {
				pixel[1] += y_inc;
				err_1 -= dx2;
			}

			if (err_2 > 0) {
				pixel[2] += z_inc;
				err_2 -= dx2;
			}

			err_1 += dy2;
			err_2 += dz2;
			pixel[0] += x_inc;
		}
	}
	else if ((m >= l) && (m >= n))
	{
		err_1 = dx2 - m;
		err_2 = dz2 - m;

		for (i = 0; i < m; i++)
		{
			DrawZPixel(pixel[0], pixel[1], pixel[2], Color);

			if (err_1 > 0)
			{
				pixel[0] += x_inc;
				err_1 -= dy2;
			}

			if (err_2 > 0)
			{
				pixel[2] += z_inc;
				err_2 -= dy2;
			}

			err_1 += dx2;
			err_2 += dz2;
			pixel[1] += y_inc;
		}
	}
	else
	{
		err_1 = dy2 - n;
		err_2 = dx2 - n;

		for (i = 0; i < n; i++)
		{
			DrawZPixel(pixel[0], pixel[1], pixel[2], Color);

			if (err_1 > 0)
			{
				pixel[1] += y_inc;
				err_1 -= dz2;
			}

			if (err_2 > 0)
			{
				pixel[0] += x_inc;
				err_2 -= dz2;
			}

			err_1 += dy2;
			err_2 += dx2;
			pixel[2] += z_inc;
		}
	}

	DrawZPixel(pixel[0], pixel[1], pixel[2], Color);
}

void XRenderWired::DrawTriangle(XVector3d A, XVector3d B, XVector3d C, COLORREF Color)
{
	DrawZLine(A, B, Color);
	DrawZLine(B, C, Color);
	DrawZLine(C, A, Color);
}

void XRenderWired::DrawMesh(XMesh &Mesh)
{
	double intensive;
	int R, G, B;
	XFace *Face;

	if (Mesh.selected)
	{
		for (int i = 0; i < Mesh.Vertices.Count(); i++)
		{
			if (Mesh.Vertices[i].selected)
			{
				DrawPoint(Mesh.Vertices[i].Pos, 0x000000FF);
			}
			else
			{
				DrawPoint(Mesh.Vertices[i].Pos, 0x00FF0000);
			}
		}
	}

	for (int j = 0; j < Mesh.Faces.Count(); j++)
	{
		R = GetRValue(Mesh.Color);
		G = GetGValue(Mesh.Color);
		B = GetBValue(Mesh.Color);
		Face = &Mesh.Faces[j];

		if (Face->selected)
		{
			R = 255;
			G = 0;
			B = 0;
		}

		Face->UpdateNormal();

		DrawTriangle(
			Face->Vertex[0]->Pos,
			Face->Vertex[1]->Pos,
			Face->Vertex[2]->Pos,
			RGB(R, G, B)
		);
	}
}

void XRenderWired::RenderScene(const XScene& Scene, const XCamera* Camera)
{
	XMesh Mesh;
	XCoordSystem CS;

	for (int i = 0; i < Scene.Meshs.Count(); i++)
	{
		Mesh = (Scene.Meshs[i]);
		CS = Scene.Meshs[i].CoordSystem;
		CS = CS.Back();

		for (int j = 0; j < Mesh.Vertices.Count(); j++)
		{
			Mesh.Vertices[j].Pos = CS.TransformVector(Mesh.Vertices[j].Pos);
			Mesh.Vertices[j].Pos += CS.GetOrigin();
			Mesh.Vertices[j].Pos = Camera->CCS(Mesh.Vertices[j].Pos);
		}

		DrawMesh(Mesh);
	}
}

void XRenderWired::OutputText(const CString &Text, int x, int y) const
{
	TargetDC->TextOut(x, y, Text, Text.GetLength());
}
