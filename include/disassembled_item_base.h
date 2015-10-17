#ifndef DISASSEMBLEDITEMBASE_H
#define DISASSEMBLEDITEMBASE_H


#include "rawdata.h"
#include "mnemonic_item.h"

typedef unsigned char ExplicitArguments [2];

class DisassembledItemBase
{
    protected:
        RawData     *binary_data_;
        MnemonicItem    *mnemonic_;
        unsigned int    mnemonic_signature_;

        ExplicitArguments   arguments_;
        FileOffset  offset_in_file_;
};

#endif // DISASSEMBLEDITEMBASE_H
