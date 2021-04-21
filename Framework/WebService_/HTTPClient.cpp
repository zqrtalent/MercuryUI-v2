#include "StdAfx.h"
#include "HTTPClient.h"
#include "../Utility/StringHelper.h"
#include "../Array/StringToStringArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HTTPClient::HTTPClient(void){
	}

HTTPClient::~HTTPClient(void){
	}

int
HTTPClient::ParseHTTPRequest(_String* psHTTPReq, StringToStringArray* pResult){
	CStringVector	arrHTTPHeaders;
	StringHelper::Split(psHTTPReq, "\r\n", arrHTTPHeaders);

	_String s, sKey, sValue;
	for(int i=0; i<arrHTTPHeaders.size(); i++){
		s = arrHTTPHeaders[i];
		if( s.IsEmpty() )
			continue;
		int nFind = s.Find(':');
		if( nFind == -1 ){
			nFind = s.Find(' ');
			if( nFind != -1 )
				pResult->Add(s.Mid(0, nFind), s);
			}
		else{
			sKey	= s.Mid(0, nFind);
			sValue	= s.Mid(nFind + 1, s.GetLength() - nFind);
			sKey	.Trim();
			sValue	.Trim();
			pResult->Add(sKey, sValue);
			}
		}

	return pResult->GetCount();
	}

BOOL
HTTPClient::SplitRequest(_String sRequestURL, _String& sHost, _String& sPageURL){
	if( sRequestURL.IsEmpty() )
		return FALSE;

	if( !sRequestURL.Left(7).CompareNoCase(_T("http://")) )
		sRequestURL.Delete(0, 7);

	int nFind = sRequestURL.Find('/');
	if( nFind == -1 )
		return FALSE;

	sHost		= sRequestURL.Left(nFind);
	sPageURL	= sRequestURL.Right(sRequestURL.GetLength() - sHost.GetLength());
	return TRUE;
	}

BOOL
HTTPClient::ParseResponse(_String& sResponse, int& nContentLength, int& nErrorCode){
	if( sResponse.IsEmpty() )
		return FALSE;

	int nFind = sResponse.Find("\r\n", 0);
	if( nFind == -1 )
		return FALSE;

	_String sLine;
	int nLoop = 0;
	while( nFind != -1 ){
		sLine = sResponse.Mid(0, nFind + 2);
		sLine.MakeLower();
		// Remove line.
		sResponse.Delete(0, nFind + 2);

		if( nLoop == 0 ){
			if( sLine.Find("http/1.1") == 0 ){
				sLine.Delete(0, 9);
				nErrorCode = atoi(sLine.GetBuffer());
				if( nErrorCode != 200 )
					return FALSE;
				}
			else
				return FALSE;
			}

		if( sLine.Find("content-length: ") != -1 ){
			sLine.Replace("content-length: ", "");
			nContentLength = atoi(sLine.GetBuffer());
			sResponse.TrimLeft();
			sResponse.TrimRight();
			return TRUE;
			}

		nFind	= sResponse.Find("\r\n");
		nLoop	++;
		}
	return FALSE;
	}

BOOL
HTTPClient::Post(_String sRequest, int nPort, _String sPostParam,  _String& sResponse){
	_String sHost, sPost;
	if( !SplitRequest(sRequest, sHost, sPost) )
		return FALSE;

	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( clientSock == 0 )
		return FALSE;

	// Resolve host address.
	hostent* pHost = gethostbyname(sHost);
	unsigned long ipAddr = *((unsigned long*)((char*)pHost->h_addr_list[0]));
	// }}

	sockaddr_in conn;
	conn.sin_family			= AF_INET;
	conn.sin_addr.s_addr	= ipAddr;
	conn.sin_port			= htons(nPort);

	// Connect to server.
	if( connect(clientSock, (SOCKADDR*) &conn, sizeof(conn)) == SOCKET_ERROR){
		closesocket(clientSock);
		return FALSE;
		}

	// Send POST request. ######################
	_String s;
	s.Format(_T("Content-Length:%d \r\n\r\n"), sPostParam.GetLength());
	CStringArray strArrSend;
	strArrSend.Add("POST " + sPost + " HTTP/1.1\r\n");
	strArrSend.Add("Host: " + sHost + " \r\n");
	strArrSend.Add("Content-Type: application/x-www-form-urlencoded\r\n");
	strArrSend.Add(s);
	strArrSend.Add(sPostParam);

	int nTryCt = 0;
	int nMaxTryCt = 3;
	int	nResult = 0;

	for( int i=0; i<strArrSend.GetCount(); i++ ){
		nResult = send(clientSock, strArrSend[i].GetBuffer(), strArrSend[i].GetLength(), 0);
		if( nResult == -1 ){
			shutdown(clientSock, SD_BOTH);
			closesocket(clientSock);
			return FALSE;
			}
		}
	// #######################################

try_to_receive_response:

	// Recive response. ######################
	fd_set readSocks;
	readSocks.fd_count = 1;
	readSocks.fd_array[0] = clientSock;

	timeval timeout = {5, 0};
	nResult = select(0, &readSocks, NULL, NULL, &timeout);
	if( nResult <= 0 ){ // No response
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}
	
	// Check recieved bytes length.
	int nLengthBytes = 0;
	nResult = ioctlsocket(clientSock, FIONREAD, (u_long*)&nLengthBytes);
	if( nResult != NO_ERROR ){
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}

	BYTE* lpBytesRead = new BYTE[nLengthBytes + 1];
	recv(clientSock, (char*)lpBytesRead, nLengthBytes, 0);
	lpBytesRead[nLengthBytes] = '\0';

	sResponse = (char*)lpBytesRead; 
	delete [] lpBytesRead;

	int nContentLength = 0;
	int nErrorCode = 0;
	if( !ParseResponse(sResponse, nContentLength, nErrorCode) || nContentLength <= 0 ){
		// HTTP 100 Continue.
		if( nErrorCode == 100 ){
			goto try_to_receive_response;
			//goto send_again;
			}
		sResponse.Empty();
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}

	// Response XML included in header.
	if( sResponse.GetLength() > 0 ){
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 1;
		}

	// Wait while date receive.
	nResult = select(0, &readSocks, NULL, NULL, &timeout);
	if( nResult <= 0 ){ // No response
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}

	// Check recieve data length.
	nResult = ioctlsocket(clientSock, FIONREAD, (u_long*)&nLengthBytes);
	if( nResult != NO_ERROR ){
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}
	
	lpBytesRead = new BYTE[nLengthBytes + 1];
	recv(clientSock, (char*)lpBytesRead, nLengthBytes, 0);
	lpBytesRead[nLengthBytes] = '\0';
	sResponse = (char*)lpBytesRead; 
	delete [] lpBytesRead;
	// #######################################

	shutdown(clientSock, SD_BOTH);
	closesocket(clientSock);
	return 1;
	}

BOOL
HTTPClient::GetResponse(_String sRequest, int nPort, _String& sResponse){
	_String sHost, sGet;
	if( !SplitRequest(sRequest, sHost, sGet) )
		return FALSE;

	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( clientSock == 0 )
		return FALSE;

	// Resolve host address.
	hostent* pHost = gethostbyname(sHost);
	unsigned long ipAddr = *((unsigned long*)((char*)pHost->h_addr_list[0]));
	// }}

	sockaddr_in conn;
	conn.sin_family			= AF_INET;
	conn.sin_addr.s_addr	= ipAddr;
	conn.sin_port			= htons(nPort);

	// Connect to server.
	if( connect(clientSock, (SOCKADDR*) &conn, sizeof(conn)) == SOCKET_ERROR){
		closesocket(clientSock);
		return FALSE;
		}

	// Send GET request. ######################
	CStringArray strArrSend;
	strArrSend.Add("GET " + sRequest + " HTTP/1.1\r\n");
	strArrSend.Add("Host: " + sHost + " \r\n");
	strArrSend.Add("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8 \r\n");
	strArrSend.Add("Accept-Encoding: gzip, deflate \r\n");
	strArrSend.Add("Connection: keep-alive \r\n\r\n");

	int nTryCt		= 0;
	int nMaxTryCt	= 3;
	int	nResult		= 0;

send_again:	
	for( int i=0; i<strArrSend.GetCount(); i++ ){
		nResult = send(clientSock, strArrSend[i].GetBuffer(), strArrSend[i].GetLength(), 0);
		if( nResult == -1 ){
			shutdown(clientSock, SD_BOTH);
			closesocket(clientSock);
			return FALSE;
			}
		}
	// #######################################

try_to_receive_response:
	// Recive response. ######################
	fd_set readSocks;
	readSocks.fd_count = 1;
	readSocks.fd_array[0] = clientSock;

	timeval timeout = {30, 0};
	nResult = select(0, &readSocks, NULL, NULL, &timeout);
	if( nResult <= 0 ){ // No response
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}
	
	// Check recieved bytes length.
	int nLengthBytes = 0;
	nResult = ioctlsocket(clientSock, FIONREAD, (u_long*)&nLengthBytes);
	if( nResult != NO_ERROR ){
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}

	BYTE* lpBytesRead = new BYTE[nLengthBytes + 1];
	recv(clientSock, (char*)lpBytesRead, nLengthBytes, 0);
	lpBytesRead[nLengthBytes] = '\0';

	sResponse = (char*)lpBytesRead; 
	delete [] lpBytesRead;

	int nContentLength = 0;
	int nErrorCode = 0;
	if( !ParseResponse(sResponse, nContentLength, nErrorCode) || nContentLength <= 0 ){
		// HTTP 100 Continue.
		if( nErrorCode == 100 ){
			goto try_to_receive_response;
			//goto send_again;
			}
		sResponse.Empty();
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}

	// Response XML included in header.
	if( sResponse.GetLength() > 0 ){
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 1;
		}

	// Wait while date receive.
	nResult = select(0, &readSocks, NULL, NULL, &timeout);
	if( nResult <= 0 ){ // No response
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}

	// Check recieve data length.
	nResult = ioctlsocket(clientSock, FIONREAD, (u_long*)&nLengthBytes);
	if( nResult != NO_ERROR ){
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}
	
	lpBytesRead = new BYTE[nLengthBytes + 1];
	recv(clientSock, (char*)lpBytesRead, nLengthBytes, 0);
	lpBytesRead[nLengthBytes] = '\0';
	sResponse = (char*)lpBytesRead; 
	delete [] lpBytesRead;
	// #######################################

	shutdown(clientSock, SD_BOTH);
	closesocket(clientSock);
	return 1;
	}