/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-10-18
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module manages Z80 registers
 **************************************************************/

#include "z80register.h"



Z80Register::Z80Register(int ttl)
{
    m_timetolive = ttl;
    Clear();
}


void Z80Register::Clear()
{
    m_TTL = -1;
    m_Loaded = false;
    m_Value = 0;
    m_FileOffset = 0;
}



void Z80Register::Init(uint value, uint offset)
{
    m_Loaded = true;
    m_Value = value;
    m_FileOffset = offset;
    m_TTL = m_timetolive;
}


void Z80Register::DecLife()
{
    if (m_TTL > 0)
        m_TTL--;
    if (m_TTL == 0)
        Clear();
}




uint Z80Register::GetValue()
{
    return m_Value;
}




uint Z80Register::GetOffset()
{
    return m_FileOffset;
}

bool Z80Register::isLoaded()
{
    return m_Loaded;
}
