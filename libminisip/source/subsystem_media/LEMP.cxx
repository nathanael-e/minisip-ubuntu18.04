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
    thread = new Thread(this);
    return !thread.isNull();
}

void LEMP::stop()
{
    running = false;

    if(s)
    {
        shutdown(s, 0);
    }
}

void LEMP::run()
{
    while(running)
    {
        ns = accept(s, (struct sockaddr *)&client_address, &client_length);

        if(recv(ns, buf, sizeof(buf), 0) == -1)
        {
            perror("Recv() from LEMP");
        }

        std::cout<<std::string(buf)<<std::endl;
     
        if (send(ns, buf, sizeof(buf), 0) < 0)
        {
            perror("Send() from LEMP");
        }
    }
}

void LEMP::join()
{
    if(thread.isNull())
        return;
    thread->join();
}
