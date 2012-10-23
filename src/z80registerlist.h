/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-10-18
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module manages Z80 registers
 **************************************************************/

#ifndef Z80REGISTERLIST_H
#define Z80REGISTERLIST_H

#include "IDZ80Base.h"
#include "z80register.h"

class Z80RegisterList
{
	private:
	    static const int TimeToLive = 3;

	public:
	    Z80Register *A;
	    Z80Register *B;
	    Z80Register *C;
	    Z80Register *D;
	    Z80Register *E;
	    Z80Register *H;
	    Z80Register *L;
	    Z80Register *BC;
	    Z80Register *DE;
	    Z80Register *HL;
	    Z80Register *SP;
	    Z80Register *IX;
	    Z80Register *IY;

	    Z80RegisterList();
	    ~Z80RegisterList();

        void UpdateLife();
};

#endif


