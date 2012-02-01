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
#include "idz80base.h"

#define MAX_NUM_ARG 2
#define MAX_OPCODE_SIZE 4
#define ARG_POSITION_CHAR   '$'

enum ArgType
{
    ARG_NONE=0,
    ARG_LITERAL,
    ARG_OFFSET,
    ARG_VARIABLE,
    ARG_ABS_ADDR,
    ARG_REL_ADDR,
    ARG_IO_ADDR
};

enum BranchType
{
    BR_NONE=0,
    BR_JUMP,
    BR_JUMP_CND,
    BR_CALL
};


typedef unsigned char ByteCode [MAX_OPCODE_SIZE];


class MnemonicItem
{
    public:
        MnemonicItem();
        bool hasArgument();
        enum ArgType getArgType(unsigned int argno);
        unsigned int getArgNo();
        enum BranchType getBranchType();
        void addOpCode(unsigned char opcode);
        void addArgument(enum ArgType argument);
        void setMnemonicStr(wxString str);
        void setOpCodeArgPos(unsigned int argpos);
        void setBranchType(enum BranchType brunchtype);
        void Clear();

        unsigned int getBytesNo();
        unsigned int getOpCode(unsigned int opcode);
        ByteCode *getOpCode();
        unsigned int getArgSize();
        unsigned int getArgPos();
        wxArrayString MnemonicString;

    protected:
    private:
        unsigned char   m_bytesNo;      // opcode´s number of bytes
        unsigned char   m_argNo;        // number of arguments
        unsigned char   m_argSize;      // size of argument
        unsigned int    m_argCount;     // internal use
        enum ArgType    m_lastArg;      //     "     "
        unsigned char   m_argPos;       // position of argument within opcode
        enum BranchType m_branchType;   // deviation type : none,jump or call
        ByteCode        m_opCode;       // the opcode itself
        enum ArgType    m_argType[MAX_NUM_ARG];      // type of argument
};

#endif // MNEMONICITEM_H
