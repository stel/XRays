#include "stdafx.h"
#include "XRender.h"

void XRender::SetTargetDC(CDC *nDC)
{
	TargetDC = nDC;
}

void XRender::SetTargetRect(CRect nRect)
{
	TargetRect = nRect;
}
