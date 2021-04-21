// MipmapImage.h header file.
//
//	ESCL (Elite Soft Class Library)
//	
//	Author: ZqrTalent
//

#pragma once
#include <atlimage.h>
#include "..\Array\AutoSortedArray.h"

class MipmapImage
{
public:
	// Constructor / Destructor
	MipmapImage();
	virtual ~MipmapImage();

	bool	LoadImage		(CString sImageFile);
	bool	SetImage		(CImage* pImage);
	void	Destroy			();

	CImage*	GetImageOriginal(){return (!m_imageOriginal.IsNull()) ? &m_imageOriginal : NULL;};
	BOOL	Draw			(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, 
							int xSrc, int ySrc, int nSrcWidth, int nSrcHeight);

	int		GetWidth		(){ return m_imageOriginal.GetWidth(); };
	int		GetHeight		(){ return m_imageOriginal.GetHeight(); };

protected:
	CImage*	GetNearestImage	(int cx, int cy);

protected:

	CImage				m_imageOriginal;
	AutoSortedArray		m_arrImageBySize;	// <[width][height]><CImage*>

};