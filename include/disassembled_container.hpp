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

#include "debug_logbase.hpp"

#include "disassembled_item.hpp"
#include "idz80_basic_types.hpp"

#include <vector>


struct RangeItems
{
    uint Index;
    uint Count;
};


typedef std::vector<DisassembledItem *> DisassembledList;
//DisassembledIndex transferred to idz80_basic_types.hpp

class DisassembledContainer : public DebugLogBase
{
    public:
        void clear();
        int  add(DisassembledItem *d_item);
        void del(DisassembledIndex position);
        void del(DisassembledIndex index, uint count);
        int  insert(DisassembledItem *dasmelement, DisassembledIndex beforeitem);
        DisassembledItem *getData(DisassembledIndex index);
        uint getCount();
        bool isLoaded();
        uint getUsedMem();
        uint getBaseAddress(DisassembledIndex index);
        void addOrigin(DisassembledIndex index, AbsoluteAddress address);
        void delOrigin(AbsoluteAddress address);
        DisassembledIndex findAddress(const AbsoluteAddress t_address);

        RawData *getProgram() const;

        DisassembledContainer(RawData *t_rawdata);
        ~DisassembledContainer();

    private:
        struct OriginData
        {
            DisassembledIndex   index;
            AbsoluteAddress     address;
        };
        typedef std::vector<OriginData *> AddressList;

        DisassembledList    m_disassembled_list;
        RawData *m_file_program;  //Keeps the track of the file
        uint    m_total_allocated;
        AddressList     m_origin_list;		// keeps Origin Addresses
};

#endif

