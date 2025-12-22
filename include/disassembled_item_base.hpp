#ifndef DISASSEMBLEDITEMBASE_H
#define DISASSEMBLEDITEMBASE_H


#include "rawdata.hpp"
#include "mnemonic_item.hpp"

//typedef unsigned char ExplicitArguments [2];

struct stExplicitArguments {
    unsigned int unsigned_16bit;
    unsigned int unsigned_8bit_low;   // first
    unsigned int unsigned_8bit_high;  // second
    signed int signed_value;
};
typedef struct stExplicitArguments ExplicitArguments;

class DisassembledItemBase
{
    protected:
        RawData         *m_program;
        MnemonicItem    *m_mnemonic;
        unsigned int    m_mnemonic_signature;
        ByteCode        m_real_bytecode;    // typedef from mnemonic_item_base.h

        ExplicitArguments   m_arguments;

        FileOffset      m_file_offset;
        bool            m_is_data;
        unsigned int    m_length;
};

#endif // DISASSEMBLEDITEMBASE_H
