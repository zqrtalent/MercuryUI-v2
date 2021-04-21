#pragma once
#include "SocketLibraryDef.h"

class SSLContext 
{
public:
	SSLContext			();
	virtual ~SSLContext	();

	bool			Init				(X509 *pCert, EVP_PKEY *pPrivatekey);
	bool			Destroy				();
	SSL_CTX*		GetSSLContextHandle	(){return m_pssl_ctx;};

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

	static void						ssl_lock_callback					(int mode, int n, const char *file, int line);
	static CRYPTO_dynlock_value*	ssl_lock_dyn_create_callback		(const char *file, int line);
	static void						ssl_lock_dyn_callback				(int mode, CRYPTO_dynlock_value* l, const char *file, int line);
	static void						ssl_lock_dyn_destroy_callback		(CRYPTO_dynlock_value* l, const char *file, int line);

private:			
	SSL_CTX*			m_pssl_ctx;						// SSL context.
};
