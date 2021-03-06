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
 * 	SdpHeaderT.h
 * Author
 * 	Erik Eliasson, eliasson@it.kth.se
 * Purpose
 * 
*/

#ifndef SDPHEADERT_H
#define SDPHEADERT_H

#include<libminisip/libminisip_config.h>

#include<libminisip/signaling/sdp/SdpHeader.h>


class LIBMINISIP_API SdpHeaderT : public SdpHeader{
	public:
		SdpHeaderT(std::string buildFrom);
		SdpHeaderT(int32_t start_time, int32_t stop_time);
		~SdpHeaderT();

		virtual std::string getMemObjectType() const {return "SdpHeaderT";}

		int32_t getStartTime();
		void setStartTime(int32_t time);
		
		int32_t getStopTime();
		void setStopTime(int32_t time);

		virtual std::string getString();

	private:
		int32_t start_time;
		int32_t stop_time;
};

#endif
