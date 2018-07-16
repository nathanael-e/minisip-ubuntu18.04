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

#ifndef RTCPREPORTSDES_H
#define RTCPREPORTSDES_H

#include<libminisip/libminisip_config.h>

#include<libminisip/media/rtp/RtcpReport.h>
#include<libminisip/media/rtp/SDESChunk.h>

class LIBMINISIP_API RtcpReportSDES : public RtcpReport{
	public:
		RtcpReportSDES(void * build_from, int max_length);
		virtual ~RtcpReportSDES();
//		virtual vector<unsigned char> get_packet_bytes();
		int size();
		
#ifdef DEBUG_OUTPUT
		virtual void debug_print();
#endif
	private:
		std::vector<SDESChunk>chunks;
};

#endif
