/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   01-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Stores a mnemonic item (base)
 **************************************************************/

#include "mnemonic_item_base.h"

MnemonicItemBase::MnemonicItemBase()
{
    Reset();
    mnemonic_string_ = 0;
}




MnemonicItemBase::~MnemonicItemBase()
{
    if (mnemonic_string_)
    {
        mnemonic_string_->Clear();
        mnemonic_string_->Shrink();
    }
}




void MnemonicItemBase::Reset()
{
    group_= GRP_NONE;
    source_.operand = OP_NONE;
    source_.type = OT_NONE;
    destination_.operand = OP_NONE;
    destination_.type = OT_NONE;
    memset(bytecode_, 0, sizeof(ByteCode));
    opcode_size_ = 0;
    argument_count_ = 0;
    argument_size_ = 0;
    argument_opcode_position_ = 0;
    conditional_ = false;
    explicit_arguments_ = false;
}
