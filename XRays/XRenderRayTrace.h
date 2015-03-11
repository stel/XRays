#pragma once

#include "XVector3d.h"
#include "XRay.h"
#include "XColor.h"
#include "XLight.h"
#include "XCamera.h"
#include "XTriangle.h"
#include "XRayTraceScene.h"

class XRenderRayTrace
{

public:
	XRenderRayTrace();

	void SetTargetDC(CDC *nDC);
	void SetTargetRect(CRect nRect);

	void DrawPixel(int x, int y, XColor Color);
	void RenderScene(XRayTraceScene *RayTraceScene, XCamera *ViewCamera);

	void BackgroundColor(const XColor& rBackgroundColor);
	void AmbientColor(const XColor& rAmbient);
	void AmbientIntensity(double vAmbientIntensity);

	XCamera Camera;

	double mAmbientIntensity;
	int mAntiAliasDetail;

private:
	XColor TraceRay(const XRay& rRay, bool vIsReflecting = false, const XTriangle* pReflectingFrom = 0);
	XTriangle* QueryScene(const XRay& rRay, XVector3d& rIntersectionPoint, bool vIsReflecting = false, const XTriangle* pReflectingFrom = 0);
	double Shade(const XTriangle* pShapeToShade, const XVector3d& rPointOnShapeToShade);
	double Specular(const XTriangle* pShapeToShade, const XVector3d& rPointOnShapeToShade, const XLight* pLight);

	XColor mBackgroundColor;
	XColor mAmbientColor;
	XColor mSpecularColor;

	XRayTraceScene *Scene;

	int mRecursions;
	int mRecursionLimit;

	CRect TargetRect;
	CDC* TargetDC;
};
