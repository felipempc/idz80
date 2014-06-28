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

#include "mndb.h"
#include "dasmdata.h"
#include "rawdata.h"
#include "SourceCodeLines.h"

class ProcessBase
{
    public:
        SourceCodeLines     *CodeViewLines;
        DAsmData            *Disassembled;
        RawData             *Program;
        MnemonicDataBase    *Mnemonics;
};


#endif // _PROCESSBASE_H_
