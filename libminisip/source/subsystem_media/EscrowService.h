#ifndef ESCROW_SERVICE
#define ESCROW_SERVICE

#include<libmutil/MemObject.h>
#include<libmikey/Mikey.h>
#include<iostream>

class EscrowService: 
    public MObject
{
    public:
        EscrowService(MRef<Mikey*> _mikey_);
        void start();
        void stop();

	private:
        MRef<Mikey*> mikey;
};

#endif
