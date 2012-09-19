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

#include <wx/dynarray.h>
#include "rawdata.h"
#include "d_asm_element.h"


class DAsmData
{
    public:
        void Clear();
        void AddDasm(DAsmElement *dasmelement);
        void DelDasm(DAsmElement *dasmelement);
        void DelDasm(uint index, uint count = 1);
        void InsertDasm(DAsmElement *dasmelement,uint beforeitem);
        DAsmElement *GetData(uint index);
        uint GetCount();
        bool IsLoaded();
        uint GetUsedMem();
        uint GetBaseAddress(uint index);
        void AddOrgAddress(uint index, uint address);
        void DelOrgAddress(uint address);
        
        wxTextCtrl *dbglog;
        
        
        DAsmData();
        ~DAsmData();

    private:
        wxArrayPtrVoid  Data;
        uint            totalAllocated;
        wxArrayInt		m_baseAddress;		// keeps Origin Addresses
};


#endif

