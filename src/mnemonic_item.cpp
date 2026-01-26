/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   19-04-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Stores a mnemonic item
 **************************************************************/

#include "mnemonic_item.hpp"
#include <stdexcept>



MnemonicItem::MnemonicItem()
{
    Reset();
}



void MnemonicItem::Reset()
{
    m_group= GRP_NONE;
    m_source.operand = OP_NONE;
    m_source.type = OT_NONE;
    m_destination.operand = OP_NONE;
    m_destination.type = OT_NONE;
    memset(m_bytecode, 0, sizeof(ByteCode));
    m_opcode_size = 0;
    m_argument_count = 0;
    m_argument_size = 0;
    m_argument_opcode_position = 0;
    m_conditional = false;
    m_explicit_arguments = false;
    m_mnemonic_signature = 0;
}



unsigned int MnemonicItem::GetMnemonicSignature()
{
    return m_mnemonic_signature;
}



Groups MnemonicItem::GetGroup()
{
    return m_group;
}



Arguments MnemonicItem::GetSourceArgument()
{
    return m_source;
}



Arguments MnemonicItem::GetDestinationArgument()
{
    return m_destination;
}



Arguments MnemonicItem::GetArgument(const unsigned int index)
{
    if (index == 0)
        return m_source;
    if (index == 1)
        return m_destination;

    return ARGUMENT_NONE;
}



ByteCode &MnemonicItem::GetByteCode()
{
    return m_bytecode;
}



byte MnemonicItem::GetByteCode(byte index)
{
    if (index >= MAX_OPCODE_SIZE)
        throw MAX_BYTECODE_REACHED;
    else
        return m_bytecode[index];
}



unsigned int MnemonicItem::GetByteCodeSize()
{
    return m_opcode_size;
}



unsigned int MnemonicItem::GetArgumentCount()
{
    return m_argument_count;
}



unsigned int MnemonicItem::GetArgumentSize()
{
    return m_argument_size;
}



unsigned int MnemonicItem::GetArgumentPosition()
{
    return m_argument_opcode_position;
}



bool MnemonicItem::GetConditionalBranch()
{
    return m_conditional;
}



bool MnemonicItem::HasExplicitArguments()
{
    return m_explicit_arguments;
}



unsigned int MnemonicItem::GetMnemonicStrCount()
{
    return m_mnemonic_string.size();
}



wxString MnemonicItem::GetMnemonicStr(unsigned int index)
{
    try
    {
        return m_mnemonic_string.at(index);
    }
    catch(const std::out_of_range& e)
    {
        return wxEmptyString;
    }
}



void MnemonicItem::SetGroup(Groups group)
{
    m_group = group;
}



void MnemonicItem::SetSourceArgument(OperandType type, Operands operand)
{
    m_source.operand = operand;
    m_source.type = type;
}



void MnemonicItem::SetDestinationArgument(OperandType type, Operands operand)
{
    m_destination.operand = operand;
    m_destination.type = type;
}



void MnemonicItem::SetByteCode(ByteCode &bytecode, byte opcodesize)
{
    memcpy(&m_bytecode, &bytecode, opcodesize);
    m_opcode_size = opcodesize;
}



void MnemonicItem::ConfigArguments(byte argcount, byte argsize, byte argposition)
{
    m_argument_count = argcount;
    m_argument_size = argsize;
    m_argument_opcode_position = argposition;
}



void MnemonicItem::SetConditionalBranch(bool isconditional)
{
    m_conditional = isconditional;
}



void MnemonicItem::SetExplicitArguments(bool isexplicit)
{
    m_explicit_arguments = isexplicit;
}



void MnemonicItem::SetMnemonicSignature(unsigned int signature)
{
    m_mnemonic_signature = signature;
}



/// @brief Copy mnemonic string slices from t_strings
/// @param t_strings mnemonic string slices
void MnemonicItem::SetMnemonicStrings(const StringVector &t_strings)
{
    m_mnemonic_string = t_strings;
}
