#pragma once

#include "XVector3d.h"

class XCoordSystem
{
public:
	XCoordSystem() : Origin(0, 0, 0), XAxis(1, 0, 0), YAxis(0, 1, 0), ZAxis(0, 0, 1) {}
	XCoordSystem(const XCoordSystem &CoordSystem)
	{
		Origin = CoordSystem.Origin;
		XAxis = CoordSystem.XAxis;
		YAxis = CoordSystem.YAxis;
		ZAxis = CoordSystem.ZAxis;
	}

	XCoordSystem(const XVector3d &O,
		const XVector3d &X, const XVector3d &Y, const XVector3d &Z)
		: Origin(O), XAxis(X), YAxis(Y), ZAxis(Z) {}

	XCoordSystem& operator =(const XCoordSystem CoordSystem)
	{
		Origin = CoordSystem.Origin;
		XAxis = CoordSystem.XAxis;
		YAxis = CoordSystem.YAxis;
		ZAxis = CoordSystem.ZAxis;

		return *this;
	}

	XCoordSystem& operator *= (const XCoordSystem &CoordSystem)
	{
		Origin = CoordSystem.Origin;
		XAxis = CoordSystem.TransformVector(XAxis);
		YAxis = CoordSystem.TransformVector(YAxis);
		ZAxis = CoordSystem.TransformVector(ZAxis);

		return *this;
	}

	XCoordSystem operator * (const XCoordSystem &CoordSystem) const
	{
		return XCoordSystem(*this) *= CoordSystem;
	}

	XCoordSystem Back()
	{
		return XCoordSystem() * *this;
	}

	void SetTranslation(const XVector3d &Translation)
	{
		Origin = Translation;
	}

	void AddTranslation(const XVector3d &Translation)
	{
		Origin += Translation;
	}

	void SetRotation(const XVector3d &nRotation)
	{
		Rotation = nRotation*0.017453292519943295769236907684886; // pi/180;

		double cr = cos(Rotation.X);
		double sr = sin(Rotation.X);
		double cp = cos(Rotation.Y);
		double sp = sin(Rotation.Y);
		double cy = cos(Rotation.Z);
		double sy = sin(Rotation.Z);

		double srsp = sr*sp;
		double crsp = cr*sp;

		*this = XCoordSystem(
			Origin,
			XVector3d((cp*cy), (cp*sy), (-sp)),
			XVector3d((srsp*cy - cr*sy), (srsp*sy + cr*cy), (sr*cp)),
			XVector3d((crsp*cy + sr*sy), (crsp*sy - sr*cy), (cr*cp))
		);
	}

	XVector3d GetRotation() const
	{
		return Rotation*57.295779513082320876798154814105; // 180/pi;
	}

	void SetRotationZ(const float Z)
	{
		*this = XCoordSystem(
			XVector3d(0.0, 0.0, 0.0),
			XVector3d(cosf(Z), -sinf(Z), 0.0),
			XVector3d(sinf(Z), cosf(Z), 0.0),
			XVector3d(0.0, 0.0, 1.0)
		).Back();
	}

	void SetRotationX(const float Z)
	{
		*this = XCoordSystem(
			XVector3d(0.0, 0.0, 0.0),
			XVector3d(1.0, 0.0, 0.0),
			XVector3d(0.0, cosf(Z), -sinf(Z)),
			XVector3d(0.0, sinf(Z), cosf(Z))
		).Back();
	}

	void SetRotationY(const float Z)
	{
		*this = XCoordSystem(
			XVector3d(0.0, 0.0, 0.0),

			XVector3d(cosf(Z), 0.0, sinf(Z)),
			XVector3d(0.0, 1.0, 0.0),
			XVector3d(-sinf(Z), 0.0, cosf(Z))
			).Back();
	}

	void AddRotationZ(const float Z)
	{
		*this *= XCoordSystem(
			XVector3d(0.0, 0.0, 0.0),
			XVector3d(cosf(Z), -sinf(Z), 0.0),
			XVector3d(sinf(Z), cosf(Z), 0.0),
			XVector3d(0.0, 0.0, 1.0)
		).Back();
	}

	void AddRotationX(const float Z)
	{
		*this *= XCoordSystem(
			XVector3d(0.0, 0.0, 0.0),
			XVector3d(1.0, 0.0, 0.0),
			XVector3d(0.0, cosf(Z), -sinf(Z)),
			XVector3d(0.0, sinf(Z), cosf(Z))
		).Back();
	}

	void AddRotationY(const float Z)
	{
		*this *= XCoordSystem(
			XVector3d(0.0, 0.0, 0.0),
			XVector3d(cosf(Z), 0.0, sinf(Z)),
			XVector3d(0.0, 1.0, 0.0),
			XVector3d(-sinf(Z), 0.0, cosf(Z))
		).Back();
	}

	void SetOrigin(const XVector3d &nOrigin) { Origin = nOrigin; }
	void SetXAxis(const XVector3d &nXAxis) { XAxis = nXAxis; XAxis.Normalize(); }
	void SetYAxis(const XVector3d &nYAxis) { YAxis = nYAxis; YAxis.Normalize(); }
	void SetZAxis(const XVector3d &nZAxis) { ZAxis = nZAxis; ZAxis.Normalize(); }

	XVector3d GetOrigin() const { return Origin; }
	XVector3d GetXAxis() const { return XAxis; }
	XVector3d GetYAxis() const { return YAxis; }
	XVector3d GetZAxis() const { return ZAxis; }

	XVector3d TransformVector(const XVector3d &Src) const
	{
		return XVector3d(Src.DotProduct(XAxis), Src.DotProduct(YAxis), Src.DotProduct(ZAxis));
	}

	XVector3d TransformPoint(const XVector3d &Src) const
	{
		return TransformVector(Src - Origin);
	}

private:
	XVector3d
		Origin,
		XAxis, YAxis, ZAxis,
		Rotation;
};