#include"EDS.h"

EDS::EDS()
{
	std::cout<<"Hello, world"<<std::endl;
}

void EDS::setMikey(MRef<Mikey*> _mikey_)
{
    mikey = _mikey_;
}

void EDS::start()
{
    std::cout<<"Starting EDS"<<std::endl;
}

void EDS::stop()
{
    if(mikey)
        mikey->escrowSessionKey();
}
