/****************************************************************
* Name:      IDZ80
* Purpose:   Interactive Disassembler for Z80 processors
* Author:    Felipe MPC (idz80a@gmail.com)
* Created:   09-11-2009 (D-M-Y) / Rewrited 30/06/2015
* License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
**************************************************************
* Represents one data item
**************************************************************/

#ifndef DISASSEMBLEDITEMDATA_H
#define DISASSEMBLEDITEMDATA_H

#include "disassembled_item_base.h"


class DisassembledItemData : public DisassembledItemBase
{
    public:
        DisassembledItemData(RawData *program);
        unsigned int GetLength();
        void SetLength(unsigned int len);
        bool isData();

    protected:
        bool m_is_data;
        unsigned int m_length;
    private:
};

#endif // DISASSEMBLEDITEMDATA_H
