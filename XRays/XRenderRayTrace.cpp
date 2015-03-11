#include "stdafx.h"

#include "XRenderRayTrace.h"

double unit_limiter1(double vUnitDouble)
{
	double result = vUnitDouble;

	if (result < 0.0)
	{
		result = 0.0;
	}
	else if (result > 1.0)
	{
		result = 1.0;
	}

	return result;
}

void XRenderRayTrace::SetTargetRect(CRect nRect)
{
	TargetRect = nRect;
}

void XRenderRayTrace::SetTargetDC(CDC *nDC)
{
	TargetDC = nDC;
}

XRenderRayTrace::XRenderRayTrace() : mBackgroundColor(1, 1, 1), mAmbientColor(0, 0, 0), mSpecularColor(1, 1, 1)
{
	mBackgroundColor = XColor(0, 0, 0);
	mAmbientIntensity = 0.3;
	mRecursionLimit = 700;
	mAntiAliasDetail = 1;

	TargetRect.right = 640;
	TargetRect.bottom = 480;

	mRecursions = 0;
}

void XRenderRayTrace::DrawPixel(int x, int y, XColor Color)
{
	TargetDC->SetPixel(x, y, Color.ToCOLORREF());
}

void XRenderRayTrace::RenderScene(XRayTraceScene *RayTraceScene, XCamera *ViewCamera)
{
	Scene = RayTraceScene;
	Camera = *ViewCamera;
	XRay eye_ray(Camera.GetPosition(), XVector3d(0, 0, 1));
	XColor draw_color;
	double i_inc, j_inc, anti_alias_i_inc, anti_alias_j_inc;
	double i, j, anti_alias_i, anti_alias_j;
	int pixel_x, pixel_y, anti_alias_pixel_x, anti_alias_pixel_y;

	double average_red_byte, average_green_byte, average_blue_byte;
	int anti_alias_count;

	i_inc = 2.0 / (double)TargetRect.Width();
	j_inc = 2.0 / (double)TargetRect.Height();
	anti_alias_i_inc = 1.0 / (double)mAntiAliasDetail;
	anti_alias_j_inc = 1.0 / (double)mAntiAliasDetail;

	pixel_y = 0;
	j = 1.0;

	for (; pixel_y < TargetRect.Height(); j -= j_inc, pixel_y++)
	{
		pixel_x = 0;
		i = -1.0;

		for (; pixel_x < TargetRect.Width(); i += i_inc, pixel_x++)
		{
			anti_alias_pixel_y = 0;
			anti_alias_j = 0.0;
			average_red_byte = 0;
			average_green_byte = 0;
			average_blue_byte = 0;
			anti_alias_count = 0;

			for (; anti_alias_pixel_y < mAntiAliasDetail; anti_alias_j += anti_alias_j_inc, anti_alias_pixel_y++)
			{
				anti_alias_pixel_x = 0;
				anti_alias_i = 0.0;

				for (; anti_alias_pixel_x < mAntiAliasDetail; anti_alias_i += anti_alias_i_inc, anti_alias_pixel_x++)
				{
					anti_alias_count++;
					eye_ray.SetDirection(XVector3d(i + (anti_alias_i*i_inc), j + (anti_alias_j*j_inc), 1.0));
					draw_color = TraceRay(eye_ray);

					average_red_byte = average_red_byte + ((double)draw_color.RedByte() - average_red_byte) / (double)anti_alias_count;
					average_green_byte = average_green_byte + ((double)draw_color.GreenByte() - average_green_byte) / (double)anti_alias_count;
					average_blue_byte = average_blue_byte + ((double)draw_color.BlueByte() - average_blue_byte) / (double)anti_alias_count;
				}
			}

			TargetDC->SetPixel(pixel_x, pixel_y, RGB(average_red_byte, average_green_byte, average_blue_byte));
		}
	}
}

XColor XRenderRayTrace::TraceRay(const XRay& rRay, bool vIsReflecting, const XTriangle* pReflectingFrom)
{
	mRecursions++;
	XTriangle* closest_shape;
	XVector3d intersect_point;
	XColor result;

	if (vIsReflecting)
	{
		closest_shape = QueryScene(rRay, intersect_point, vIsReflecting, pReflectingFrom);
	}
	else
	{
		closest_shape = QueryScene(rRay, intersect_point);
	}

	if (closest_shape == NULL && !vIsReflecting)
	{
		mRecursions = 0;
		return mBackgroundColor;
	}

	if (closest_shape == NULL && vIsReflecting)
	{
		mRecursions = 0;
		return mAmbientColor*mAmbientIntensity;
	}

	if (mRecursions > mRecursionLimit)
	{
		mRecursions = 0;
		return XColor(0, 0, 0);
	}

	result = closest_shape->GetColor()*Shade(closest_shape, intersect_point);

	XRay backwards_ray(intersect_point, -rRay.GetDirection());
	
	if (closest_shape->GetDiffuseReflectivity() > 0.0)
	{
		result = result + (TraceRay(closest_shape->Reflect(intersect_point, backwards_ray), true, closest_shape)*closest_shape->GetDiffuseReflectivity());
	}

	return (result + mSpecularColor);
}

double XRenderRayTrace::Shade(const XTriangle* pShapeToShade, const XVector3d& rPointOnShapeToShade)
{
	XLight *Light;
	double intensity = mAmbientIntensity * pShapeToShade->GetAmbientReflectivity();
	XRay light_ray; 
	double dot_product;
	XTriangle* closest_shape;
	XVector3d light_intersect;
	light_ray.SetOrigin(rPointOnShapeToShade);
	XRay light_ray_from_light;

	mSpecularColor.Red(0);
	mSpecularColor.Green(0);
	mSpecularColor.Blue(0);

	for (int i = 0; i < Scene->Lights.Count(); i++)
	{
		Light = &Scene->Lights[i];

		light_ray.SetDirection(XVector3d(rPointOnShapeToShade, Light->GetPosition()));

		light_ray_from_light.SetOrigin(Light->GetPosition());
		light_ray_from_light.SetDirection(XVector3d(Light->GetPosition(), rPointOnShapeToShade));

		closest_shape = QueryScene(light_ray_from_light, light_intersect);
		
		if (closest_shape == NULL || (closest_shape == pShapeToShade && light_ray.GetDirection().DotProduct(pShapeToShade->Normal(rPointOnShapeToShade, Camera.GetPosition() /*light_ray_from_light.GetOrigin()*/)) >= 0.0)) //if (QueryScene( lightRay)= NIL)
		{
			XVector3d normal_vector = pShapeToShade->Normal(rPointOnShapeToShade, XVector3d());
			dot_product = normal_vector.DotProduct(light_ray.GetDirection().GetNormalized());
			dot_product *= Light->GetIntensity();

			if (dot_product < 0.0)
			{
				dot_product = dot_product*-1.0;
			}

			intensity = unit_limiter1(intensity + dot_product);

			if (light_ray.GetDirection().DotProduct(pShapeToShade->Normal(rPointOnShapeToShade, light_ray_from_light.GetOrigin())) >= 0.0)
			{
				double specular = Specular(pShapeToShade, rPointOnShapeToShade, Light);
				mSpecularColor = mSpecularColor + XColor(specular, specular, specular);
			}
		}
	}

	return intensity;
}

double XRenderRayTrace::Specular(const XTriangle* pShapeToShade, const XVector3d& rPointOnShapeToShade, const XLight* pLight)
{
	XRay reflected = pShapeToShade->Reflect(rPointOnShapeToShade, XRay(rPointOnShapeToShade, XVector3d(rPointOnShapeToShade, pLight->GetPosition())));

	XVector3d eye_vector = XVector3d(rPointOnShapeToShade, Camera.GetPosition());
	XVector3d reflected_vector = reflected.GetDirection().GetNormalized();
	eye_vector.Normalize();
	double dot_product = eye_vector.DotProduct(reflected_vector);

	int n = pShapeToShade->GetSpecularSize();
	double specular_intensity = dot_product / (n - n*dot_product + dot_product);
	return unit_limiter1(specular_intensity*(pLight->GetIntensity()));
}

XTriangle* XRenderRayTrace::QueryScene(const XRay& rRay, XVector3d& rIntersectionPoint, bool vIsReflecting, const XTriangle* pReflectingFrom)
{
	XTriangle* closest_shape = NULL, *CurTriangle;
	XVector3d intersect_point;
	double closest_distance = 0;
	double intersect_distance;
	bool found_intersection = false;

	for (int i = 0; i < Scene->Triangles.Count(); i++)
	{
		CurTriangle = &Scene->Triangles[i];

		if (CurTriangle->Intersect(rRay, intersect_point))
		{
			intersect_distance = intersect_point.DistanceTo(rRay.GetOrigin());

			if (!found_intersection && (CurTriangle) != pReflectingFrom)
			{
				found_intersection = true;
				rIntersectionPoint = intersect_point;
				closest_shape = CurTriangle;
				closest_distance = intersect_distance;
			}
			else if (intersect_distance < closest_distance && (CurTriangle) != pReflectingFrom)
			{
				rIntersectionPoint = intersect_point;
				closest_shape = CurTriangle;
				closest_distance = intersect_distance;
			}
		}
	}

	return closest_shape;
}

void XRenderRayTrace::BackgroundColor(const XColor& rBackgroundColor)
{
	mBackgroundColor = rBackgroundColor;
}

void XRenderRayTrace::AmbientColor(const XColor& rAmbientColor)
{
	mAmbientColor = rAmbientColor;
}
void XRenderRayTrace::AmbientIntensity(double vAmbientIntensity)
{
	mAmbientIntensity = unit_limiter1(vAmbientIntensity);
}
