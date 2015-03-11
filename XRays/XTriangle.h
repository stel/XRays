#pragma once

#include "XVector3d.h"
#include "XRay.h"

class XTriangle
{
public:
	XTriangle() {};
	XTriangle(const XVector3d& nA, const XVector3d& nB, const XVector3d& nC) : A(nA), B(nB), C(nC) {	};
	~XTriangle() {};


	void SetVertices(const XVector3d& nA, const XVector3d& nB, const XVector3d& nC)
	{
		A = nA;
		B = nB;
		C = nC;
	}

	bool Intersect(const XRay& Ray, XVector3d& IntersectPoint) const
	{
		XVector3d pointb_minus_pointa(A, B);
		XVector3d pointb_minus_pointc(A, C);

		XVector3d plane_normal = this->Normal(A, Ray.GetOrigin());
		XVector3d ray_direction_normalized = Ray.GetDirection().GetNormalized();

		if (plane_normal.IsSameDirection(ray_direction_normalized))
		{
			return false;
		}

		XVector3d b_minus_u(Ray.GetOrigin(), B);

		double t = plane_normal.DotProduct(b_minus_u) / plane_normal.DotProduct(Ray.GetDirection());
		XVector3d p = (Ray.GetDirection() * t) + Ray.GetOrigin();

		XVector3d p_minus_a(A, p);
		XVector3d p_minus_b(B, p);
		XVector3d p_minus_c(C, p);
		XVector3d pointc_minus_pointb(B, C);
		XVector3d pointa_minus_pointc(C, A);

		double test1 = (pointb_minus_pointa.CrossProduct(p_minus_a)).DotProduct(plane_normal);
		double test2 = (pointc_minus_pointb.CrossProduct(p_minus_b)).DotProduct(plane_normal);
		double test3 = (pointa_minus_pointc.CrossProduct(p_minus_c)).DotProduct(plane_normal);

		if ((test1 > 0 && test2 > 0 && test3 > 0) || (test1 < 0 && test2 < 0 && test3 < 0))
		{
			IntersectPoint = p;
			return true;
		}
		else
		{
			return false;
		}
	}

	XVector3d Normal(const XVector3d& SurfacePoint, const XVector3d& OffSurface) const
	{
		XVector3d pointb_minus_pointa(A, B);
		XVector3d pointb_minus_pointc(A, C);
		XVector3d plane_normal = pointb_minus_pointa.CrossProduct(pointb_minus_pointc).GetNormalized();

		XVector3d triangle_to_off_surface_point = XVector3d(A, OffSurface);
		triangle_to_off_surface_point.Normalize();

		if (!plane_normal.IsSameDirection(triangle_to_off_surface_point))
		{
			plane_normal = -plane_normal;
		}

		return plane_normal;
	}

	XRay Reflect(const XVector3d& rReflectFrom, const XRay& rIncidentRay) const
	{
		XRay result;
		XVector3d result_direction;
		XVector3d incident_unit = rIncidentRay.GetDirection().GetNormalized();
		XVector3d normal = this->Normal(rReflectFrom, rIncidentRay.GetOrigin());

		if (!normal.IsSameDirection(incident_unit))
		{
			normal = -normal;
		}

		result.SetOrigin(rReflectFrom);
		result.SetDirection(normal*2.0*normal.DotProduct(incident_unit) - incident_unit);

		return result;
	}

	XColor GetColor() const
	{
		return Color;
	}

	void SetColor(const XColor nColor)
	{
		Color = nColor;
	}

	double GetDiffuseReflectivity() const
	{
		return DiffuseReflectivity;
	}

	void SetDiffuseReflectivity(const double nDiffuseReflectivity)
	{
		DiffuseReflectivity = nDiffuseReflectivity;
	}

	double GetAmbientReflectivity() const
	{
		return AmbientReflectivity;
	}

	void SetAmbientReflectivity(const double nAmbientReflectivity)
	{
		AmbientReflectivity = nAmbientReflectivity;
	}

	double GetSpecularSize() const
	{
		return SpecularSize;
	}

	void SetSpecularSize(const double nSpecularSize)
	{
		SpecularSize = nSpecularSize;
	}

	XVector3d A, B, C;
	XColor Color;

private:
	double AmbientReflectivity, DiffuseReflectivity, SpecularSize;
};
