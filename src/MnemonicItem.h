/****************************************************************
 * Name:      IDZ80
 * Purpose:   Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module holds one mnemonic item
 **************************************************************/





#ifndef MNEMONICITEM_H
#define MNEMONICITEM_H

#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>
#include "idz80base.h"

#define MAX_NUM_ARG 2

#define ARG_POSITION_CHAR   '$'

enum ArgumentTypes
{
    ARG_NONE = 0,
    ARG_LITERAL,
    ARG_OFFSET,
    ARG_VARIABLE,
    ARG_ABS_ADDR,
    ARG_REL_ADDR,
    ARG_IO_ADDR
};

enum BranchType
{
    BR_NONE = 0,
    BR_JUMP,
    BR_JUMP_CND,
    BR_CALL,
    BR_CALL_CND,
    BR_RETURN,
    BR_RETURN_CND
};


enum InstructionTypes
{
    IT_NONE = 0,
    IT_RST,
    IT_CALL,
    IT_RET,
    IT_JUMP,
    IT_LOAD,
    IT_ERROR
};

enum InstructionDetails
{
    II_NONE = 0,
    II_RST_00,
    II_RST_08,
    II_RST_10,
    II_RST_18,
    II_RST_20,
    II_RST_28,
    II_RST_30,
    II_RST_38,
    II_CONDITIONAL,
    II_LD_A_N,
    II_LD_B_N,
    II_LD_C_N,
    II_LD_D_N,
    II_LD_E_N,
    II_LD_H_N,
    II_LD_L_N,
    II_LD_HL_N,
    II_LD_DE_N,
    II_LD_BC_N,
    II_LD_SP_N,
    II_LD_IX_N,
    II_LD_IY_N,
    II_ERROR
};

typedef unsigned char ByteCode [MAX_OPCODE_SIZE];


class MnemonicItem
{
    public:
        MnemonicItem();
        bool hasArgument();
        enum ArgumentTypes getArgType(unsigned int argno);
        uint getArgNo();
        enum BranchType getBranchType();
        void addOpCode(unsigned char opcode);
        void addArgument(enum ArgumentTypes argument);
        void setMnemonicStr(wxString str);
        void setOpCodeArgPos(unsigned int argpos);
        void setBranchType(enum BranchType brunchtype);
        void setInstructionType(enum InstructionTypes itype);
        bool setInstructionType(uint itype);
        void setInstructionDetail(enum InstructionDetails iinfo);
        bool setInstructionDetail(uint iinfo);
        enum InstructionTypes GetInstructionType();
        enum InstructionDetails GetInstructionDetail();
        void Clear();

        uint getBytesNo();
        uint getOpCode(unsigned int opcode);
        ByteCode *getOpCode();
        uint getArgSize();
        uint getArgPos();

        bool isUnconditionalJump();
        bool isConditionalJump();
        bool isJump();
        bool isCall();
        bool isReturn();
        bool isConditionalReturn();

        wxArrayString MnemonicString;

    protected:
    private:

        ByteCode        Opcode;
        byte            OpcodeSize;
        byte            ArgumentCount;
        byte            ArgumentSize;
        byte            ArgumentOpcodePostion;

        enum BranchType         m_branchType;
        enum ArgumentTypes      ArgumentTypeList[MAX_NUM_ARG];
        enum InstructionTypes   InstructionType;
        enum InstructionDetails InstructionDetail;
};


#endif // MNEMONICITEM_H
