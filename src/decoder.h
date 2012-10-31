/****************************************************************
 * Name:      IDZ80
 * Purpose:   Disassembler for Z80 MSX
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 ***
 * This module disassemble a program
 **************************************************************/

#ifndef _DECODER_H_
#define _DECODER_H_

#include <wx/dynarray.h>
#include <wx/textctrl.h>
#include <stack>

#include "IDZ80Base.h"
#include "mndb.h"
#include "dasmdata.h"
#include "rawdata.h"
#include "d_asm_element.h"
#include "labelslist.h"
#include "systemlabels.h"
#include "segmentmgr.h"
#include "IDZ80debugbase.h"
#include "z80registerlist.h"
#include "labelmanager.h"


class Decoder: public IDZ80LogBase
{
    public:
        Decoder(RawData *program, DAsmData *dasmdata, MnemonicDataBase *mnemonics);
        ~Decoder();

        bool FirstDisassemble(LabelManager *parent);

        wxString GetCodeSegmentStr();
        void OptimizeCodeSegment();

    private:
        static const int OPCODE_NOT_FOUND = -1;
        RawData             *m_program;
        DAsmData            *m_dasmeditems;
        MnemonicDataBase    *m_mnemonics;

        SegmentMgr          *m_codesegments,
                            *m_datasegments;

        SortedIntArray      *m_unconditionaljumplist,
                            *m_conditionaljumplist,
                            *m_callList,
                            *m_entrylist,
                            *m_varlist,
                            *m_iolist;

        LabelManager        *Labels;

        Z80RegisterList     m_Register;


        uint                m_startaddress,
                            m_endaddress,
                            m_execaddress,
                            m_prg_counter,
                            m_last_prg_counter,
                            m_nextaddress,
                            m_actualaddress;
        std::stack<int>     m_CallStack;


        uint Fetch(const uint startpoint, uint maxitems);
        uint Decode(DAsmElement *de, uint prg_index, uint dasm_position = 0xFFFFFFFF);
        void ProcessArgument(DAsmElement *de, uint index);

        void ProcessCallSubrotine();
        void ProcessReturnSubrotine();

        bool GetNextNearJump(SortedIntArray *jmplist, uint _start, uint _end, uint &nextaddr);
        bool GetNextFarJump(SortedIntArray *jmplist, uint &nextaddr);
        bool OutBoundaryAddress(uint _addr);
        void UpdateBoundary();
        bool CallSubroutine(DAsmElement *de, uint address);
        bool ReturnSubroutine(uint &dest_address);
        bool ProcessBranch(DAsmElement *de, int &num_call, bool &processing_status);
        void ProcessLoadReg(DAsmElement *de);
        void FillData();

        void MSXCheckFunctionRegisters(DAsmElement *de);
        bool MSXWeirdRST(DAsmElement *de, uint dasm_position);

        void SetCartridgeLabels();

        void debugShowList(const wxString &listname, SortedIntArray *_list);
        void debugShowJmpList(const wxString &listname, SortedIntArray *_list);
};



#endif
