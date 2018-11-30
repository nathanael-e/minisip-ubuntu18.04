#include"LEMP.h"

using namespace std::chrono;

std::string LEMP::Client::toString()
{
    std::stringstream ss;

    for(const auto& _ip_:ips)
        ss<<_ip_<<",";

    return ss.str();
}

bool LEMP::Client::operator==(int _fd_) const
{
    return fd ==_fd_;
}

LEMP::LEMP()
: client_sockets(MAX_CLIENTS){}

LEMP::~LEMP()
{
    SSL_CTX_free(ctx);
    EVP_cleanup();

    if(server_socket)
    {
        std::cout<<"Closing LEMP server"<<std::endl;
        close(server_socket);
    }

    std::cout<<"Destroying LEMP"<<std::endl;
}

bool LEMP::init()
{
    std::cout<<"Init new LEMP"<<std::endl;

    /*
     * Create the server socket.
     */

    if((server_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        std::cout<<"Failed to initialize LEMP socket"<<std::endl;
        return false;
    }

    /*
     * Adjust the server socket to allow multiple connections.
     */

    if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0 )
    {
        std::cout<<"Failed to adjust the LEMP socket·"<<std::endl;
        return false;
    }
    
    /*
     * Set the socket type.
     */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5000);

    /*
     * Bind the socket.
     */

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        std::cout<<"Failed to bind the LEMP socket"<<std::endl;
        return false;
    }

    /*
     * Specify the number of simultanious server connections.
     */

    if (listen(server_socket, 3) < 0)
    {
        std::cout<<"Failed to specify the maximum number of pending connections."<<std::endl;
    }

    addrlen = sizeof(address);

    SSL_load_error_strings();
    
    OpenSSL_add_ssl_algorithms();

    if(!create_ssl_context())
    {
        std::cout<<"LEMP failed to create SSL context"<<std::endl;
    }

    if(!configure_ssl_context())
    {
        std::cout<<"LEMP failed to configure SSL context"<<std::endl;
    }
    
    return true;
}

bool LEMP::create_ssl_context()
{
    const SSL_METHOD* method;
    method = TLSv1_2_server_method();
    ctx = SSL_CTX_new(method);

    if(!ctx)
    {
        return false;
    }
    
    return true;
}

bool LEMP::configure_ssl_context()
{
    if((SSL_CTX_use_certificate_file(ctx, "/home/kamailio/cert.pem", SSL_FILETYPE_PEM) <= 0) ||
       (SSL_CTX_use_PrivateKey_file(ctx, "/home/kamailio/key.pem", SSL_FILETYPE_PEM) <= 0 ))
    {
       return false;
    }

    return true;
}

void LEMP::setMikey(MRef<Mikey*> _mikey_)
{
    mikey = _mikey_;
}

bool LEMP::start()
{
    hasEnded = false;
    thread = NULL;
    thread = new Thread(this);
    return !thread.isNull();
}

void LEMP::stop()
{
    hasEnded = true;
}

void LEMP::run()
{
    std::cout<<"Starting LEMP server"<<std::endl;

    while(1)
    {
       /*
        * Clear the socket set, and specify the select timeout to 5 seconds.
        * The socket set needs to be re-initialized for every new client
        * connection, and the timeout must be re-initialized each iteration.
        */

        FD_ZERO(&readfds);
	    timeout.tv_sec = 1;
        timeout.tv_usec = 0;

       /*
        * Add the server socket to the listening socket set, and set the highest 
        * file descriptor to the value of server_socket.
        */

        FD_SET(server_socket, &readfds);
        max_sd = server_socket;

       /*
        * Find open client connections.
        */

        for(const auto& client:client_sockets)
        {
           //Add open client connections to the listening set.
           if(client.fd > 0)
              FD_SET(client.fd, &readfds);
           
           //Find the highest file descriptor.
           if(client.fd > max_sd)
               max_sd = client.fd;   
        }

        /*
         * Wait for a timeout, a new client connection, or a client request.
         */

        activity = select(max_sd + 1 , &readfds , NULL , NULL , &timeout);

        /*
         * (1) A select error has occured if select returns < 0.
         * (2) A timeout has occured if select returns 0.
         * (3) Check if the server_socket is triggered. 
         *     If this is the case, a new client connection is incomming.
         * (4) If the server_socket was not triggered, an already connected 
         *     client has sent a message, or a client has disconnected.
         */

        if(activity < 0 )
        {
            //Check if an error has occured.
            std::cout<<"LEMP select error."<<std::endl;
        }
        else if(activity == 0)
        {
            //Distribute the keys to the LEMF if the call has ended.
            if(hasEnded)
            {
                std::ofstream key_distribution;
                key_distribution.open("distribute_2_2_hmr.txt", std::ios::app);
                
                high_resolution_clock::time_point t1 = high_resolution_clock::now();
                std::vector<std::string> keys = key_split(2, 2);
                std::string all_ips = Client::toString();

                for(const auto& key:keys)
                {
                    for(auto& client:client_sockets)
                    { 
                        if(client.fd > 0)
                        {
                            std::string to_escrow = key + ";" + mikey->getRAND() + ";" + all_ips;
                            SSL_set_fd(client.ssl, client.fd);
                            SSL_write(client.ssl, to_escrow.c_str(), to_escrow.length());
                            SSL_free(client.ssl);
                            close(client.fd);
                            client.fd = 0;
                            auto it = std::find(client.ips.begin(),client.ips.end(), client.ip);
                            if (it != client.ips.end())
                                client.ips.erase(it);
                            break; 
                        }
                    }
                }

                high_resolution_clock::time_point t2 = high_resolution_clock::now();
                key_distribution<<duration_cast<milliseconds>( t2 - t1 ).count()<<'\n';
                
                std::cout<<"Closed all client connections"<<std::endl;
                mikey = NULL;
                return;
            }
        }
        else if(FD_ISSET(server_socket, &readfds))
        {
            //Accept the new client connection.
            if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
            {
                std::cout<<"LEMP client accept error"<<std::endl;
            }

            std::cout<<"New LEMF connection. File descriptor is " << new_socket 
                     <<  " at address " << inet_ntoa(address.sin_addr) 
                     << " on port " << ntohs(address.sin_port) <<"."<<std::endl;

            SSL* ssl = SSL_new(ctx);
            SSL_set_fd(ssl, new_socket);

            if(SSL_accept(ssl) <= 0)
            {
                std::cout<<"SSL accept failed"<<std::endl;
            }
             
            //Add the new client to the list of connected clients.
            for(auto& client:client_sockets)
            {
                if(client.fd == 0)
                {
                    client.fd = new_socket;
                    client.ssl = ssl;
                    client.ip = inet_ntoa(address.sin_addr);
                    client.ips.push_back(std::string(inet_ntoa(address.sin_addr)));
                    n_connections++;
                    break;
                }
            }
        }
        else
        {
            for(auto& client:client_sockets)
            {
                if(FD_ISSET(client.fd, &readfds))
                {
                    if((valread = read(client.fd, buffer, 4095) <= 0))
                    {
                        getpeername(client.fd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                        std::cout<<"LEMF at address " << inet_ntoa(address.sin_addr) 
                        << " on port " << ntohs(address.sin_port) <<"closed connection prematurly."<<std::endl;
                        SSL_free(client.ssl);
                        close(client.fd);
                        client.fd = 0;
                        client.ssl = NULL;
                        auto it = std::find(client.ips.begin(),client.ips.end(), client.ip);
                        if (it != client.ips.end())
                             client.ips.erase(it);
                        client.ip = "";
                        n_connections--;
                    }
                    else
                    {
                        std::cout<<std::string(buffer)<<std::endl;    
                    }
                }
            }
        }
    }
}

std::vector<std::string> LEMP::key_split(int n, int m)
{
    std::vector<std::string> keys;
    std::string key;
    seed_random();
    std::string tgk = mikey->getTGK();
    
    char* c_tgk = new char[tgk.length() + 1];
    strcpy(c_tgk, tgk.c_str());
    char* shares = generate_share_strings(c_tgk, n, m);

    std::stringstream ss(shares);

    if (shares != NULL)
    {
        while(std::getline(ss, key, '\n'))
        {
            keys.push_back(key);
        }
    }

    free(shares);

    return keys;
}

void LEMP::join()
{
    if(thread.isNull())
        return;
    thread->join();
}

std::vector<std::string> LEMP::LEMP::Client::ips(0);
