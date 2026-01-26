/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   01-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Stores a mnemonic item (base)
 **************************************************************/

#ifndef MNEMONICITEMBASE_H
#define MNEMONICITEMBASE_H

#include "idz80_basic_types.hpp"

#define MAX_OPCODE_SIZE     4

typedef unsigned char ByteCode [MAX_OPCODE_SIZE];




enum Groups
{
    GRP_NONE,
    GRP_LOAD_8BIT,
    GRP_LOAD_16BIT,
    GRP_EXCHANGE,
    GRP_BLOCK_TRANSFER,
    GRP_BLOCK_SEARCH,
    GRP_ARITHMETIC_8BIT,
    GRP_ARITHMETIC_16BIT,
    GRP_GP_ARITHMETIC,
    GRP_LOGICAL,
    GRP_CPU_CONTROL,
    GRP_ROTATE_SHIFT,
    GRP_BIT_SET,
    GRP_BIT_RESET,
    GRP_BIT_TEST,
    GRP_JUMP,
    GRP_CALL,
    GRP_RETURN,
    GRP_INPUT,
    GRP_OUTPUT,
    GRP_RST,
    GRP_STACK
};




enum Operands
{
    OP_NONE,
    OP_LITERAL,
    //8 BIT
    OP_A,
    OP_B,
    OP_C,
    OP_D,
    OP_E,
    OP_F,
    OP_H,
    OP_L,
    OP_I,
    OP_R,
    // 16 BIT
    OP_BC,
    OP_DE,
    OP_HL,
    OP_IX,
    OP_IY,
    OP_SP,
    // RST xx
    OP_00,
    OP_08,
    OP_10,
    OP_18,
    OP_20,
    OP_28,
    OP_30,
    OP_38
};



enum OperandType
{
	OT_NONE,
	OT_REGISTER,
	OT_REGISTER_ADDRESS,
	OT_REGISTER_PLUS_OFFSET,
	OT_DATA,
	OT_OFFSET,
	OT_VARIABLE,
	OT_ABSOLUTE_ADDRESS,
	OT_RELATIVE_ADDRESS,
	OT_IO_ADDRESS
};




struct Arguments
{
    Operands operand;
	OperandType	type;
};




class MnemonicItemBase
{
    public:
        MnemonicItemBase();
        void Reset();


    protected:
        unsigned int m_mnemonic_signature;
        Groups		m_group;
        Arguments   m_source,
                    m_destination;
		ByteCode	m_bytecode;
        unsigned int    m_opcode_size;
        unsigned int    m_argument_count;
        unsigned int    m_argument_size;
        unsigned int    m_argument_opcode_position;
        bool        m_conditional;
        bool        m_explicit_arguments;
        StringVector m_mnemonic_string;
};




#endif // MNEMONICITEM_H
