#ifndef EDS_H
#define EDS_H

#include<iostream>

/* Minisip exsclusive headers */

#include<libmutil/MemObject.h>
#include<libmikey/Mikey.h>

class EDS
    :public MObject
{
	public:
		EDS();
        void start();
        void stop();
        void setMikey(MRef<Mikey*>);

    private:
        MRef<Mikey*> mikey;
};

#endif /*EDS.h*/
