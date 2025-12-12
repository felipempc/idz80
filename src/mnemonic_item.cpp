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



MnemonicItem::MnemonicItem()
{
    Reset();
    mnemonic_string_ = 0;
}



MnemonicItem::~MnemonicItem()
{
    if (mnemonic_string_)
    {
        mnemonic_string_->Clear();
        mnemonic_string_->Shrink();
        delete mnemonic_string_;
    }
}



void MnemonicItem::Reset()
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
    mnemonic_signature_ = 0;
}



unsigned int MnemonicItem::GetMnemonicSignature()
{
    return mnemonic_signature_;
}



Groups MnemonicItem::GetGroup()
{
    return group_;
}



Arguments MnemonicItem::GetSourceArgument()
{
    return source_;
}



Arguments MnemonicItem::GetDestinationArgument()
{
    return destination_;
}



Arguments MnemonicItem::GetArgument(const unsigned int index)
{
    if (index == 0)
        return source_;
    if (index == 1)
        return destination_;

    return ARGUMENT_NONE;
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
    return (static_cast<unsigned int>(opcode_size_) & 0x000000FF);
}



unsigned int MnemonicItem::GetArgumentCount()
{
    return (static_cast<unsigned int>(argument_count_)) & 0x000000FF;
}



unsigned int MnemonicItem::GetArgumentSize()
{
    return (static_cast<unsigned int>(argument_size_)) & 0x000000FF;
}



unsigned int MnemonicItem::GetArgumentPosition()
{
    return (static_cast<unsigned int>(argument_opcode_position_)) & 0x000000FF;
}



bool MnemonicItem::GetConditionalBranch()
{
    return conditional_;
}



bool MnemonicItem::HasExplicitArguments()
{
    return explicit_arguments_;
}



unsigned int MnemonicItem::GetMnemonicStrCount()
{
    if (mnemonic_string_)
        return mnemonic_string_->GetCount();
    return 0;
}



wxString MnemonicItem::GetMnemonicStr(unsigned int index)
{
    if ((mnemonic_string_) && (mnemonic_string_->GetCount() > index))
        return mnemonic_string_->Item(index);

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



void MnemonicItem::ConfigArguments(byte argcount, byte argsize, byte argposition)
{
    argument_count_ = argcount;
    argument_size_ = argsize;
    argument_opcode_position_ = argposition;
}



void MnemonicItem::SetConditionalBranch(bool isconditional)
{
    conditional_ = isconditional;
}



void MnemonicItem::SetExplicitArguments(bool isexplicit)
{
    explicit_arguments_ = isexplicit;
}



void MnemonicItem::SetMnemonicString(const wxArrayString &mnemonicstring)
{
    if (mnemonic_string_)
        delete mnemonic_string_;

    mnemonic_string_ = new wxArrayString(mnemonicstring);
}



void MnemonicItem::SetMnemonicSignature(unsigned int signature)
{
    mnemonic_signature_ = signature;
}
