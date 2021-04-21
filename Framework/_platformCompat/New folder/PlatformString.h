#if !defined(PLATFORMSTRING_H_INCLUDED)
#define      PLATFORMSTRING_H_INCLUDED

#include "PlatformCompat.h"

#ifdef MFCSTRING
class _String : public CString
{
public:
	_String(){};
	virtual ~_String(){};

	_String(const TCHAR* pszSrc) : CString(pszSrc){}
	_String(const TCHAR* pch, int nLength) : CString(pch, nLength){}
	_String(const CString& strSrc) : CString(strSrc){}
	_String(const _String& strSrc) : CString(strSrc){}

	inline operator TCHAR*() const throw(){
		return (TCHAR*)CString::operator PCXSTR();
		}

	_String& operator=(PCXSTR pszSrc){
		return (_String&)CString::operator=(pszSrc);
		}

	// Assignment operators
	_String& operator=(const CString& strSrc){
		return (_String&)CString::operator=(strSrc);
		}
	/*
	_String& operator+=(const _String& str){
		return (_String&)CString::operator+=(str);
		}*/
	
	_String& operator+=(PCXSTR pszSrc){
		return (_String&)CString::operator+=(pszSrc);
		}

	_String& operator+=(TCHAR ch){
		return (_String&)CString::operator+=(ch);
		}

	//Comparision
	 friend bool operator==(const _String& str1, const _String& str2){
		return( str1.Compare( str2 ) == 0 );
		}

	friend bool operator==(const _String& str1,const TCHAR* psz2){
		return( str1.Compare( psz2 ) == 0 );
		}

	friend bool operator==(PCXSTR psz1, const _String& str2){
		return( str2.Compare( psz1 ) == 0 );
		}

	 /*friend bool operator!=(const _String& str1, const _String& str2){
		return( str1.Compare( str2 ) != 0 );
		}
	 
	friend bool operator!=(const _String& str1,const TCHAR* psz2){
		return( str1.Compare( psz2 ) != 0 );
		}

	friend bool operator!=(PCXSTR psz1, const _String& str2){
		return( str2.Compare( psz1 ) != 0 );
		}
		*/
	friend bool operator!=(_String& str1, _String& str2){
		return( str1.Compare( str2 ) != 0 );
		}
	
protected:
};

#else
#include <string>
#include <algorithm>
#include <memory>

class _String : public std::string
{
public:
	_String() : std::string() {};
	virtual ~_String(){};
	_String(const TCHAR* pszSrc) : std::string(pszSrc){}
	_String(const std::string& strSrc) : std::string(strSrc){}
	_String(const _String& strSrc) : std::string(strSrc){}

	// Assignment operators
	_String& operator=(const TCHAR* pszSrc){
		return (_String&)std::string::operator=(pszSrc);
		}

	_String& operator=(const std::string& strSrc){
		return (_String&)std::string::operator=(strSrc);
		}
	_String& operator=(TCHAR ch){
		return (_String&)std::string::operator=(ch);
		}

	bool IsEmpty() const{
		return (length() == 0);
		}

	int CompareNoCase(const _String& compareTo) const{
		return -1;
	}

	int CompareNoCase(const TCHAR* compareTo) const{
		return -1;
	}

	bool Delete(int iPos, int length = 1) {
		return false;
	}

	TCHAR GetAt(int iPos) {
		return at(iPos);
	}

	bool InsertAt(int iPos, TCHAR* pszInsert) {
		return false;
	}

	// Append
	/*
	_String& operator+=(const std::string& strSrc){
		return (_String&)std::string::operator+=(strSrc);
		}*/

	_String& operator+=(const TCHAR* pszRight){
		return (_String&)std::string::operator+=(pszRight);
		}

	_String& operator+=(const _String& pszRight) {
		return (_String&)std::string::operator+=(pszRight);
	}

	_String& operator+=(TCHAR ch){
		return (_String&)std::string::operator+=(ch);
		}

	//Comparision
	 friend bool operator==(const _String& str1, const _String& str2){
		return( str1.compare(str2) == 0 );
		}

	friend bool operator==(const _String& str1,const TCHAR* psz2){
		return( str1.compare(psz2) == 0 );
		}

	friend bool operator==(TCHAR* psz1, const _String& str2){
		return( str2.compare(psz1) == 0 );
		}

	friend bool operator!=(const _String& str1, const _String& str2){
		return( str1.compare(str2) != 0 );
		}

	friend bool operator!=(const _String& str1,const TCHAR* psz2){
		return( str1.compare(psz2) != 0 );
		}

	friend bool operator!=(TCHAR* psz1, const _String& str2){
		return( str2.compare(psz1) != 0 );
		}

	friend bool operator!=(_String& str1, _String& str2){
		return( str1.compare( str2 ) != 0 );
		}

	// Operations
	_String& MakeUpper(){
		for(std::string::iterator i=begin(); i!=end(); ++i)
			*i = toupper(*i);
		return *this;
		}

	_String& MakeLower(){
		for(std::string::iterator i=begin(); i!=end(); ++i)
			*i = tolower(*i);
		return *this;
		}

	TCHAR* GetBuffer() const{
		return (TCHAR*)c_str();
		}

	int GetLength(){
		return length();
		}

	int Remove(TCHAR ch){
		//erase(std::remove_if(begin(), end(), [](TCHAR c){return (c == 'a');}), end());
		int nLen = length();
		erase(std::remove(begin(), end(), ch), end());
		return (nLen - length());
		}

	int Replace(TCHAR chOld, TCHAR chNew){
		std::replace(begin(), end()-sizeof(TCHAR), chOld, chNew); 
		return 0;
		}

	void Empty() throw(){
		empty();
		}

	_String& TrimLeft(){
		return *this;
		}

	_String& TrimRight(){
		return *this;
		}

	_String& Trim(){
		return *this;
		}

	int Find(const TCHAR* pszSub, int iStart = 0){
		return find(pszSub, iStart);
		}

	int ReverseFind(TCHAR ch){
		return rfind(ch);
		}

	_String Left(int nCount){
		return substr(0, nCount);
		}

	_String Right(int nCount){
		std::string result;
		int len = length();
		if( len < nCount )
			return *this;
		else
			return Mid(len - nCount, nCount);
		}

	_String Mid(int iFirst, int nCount){
		return substr(iFirst, nCount);
		}

	void Format(const TCHAR* pszFormat, ...){
		/*
		va_list argList;
		va_start( argList, pszFormat );
		FormatV( pszFormat, argList );
		va_end( argList );
		*/

#if !defined(_TRUNCATE)
	#define _TRUNCATE ((size_t)-1)
#endif
		int final_n, n = ((int)_tcsclen(pszFormat)) * 2; /* reserve 2 times as much as the length of the fmt_str */
		std::string str;
		std::unique_ptr<char[]> formatted;
		va_list args;
		while(1) {
			va_start(args, pszFormat);
			formatted.reset(new TCHAR[n]); /* wrap the plain char array into the unique_ptr */
			//strcpy_s(&formatted[0], n, fmt_str);
			final_n = vsnprintf_s(&formatted[0], n, _TRUNCATE, pszFormat, args);
			va_end(args);
			if (final_n < 0 || final_n >= n)
				n += abs(final_n - n + 1);
			else
				break;
			}
		*this = formatted.get();
		}
};
#endif

#endif // !defined(PLATFORMSTRING_H_INCLUDED)

