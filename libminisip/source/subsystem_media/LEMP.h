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
#include <sys/select.h>

/* SSL Headers */

#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAX_CLIENTS 30

class LEMP
    :public Runnable
{
    	public:
 
        LEMP();
        LEMP(LEMP const&) = delete;
        void operator=(LEMP const&) = delete;
        ~LEMP();
        bool start();
        bool init();
        void stop();
        void join();
        virtual void run();
        void setMikey(MRef<Mikey*>);

    private:

        struct Client
        {
            int fd = 0;
            SSL* ssl;
        };

        bool create_ssl_context();
        bool configure_ssl_context();
        

        int server_socket, addrlen, new_socket, 
            activity, sd, max_sd, on=1, valread;
        
        struct sockaddr_in address;
        struct timeval timeout;
        
        char buffer[4096];
        fd_set readfds;
        std::vector<Client> client_sockets;

        SSL_CTX* ctx;
        
        bool hasEnded = false;
        MRef<Thread*> thread;
        MRef<Mikey*> mikey;
};

#endif /*LEMP.h*/
