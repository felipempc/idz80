/****************************************************************
* Name:      IDZ80
* Purpose:   Interactive Disassembler for Z80 processors
* Author:    Felipe MPC (idz80a@gmail.com)
* Created:   09-11-2009 (D-M-Y) / Rewrited 30/06/2015
* License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
**************************************************************
* Represents one data item
**************************************************************/

// DELETE THIS FILE

#include "disassembled_item_data.h"

DisassembledItemData::DisassembledItemData(RawData *program)
{
    m_is_data = true;
    m_program = program;
    m_length = 0;
}




unsigned int DisassembledItemData::GetLength()
{
    return m_length;
}




void DisassembledItemData::SetLength(unsigned int len)
{
    m_length = len;
}




bool DisassembledItemData::isData()
{
    return m_is_data;
}

void DisassembledItemData::MarkAsData(const bool isdata)
{
    m_is_data = isdata;
}
