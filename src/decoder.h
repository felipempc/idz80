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
#include "disassembled_item.h"
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

        void Clear();

        wxString GetCodeSegmentStr();
        void OptimizeCodeSegment();

    private:
        static const uint OPCODE_NOT_FOUND = 0xFFFFFFFF;

        ProcessBase         *Process;
        SubRoutineCtrl      *SubRoutine;

        SortedIntArray      //*m_unconditionaljumplist,
                            //*m_conditionaljumplist,
                            *AddressList,
                            *AddressListProcessed;

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


        uint Fetch(const FileOffset startpoint, uint maxitems);
        uint Decode(DisassembledItem *de, FileOffset prg_index, DisassembledIndex dasm_position = 0xFFFFFFFF);
        void SetupArgumentLabels(DisassembledItem *de, DisassembledIndex index);

        void ProcessCallSubrotine();
        void ProcessReturnSubrotine();

        bool GetNextNearJump(SortedIntArray *jmplist, ProgramAddress _start, ProgramAddress _end, ProgramAddress &nextaddr);
        bool GetNextFarJump(SortedIntArray *jmplist, ProgramAddress &nextaddr);
        bool OutBoundaryAddress(ProgramAddress _addr);
        void UpdateBoundary();
        bool CallSubroutine(DisassembledItem *de);
        bool ReturnSubroutine(DisassembledItem *de, ProgramAddress &dest_address);
        bool ProcessBranch(DisassembledItem *de, bool &processing_status);
        void FillData();

        void MSXCheckFunctionRegisters(DisassembledItem *de);
        bool MSXWeirdRST(DisassembledItem *de, DisassembledIndex dasm_position);

        void SetCartridgeLabels();

        void debugShowList(const wxString &listname, SortedIntArray *_list);
        void debugShowJmpList(const wxString &listname, SortedIntArray *_list);
};



#endif
