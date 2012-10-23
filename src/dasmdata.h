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

//#include <wx/dynarray.h>
#include "rawdata.h"
#include "d_asm_element.h"
#include <vector>

typedef std::vector<DAsmElement *> DasmArray;

class DAsmData : public IDZ80LogBase
{
    public:
        void Clear();
        int  AddDasm(DAsmElement *dasmelement);
        void DelDasm(uint position);
        void DelDasm(uint index, uint count);
        int  InsertDasm(DAsmElement *dasmelement,uint beforeitem);
        DAsmElement *GetData(uint index);
        uint GetCount();
        bool IsLoaded();
        uint GetUsedMem();
        uint GetBaseAddress(uint index);
        void AddOrgAddress(uint index, uint address);
        void DelOrgAddress(uint address);
        int FindAddress(uint address);


        DAsmData();
        ~DAsmData();

    private:
        DasmArray		m_DasmList;
        uint            totalAllocated;
        wxArrayInt		m_baseAddress;		// keeps Origin Addresses
};


#endif

