#pragma once

class XColor
{
public:
	XColor();
	XColor(double nRed, double nGreen, double nBlue);
	XColor(COLORREF Color);
	~XColor();

	XColor operator* (double Value) const;
	XColor operator+ (const XColor &Color) const;
	XColor& operator= (const XColor &Color);

	void Red(double nRed);
	double Red() const;
	void Green(double nGreen);
	double Green() const;
	void Blue(double nBlue);
	double Blue() const;

	unsigned char RedByte() const;
	unsigned char GreenByte() const;
	unsigned char BlueByte() const;

	COLORREF ToCOLORREF();
private:
	unsigned char ByteValue(double vZeroToOne) const;
	double cRed, cGreen, cBlue;
};