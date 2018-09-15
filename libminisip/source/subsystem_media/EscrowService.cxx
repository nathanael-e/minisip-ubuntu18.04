#include "EscrowService.h"

EscrowService::EscrowService(MRef<Mikey*> _mikey_)
{
    this->mikey = _mikey_;
}

void EscrowService::start()
{}

void EscrowService::stop()
{
    mikey->escrowSessionKey();
}
