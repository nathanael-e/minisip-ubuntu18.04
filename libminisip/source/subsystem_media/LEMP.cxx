#include"LEMP.h"

LEMP::LEMP()
{
	init();
}

void LEMP::init()
{
    s = socket(PF_INET, SOCK_STREAM, 0);
    
    if(s < 0)
    {
        perror("Failed to initialize EDS socket");
    }

    rc = setsockopt(s, SOL_SOCKET, 
            SO_REUSEADDR, &on, sizeof on);
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(5000);

    rc = bind(s, (struct sockaddr *)&server_address, sizeof(server_address));

    if(rc < 0)
    {
        perror("Failed to bind EDS server socket");
    }

    rc = listen(s , 10);

    if( rc < 0)
    {
        perror("Failed to listen");
    }
}

void LEMP::setMikey(MRef<Mikey*> _mikey_)
{
    mikey = _mikey_;
}

bool LEMP::start()
{
    thread = NULL;
    thread = new Thread( this );
    return !thread.isNull();
}

void LEMP::stop()
{
    running = false;

    if(mikey)
        mikey->escrowSessionKey();

    if(s)
    {
        shutdown(s, 0);
    }
}

void LEMP::run()
{
    std::cout<<"Starting EDS server"<<std::endl;

    while(running)
    {
        rc = accept(s, (struct sockaddr *)&client_address, &client_length);
    }

    std::cout<<"EDS server is closing"<<std::endl;
}

void LEMP::join()
{
    if(thread.isNull())
        return;
    thread->join();
}
