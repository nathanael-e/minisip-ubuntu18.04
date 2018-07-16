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


/* Name
 * 	SipTransaction.h
 * Author
 * 	Erik Eliasson, eliasson@it.kth.se
 * Purpose
 * 
*/


#ifndef SIPTRANSACTION_H
#define SIPTRANSACTION_H

#include<libmsip/libmsip_config.h>

#include<libmutil/StateMachine.h>
#include<libmutil/MemObject.h>
#include<libmsip/SipSMCommand.h>
#include<libmsip/SipDialogConfig.h>
#include"../SipLayerTransport.h"

class SipDialog;
class SipMessage;
class SipCommandDispatcher;
class SipLayerTransport;
class SipStackInternal;
class Socket;

/**
 * SipTransaction
 */
class SipTransaction : public StateMachine<SipSMCommand,std::string>{
	public:
		
		SipTransaction(MRef<SipStackInternal*> stackInternal, 
				int cseq, 
				const std::string &cseqMethod, 
				const std::string &branch, 
				const std::string &callid);
                
		virtual ~SipTransaction();

		/**
		 * Creates a new transaction and initiates it using
		 * the values of a request. This is used by the
		 * transaction layer to create transactions.
		 *
		 * @param stack All transactions operate within a SIP
		 *              stack.
		 * @param msg	SIP request that initiates a transaction
		 * @param fromTU  true if the request is generated by the
		 *                local UA (client transaction) and false if it has been 
		 *                received from a remote one (server transaction).
		 * @param handleAck  TRUE if the "UA" versions of the
		 *                   INVITE server transactions should be used.
		 */
		static MRef<SipTransaction*> create(MRef<SipStackInternal*> stackInternal, 
				MRef<SipRequest*> msg, 
				bool fromTU, 
				bool handleAck=false);
		
		virtual std::string getName()=0;

		virtual bool handleCommand(const SipSMCommand &command);

		virtual void handleTimeout(const std::string &c);
		
		std::string getBranch();
		void setBranch(std::string branch);

		std::string getTransactionId(){ return getBranch() + getCSeqMethod(); }
				
		void send(MRef<SipMessage*>  pack, bool addVia, std::string branch=""); // if not specified branch, use the attribute one - ok in most cases.
		void setSocket(Socket * sock);
		MRef<Socket *> getSocket();

		virtual std::string getMemObjectType() const;
		void setDebugTransType(std::string t);
		std::string getDebugTransType();

		int getCSeqNo();
		std::string getCSeqMethod();
                
		std::string getCallId();

		//The transition to cancel a transaction is common to all
		//transactions and is defined in this class.
		bool a1000_anyState_terminated_canceltransaction(const SipSMCommand &command);

		//FIXME: get the reliability correctly
		bool isUnreliable();

		
	protected:
		
		void setCSeqNo(int n){cSeqNo=n;}
		MRef<SipCommandDispatcher*> dispatcher;
		MRef<SipStackInternal*> sipStackInternal;
		MRef<SipLayerTransport*> transportLayer;
		MRef<Socket *> socket;
		
		std::string callId;
		
	private:
		int cSeqNo;
		std::string cSeqMethod;
		std::string branch;

		std::string debugTransType;
};


//All sub-classes of SipTransaction are local to libmsip
class SipTransactionClient: public SipTransaction{
        public:
                SipTransactionClient(MRef<SipStackInternal*> stackInternal, 
				int seq_no, 
				const std::string &cSeqMethod, 
				const std::string &branch, 
				const std::string &callid);
                ~SipTransactionClient();
};

class SipTransactionServer: public SipTransaction{
        public:
                SipTransactionServer(MRef<SipStackInternal*> stackInternal, 
				int seq_no, 
				const std::string &cSeqMethod, 
				const std::string &branch, 
				const std::string &callid);
                ~SipTransactionServer();
};

#endif
