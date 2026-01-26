/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   01-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Stores a mnemonic item (base)
 **************************************************************/

#include "mnemonic_item_base.hpp"

/** TODO Remove mnemonic_item_base.cpp/hpp */

MnemonicItemBase::MnemonicItemBase()
{
    Reset();
}



void MnemonicItemBase::Reset()
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
    m_mnemonic_string.clear();
}
