/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   18-10-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Manages Z80 registers
 **************************************************************/

#ifndef Z80REGISTERLIST_H
#define Z80REGISTERLIST_H

#include "z80register.h"
#include "disassembled_item.h"


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

        void LoadRegister(DisassembledItem *de);
        void UpdateLife();
};

#endif


