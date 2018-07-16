/*
 Copyright (C) 2004-2006 the Minisip Team
 
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
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

/* Copyright (C) 2004 
 *
 * Authors: Erik Eliasson <eliasson@it.kth.se>
 *          Johan Bilien <jobi@via.ecp.fr>
*/

/* Name
 * 	SdpHeaderI.h
 * Author
 * 	Erik Eliasson, eliasson@it.kth.se
 * Purpose
 * 
*/

#ifndef SDPHEADERI_H
#define SDPHEADERI_H

#include<libminisip/libminisip_config.h>

#include<libminisip/signaling/sdp/SdpHeader.h>


class LIBMINISIP_API SdpHeaderI : public SdpHeader{
	public:
		SdpHeaderI(std::string buildFrom);
		virtual ~SdpHeaderI();
		
		virtual std::string getMemObjectType() const {return "SdpHeaderI";}

		std::string getSessionInformation();
		void setSessionInformation(std::string s);

		virtual  std::string getString();

	private:
		std::string session_information;

};

#endif
