/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y) / Rewrite 30/06/2015
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module stores / manages disassembled code
 **************************************************************/

#ifndef _DISASSEMBLED_CONTAINER_H
#define _DISASSEMBLED_CONTAINER_H

#include "logbase.h"

#include "disassembled_item.h"

#include <vector>


struct RangeItems
{
    uint Index;
    uint Count;
};


typedef std::vector<DisassembledItem *> DisassembledList;
typedef unsigned int DisassembledIndex;

class DisassembledContainer : public LogBase
{
    public:
        void Clear();
        int  Add(DisassembledItem *d_item);
        void Del(DisassembledIndex position);
        void Del(DisassembledIndex index, uint count);
        int  Insert(DisassembledItem *dasmelement, DisassembledIndex beforeitem);
        DisassembledItem *GetData(DisassembledIndex index);
        uint GetCount();
        bool IsLoaded();
        uint GetUsedMem();
        uint GetBaseAddress(DisassembledIndex index);
        void AddOrigin(DisassembledIndex index, AbsoluteAddress address);
        void DelOrigin(AbsoluteAddress address);
        DisassembledIndex FindAddress(AbsoluteAddress address);


        DisassembledContainer(LogBase *logparent);
        ~DisassembledContainer();

    private:
        struct OriginData
        {
            DisassembledIndex index;
            AbsoluteAddress address;
        };
        typedef std::vector<OriginData *> AddressList;

        DisassembledList    disassembled_list_;
        uint    totalAllocated;
        AddressList     origin_list_;		// keeps Origin Addresses
};


#endif
