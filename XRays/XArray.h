#pragma once

template <class T>
class XArray
{
public:
	XArray() : Data(0), Size(0), Used(0) {};
	XArray(const XArray<T>& Array) : Data(0) { *this = Array; }
	XArray(int nSize) : Data(0), Size(0), Used(0)	{ Resize(nSize); }

	~XArray()
	{
		delete[] Data;
	}

	void Add(const T& Element)
	{
		if (Used + 1 > Size)
		{
			Resize(Used * 2 + 1);
		}

		Data[Used++] = Element;
	}

	void Delete(const int index)
	{
		if (index > Used - 1) {
			return;
		}

		Used--;

		for (int i = index; i < Used; i++)
		{
			Data[i] = Data[i + 1];
		}
	}

	T& operator[](int n)
	{
		return Data[n];
	}

	const T& operator[](int n) const
	{
		return Data[n];
	}

	XArray<T>& operator=(const XArray<T> &Array)
	{
		Clear();
		Resize(Array.Count());

		for (Used = 0; Used < Size; Used++)
		{
			Data[Used] = Array[Used];
		}

		return *this;
	}

	void Resize(int nSize)
	{
		T *OldData = Data;
		Size = nSize;
		Data = new T[nSize];
		int n = Used < nSize ? Used : nSize;

		for (int i = 0; i < n; i++)
		{
			Data[i] = OldData[i];
		}

		if (Used > Size)
		{
			Used = Size;
		}

		delete[] OldData;
	}

	int Count() const { return Used; }

	void Clear()
	{
		if (!Data) return;

		delete[] Data;
		Data = 0;
		Used = 0;
		Size = 0;
	}

private:
	T* Data;
	int Size;
	int Used;
};