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
#include "processbase.h"
#include "d_asm_element.h"
#include "logbase.h"
#include "z80registerlist.h"
#include "labelmanager.h"
#include "subroutine.h"


class Decoder: public LogBase
{
    public:
        Decoder(ProcessBase *parent, LogWindow *logparent);
        ~Decoder();

        bool FirstDisassemble(LabelManager *parent);
        void FullDisassemble(LabelManager *parent);
        void DisassembleItems(RangeItems &dasm_range);

        wxString GetCodeSegmentStr();
        void OptimizeCodeSegment();

    private:
        static const int OPCODE_NOT_FOUND = -1;

        ProcessBase         *Process;
        SubRoutineCtrl      *SubRoutine;

        SortedIntArray      *m_unconditionaljumplist,
                            *m_conditionaljumplist;

        LabelManager        *Labels;

        Z80RegisterList     Registers;


        uint                m_startaddress,
                            m_endaddress,
                            m_execaddress,
                            m_prg_counter,
                            m_last_prg_counter,
                            m_nextaddress,
                            m_actualaddress,
                            io_label_counter,
                            var_label_counter,
                            program_label_counter;


        uint Fetch(const uint startpoint, uint maxitems);
        uint Decode(DAsmElement *de, uint prg_index, uint dasm_position = 0xFFFFFFFF);
        void SetupArgumentLabels(DAsmElement *de, uint index);

        void ProcessCallSubrotine();
        void ProcessReturnSubrotine();

        bool GetNextNearJump(SortedIntArray *jmplist, uint _start, uint _end, uint &nextaddr);
        bool GetNextFarJump(SortedIntArray *jmplist, uint &nextaddr);
        bool OutBoundaryAddress(uint _addr);
        void UpdateBoundary();
        bool CallSubroutine(DAsmElement *de);
        bool ReturnSubroutine(DAsmElement *de, uint &dest_address);
        bool ProcessBranch(DAsmElement *de, bool &processing_status);
        void FillData();

        void MSXCheckFunctionRegisters(DAsmElement *de);
        bool MSXWeirdRST(DAsmElement *de, uint dasm_position);

        void SetCartridgeLabels();

        void debugShowList(const wxString &listname, SortedIntArray *_list);
        void debugShowJmpList(const wxString &listname, SortedIntArray *_list);
};



#endif
