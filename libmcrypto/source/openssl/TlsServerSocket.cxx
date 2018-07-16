/*
  Copyright (C) 2005, 2004 Erik Eliasson, Johan Bilien
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
 * Authors: Erik Eliasson <eliasson@it.kth.se>
 *          Johan Bilien <jobi@via.ecp.fr>
*/


#include<config.h>

#include<libmcrypto/openssl/TlsServerSocket.h>
#include<libmcrypto/openssl/TlsSocket.h>
#include<libmcrypto/openssl/cert.h>

#ifdef WIN32
#	include<winsock2.h>
#else
#include<sys/socket.h>
#endif

#ifndef _WIN32_WCE
#	include<openssl/err.h>
#endif


#include<libmutil/merror.h>
#include<libmutil/massert.h>

#ifdef DEBUG_OUTPUT
#include<iostream>
#endif

using namespace std;

#ifdef WIN32
typedef int socklen_t;
#endif


TLSServerSocket::TLSServerSocket()
		:ServerSocket()
{
}

TLSServerSocket::~TLSServerSocket()
{
}

TLSServerSocket *TLSServerSocket::create( MRef<ServerSocket*> sock, MRef<Certificate *> cert, MRef<CertificateSet *> cert_db ){
	MRef<OsslCertificate*> ssl_cert;
	MRef<OsslCertificateSet*> ssl_db;

	if( cert )
		ssl_cert = (OsslCertificate*)*cert;

	if( cert_db )
		ssl_db = (OsslCertificateSet*)*cert_db;

	return new OsslServerSocket( sock, ssl_cert, ssl_db );
}



OsslServerSocket::OsslServerSocket( MRef<ServerSocket*> sock, MRef<OsslCertificate *> cert_, MRef<OsslCertificateSet *> cert_db_)
{
	init(sock, cert_, cert_db_);
}

void OsslServerSocket::init( MRef<ServerSocket*> sock, 
			    MRef<OsslCertificate *> cert,
			    MRef<OsslCertificateSet *> cert_db_)
{
	type = MSOCKET_TYPE_TLS;
	this->cert_db = cert_db_;
	this->sock = sock;
	const SSL_METHOD * meth;
	const unsigned char * sid_ctx = (const unsigned char *)"Minisip TLS";
	
	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();
	meth = SSLv23_server_method();
	this->ssl_ctx = SSL_CTX_new( meth );

	if( ssl_ctx == NULL ){
#ifdef DEBUG_OUTPUT
		cerr << "Could not initialize SSL context" << endl;
#endif

		exit( 1 );
	}

	if( OsslSocket::sslCipherListIndex != 0 ) 
		OsslSocket::setSSLCTXCiphers ( this->ssl_ctx, OsslSocket::sslCipherListIndex );
	/* Set options: do not accept SSLv2*/
	SSL_CTX_set_options(ssl_ctx, SSL_OP_NO_SSLv2);
	
	SSL_CTX_set_verify( ssl_ctx, SSL_VERIFY_PEER | SSL_VERIFY_CLIENT_ONCE, 0);
	//SSL_CTX_set_verify( ssl_ctx, SSL_VERIFY_NONE, 0);
	SSL_CTX_set_verify_depth( ssl_ctx, 5);
	
	//SSL_CTX_set_session_cache_mode( ssl_ctx, SSL_SESS_CACHE_BOTH );
	SSL_CTX_set_session_cache_mode( ssl_ctx, SSL_SESS_CACHE_SERVER );
	SSL_CTX_set_session_id_context( ssl_ctx, sid_ctx, (unsigned int)strlen( (const char *)sid_ctx ) );

	if( !cert_db.isNull() ){
		/* Use this database for the Certificates check */
		SSL_CTX_set_cert_store( this->ssl_ctx, this->cert_db->getDb());
	}
	
	MRef<PrivateKey *> PrivateKey = cert->getPk();
	MRef<OsslPrivateKey *> Osslpk =
	  dynamic_cast<OsslPrivateKey*>(*PrivateKey);
	if( SSL_CTX_use_PrivateKey( ssl_ctx, Osslpk->getOpensslPrivateKey() ) <= 0 ){
#ifdef DEBUG_OUTPUT
		cerr << "Could not use the given private key" << endl;
#endif

		ERR_print_errors_fp(stderr);
		exit( 1 );
	}
	
		
	if( SSL_CTX_use_certificate( ssl_ctx, cert->getOpensslCertificate() ) <= 0 ){
#ifdef DEBUG_OUTPUT
		cerr << "Could not use the given Certificate" << endl;
#endif

		ERR_print_errors_fp(stderr);
		exit( 1 );
	}

	if( !SSL_CTX_check_private_key( ssl_ctx ) ){
#ifdef DEBUG_OUTPUT
		cerr << "Given private key does not match the Certificate"<<endl;
#endif

		exit( 1 );
	}
}

MRef<StreamSocket *> OsslServerSocket::accept(){
	MRef<StreamSocket *> ssocket = sock->accept();

	return new OsslSocket( ssocket, ssl_ctx );
}

MRef<StreamSocket *> OsslServerSocket::createSocket( int32_t sd,
						     struct sockaddr *sa,
						     int32_t salen ){
	// Unused
	return NULL;
}

int32_t OsslServerSocket::getFd(){
	return sock->getFd();
}

int32_t OsslServerSocket::getPort(){
	return sock->getPort();
}

int OsslServerSocket::getAddressFamily(){
	return sock->getAddressFamily();
}

MRef<IPAddress *> OsslServerSocket::getLocalAddress() const{
	return sock->getLocalAddress();
}
