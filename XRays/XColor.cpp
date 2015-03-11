#include "stdafx.h"
#include "XColor.h"

double unit_limiter(double vUnitDouble)
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

XColor::XColor()
{
}

XColor::XColor(double nRed, double nGreen, double nBlue)
{
	Red(nRed);
	Green(nGreen);
	Blue(nBlue);
}

XColor::XColor(COLORREF Color)
{
	Red(GetRValue(Color) / 255.0);
	Green(GetGValue(Color) / 255.0);
	Blue(GetBValue(Color) / 255.0);
}

XColor::~XColor()
{
}

XColor XColor::operator* (double Value) const
{
	return XColor(cRed*Value, cGreen*Value, cBlue*Value);
}

XColor XColor::operator+ (const XColor& Color) const
{
	return XColor(cRed + Color.Red(), cGreen + Color.Green(), cBlue + Color.Blue());
}

XColor& XColor::operator= (const XColor& Color)
{
	cRed = Color.Red();
	cGreen = Color.Green();
	cBlue = Color.Blue();

	return *this;
}

void XColor::Red(double nRed)
{
	cRed = unit_limiter(nRed);
}

double XColor::Red() const
{
	return cRed;
}

void XColor::Green(double nGreen)
{
	cGreen = unit_limiter(nGreen);
}

double XColor::Green() const
{
	return cGreen;
}

void XColor::Blue(double nBlue)
{
	cBlue = unit_limiter(nBlue);
}

double XColor::Blue() const
{
	return cBlue;
}

unsigned char XColor::RedByte() const
{
	return ByteValue(cRed);
}

unsigned char XColor::GreenByte() const
{
	return ByteValue(cGreen);
}

unsigned char XColor::BlueByte() const
{
	return ByteValue(cBlue);
}

COLORREF XColor::ToCOLORREF()
{
	return RGB(RedByte(), GreenByte(), BlueByte());
}

unsigned char XColor::ByteValue(double vZeroToOne) const
{
	return (unsigned char)(vZeroToOne*255.0);
}
