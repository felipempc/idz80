/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   19-04-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Stores a mnemonic item
 **************************************************************/



#include "mnemonic_item.h"

/*
MnemonicItem::MnemonicItem()
{
    //ctor
}

MnemonicItem::~MnemonicItem()
{
    //dtor
}
*/



Groups MnemonicItem::GetGroup()
{
    return group_;
}




Operands MnemonicItem::GetSourceArgument()
{
    return source_;
}




Operands MnemonicItem::GetDestinationArgument()
{
    return destination_;
}




ByteCode &MnemonicItem::GetByteCode()
{
    return bytecode_;
}




byte MnemonicItem::GetByteCode(byte index)
{
    if (index >= MAX_OPCODE_SIZE)
        throw MAX_BYTECODE_REACHED;
    else
        return bytecode_[index];
}




unsigned int MnemonicItem::GetByteCodeSize()
{
    return (static_cast<unsigned int>(opcode_size_)) && 0xFF;
}




unsigned int MnemonicItem::GetArgumentCount()
{
    return (static_cast<unsigned int>(argument_count_)) && 0xFF;
}




unsigned int MnemonicItem::GetArgumentSize()
{
    return (static_cast<unsigned int>(argument_size_)) && 0xFF;
}




unsigned int MnemonicItem::GetArgumentPosition()
{
    return (static_cast<unsigned int>(argument_opcode_position_)) && 0xFF;
}




bool MnemonicItem::GetConditionalBranch()
{
    return conditional_;
}




unsigned int MnemonicItem::GetMnemonicStrCount()
{
    if (mnemonic_string_)
        return mnemonic_string_->GetCount();
}




wxString MnemonicItem::GetMnemonicStr(unsigned int index)
{
    if ((mnemonic_string_) && (mnemonic_string_->GetCount() > index))
    {
        return mnemonic_string_->Item(index);
    }
    return wxEmptyString;
}




void MnemonicItem::SetGroup(Groups group)
{
    group_ = group;
}




void MnemonicItem::SetSourceArgument(OperandType type, Operands operand)
{
    source_.operand = operand;
    source_.type = type;
}




void MnemonicItem::SetDestinationArgument(OperandType type, Operands operand)
{
    destination_.operand = operand;
    destination_.type = type;
}




void MnemonicItem::SetByteCode(ByteCode &bytecode, byte opcodesize)
{
    memcpy(&bytecode_, &bytecode, opcodesize);
    opcode_size_ = opcodesize;
}




void MnemonicItem::SetArgumentConfig(byte argcount, byte argsize, byte argposition)
{
    argument_count_ = argcount;
    argument_size_ = argsize;
    argument_opcode_position_ = argposition;
}




void MnemonicItem::SetConditionalBranch(bool isconditional = false);
{
    conditional_ = isconditional;
}




void MnemonicItem::SetMnemonicString(const wxArrayString &mnemonicstring)
{
    if (mnemonic_string_)
    {
        delete mnemonic_string_;
    }
    mnemonic_string_ = new wxArrayString(mnemonicstring);
}




