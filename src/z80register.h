/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   18-10-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Represents one Z80 register
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

