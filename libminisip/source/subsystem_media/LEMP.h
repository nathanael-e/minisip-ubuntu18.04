#ifndef LEMP_H
#define LEMP_H

#include<iostream>

/* Minisip exsclusive headers */

#include<libmutil/MemObject.h>
#include<libmikey/Mikey.h>
#include<libmutil/Thread.h>

/* TCP Server Socket */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

class LEMP
    :public Runnable
{
	public:
		LEMP();
        bool start();
        void stop();
        void join();
        virtual void run();
        void setMikey(MRef<Mikey*>);

    private:
       
        socklen_t client_length; 
        int s, c, rc, on=1, ns;
        struct sockaddr_in server_address;
        struct sockaddr_in client_address;
        char buf[4096];
        std::vector<int> connections;

        bool running=true;
        MRef<Thread*> thread;
        void init();
        MRef<Mikey*> mikey;
};

#endif /*LEMP.h*/
