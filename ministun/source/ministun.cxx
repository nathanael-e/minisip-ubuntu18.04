/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* Copyright (C) 2004-2006
 *
 * Authors: Zachary T Welch <zach-minisip@splitstring.com>
 *          Erik Eliasson <eliasson@it.kth.se>
 *          Johan Bilien <jobi@via.ecp.fr>
*/

#include<libmnetutil/IPAddress.h>
#include<libmnetutil/UDPSocket.h>
#include<libmnetutil/NetworkFunctions.h>
#include<libmutil/TimeoutProvider.h>
#include<libmstun/STUN.h>

using namespace std;

int main(int argc, char **argv)
{
	MRef<IPAddress *> addr =
		IPAddress::create("stun.fwdnet.net", false);
	unsigned short stunPort = 3478;
	UDPSocket sock;

	vector<string> interfaces = NetworkFunctions::getAllInterfaces();
	vector<string> localAddr;
	for (uint32_t i; i < interfaces.size(); i++) {
		string &iface = interfaces[i];
		cerr << "i" << i << ": " << iface << endl;
		localAddr[i] = NetworkFunctions::getInterfaceIPStr(iface);
	}

	unsigned short testPort = 5060;
	char tmp[16];
	uint16_t mappedPort;
	int type = STUN::getNatType(**addr, stunPort, sock, 
			localAddr, testPort, tmp, mappedPort);
	cerr << "The NAT type is "<< STUN::typeToString(type)<< endl;
	cerr << "External mapping is: "<< tmp << ":" << mappedPort << endl;
	return 0;
}

