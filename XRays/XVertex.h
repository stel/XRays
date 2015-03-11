#pragma once
#include "XVector3d.h"

class XVertex
{
public:
	XVertex() {};
	XVertex(const XVector3d &nPos, int nNum)
	{
		Pos = nPos;
		Num = nNum;
		selected = false;
		marked = false;
	}

	XVertex& operator =(const XVertex &Vertex)
	{
		Pos = Vertex.Pos;
		Num = Vertex.Num;
		selected = Vertex.selected;
		marked = Vertex.marked;

		return *this;
	}

	bool operator ==(const XVertex &Vertex)
	{
		return (Num == Vertex.Num);
	}

	bool Near(const XVector3d Point) const
	{
		return (absd(Pos.X - Point.X) < 6 && absd(Pos.Y - Point.Y) < 6 && absd(Pos.Z - Point.Z) < 6);
	}

	XVector3d Pos;
	int Num;
	bool selected;
	bool marked;

private:
	double absd(double a) const
	{
		if (a < 0)
		{
			a = -a;
		}

		return a;
	}
};
