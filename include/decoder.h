/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module disassembles a program
 **************************************************************/


  // MUST BE COMPLETELY REWRITED !!!!


#ifndef _DECODER_H_
#define _DECODER_H_

#include <wx/dynarray.h>
#include <wx/textctrl.h>
#include <stack>

#include "idz80_base.h"
#include "process_base.h"
#include "disassembled_item.h"
#include "logbase.h"
#include "z80registerlist.h"
#include "labelmanager.h"
#include "subroutine.h"


class Decoder: public LogBase
{
    public:
        Decoder(ProcessBase *parent, LogWindow *logparent);

        void FullDisassemble(LabelManager *parent);
        void DisassembleItems(RangeItems &dasm_range);


    protected:
        static const uint OPCODE_NOT_FOUND = 0xFFFFFFFF;

        ProcessBase         *process_;

        LabelManager        *labels_;
        Z80RegisterList     registers_;


        uint Fetch(const FileOffset startpoint, uint maxitems);
        uint Decode(DisassembledItem *de, FileOffset prg_index, DisassembledIndex dasm_position = 0xFFFFFFFF);
        void SetupArgumentLabels(DisassembledItem *de, DisassembledIndex index);

        void MSXCheckFunctionRegisters(DisassembledItem *de);
        uint MSXWeirdRST(DisassembledItem *de, DisassembledIndex dasm_position);

        void SetCartridgeLabels();


    private:

};



#endif
