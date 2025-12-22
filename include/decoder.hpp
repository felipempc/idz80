/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module disassembles a program
 **************************************************************/


  // MUST BE COMPLETELY REWRITTEN !!!!


#ifndef _DECODER_H_
#define _DECODER_H_

#include <wx/dynarray.h>
#include <wx/textctrl.h>
#include <stack>

#include "project_base.hpp"
#include "disassembled_item.hpp"
#include "debug_logbase.hpp"
#include "z80registerlist.hpp"
#include "labelmanager.hpp"
#include "subroutine.hpp"


class Decoder: public DebugLogBase
{
    public:
        Decoder(ProjectBase *parent);

        void SetProgramIndex(const unsigned int program_index);

        void FullDisassemble();
        void DisassembleItems(RangeItems &dasm_range);


    protected:
        static const uint OPCODE_NOT_FOUND = 0xFFFFFFFF;

        ProjectBase         *m_project_base;
        RawData             *m_program;
        MnemonicContainer   *m_mnemonic;
        LabelManager        *m_labels;
        DisassembledContainer   *m_disassembled_list;

        Z80RegisterList     m_registers;


        uint Fetch(const FileOffset startpoint, uint maxitems);
        uint Decode(DisassembledItem *de, FileOffset prg_index, DisassembledIndex dasm_position = 0xFFFFFFFF);
        void SetupArgumentLabels(DisassembledItem *de, DisassembledIndex index);

        void MSXCheckFunctionRegisters(DisassembledItem *de);
        uint MSXWeirdRST(DisassembledItem *de, DisassembledIndex dasm_position);

        void SetCartridgeLabels();


    private:

};



#endif
