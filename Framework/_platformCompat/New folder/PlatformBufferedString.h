#if !defined(PLATFORMBUFFEREDSTRING_H_INCLUDED)
#define      PLATFORMBUFFEREDSTRING_H_INCLUDED

#include "PlatformCompat.h"

class BufferedString : public std::string
{
public:
	BufferedString();
	virtual ~BufferedString();

	//operators
	void operator=(std::string& str);
	void operator=(LPCTSTR lpsz);

	TCHAR*	GetBuffer		(UINT minBufferLen);
	void	ReleaseBuffer	(int nNewLen = -1);
	UINT	GetBufferLength	() {return m_nBufferSize / sizeof(TCHAR);};

protected:
	TCHAR*	m_pBuffer;
	UINT	m_nBufferSize;
};

#endif // !defined(PLATFORMBUFFEREDSTRING_H_INCLUDED)

