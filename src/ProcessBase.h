/****************************************************************
 * Name:      ProcessBase
 * Purpose:   Disassembler for Z80 MSX
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-11-08
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 ***
 **************************************************************/


#ifndef _PROCESSBASE_H_
#define _PROCESSBASE_H_

#include "mndb.h"
#include "dasmdata.h"
#include "rawdata.h"
#include "codeviewline.h"

class ProcessBase
{
    public:
        CodeViewLine        *CodeViewLines;
        DAsmData            *Disassembled;
        RawData             *Program;
        MnemonicDataBase    *Mnemonics;
};


#endif // _PROCESSBASE_H_
