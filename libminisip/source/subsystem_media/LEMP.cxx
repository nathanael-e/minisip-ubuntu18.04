#include"LEMP.h"

LEMP::LEMP()
: client_sockets(MAX_CLIENTS){}

LEMP::~LEMP()
{
    std::cout<<"Destroying LEMP"<<std::endl;

    if(server_socket)
    {
        std::cout<<"Closing LEMP server"<<std::endl;
        close(server_socket);
    }
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
	    timeout.tv_sec = 5;
        timeout.tv_usec = 0;

       /*
        * Add to server socket to the listening socket set and set the highest 
        * file descriptor to the value of server_socket.
        */

        FD_SET(server_socket, &readfds);
        max_sd = server_socket;

       /*
        * Find open client connections.
        */

        for(const auto& sd_client:client_sockets)
        {
           //Add open client connections to the listening set.
           if(sd_client > 0)
              FD_SET(sd_client, &readfds);
           
           //Find the highest file descriptor.
           if(sd_client > max_sd)
               max_sd = sd_client;   
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
                for(auto& sd_client:client_sockets)
                {
                    if(sd_client > 0)
                    {
                        close(sd_client);
                        sd_client = 0;
                    }
                }
                
                std::cout<<"Closed all client connections"<<std::endl;
                close(server_socket);
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

            //Add the new client to the list of connected clients.
            for(auto& sd_client:client_sockets)
            {
                if(sd_client == 0)
                {
                    sd_client = new_socket;
                    break;
                }
            }
        }
        else
        {
            for(auto& sd_client:client_sockets)
            {
                if(FD_ISSET(sd_client, &readfds))
                {
                    if((valread = read(sd_client, buffer, 4095) <= 0))
                    {
                        getpeername(sd_client , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                        std::cout<<"LEMF at address " << inet_ntoa(address.sin_addr) 
                        << " on port " << ntohs(address.sin_port) <<"closed connection prematurly."<<std::endl;
                        close(sd_client);
                        sd_client = 0;
                    }
                    else
                    {
                        std::cout<<std::string(buffer)<<std::endl;    
                    }
                }
            }
        }
    }

    close(server_socket);
}

void LEMP::join()
{
    if(thread.isNull())
        return;
    thread->join();
}
