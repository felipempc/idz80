/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   08-11-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class for process data.
 **************************************************************/


#ifndef _PROCESSBASE_H_
#define _PROCESSBASE_H_

#include "mnemonic_container.h"

#include "disassembled_container.h"
#include "rawdata.h"
#include "SourceCodeLines.h"

 // MUST BE COMPLETELY REWRITED !!!!


class ProcessBase
{
    public:
        SourceCodeLines     *CodeViewLines;
        DisassembledContainer            *Disassembled;
        RawData             *Program;
        MnemonicContainer   *Mnemonics_;
};


#endif // _PROCESSBASE_H_
