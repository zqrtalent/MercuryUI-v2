#include "HTTPClient.h"

HTTPClient::HTTPClient(void){
	}

HTTPClient::~HTTPClient(void){
	}

BOOL
HTTPClient::SplitRequest(std::string sRequestURL, std::string& sHost, std::string& sPageURL){
	if( !sRequestURL.length() )
		return FALSE;

	if( !sRequestURL.substr(0, 7).compare(_T("http://")) )
		sRequestURL.erase(0, 7);

	int nFind = sRequestURL.find('/');
	if( nFind == -1 )
		return FALSE;

	sHost		= sRequestURL.substr(0, nFind);
	sPageURL	= stringEx::Right(sRequestURL, sRequestURL.length() - sHost.length());
	return TRUE;
	}

BOOL
HTTPClient::ParseResponse(std::string& sResponse, int& nContentLength, int& nErrorCode){
	if( !sResponse.length() )
		return FALSE;

	int nFind = sResponse.find("\r\n", 0);
	if( nFind == -1 )
		return FALSE;

	std::string sLine;
	int nLoop = 0;
	while( nFind != -1 ){
		sLine = sResponse.substr(0, nFind + 2);
		stringEx::MakeLower(sLine);
		// Remove line.
		sResponse.erase(0, nFind + 2);

		if( nLoop == 0 ){
			if( sLine.find("http/1.1") == 0 ){
				sLine.erase(0, 9);
				nErrorCode = atoi(sLine.c_str());
				if( nErrorCode != 200 )
					return FALSE;
				}
			else
				return FALSE;
			}

		if( sLine.find("content-length: ") != -1 ){
			//Temp
			//sLine.replace("content-length: ", "");
			nContentLength = atoi(sLine.c_str());
			stringEx::TrimLeft(sResponse);
			stringEx::TrimRight(sResponse);
			return TRUE;
			}

		nFind	= sResponse.find("\r\n");
		nLoop	++;
		}
	return FALSE;
	}

BOOL
HTTPClient::Post(std::string sRequest, int nPort, std::string sPostParam,  std::string& sResponse){
	std::string sHost, sPost;
	if( !SplitRequest(sRequest, sHost, sPost) )
		return FALSE;

	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( clientSock == 0 )
		return FALSE;

	// Resolve host address.
	hostent* pHost = gethostbyname(sHost.c_str());
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
	std::string s;
	stringEx::Format(s, _T("Content-Length:%d \r\n\r\n"), sPostParam.length());
	CStringArray strArrSend;
	strArrSend.push_back("POST " + sPost + " HTTP/1.1\r\n");
	strArrSend.push_back("Host: " + sHost + " \r\n");
	strArrSend.push_back("Content-Type: application/x-www-form-urlencoded\r\n");
	strArrSend.push_back(s);
	strArrSend.push_back(sPostParam);

	int nTryCt = 0;
	int nMaxTryCt = 3;
	int	nResult = 0;

	for( int i=0; i<strArrSend.size(); i++ ){
		nResult = send(clientSock, strArrSend[i].c_str(), strArrSend[i].length(), 0);
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
		sResponse.empty();
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}

	// Response XML included in header.
	if( sResponse.length() > 0 ){
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
HTTPClient::GetResponse(std::string sRequest, int nPort, std::string& sResponse){
	std::string sHost, sGet;
	if( !SplitRequest(sRequest, sHost, sGet) )
		return FALSE;

	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( clientSock == 0 )
		return FALSE;

	// Resolve host address.
	hostent* pHost = gethostbyname(sHost.c_str());
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
	strArrSend.push_back("GET " + sRequest + " HTTP/1.1\r\n");
	strArrSend.push_back("Host: " + sHost + " \r\n");
	strArrSend.push_back("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8 \r\n");
	strArrSend.push_back("Accept-Encoding: gzip, deflate \r\n");
	strArrSend.push_back("Connection: keep-alive \r\n\r\n");

	int nTryCt		= 0;
	int nMaxTryCt	= 3;
	int	nResult		= 0;

send_again:	
	for( int i=0; i<strArrSend.size(); i++ ){
		nResult = send(clientSock, strArrSend[i].c_str(), strArrSend[i].length(), 0);
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
		sResponse.empty();
		shutdown(clientSock, SD_BOTH);
		closesocket(clientSock);
		return 0;
		}

	// Response XML included in header.
	if( sResponse.length() > 0 ){
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