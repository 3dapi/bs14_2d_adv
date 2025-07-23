// GDICanvas.h: interface for the CGDICanvas class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDICANVAS_H__F5EB4F68_2CBC_11D4_A1EE_8F7A3049432E__INCLUDED_)
#define AFX_GDICANVAS_H__F5EB4F68_2CBC_11D4_A1EE_8F7A3049432E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

//GDICanvas--wrapper for a dc and a bitmap
class CGDICanvas  
{
private:
	//memory dc
	HDC hdcMem;

	//new bitmap
	HBITMAP hbmNew;

	//old bitmap
	HBITMAP hbmOld;

	//width and height
	int nWidth;
	int nHeight;

public:
	//constructor
	CGDICanvas();

	//loads bitmap from a file
	void Load(HDC hdcCompatible,LPCTSTR lpszFilename);

	//creates a blank bitmap
	void CreateBlank(HDC hdcCompatible, int width, int height);

	//destroys bitmap and dc
	void Destroy();

	//converts to HDC
	operator HDC();

	//return width
	int GetWidth();

	//return height
	int GetHeight();

	//destructor
	~CGDICanvas();
};

#endif // !defined(AFX_GDICANVAS_H__F5EB4F68_2CBC_11D4_A1EE_8F7A3049432E__INCLUDED_)
