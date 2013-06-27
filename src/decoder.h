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
//        ~Decoder();

        void FullDisassemble(LabelManager *parent);
        void DisassembleItems(RangeItems &dasm_range);

//        void Clear();

    protected:
        static const uint OPCODE_NOT_FOUND = 0xFFFFFFFF;

        ProcessBase         *process_;

        LabelManager        *labels_;
        Z80RegisterList     registers_;


        uint Fetch(const FileOffset startpoint, uint maxitems);
        uint Decode(DisassembledItem *de, FileOffset prg_index, DisassembledIndex dasm_position = 0xFFFFFFFF);
        void SetupArgumentLabels(DisassembledItem *de, DisassembledIndex index);

        void MSXCheckFunctionRegisters(DisassembledItem *de);
        bool MSXWeirdRST(DisassembledItem *de, DisassembledIndex dasm_position);

        void SetCartridgeLabels();


    private:

};



#endif
