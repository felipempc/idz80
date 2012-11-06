/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-10-18
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module manages Z80 registers
 **************************************************************/

#ifndef Z80REGISTER_H
#define Z80REGISTER_H

#include "IDZ80Base.h"


class Z80Register
{
	public:
	    Z80Register(int ttl);

	    void Init(uint value, uint offset);
	    void DecLife();
	    void Clear();

	    uint GetValue();
	    uint GetOffset();

	    bool isLoaded();

	private:
	    int m_timetolive;
        int m_TTL;
        bool m_Loaded;
        uint m_Value;
        uint m_FileOffset;
};

#endif

