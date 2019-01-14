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
#include<algorithm>
#include<fstream>
#include<chrono>

/* SSL Headers */

#include <openssl/ssl.h>
#include <openssl/err.h>

/* Shamir */
#include"shamir.h" 

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
            static std::string toString();
            bool operator==(int fd) const;
            int fd = 0;
            SSL* ssl;
            std::string ip;
            static std::vector<std::string> ips;
        };

        bool create_ssl_context();
        bool configure_ssl_context();
        std::vector<std::string> key_split(int n, int m);
        

        int server_socket, addrlen, new_socket, 
            activity, sd, max_sd, on=1, valread;

        int n_connections = 0;
        
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
