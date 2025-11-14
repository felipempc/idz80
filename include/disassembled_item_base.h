#ifndef DISASSEMBLEDITEMBASE_H
#define DISASSEMBLEDITEMBASE_H


#include "rawdata.h"
#include "mnemonic_item.h"

typedef unsigned char ExplicitArguments [2];

class DisassembledItemBase
{
    protected:
        RawData         *m_program;
        MnemonicItem    *m_mnemonic;
        unsigned int    m_mnemonic_signature;

        ExplicitArguments   m_arguments;
        FileOffset          m_file_offset;
};

#endif // DISASSEMBLEDITEMBASE_H
