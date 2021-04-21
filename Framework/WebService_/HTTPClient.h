#pragma once

#ifndef _WINSOCK2API_
	#include <winsock2.h>
#endif

class StringToStringArray;
class HTTPClient
{
public:
	HTTPClient(void);
	~HTTPClient(void);

	static int ParseHTTPRequest(_String* psHTTPReq, StringToStringArray* pResult);

public:
//	_String Get(_String sRequest, _String sProxyServer, _String sProxyPort);
	BOOL	Post		(_String sRequest, int nPort, _String sPostParam,  _String& sResponse);

	BOOL	GetResponse	(_String sRequest, int nPort, _String& sResponse);

protected:
	BOOL	SplitRequest(_String sRequestURL, _String& sHost, _String& sPageURL);
	BOOL	ParseResponse(_String& sResponse, int& nContentLength, int& nErrorCode);
};

/*
GET /modules/rating.php?prodid=111&romeli=5&ip=95.168.183.226 HTTP/1.1
Host: konkursebi.ge
User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*//*;q=0.8
Accept-Language: en-us,en;q=0.5
Accept-Encoding: gzip,deflate
Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
Keep-Alive: 115
Connection: keep-alive
Cache-Control: max-age=0
*/

/*
HTTP/1.1 200 OK
Date: Mon, 28 Feb 2011 06:11:43 GMT
Server: Apache
X-Powered-By: PHP/5.2.12
Vary: Accept-Encoding,User-Agent
Content-Encoding: gzip
Content-Length: 181
Keep-Alive: timeout=1, max=1000
Connection: Keep-Alive
Content-Type: text/html
X-Pad: avoid browser bug
*/