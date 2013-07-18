/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Stores a mnemonic item
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
        bool HasArgument();
        enum ArgumentTypes GetArgumentType(uint argno);
        uint GetArgumentCount();
        enum BranchType GetBranchType();
        void AddOpCode(byte opcode);
        void AddArgument(enum ArgumentTypes argument);
        void SetMnemonicStr(wxString str);
        void SetOpCodeArgumentPos(uint argpos);
        void SetBranchType(enum BranchType brunchtype);
        void SetInstructionType(enum InstructionTypes itype);
        bool SetInstructionType(uint itype);
        void SetInstructionDetail(enum InstructionDetails iinfo);
        bool SetInstructionDetail(uint iinfo);
        enum InstructionTypes GetInstructionType();
        enum InstructionDetails GetInstructionDetail();
        void Clear();

        uint GetOpCodeSize();
        uint GetOpCode(uint opcode);
        ByteCode *GetOpCode();
        uint GetArgumentSize();
        uint GetArgumentPosition();

        bool IsUnconditionalJump();
        bool IsConditionalJump();
        bool IsJump();
        bool IsCall();
        bool IsReturn();
        bool IsConditionalReturn();

        wxArrayString mnemonic_strings_;

    protected:
    private:

        ByteCode        opcode_;
        byte            opcode_size_;
        byte            argument_count_;
        byte            argument_size_;
        byte            argument_opcode_postion_;

        enum BranchType         branch_type_;
        enum ArgumentTypes      argument_type_list_[MAX_NUM_ARG];
        enum InstructionTypes   instruction_type_;
        enum InstructionDetails instruction_detail_;
};


#endif // MNEMONICITEM_H
