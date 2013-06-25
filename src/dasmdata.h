/****************************************************************
 * Name:      IDZ80
 * Purpose:   Z80 code disassembler
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module stores / manages disassembled code
 **************************************************************/



/*
 * Holds the disassembled program
 */


#ifndef _IDZ80_DASMDATA_H
#define _IDZ80_DASMDATA_H

#include "logbase.h"
#include "rawdata.h"
#include "d_asm_element.h"
#include <vector>
#include "idz80base.h"

struct RangeItems
{
    uint Index;
    uint Count;
};


typedef std::vector<DAsmElement *> DasmArray;

class DAsmData : public LogBase
{
    public:
        void Clear();
        int  AddDasm(DAsmElement *dasmelement);
        void DelDasm(DisassembledIndex position);
        void DelDasm(DisassembledIndex index, uint count);
        int  InsertDasm(DAsmElement *dasmelement, DisassembledIndex beforeitem);
        DAsmElement *GetData(DisassembledIndex index);
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

