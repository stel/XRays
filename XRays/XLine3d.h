#pragma once

#include "XVector3d.h"

class XLine3d
{
public:
	XVector3d start, end;

	XLine3d() : start(0, 0, 0), end(1, 1, 1) {};
	XLine3d(const XVector3d &s, XVector3d &e) : start(s), end(e) {};
	XLine3d(double Xs, double Ys, double Zs, double Xe, double Ye, double Ze) : start(Xs, Ys, Zs), end(Xe, Ye, Ze) {};
};
