#include "stdafx.h"
#include "SSLContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	SSLContext.
//
/////////////////////////////////////////////////////////////////////

SSLContext::SSLContext(){
	m_pssl_ctx	= nullptr;
	} 

SSLContext::~SSLContext(){
	Destroy();
	}

bool
SSLContext::Init(X509 *pCert, EVP_PKEY *pPrivatekey){
	int nLockCt = CRYPTO_num_locks();
	InitializeCryptoLocks(nLockCt);

#ifdef _DEBUG
    CRYPTO_malloc_debug_init();
    CRYPTO_dbg_set_options	(V_CRYPTO_MDEBUG_ALL);
    CRYPTO_mem_ctrl			(CRYPTO_MEM_CHECK_ON);
#endif
	
	CRYPTO_set_locking_callback			(&ssl_lock_callback);
    CRYPTO_set_dynlock_create_callback	(&ssl_lock_dyn_create_callback);
	CRYPTO_set_dynlock_lock_callback	(&ssl_lock_dyn_callback);
    CRYPTO_set_dynlock_destroy_callback	(&ssl_lock_dyn_destroy_callback);

    SSL_load_error_strings	();
    SSL_library_init		();

	// Initialize and verify SSL context. {{
	const SSL_METHOD* meth = SSLv23_method();
	m_pssl_ctx = SSL_CTX_new(meth);
	SSL_CTX_set_verify(m_pssl_ctx, SSL_VERIFY_NONE, nullptr);
	// }}

#ifdef _SERVER
	SSL_CTX_set_options(m_pssl_ctx, SSL_OP_CIPHER_SERVER_PREFERENCE); 
#endif

	if( pCert )
		SSL_CTX_use_certificate	(m_pssl_ctx, pCert);
	if( pPrivatekey )
		SSL_CTX_use_PrivateKey	(m_pssl_ctx, pPrivatekey);
	return true;
	}

bool
SSLContext::Destroy(){
	if( !m_pssl_ctx ) 
		return false;
	SSL_CTX_free(m_pssl_ctx);
	m_pssl_ctx = NULL;

	CRYPTO_set_locking_callback			(NULL);
	CRYPTO_set_dynlock_create_callback	(NULL);
	CRYPTO_set_dynlock_lock_callback	(NULL);
	CRYPTO_set_dynlock_destroy_callback	(NULL);

    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data	();
    ERR_remove_state			(0);
    ERR_free_strings			();

	// Destroy crypto locks.
	DestroyCryptoLocks			();
	return true;
	}

void 
SSLContext::ssl_lock_callback(int mode, int n, const char *file, int line){
	int nLockCt = 0;
	ssl_lock** ppLocks = GetCryptoLocks(nLockCt, NULL);
	if( n >= nLockCt )
		ASSERT(FALSE);

	ssl_lock* pLocks = *ppLocks;
	if(mode & CRYPTO_LOCK)
		EnterCriticalSection(&pLocks[n]);
	else
		LeaveCriticalSection(&pLocks[n]);
	}

CRYPTO_dynlock_value* 
SSLContext::ssl_lock_dyn_create_callback(const char *file, int line){
	CRYPTO_dynlock_value *l = (CRYPTO_dynlock_value*)malloc(sizeof(CRYPTO_dynlock_value));
	InitializeCriticalSection(&l->lock);
	return l;
	}

void 
SSLContext::ssl_lock_dyn_callback(int mode, CRYPTO_dynlock_value* l, const char *file, int line){
	if(mode & CRYPTO_LOCK)
		EnterCriticalSection(&l->lock);
	else
		LeaveCriticalSection(&l->lock);
	}

void 
SSLContext::ssl_lock_dyn_destroy_callback(CRYPTO_dynlock_value* l, const char *file, int line){
	DeleteCriticalSection(&l->lock);
	free(l);
	}