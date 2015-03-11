#include "stdafx.h"
#include "XRenderFlat.h"

XRenderFlat::XRenderFlat()
{
}

void XRenderFlat::SetTargetRect(CRect nRect)
{
	ZBuffer.Resize(nRect.Width(), nRect.Height());
	XRender::SetTargetRect(nRect);
}

void XRenderFlat::SetTargetDC(CDC *nDC)
{
	ZBuffer.Restore();
	XRender::SetTargetDC(nDC);
}

void XRenderFlat::DrawPixel(int x, int y, COLORREF Color)
{
	TargetDC->SetPixel(x, y, Color);
}

inline void XRenderFlat::DrawZPixel(int x, int y, int z, COLORREF Color)
{
	if (z < ZBuffer.GetZValue(x, y))
	{
		ZBuffer.SetValue(x, y, z);
		TargetDC->SetPixel(x, y, Color);
	}
}

void XRenderFlat::DrawPoint(const XVector3d &A, COLORREF Color)
{
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			DrawZPixel((int)A.X + dx, (int)A.Y + dy, (int)A.Z, Color);
		}
	}
}

void XRenderFlat::DrawLine(const XVector3d &A, const XVector3d &B)
{
	TargetDC->MoveTo(A.X, A.Y);
	TargetDC->LineTo(B.X, B.Y);
}

void XRenderFlat::DrawTriangle(XVector3d A, XVector3d B, XVector3d C, COLORREF Color)
{
	const XVector3d *v1, *v2, *v3;

	double tmpDiv;
	double longest;
	double height;
	int spanEnd;
	double leftdeltaxf;
	double rightdeltaxf;
	int leftx, rightx;
	double leftxf, rightxf;
	int span;
	int x;
	int ZBufferPos;

	double leftZValue, rightZValue;
	double leftZStep, rightZStep;
	double spanZValue, spanZStep;

	v1 = &A;
	v2 = &B;
	v3 = &C;

	if (v1->X > v2->X)	swapVertices(&v1, &v2);
	if (v1->X > v3->X)	swapVertices(&v1, &v3);
	if (v2->X > v3->X)	swapVertices(&v2, &v3);

	if ((v1->X - v3->X) == 0)
		return;

	if (v1->Y > v2->Y)	swapVertices(&v1, &v2);
	if (v1->Y > v3->Y)	swapVertices(&v1, &v3);
	if (v2->Y > v3->Y)	swapVertices(&v2, &v3);

	if (v1->Y > TargetRect.bottom || v3->Y < TargetRect.top)
		return;

	height = v3->Y - v1->Y;
	if (!height)
		return;

	longest = (v2->Y - v1->Y) / height * (v3->X - v1->X) + (v1->X - v2->X);

	spanEnd = (int)v2->Y;
	span = (int)v1->Y;
	leftxf = rightxf = v1->X;
	leftZValue = rightZValue = v1->Z;

	if (longest < 0.0)
	{
		tmpDiv = 1.0 / (v2->Y - v1->Y);
		rightdeltaxf = (v2->X - v1->X) * tmpDiv;
		rightZStep = (v2->Z - v1->Z) * tmpDiv;

		tmpDiv = 1.0 / height;
		leftdeltaxf = (v3->X - v1->X) * tmpDiv;
		leftZStep = (v3->Z - v1->Z) * tmpDiv;
	}
	else
	{
		tmpDiv = 1.0 / height;
		rightdeltaxf = (v3->X - v1->X) * tmpDiv;
		rightZStep = (v3->Z - v1->Z) * tmpDiv;

		tmpDiv = 1.0 / (v2->Y - v1->Y);
		leftdeltaxf = (v2->X - v1->X) * tmpDiv;
		leftZStep = (v2->Z - v1->Z) * tmpDiv;
	}

	for (int triangleHalf = 0; triangleHalf < 2; ++triangleHalf)
	{
		if (spanEnd > TargetRect.bottom)
			spanEnd = TargetRect.bottom;

		if (span < TargetRect.top)
		{
			if (spanEnd < TargetRect.top)
			{
				leftx = spanEnd - span;
				span = spanEnd;
			}
			else
			{
				leftx = TargetRect.top - span;
				span = TargetRect.top;
			}

			leftxf += leftdeltaxf*leftx;
			rightxf += rightdeltaxf*leftx;
			leftZValue += leftZStep*leftx;
			rightZValue += rightZStep*leftx;
		}

		ZBufferPos = span*TargetRect.right;

		while (span < spanEnd)
		{
			leftx = (int)(leftxf);
			rightx = (int)(rightxf + 0.5);

			if (leftx < TargetRect.left)
			{
				leftx = TargetRect.left;
			}
			else
			{
				if (leftx > TargetRect.right)
				{
					leftx = TargetRect.right;
				}
			}

			if (rightx < TargetRect.left)
			{
				rightx = TargetRect.left;
			}
			else
			{
				if (rightx > TargetRect.right)
				{
					rightx = TargetRect.right;
				}
			}

			if (rightx != leftx)
			{
				spanZValue = leftZValue;
				spanZStep = (rightZValue - leftZValue) / (rightx - leftx);

				x = leftx;

				while (x < rightx)
				{
					if (spanZValue < ZBuffer.GetZValue(ZBufferPos + x))
					{
						ZBuffer.SetValue(ZBufferPos + x, spanZValue);
						TargetDC->SetPixel(x, span, Color);
					}

					spanZValue += spanZStep;
					x++;
				}
			}

			leftxf += leftdeltaxf;
			rightxf += rightdeltaxf;
			span++;
			leftZValue += leftZStep;
			rightZValue += rightZStep;

			ZBufferPos += TargetRect.right;
		}

		if (triangleHalf > 0)
		{
			break;
		}


		if (longest < 0.0)
		{
			tmpDiv = 1.0 / (v3->Y - v2->Y);

			rightdeltaxf = (v3->X - v2->X) * tmpDiv;
			rightxf = v2->X;

			rightZValue = v2->Z;
			rightZStep = (v3->Z - v2->Z) * tmpDiv;
		}
		else
		{
			tmpDiv = 1.0 / (v3->Y - v2->Y);

			leftdeltaxf = (v3->X - v2->X) * tmpDiv;
			leftxf = v2->X;

			leftZValue = v2->Z;
			leftZStep = (v3->Z - v2->Z) * tmpDiv;
		}

		spanEnd = (int)v3->Y;
	}
}

void XRenderFlat::DrawMesh(XMesh &Mesh)
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
			else if (Mesh.Vertices[i].marked)
			{
				DrawPoint(Mesh.Vertices[i].Pos, 0x0000FF00);
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

		intensive = Face->GetNormal().DotProduct(XVector3d(0, 0, 1));

		R *= intensive;
		G *= intensive;
		B *= intensive;

		DrawTriangle(
			Face->Vertex[0]->Pos,
			Face->Vertex[1]->Pos,
			Face->Vertex[2]->Pos,
			RGB(R, G, B)
		);
	}
}

void XRenderFlat::RenderScene(const XScene& Scene, const XCamera* Camera)
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

void XRenderFlat::OutputText(const CString &Text, int x, int y) const
{
	TargetDC->TextOut(x, y, Text, Text.GetLength());
}
