#pragma once

#include "math.h"

class XVector3d
{
public:
	double X, Y, Z;

	XVector3d() : X(0), Y(0), Z(0) {};
	XVector3d(double nX, double nY, double nZ) : X(nX), Y(nY), Z(nZ) {};
	XVector3d(const XVector3d &Vector3d) : X(Vector3d.X), Y(Vector3d.Y), Z(Vector3d.Z) {};
	XVector3d(const XVector3d &Start, const XVector3d &End)
	{
		X = End.X - Start.X;
		Y = End.Y - Start.Y;
		Z = End.Z - Start.Z;
	};

	~XVector3d() {};

	void SetXYZ(const double nX, const double nY, const double nZ)
	{
		X = nX;
		Y = nY;
		Z = nZ;
	};

	XVector3d operator-()
	{
		return XVector3d(-X, -Y, -Z);
	};

	XVector3d& operator= (const XVector3d &Vector3d)
	{
		X = Vector3d.X;
		Y = Vector3d.Y;
		Z = Vector3d.Z;

		return *this;
	};

	XVector3d operator+ (const XVector3d &Vector3d) const
	{
		return XVector3d(X + Vector3d.X, Y + Vector3d.Y, Z + Vector3d.Z);
	};

	XVector3d operator- (const XVector3d &Vector3d) const
	{
		return XVector3d(X - Vector3d.X, Y - Vector3d.Y, Z - Vector3d.Z);
	};

	XVector3d operator* (const XVector3d &Vector3d)
	{
		return XVector3d(X * Vector3d.X, Y * Vector3d.Y, Z * Vector3d.Z);
	};

	XVector3d& operator+= (const XVector3d &Vector3d)
	{
		X += Vector3d.X;
		Y += Vector3d.Y;
		Z += Vector3d.Z;

		return *this;
	};

	XVector3d& operator-= (const XVector3d &Vector3d)
	{
		X -= Vector3d.X;
		Y -= Vector3d.Y;
		Z -= Vector3d.Z;

		return *this;
	};

	XVector3d& operator*= (const XVector3d &Vector3d)
	{
		X *= Vector3d.X;
		Y *= Vector3d.Y;
		Z *= Vector3d.Z;

		return *this;
	};

	XVector3d& operator/= (const XVector3d &Vector3d)
	{
		X /= Vector3d.X;
		Y /= Vector3d.Y;
		Z /= Vector3d.Z;

		return *this;
	};

	XVector3d operator* (const double value) const
	{
		return XVector3d(X * value, Y * value, Z * value);
	}

	XVector3d operator/ (const double value) const
	{
		return XVector3d(X / value, Y / value, Z / value);
	}

	const double Length() const
	{
		return sqrt(SqLength());
	}

	const double SqLength() const
	{
		return X * X + Y * Y + Z * Z;
	}

	void Normalize()
	{
		double Len;
		Len = Length();

		if (Len == 1)
		{
			return;
		}

		X /= Len;
		Y /= Len;
		Z /= Len;
	}

	XVector3d GetNormalized() const
	{
		XVector3d Vector3d(*this);
		Vector3d.Normalize();

		return Vector3d;
	}

	bool IsSameDirection(const XVector3d& Vector3d) const
	{
		return (this->GetNormalized().DotProduct(Vector3d.GetNormalized()) > 0.0);
	}

	double DotProduct(const XVector3d &Vector3d) const
	{
		return X * Vector3d.X + Y * Vector3d.Y + Z * Vector3d.Z;
	}

	XVector3d CrossProduct(const XVector3d &Vector3d) const
	{
		return XVector3d(
			Y * Vector3d.Z - Z * Vector3d.Y,
			Z * Vector3d.X - X * Vector3d.Z,
			X * Vector3d.Y - Y * Vector3d.X
		);
	}

	double DistanceTo(const XVector3d Destination) const
	{
		return sqrt(
			(Destination.X - X)*(Destination.X - X) +
			(Destination.Y - Y)*(Destination.Y - Y) +
			(Destination.Z - Z)*(Destination.Z - Z)
		);
	}
};
