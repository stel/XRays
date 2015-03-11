#pragma once
#include "XArray.h"

class XZBuffer
{
public:
	XZBuffer() : Width(0), Height(0), Buffer(0) {};
	XZBuffer(int nWidth, int nHeight) : Width(nWidth), Height(nHeight), Buffer(0)
	{
		Resize(Width, Height);
	}

	~XZBuffer()
	{
		delete[] Buffer;
	}

	void Restore()
	{
		for (int i = 0; i < Width*Height; i++)
		{
			Buffer[i] = 999999999;
		}
	}

	void Resize(int nWidth, int nHeight)
	{
		Width = nWidth;
		Height = nHeight;
		Length = Width * Height;

		if (Buffer != 0)
		{
			delete[] Buffer;
		}

		Buffer = new double[Length];
		Restore();
	}


	inline double GetZValue(int x, int y)
	{
		if (y < 0 || x < 0 || x >= Width || y >= Height)
		{
			return -999999999;
		}

		return Buffer[y * Width + x];
	}

	inline double GetZValue(int Pos)
	{
		if (Pos < 0 || Pos >= Length)
		{
			return -999999999;
		}

		return Buffer[Pos];
	}

	inline void SetValue(int x, int y, double value)
	{
		if (y < 0 || x < 0 || x >= Width || y >= Height)
		{
			return;
		}

		Buffer[y * Width + x] = value;
	}

	inline void SetValue(int Pos, double value)
	{
		if (Pos < 0 || Pos >= Length)
		{
			return;
		}

		Buffer[Pos] = value;
	}


private:
	int Width, Height, Length;
	double *Buffer;
};