/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-10-18
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module manages Z80 registers
 **************************************************************/



#include "z80registerlist.h"



const int Z80RegisterList::TimeToLive;

Z80RegisterList::Z80RegisterList()
{
    A = new Z80Register(TimeToLive);
    B = new Z80Register(TimeToLive);
    C = new Z80Register(TimeToLive);
    D = new Z80Register(TimeToLive);
    E = new Z80Register(TimeToLive);
    H = new Z80Register(TimeToLive);
    L = new Z80Register(TimeToLive);
    BC = new Z80Register(TimeToLive);
    DE = new Z80Register(TimeToLive);
    HL = new Z80Register(TimeToLive);
    SP = new Z80Register(TimeToLive);
    IX = new Z80Register(TimeToLive);
    IY = new Z80Register(TimeToLive);
}


Z80RegisterList::~Z80RegisterList()
{
    delete A;
    delete B;
    delete C;
    delete D;
    delete E;
    delete H;
    delete L;
    delete BC;
    delete DE;
    delete HL;
    delete SP;
    delete IX;
    delete IY;
}


void Z80RegisterList::UpdateLife()
{
    A->DecLife();
    B->DecLife();
    C->DecLife();
    D->DecLife();
    E->DecLife();
    H->DecLife();
    L->DecLife();
    BC->DecLife();
    DE->DecLife();
    HL->DecLife();
    SP->DecLife();
    IX->DecLife();
    IY->DecLife();
}
