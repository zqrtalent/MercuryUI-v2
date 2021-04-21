#pragma once
#include "SocketServer.h"

#include "..\openssl\include\openssl\ssl.h"
#include "..\openssl\include\openssl\err.h"
#include "..\openssl\include\openssl\x509v3.h"

#ifndef ssl_lock
typedef CRITICAL_SECTION	ssl_lock;
#endif

#ifndef CRYPTO_dynlock_value
struct CRYPTO_dynlock_value {
	ssl_lock lock;
};
#endif

class SSLSocketServer : public SocketServer
{
public:
	SSLSocketServer			();
	virtual ~SSLSocketServer();

public:
	virtual bool					Start	(SockListenOptions* pInfo, SocketServerEventHandler* pHandler);
	virtual bool					Stop	();

protected:
	static ssl_lock**	GetCryptoLocks(int& nLockCt, int** pLockCtPtr = NULL){
		static ssl_lock* _cryptoLocks;
		static int _nCryptoLocksCt;
		nLockCt = _nCryptoLocksCt;
		if( pLockCtPtr )
			*pLockCtPtr = &_nCryptoLocksCt;
		return &_cryptoLocks;
		}

	static bool InitializeCryptoLocks(int nLockCt){
		int			nLocksCtOld = 0;
		int*		pLockCtPtr = NULL;
		ssl_lock**	ppLocks = GetCryptoLocks(nLocksCtOld, &pLockCtPtr);
		if( *ppLocks == NULL ){
			ssl_lock* pArrLocks = new ssl_lock[nLockCt];
			for(int i=0; i<nLockCt; i++)
				::InitializeCriticalSection(&pArrLocks[i]);
			*ppLocks	= pArrLocks;
			*pLockCtPtr = nLockCt;
			return true;
			}
		
		if( nLockCt == nLocksCtOld )
			return true;
		ASSERT(FALSE);
		return false;
		}

	static void DestroyCryptoLocks(){
		int			nLockCt		= 0;
		int*		pLockCtPtr	= NULL;
		ssl_lock**	ppLocks		= GetCryptoLocks(nLockCt, &pLockCtPtr);
		if( *ppLocks ){
			ssl_lock* pArrLocks = *ppLocks;
			for(int i=0; i<nLockCt; i++)
				::DeleteCriticalSection(&pArrLocks[i]);
			delete [] pArrLocks;
			*ppLocks	= NULL;
			*pLockCtPtr = 0;
			}
		}

	bool							SSL_Init							();
	bool							SSL_Destroy							();

	static void						ssl_lock_callback					(int mode, int n, const char *file, int line);
	static CRYPTO_dynlock_value*	ssl_lock_dyn_create_callback		(const char *file, int line);
	static void						ssl_lock_dyn_callback				(int mode, CRYPTO_dynlock_value* l, const char *file, int line);
	static void						ssl_lock_dyn_destroy_callback		(CRYPTO_dynlock_value* l, const char *file, int line);

protected: // Events
	/*
	virtual void					OnAcceptConnection					(SOCKET sock, CString sRemoteIp, int nRemotePort, bool& bAccept);
	virtual void					OnConnected							(SOCKET sock, ConnectedSocketInfo* pSockInfo);
	virtual void					OnDataReceived						(SocketContext* pConn, GrowableMemory* pMem, bool bIsFirstData);
	virtual void					OnDisconnected						(SocketContext* pConn, DWORD dwLastError);
	*/
private:
	SSL_CTX*	m_pssl_ctx;			// SSL context.
	ssl_lock	m_lock_connect_ex;	// SSL connect lock.
};


const char * ca_cert_key_pem = 

"Bag Attributes\n"
"    localKeyID: 25 3A 15 02 C8 DD E6 AD B2 DA F2 AD 89 84 6A 16 BA D1 B8 C5 \n"
"subject=/C=US/ST=CA/L=LA/O=Test Root CA/OU=IT/CN=www.testrootca.com\n"
"issuer=/C=US/ST=CA/L=LA/O=Test Root CA/OU=IT/CN=www.testrootca.com\n"
"-----BEGIN CERTIFICATE-----\n"
"MIIDozCCAougAwIBAgIJAPjAby1t84jRMA0GCSqGSIb3DQEBBQUAMGgxCzAJBgNV\n"
"BAYTAlVTMQswCQYDVQQIDAJDQTELMAkGA1UEBwwCTEExFTATBgNVBAoMDFRlc3Qg\n"
"Um9vdCBDQTELMAkGA1UECwwCSVQxGzAZBgNVBAMMEnd3dy50ZXN0cm9vdGNhLmNv\n"
"bTAeFw0xMzAyMjcxODE5MzFaFw0yMzAyMjUxODE5MzFaMGgxCzAJBgNVBAYTAlVT\n"
"MQswCQYDVQQIDAJDQTELMAkGA1UEBwwCTEExFTATBgNVBAoMDFRlc3QgUm9vdCBD\n"
"QTELMAkGA1UECwwCSVQxGzAZBgNVBAMMEnd3dy50ZXN0cm9vdGNhLmNvbTCCASIw\n"
"DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAPp5lWbHbYPAzHFClxqukSMLwCfP\n"
"QuI/eR/VBwdVgGws4JfEkhQ+RB5flibnaxM0dovp21M/Hiayn68ZB8y/M89FLMzn\n"
"8kO0EiSgx01HXABEdzzzdUJcugXs+pRnSyBx7Ss38vvWu9qSZsE1q2b8TH6mQ29h\n"
"ZDFSxJwavcOiBJFDI4ktOWKf+eB7WIqG6Ria6v6fnfT3ed2PhaaK6s5FJULJZxc+\n"
"0tgLiKKxTuGzoJiSdlvYqN9BVZPBxtA42c/lmsk3YWc1jO79gXcwNf4gQFnn5Spb\n"
"zeEM3LGhJR7P2bAdGuc7kwjyEoq4lqrMgJMcKCgjIq1eVRKi1+7hdOlQjAECAwEA\n"
"AaNQME4wHQYDVR0OBBYEFOA4FxeNdBAXZELBXkVnT/eUC907MB8GA1UdIwQYMBaA\n"
"FOA4FxeNdBAXZELBXkVnT/eUC907MAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQEF\n"
"BQADggEBAHvqLdE8Ga/U3pzlcbndjcyywcumiTimnx0QeazY/yHTs+qrza7gH224\n"
"CMuIyMdLbeFFylrTypuM5cU1KT7SV74nvdPUE2hmnXyq8lddhVWhpMp5aQLFnKxA\n"
"xyYEBuTtYqQLlBGlz4tZsa5Cb8C5XZTmlv32loQ3ho0bWACAwdoa3AmAzxHJkkkF\n"
"tMff2IzXtkLWrqjnfDLkFHeSHH07b9+rd4o92FWAX2JusmOWENNXyvFaL9YCdATz\n"
"36flXW4LTaJjumrfW+v2Ez0d1l6ZShoGh2/neG992i7gRAoEZQ0HDAsaTOpwbp4P\n"
"N/UU440JjdgBOJEhtuSmBiLCczAQN2c=\n"
"-----END CERTIFICATE-----\n"
"Bag Attributes\n"
"    localKeyID: 25 3A 15 02 C8 DD E6 AD B2 DA F2 AD 89 84 6A 16 BA D1 B8 C5 \n"
"Key Attributes: <No Attributes>\n"
"-----BEGIN PRIVATE KEY-----\n"
"MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQD6eZVmx22DwMxx\n"
"QpcarpEjC8Anz0LiP3kf1QcHVYBsLOCXxJIUPkQeX5Ym52sTNHaL6dtTPx4msp+v\n"
"GQfMvzPPRSzM5/JDtBIkoMdNR1wARHc883VCXLoF7PqUZ0sgce0rN/L71rvakmbB\n"
"Natm/Ex+pkNvYWQxUsScGr3DogSRQyOJLTlin/nge1iKhukYmur+n53093ndj4Wm\n"
"iurORSVCyWcXPtLYC4iisU7hs6CYknZb2KjfQVWTwcbQONnP5ZrJN2FnNYzu/YF3\n"
"MDX+IEBZ5+UqW83hDNyxoSUez9mwHRrnO5MI8hKKuJaqzICTHCgoIyKtXlUSotfu\n"
"4XTpUIwBAgMBAAECggEBALIYl3GQJb+D7BowVg7DxzEbHtNvnMgQh8hpiWjek8TX\n"
"GA7x10iLiZXrM3GQJONv70bGzotfmsm7Qq2W56Pe5DDhp9w+pFUdibT/mRbKwZDz\n"
"HbF6DotUMXUbLoUs+Q8l0pRjYONOqR3d8TA6QBinFqELCXH91ub3ShMRNJMQ9Zxu\n"
"r44AGmiamulabXPtY3Yj9Fugz9iNl91SfQE2MCxuI+Kl2goJ6jj+N/LMQyKNxqVw\n"
"zIwXL6JWh/sX39n4Z5rW1BU2l4d+zufzfW47OPyhY7NSbO7Z2Rb9sS61ueiEOVj8\n"
"tadvWoimDOZLsfVrNNs1Mtmctd7uTW7EHyUYW5Gspd0CgYEA/uY3KsLLnAkHhxlg\n"
"7xoMXJ37cD2B5epMch6SItaTzx/23kSrmAyHPtu7VQ6I92HjiKQFutEhQIm61jYM\n"
"S0Ifkz1013yFZlkwma6z6ojU04xR0jJuHqNRPuHcgKcWFKwN0qMfqIC3oBU4ww/L\n"
"wIlBtBsDoMFkbvY8PViG/QN5h9cCgYEA+456I7G3hwRYS72HIVCyMkijdNBX3w4H\n"
"4U3XlKie1mdUfTtDT8luph7QitThk9S9UnTO2YAc30N7FmkdNFAysHzDkq2+rea8\n"
"k3qJoHDTgdQ3Zm1L8J9QFQtR8/cjzcnBRL0CKAzkWN4VsVoM3Lll6PrTd4H66y/N\n"
"KnXKIn4tr+cCgYAJQ7jRlw7rdvCNGoEJb7rvStaut62W+7MXmsDY/0+UAadpYLmT\n"
"5zCrOjVO3B5iT26DsuJ6bct1mwvtaT19JsQbB+rNQjtfAvLfQxhK9/o0Ti9TIn4a\n"
"Ivl6/Z6UFn6Y9b/y3fbymusRqHPNsy9GqjZ3I1w4ZY8O8WgzGpqda5jpNQKBgQCV\n"
"yBn1kuXCINYqEumY7qGLyzB4Z21KxOHwRxcrztL0uoXWIylLIpcdlo95epPuH+ey\n"
"Zxaa7utCDL4HdAMNmQr1hvQ0K/uzNAXokjfMLiHh1eg+tqlW1qUqQplDeE37RDwt\n"
"n5TfH/3+5A1qkkHCUynY9TBgoiYzTz0MISKAMz0k1QKBgQChAgr35Hszuq4Crxjj\n"
"Uu3YP7ETPZ5vfwDd6XaR9Ds+kG5EswQ1qVqFLY4Py1rmFImi2L64HYX83sS1ZtWh\n"
"ve8K2IrrDe58r+x8ktqkQdJ630TYhIKmNJ0SGxuLLEL0uqfZw23ErCfUouUer0IO\n"
"+k/jFXw0HAlo2uWUrly5rFjOsw==\n"
"-----END PRIVATE KEY-----\n";

const char *server_cert_key_pem = 

"Bag Attributes\n"
"    localKeyID: D4 99 A5 A4 0A 78 18 E6 AC EC 55 1D B4 FE 4A 83 27 82 05 DE \n"
"subject=/C=US/ST=CA/L=LA/O=Test Company/OU=IT/CN=www.testserver.com\n"
"issuer=/C=US/ST=CA/L=LA/O=Test Root CA/OU=IT/CN=www.testrootca.com\n"
"-----BEGIN CERTIFICATE-----\n"
"MIIDRDCCAiwCAf8wDQYJKoZIhvcNAQEFBQAwaDELMAkGA1UEBhMCVVMxCzAJBgNV\n"
"BAgMAkNBMQswCQYDVQQHDAJMQTEVMBMGA1UECgwMVGVzdCBSb290IENBMQswCQYD\n"
"VQQLDAJJVDEbMBkGA1UEAwwSd3d3LnRlc3Ryb290Y2EuY29tMB4XDTEzMDIyNzE4\n"
"MjE1NloXDTIzMDIyNTE4MjE1NlowaDELMAkGA1UEBhMCVVMxCzAJBgNVBAgMAkNB\n"
"MQswCQYDVQQHDAJMQTEVMBMGA1UECgwMVGVzdCBDb21wYW55MQswCQYDVQQLDAJJ\n"
"VDEbMBkGA1UEAwwSd3d3LnRlc3RzZXJ2ZXIuY29tMIIBIjANBgkqhkiG9w0BAQEF\n"
"AAOCAQ8AMIIBCgKCAQEAzkHv+S30g5Dc+F1RJ1PUq9Hbh1YkEUJdYEj7ti+UfONV\n"
"NOT24hXzg8zaNSVO2Bhm+l8vzOVYMnjK9xcGSq5R5I633+lEeFdxURfsSJv9Vymq\n"
"tHUj5eNkmjzWBVrf4HvnZTJtRJljs941zYUgyJT9tkQXaerGFKJ6sfdXYfhGrkuK\n"
"gA1e71TwpRFYcfyYbQ3htENTh2CFBv7l5gjrITcmEJwpcU3U4nx4ZTr0IPLmV2kr\n"
"K8IJysY4dqgRcmduEI5ZgbYGkdG8L7QjggFXA6QNDPu8DfmXeeqS0gIffEm22bk7\n"
"b2fMnPfnFsJLsDdyhgrdYktnWhtZNij0y80tV4YCTwIDAQABMA0GCSqGSIb3DQEB\n"
"BQUAA4IBAQDMLn9VnUQt6BWx73J1lExYO/LWulMOnMR/WSVFy9dSwry+E807ekMY\n"
"WC8b3gpgDIqfkZjmttE9VtAdss2Baten+oBW+K13339sxHvcn30OxOs/Bln0yvaZ\n"
"Be+Zir7iE450b1IdYI98PMTSKgrK2e3vx/uUOCgG2yvs6/1v5rz5er/M1SQNzdMS\n"
"blelHWRQ1/ExwoUWBfIBkx/A4lTPmLgoC9fnXSiLhHKbZdfCJD8KLzEV0Se+ocn/\n"
"vl+6tlcUznap0TsRQpC67T/NGUimxdAhb6G1/U6z9bq0QQIuDxpOIpvwIgLvfRFx\n"
"qZQxmxOcK28fejHngmek7ZJNYKQbNewP\n"
"-----END CERTIFICATE-----\n"
"Bag Attributes\n"
"    localKeyID: D4 99 A5 A4 0A 78 18 E6 AC EC 55 1D B4 FE 4A 83 27 82 05 DE \n"
"Key Attributes: <No Attributes>\n"
"-----BEGIN PRIVATE KEY-----\n"
"MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDOQe/5LfSDkNz4\n"
"XVEnU9Sr0duHViQRQl1gSPu2L5R841U05PbiFfODzNo1JU7YGGb6Xy/M5VgyeMr3\n"
"FwZKrlHkjrff6UR4V3FRF+xIm/1XKaq0dSPl42SaPNYFWt/ge+dlMm1EmWOz3jXN\n"
"hSDIlP22RBdp6sYUonqx91dh+EauS4qADV7vVPClEVhx/JhtDeG0Q1OHYIUG/uXm\n"
"COshNyYQnClxTdTifHhlOvQg8uZXaSsrwgnKxjh2qBFyZ24QjlmBtgaR0bwvtCOC\n"
"AVcDpA0M+7wN+Zd56pLSAh98SbbZuTtvZ8yc9+cWwkuwN3KGCt1iS2daG1k2KPTL\n"
"zS1XhgJPAgMBAAECggEAIT83s27Y7yw2skI4hqJYsamOPW6BOdb8vjyFdoSM5uSu\n"
"I2yU7zSioCgxNEfjQaoNT2ZwihKd+OTHsrSfawJWaQUoVot/YfaWaX/1sm6Sk64/\n"
"uf733mKdIM+VoB9Z3xGZ5xIN0vT2wVOcUJiZBDwf+XVYYNZbP5BBPtaj20LuAcIZ\n"
"OmW9uigdXQkQ1dylUkRPitjJ92bbysrTr621JTBSmvKnF7ctcF/Ql6VfS5RcqzYI\n"
"6U1vozoFkjmUnExlYZHC6qKCFG73Z+IcC7ojdMpzMp4/EqiveV/9EVdFlLRB1YAa\n"
"tND93xU9mo7L26XQzy79Xf2dWRUgUvaJ/7EvLA1RoQKBgQD2ZhJ9ogqfQ0ahq0D6\n"
"5neZo6bPbckEKshv1GKR5ixnYpPp1kCIxM8oIzb9fOvTX4MOMeRzPJyrJNwhVgfY\n"
"otWLrvkNviGHXN0frmkdj/Y/WSWh7clzzwXmGbB/8NPG4yzREvQ8vhKBkAmZln6K\n"
"ICl8J5NxOxF6GgYJ793GcsfZVQKBgQDWS3DYMVQ3eRgFajkQ/8+Gacgdu+8/SyM1\n"
"WptHOlPvKfqg3nZYPlAjMnVmk0Q7l/d2EtFBPP07/Jz0IvC/pMz0S8XfW/NigcRn\n"
"0R5Nci3BXbmQEjxNGt0m0sX4C4/Bx8ei8pugipX96OemT/bWP05RskL6tWsofGsb\n"
"8zgIQcldEwKBgCyx90iyzBp3qahJ2E+q3qcP+IJH9965pAIlFHxCtGtMhmg0ZSBq\n"
"EunE+YSh1GVTPgKlKjt9Ey44UXX6lRHG99WOt762bn6Pac0FZivmoVR8Z0coSxKm\n"
"yvsiTdHnbYL2UnraZVNfZxv5dMRXeDy1+NB8nVI81L7BWbcTu7bzuyzBAoGAY0j4\n"
"s3HHbxwvwPKCFhovcDs6eGxGYLDTUzjzkIC5uqlccYQgmKnmPyh1tFyu1F2ITbBS\n"
"O0OioFRd887sdB5KxzUELIRRs2YkNWVyALfR8zEVdGa+gYrcw8wL5OyWYlXJbPmy\n"
"mSMcc1OhYDDUUFdsVfWdisLbLxrWFVEOuOSiAvkCgYEA2viHsxoFxOrhnZQOhaLT\n"
"RPrgaSojv9pooHQ6fJwplewt91tb1OchDIeZk9Sl1hqPAXB0167of43GDOw2vfnq\n"
"Ust7RtiyJhQhSkz0qp4aH4P9l+dZJIWnpgjcyWkcz893br9gEuVnQgh13V/lcxOn\n"
"JtpaCFuHNTU3PcFiuQW+cN0=\n"
"-----END PRIVATE KEY-----\n";