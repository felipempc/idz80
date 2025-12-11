/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module stores / manages disassembled code
 **************************************************************/

#ifndef _IDZ80_DASMDATA_H
#define _IDZ80_DASMDATA_H

#include "debug_logbase.h"
#include "rawdata.h"
#include "disassembled_item.h"
#include <vector>


struct RangeItems
{
    uint Index;
    uint Count;
};


typedef std::vector<DisassembledItem *> DasmArray;

class DAsmData : public DebugLogBase
{
    public:
        void Clear();
        int  AddDasm(DisassembledItem *dasmelement);
        void DelDasm(DisassembledIndex position);
        void DelDasm(DisassembledIndex index, uint count);
        int  InsertDasm(DisassembledItem *dasmelement, DisassembledIndex beforeitem);
        DisassembledItem *GetData(DisassembledIndex index);
        uint GetCount();
        bool IsLoaded();
        uint GetUsedMem();
        uint GetBaseAddress(DisassembledIndex index);
        void AddOrgAddress(DisassembledIndex index, ProgramAddress address);
        void DelOrgAddress(ProgramAddress address);
        int FindAddress(ProgramAddress address);


        DAsmData(LogWindow *logparent);
        ~DAsmData();

    private:
        DasmArray		m_DasmList;
        uint            totalAllocated;
        wxArrayInt		m_baseAddress;		// keeps Origin Addresses
};


#endif

