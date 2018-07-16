/*
  Copyright (C) 2005, 2004 Erik Eliasson, Johan Bilien
  Copyright (C) 2006 Mikael Magnusson

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
 *          Mikael Magnusson <mikma@users.sourceforge.net>
*/


#ifndef MLIBMCRYPTO_CERT_H
#define MLIBMCRYPTO_CERT_H

#include <libmcrypto/config.h>

#include<string>
#include<list>
#include<libmutil/Mutex.h>
#include<libmutil/MemObject.h>
#include<libmutil/Exception.h>
#include<libmutil/CacheItem.h>
#include<cstring>

#ifdef _MSC_VER
#pragma warning (disable: 4251)
#endif

class Certificate;
#if 0
#define CERT_DB_ITEM_TYPE_OTHER  0
#define CERT_DB_ITEM_TYPE_FILE   1
#define CERT_DB_ITEM_TYPE_DIR    2
#endif
class LIBMCRYPTO_API CertificatePair: public MObject {
	public:
		CertificatePair();
		CertificatePair(MRef<Certificate*> issuedToThisCA);
		CertificatePair(MRef<Certificate*> issuedToThisCA, MRef<Certificate*> issuedByThisCA);
		MRef<Certificate*> issuedToThisCA;
		MRef<Certificate*> issuedByThisCA;
};

class LIBMCRYPTO_API CertificateSetItem: public CacheItem {
	public:
		enum CERTSETITEM_IMPORTMETHOD {
			IMPORTMETHOD_OTHER = 0,
			IMPORTMETHOD_FILE = 1,
			IMPORTMETHOD_DIRECTORY = 2
		};
		//std::string item;
		//int type;

		virtual ~CertificateSetItem();

		CertificateSetItem();
		CertificateSetItem(std::string certUri);
		CertificateSetItem(MRef<Certificate*> cert);

		/*
		Getters
		*/
		std::string getSubject() const;
		std::vector<std::string> getSubjectAltNames() const;
		std::string getSubjectKeyIdentifier() const;

		std::string getIssuer() const;
		std::vector<std::string> getIssuerAltNames() const;
		std::string getIssuerKeyIdentifier() const;

		bool isSelfSigned() const;

		std::string getCertificateUri() const;
		MRef<Certificate*> getCertificate();

		CERTSETITEM_IMPORTMETHOD getImportMethod() const;
		std::string getImportParameter() const;

		/*
		Setters
		*/
		void setCertificate(const MRef<Certificate*> cert);
		void setCertificateUri(const std::string);

		void setImportMethod(const CERTSETITEM_IMPORTMETHOD type);
		void setImportParameter(const std::string param);

		/**
		 * Loads certificates specified by private variables \c certificateUri or \c certificate.
		 */
		void loadCertAndIndex();

		/**
		 * Load certificate contained in \p cert.
		 */
		//void loadToMemory(MRef<Certificate*> cert);

		/**
		 * Frees memory allocated by associated certificate data. The indexed information
		 * it still kept. This function is useful when loading an entire folder of
		 * certificates -- it is then probably not necessary/wanted to keep all certificates
		 * in memory. Thus, when a certificates has been loaded and indexes this function
		 * is called and the excess memory used by the actual certificate is released.
		 *
		 * If the \c certificateUri property has been set the certificate data can be reloaded
		 * later using the loadToMemory() function.
		 */
		void unloadCertificateFromMemory();

		bool operator ==(const CertificateSetItem item2){ return (
				item2.getImportMethod() == getImportMethod() &&
				item2.getImportParameter() == getImportParameter());};

		void reindexCert();

	private:
		/*
		Indexed attributes
		*/
		std::string subject;
		std::vector<std::string> subjectAltNames;
		std::string subjectKeyIdentifier;

		std::string issuer;
		std::vector<std::string> issuerAltNames;
		std::string issuerKeyIdentifier;

		/*
		Certificate status
		*/
		bool selfSigned;

		/*
		Information either linking directly to a instantiated certificate or providing
		an URI to where the certificate can be found.
		*/
		std::string certificateUri;
		MRef<Certificate*> certificate;

		/**
		 * Defines what method was used to load the Certificate in this CertificateSetItem.
		 *
		 * The methods can be described as "stored in file", "stored in directory" or "stored
		 * in memory". The idea behind storing this information is to simplify retrieval of
		 * user settings.
		 *
		 * When the Minisip application loads it automatically creates a certificate set
		 * containing all the client's trusted root certificates. Users can, in the GUI,
		 * specify either single certificate files or entire directories. The problem is that
		 * certificates are loaded one by one (certificates in a directory are not batch-added)
		 * and that when the user settings are to be written back to the user's configuration
		 * file there is no longer possible to determine if a particular certificate was loaded
		 * in a "directory batch" or a "single file".
		 *
		 * Since we still want to keep track of this kind of information, so that the
		 * configuration file can be properly created, we have this work-around that allows us
		 * to store additional information about "which method was used" to load the certificate.
		 */
		CERTSETITEM_IMPORTMETHOD importMethod;

		/**
		 * The interpretation of this property is dependent on the \c importMethod property.
		 *
		 * If \c importMethod is "file" then \c importParameter is the absolut path to the
		 * certifcate (meaning that it specifies the same file as \c certificateUri, but
		 * using a simpler format).
		 *
		 * If \c importMethod is "directory" then \c importParameters is the directory that
		 * the user has selected.
		 *
		 * If \c importMethod id "memory" then \c importParameter is ignored.
		 */
		std::string importParameter;

};


class LIBMCRYPTO_API CertificateSet: public MObject{
	public:
		virtual ~CertificateSet();
		static CertificateSet *create();

		virtual CertificateSet* clone();

		void addDirectory( std::string dir );
		MRef<CertificateSetItem*> addFile( std::string file );
		virtual MRef<CertificateSetItem*> addCertificate( MRef<Certificate *> cert );

		/**
		 * Returns the certificates within the set "matching the parameter \p searchFor".
		 *
		 * A certificate is considered to match the query if the subject or any of the
		 * subject alternative names contain the text \p searchFor.
		 */
		std::vector<MRef<CertificateSetItem*> > findItems(const std::string searchFor, const std::string issuer = "");

		virtual std::list<MRef<CertificateSetItem*> > &getItems();
		virtual MRef<CertificateSetItem*> getNext();
		virtual void initIndex();
		virtual void lock();
		virtual void unlock();

		virtual void remove( MRef<CertificateSetItem*> removedItem );

	protected:
		CertificateSet();
		virtual void addItem( MRef<CertificateSetItem*> item );
		//virtual MRef<CertificateSetItem*> createDirItem( std::string dir );
		//virtual MRef<CertificateSetItem*> createFileItem( std::string file );
		virtual MRef<CertificateSetItem*> createCertItem( MRef<Certificate*> cert );

	private:
		std::list<MRef<CertificateSetItem*> >::iterator items_index;
		std::list<MRef<CertificateSetItem*> > items;
                Mutex mLock;
};

class LIBMCRYPTO_API PrivateKey: public MObject{
	public:
		static PrivateKey* load( const std::string private_key_filename );
		static PrivateKey* load( char *derEncPk, int length,
				       std::string password,
				       std::string path );

		virtual ~PrivateKey();

		virtual const std::string &getFile() const = 0;

		//Note that the parameter is not a MRef because of
		//implementation issues. We want to allow
		//Certificate::setPk to be used on objects not using
		//MRefs. (If this parameter used MRefs, then the
		//cert is garbage collected if there is no other
		//reference to it)
		virtual bool checkCert( Certificate * cert)=0;

		virtual int signData( unsigned char * data, int data_length,
				       unsigned char * sign,
				       int * sign_length )=0;

		virtual int denvelopeData( unsigned char * data,
					    int size,
					    unsigned char *retdata,
					    int *retsize,
					    unsigned char *enckey,
					    int enckeylgth,
					    unsigned char *iv)=0;

		virtual bool privateDecrypt(const unsigned char *data, int size,
					     unsigned char *retdata, int *retsize)=0;
	protected:
		PrivateKey();
};

class LIBMCRYPTO_API Certificate: public MObject{
	public:
		enum SubjectAltName{
			SAN_DNSNAME = 1,
			SAN_RFC822NAME,
			SAN_URI,
			SAN_IPADDRESS
		};

		static Certificate* load( const std::string cert_filename );
		static Certificate* load( const std::string cert_filename,
					  const std::string private_key_filename );
		static Certificate* load( unsigned char * der_cert,
					  int length );
		static Certificate* load( unsigned char * certData,
					  int length,
					  std::string path );
// 		static Certificate *create();

		virtual ~Certificate();


		virtual int control( CertificateSet * cert_db )=0;

		virtual int getDerLength()=0;
		virtual void getDer( unsigned char * output,
				      unsigned int * length )=0;

		virtual int envelopeData( unsigned char * data,
					   int size,
					   unsigned char *retdata,
					   int *retsize,
					   unsigned char *enckey,
					   int *enckeylgth,
					   unsigned char** iv)=0;

		int denvelopeData( unsigned char * data,
					    int size,
					    unsigned char *retdata,
					    int *retsize,
					    unsigned char *enckey,
					    int enckeylgth,
					    unsigned char *iv);

		int signData( unsigned char * data, int data_length,
				       unsigned char * sign,
				       int * sign_length );
		virtual int verifSign( unsigned char * data, int data_length,
					unsigned char * sign, int sign_length )=0;

		virtual bool publicEncrypt(const unsigned char *data, int size,
					    unsigned char *retdata, int *retsize)=0;

		int privateDecrypt(const unsigned char *data, int size,
				    unsigned char *retdata, int *retsize);

		virtual std::string getName()=0;
		virtual std::string getCn()=0;
		virtual std::vector<std::string> getAltName(SubjectAltName type)=0;
		virtual std::vector<std::string> getSubjectInfoAccess()=0;
		virtual std::string getIssuer()=0;
		virtual std::string getIssuerCn()=0;


		bool verifySignedBy(MRef<Certificate*> cert);

		/**
		 * Returns whether or not at least one of the certificate's subjectAltNames
		 * are equal to \p uri.
		 */
		bool hasAltNameSipUri(std::string uri);
		bool hasAltName(std::string uri);
		bool hasAltName(std::string uri, SubjectAltName type);

		std::string getFile();
		std::string getPkFile();

		MRef<PrivateKey*> getPk();
		void setPk( MRef<PrivateKey *> pk);
		void setPk( const std::string &file );
		void setEncpk(char *derEncPk, int length,
			       const std::string &password,
			       const std::string &path);

		bool hasPk();

	protected:
 		Certificate();

		std::string file;

		MRef<PrivateKey *> m_pk;
};

class LIBMCRYPTO_API CertificateChain: public MObject{
	public:
		static CertificateChain* create();
		virtual ~CertificateChain();

		virtual CertificateChain* clone();
		virtual void addCertificate( MRef<Certificate *> cert );
		virtual void addCertificateFirst( MRef<Certificate *> cert );
// 		virtual void remove_Certificate( MRef<Certificate *> cert );
		virtual void removeLast();

		virtual int control( MRef<CertificateSet *> cert_db )=0;
		virtual MRef<Certificate *> getNext();
		virtual MRef<Certificate *> getFirst();
		virtual MRef<Certificate *> getLast();

		virtual void clear();

		virtual int length();
		virtual void lock();
		virtual void unlock();

		virtual bool isEmpty();

		virtual void initIndex();

	protected:
		CertificateChain();
		CertificateChain( MRef<Certificate *> cert );

		std::list< MRef<Certificate *> > cert_list;
		std::list< MRef<Certificate *> >::iterator item;
                Mutex mLock;
};

class LIBMCRYPTO_API CertificateException : public Exception{
	public:
		CertificateException( const char *desc):Exception(desc){};
};

class LIBMCRYPTO_API CertificateExceptionFile : public CertificateException{
	public:
		CertificateExceptionFile( const char *message ):CertificateException(message){};
};

class LIBMCRYPTO_API CertificateExceptionInit : public CertificateException{
	public:
		CertificateExceptionInit( const char *message ):CertificateException(message){};
};

class LIBMCRYPTO_API CertificateExceptionPkey : public CertificateException{
	public:
		CertificateExceptionPkey( const char *message ):CertificateException(message){};
};

class LIBMCRYPTO_API CertificateExceptionChain : public CertificateException{
	public:
		CertificateExceptionChain( const char *message ):CertificateException(message){};
};

#endif // MLIBMCRYPTO_CERT_H
