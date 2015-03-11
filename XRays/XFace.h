#pragma once
#include "XVector3d.h"
#include "XVertex.h"
#include "math.h"

class XFace
{
public:
	XFace() { selected = false; };
	XFace(XVertex *v1, XVertex *v2, XVertex *v3)
	{
		Vertex[0] = v1;
		VertexNum[0] = v1->Num;
		Vertex[1] = v2;
		VertexNum[1] = v2->Num;
		Vertex[2] = v3;
		VertexNum[2] = v3->Num;

		UpdateNormal();
		
		selected = false;
	};

	XFace& operator =(const XFace &Face)
	{
		for (int i = 0; i < 3; i++)
		{
			Vertex[i] = Face.Vertex[i];
			VertexNum[i] = Face.VertexNum[i];
		}

		Normal = Face.Normal;
		selected = Face.selected;
		
		return *this;
	}

	bool isPointOnSamePlane(XVector3d &Point)
	{
		double m;
		UpdateNormal();
		m = Normal.DotProduct(Point - Vertex[0]->Pos) / XVector3d(Point - Vertex[0]->Pos).Length();
		return (m<0.05 && m >-0.05);
	}

	bool isPointInside(XVector3d &Point)
	{
		if (!isPointOnSamePlane(Point))
		{
			return false;
		}

		const double MATCH_FACTOR = 0.99;
		double Angle = 0.0;
		XVector3d vA, vB;

		for (int i = 0; i < 3; i++)
		{
			vA = Vertex[i]->Pos - Point;
			vB = Vertex[(i + 1) % 3]->Pos - Point;
			Angle += acos(vA.DotProduct(vB) / (vA.Length()*vB.Length()));  
		}

		if (Angle >= (MATCH_FACTOR * (2.0 * 3.1415926535897932384626433832795)))
		{
			return true;
		}
		
		return false;
	}

	void UpdateNormal()
	{
		XVector3d pointb_minus_pointa(Vertex[0]->Pos, Vertex[1]->Pos);
		XVector3d pointb_minus_pointc(Vertex[0]->Pos, Vertex[2]->Pos);
		Normal = pointb_minus_pointa.CrossProduct(pointb_minus_pointc).GetNormalized();
		if (Normal.Z < 0) Normal = -Normal;// SwapNormal();
	}

	void SwapNormal()
	{
		Normal.Z = -Normal.Z;
	}

	XVector3d GetNormal()
	{
		return Normal;
	}

	XVertex *Vertex[3];
	int VertexNum[3];
	bool selected;

private:
	XVector3d Normal;
};
