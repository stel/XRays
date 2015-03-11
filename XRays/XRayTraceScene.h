#pragma once

#include "XTriangle.h"
#include "XArray.h"
#include "XLight.h"
#include "XScene.h"

class XRayTraceScene
{
public:
	XArray<XTriangle> Triangles;
	XArray<XLight> Lights;

	void LoadFromXScene(const XScene *Scene)
	{
		const XMesh *Mesh;
		const XFace *Face;
		XTriangle Triangle;
		XCoordSystem CS;

		Triangle.SetColor(XColor(1, 0, 0));
		Triangle.SetAmbientReflectivity(1);
		Triangle.SetDiffuseReflectivity(0);
		Triangle.SetSpecularSize(1);

		for (int i = 0; i < Scene->Meshs.Count(); i++)
		{
			Mesh = &Scene->Meshs[i];
			CS = Scene->Meshs[i].CoordSystem;
			CS = CS.Back();

			for (int j = 0; j < Mesh->Faces.Count(); j++)
			{
				Face = &Mesh->Faces[j];

				Triangle.SetVertices(
					CS.TransformVector(Face->Vertex[0]->Pos) + CS.GetOrigin(),
					CS.TransformVector(Face->Vertex[1]->Pos) + CS.GetOrigin(),
					CS.TransformVector(Face->Vertex[2]->Pos) + CS.GetOrigin()
				);
				Triangle.Color = XColor(Mesh->Color);
				Triangle.SetDiffuseReflectivity(Mesh->Reflectivity);

				Triangles.Add(Triangle);
			}
		}
	}

	void LookThroughCamera(const XCamera *Camera)
	{
		XCoordSystem CS;
		CS = Camera->CoordSystem;

		for (int i = 0; i < Triangles.Count(); i++)
		{
			Triangles[i].A = CS.TransformVector(Triangles[i].A) + CS.GetOrigin();
			Triangles[i].A.Y = -Triangles[i].A.Y;
			Triangles[i].B = CS.TransformVector(Triangles[i].B) + CS.GetOrigin();
			Triangles[i].B.Y = -Triangles[i].B.Y;
			Triangles[i].C = CS.TransformVector(Triangles[i].C) + CS.GetOrigin();
			Triangles[i].C.Y = -Triangles[i].C.Y;
		}
	}
};
