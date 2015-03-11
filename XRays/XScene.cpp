#include "stdafx.h"
#include "XScene.h"

XScene::XScene()
{
}

void XScene::AddMesh(const XMesh &Mesh)
{
	Meshs.Add(Mesh);
}

void XScene::UnselectAll()
{
	XMesh *Mesh;

	for (int i = 0; i < Meshs.Count(); i++)
	{
		Mesh = &(Meshs[i]);
		Mesh->selected = false;

		for (int j = 0; j < Mesh->Faces.Count(); j++)
		{
			Mesh->Faces[j].selected = false;
		}

		for (int j = 0; j < Mesh->Vertices.Count(); j++)
		{
			Mesh->Vertices[j].selected = false;
		}
	}
}

void XScene::UnselectAllFaces(XMesh *Mesh)
{
	for (int j = 0; j < Mesh->Faces.Count(); j++)
	{
		Mesh->Faces[j].selected = false;
	}
}

void XScene::UnselectAllVertices(XMesh *Mesh)
{
	for (int j = 0; j < Mesh->Vertices.Count(); j++)
	{
		Mesh->Vertices[j].selected = false;
	}
}

void XScene::UnMarkAllVertices()
{
	for (int i = 0; i < Meshs.Count(); i++)
	{
		for (int j = 0; j < Meshs[i].Vertices.Count(); j++)
		{
			Meshs[i].Vertices[j].marked = false;
		}
	}
}

void XScene::AddCube(const XVector3d &Pos, const float a, COLORREF Color)
{
	XMesh Cube;

	Cube.AddVertex(Pos);
	Cube.AddVertex(XVector3d(Pos.X + a, Pos.Y, Pos.Z));
	Cube.AddVertex(XVector3d(Pos.X + a, Pos.Y + a, Pos.Z));
	Cube.AddVertex(XVector3d(Pos.X, Pos.Y + a, Pos.Z));

	Cube.AddVertex(XVector3d(Pos.X, Pos.Y, Pos.Z + a));
	Cube.AddVertex(XVector3d(Pos.X + a, Pos.Y, Pos.Z + a));
	Cube.AddVertex(XVector3d(Pos.X + a, Pos.Y + a, Pos.Z + a));
	Cube.AddVertex(XVector3d(Pos.X, Pos.Y + a, Pos.Z + a));

	Cube.AddFace(0, 1, 2);
	Cube.AddFace(0, 2, 3);
	Cube.AddFace(0, 1, 4);
	Cube.AddFace(1, 4, 5);
	Cube.AddFace(1, 2, 5);
	Cube.AddFace(2, 6, 5);
	Cube.AddFace(7, 6, 5);
	Cube.AddFace(4, 5, 7);
	Cube.AddFace(0, 3, 7);
	Cube.AddFace(0, 4, 7);
	Cube.AddFace(7, 3, 6);
	Cube.AddFace(6, 3, 2);

	Cube.Color = Color;

	AddMesh(Cube);
}

XArray<XMesh*> XScene::GetSelectedMeshs()
{
	XArray<XMesh*> SelectedMeshs;

	for (int i = 0; i < Meshs.Count(); i++)
	{
		if (Meshs[i].selected)
		{
			SelectedMeshs.Add(&Meshs[i]);
		}
	}

	return SelectedMeshs;
}

XArray<XVertex*> XScene::GetSelectedVertex()
{
	XArray<XVertex*> SelectedXVertex;

	for (int i = 0; i < Meshs.Count(); i++)
	{
		for (int j = 0; j < Meshs[i].Vertices.Count(); j++)
			if (Meshs[i].Vertices[j].selected)
				SelectedXVertex.Add(&(Meshs[i].Vertices[j]));
	}

	return SelectedXVertex;
}

XArray<XFace*> XScene::GetSelectedFace()
{
	XArray<XFace*> SelectedFace;

	for (int i = 0; i < Meshs.Count(); i++)
	{
		for (int j = 0; j < Meshs[i].Faces.Count(); j++)
		{
			if (Meshs[i].Faces[j].selected)
			{
				SelectedFace.Add(&(Meshs[i].Faces[j]));
			}
		}
	}

	return SelectedFace;
}
