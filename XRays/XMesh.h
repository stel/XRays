#pragma once

#include "XVector3d.h"
#include "XArray.h"
#include "XFace.h"
#include "XSceneObject.h"


class XMesh : public XSceneObject
{
public:
	XArray<XVertex> Vertices;
	XArray<XFace> Faces;
	COLORREF Color;
	double Reflectivity;
	bool selected;

	XMesh()
	{
		Color = 0x00FF0000;
		Reflectivity = 0;
		selected = false;
	};

	XMesh& operator= (const XMesh &Mesh)
	{
		Vertices = Mesh.Vertices;
		Faces = Mesh.Faces;

		for (int i = 0; i < Faces.Count(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Faces[i].Vertex[j] = &Vertices[Mesh.Faces[i].Vertex[j]->Num];
			}
		}

		Color = Mesh.Color;
		Reflectivity = Mesh.Reflectivity;
		CoordSystem = Mesh.CoordSystem;
		selected = Mesh.selected;
		return *this;
	};

	void AddVertex(const XVector3d &Vertex)
	{
		Vertices.Add(XVertex(Vertex, Vertices.Count()));
		UpdateFaces();
	}

	void DeleteVertex(int num)
	{
		bool deleted = false;

		for (int i = 0; i < Faces.Count(); i++)
		{
			deleted = false;

			for (int j = 0; j < 3; j++)
			{
				if (num == Faces[i].Vertex[j]->Num)
				{
					DeleteFace(i);
					deleted = true;
				}
			}

			if (deleted)
			{
				i--;
			}
		}

		for (int i = 0; i < Faces.Count(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (Faces[i].Vertex[j]->Num > num)
				{
					Faces[i].VertexNum[j]--;
				}
			}
		}

		UpdateFaces();

		for (int i = 0; i < Vertices.Count(); i++)
		{
			if (Vertices[i].Num > num)
			{
				Vertices[i].Num--;
			}
		}

		Vertices.Delete(num);
	}

	void AddFace(int v1_num, int v2_num, int v3_num)
	{
		Faces.Add(XFace(&Vertices[v1_num], &Vertices[v2_num], &Vertices[v3_num]));
	}

	void DeleteFace(int num)
	{
		Faces.Delete(num);
	}

private:
	void UpdateFaces()
	{
		for (int i = 0; i < Faces.Count(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Faces[i].Vertex[j] = &Vertices[Faces[i].VertexNum[j]];
			}
		}
	}
};
