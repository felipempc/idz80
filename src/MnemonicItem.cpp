/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe MPC ()
 * License:   GPL
 * This module store one mnemonic item
 **************************************************************/

#include "MnemonicItem.h"



void MnemonicItem::Clear()
{
    int i;
    opcode_size_ = 0;
    argument_count_ = 0;
    argument_size_ = 0;
    argument_opcode_postion_ = 0;
    branch_type_ = BR_NONE;
    memset(opcode_, 0, sizeof(ByteCode));
    mnemonic_strings_.Clear();
    for (i = 0; i < MAX_NUM_ARG; i++)
        argument_type_list_[i] = ARG_NONE;
}


MnemonicItem::MnemonicItem()
{
    Clear();
}

void MnemonicItem::AddArgument(enum ArgumentTypes argument)
{
    if (argument_count_ < MAX_NUM_ARG)
    {
        argument_type_list_[argument_count_] = argument;
        if (argument_count_ > 0)
        {
            if (argument_type_list_[argument_count_ - 1] == argument)
                argument_size_++;
            else
                argument_count_++;
        }
        else
        {
            argument_count_ = 1;
            argument_size_ = 1;
        }
    }
}


void MnemonicItem::AddOpCode(byte opcode)
{
    opcode_[opcode_size_++] = opcode;
}

uint MnemonicItem::GetArgumentCount()
{
    return static_cast<uint>(argument_count_) & 0xFF;
}


ArgumentTypes MnemonicItem::GetArgumentType(unsigned int argno)
{
    enum ArgumentTypes arg = ARG_NONE;
    if (argno < argument_count_)
        arg = argument_type_list_[argno];
    return arg;
}

BranchType MnemonicItem::GetBranchType()
{
    return branch_type_;
}



bool MnemonicItem::IsUnconditionalJump()
{
	return ((instruction_type_ == IT_JUMP) && (instruction_detail_ == II_NONE));
}

bool MnemonicItem::IsConditionalJump()
{
	return ((instruction_type_ == IT_JUMP) && (instruction_detail_ == II_CONDITIONAL));
}

bool MnemonicItem::IsJump()
{
	return (instruction_type_ == IT_JUMP);
}

bool MnemonicItem::IsCall()
{
	return (instruction_type_ == IT_CALL);
}

bool MnemonicItem::IsReturn()
{
	return (instruction_type_ == IT_RET);
}

bool MnemonicItem::IsConditionalReturn()
{
    return ((instruction_type_ == IT_RET) && (instruction_detail_ == II_CONDITIONAL));
}


bool MnemonicItem::HasArgument()
{
    return (argument_count_ > 0);
}


void MnemonicItem::SetMnemonicStr(wxString str)
{
    int i1,i2;
    wxString strtemp;
    i1 = str.Find(ARG_POSITION_CHAR, false);
    i2 = str.Find(ARG_POSITION_CHAR, true);
    if ((i1 == i2) && (!(i1 == wxNOT_FOUND)))
    {
        mnemonic_strings_.Add(str.Left(i1));
        strtemp = str.Right(str.Len() - i1 - 1);
        mnemonic_strings_.Add(strtemp);
    }
    else
        if ((i1 != wxNOT_FOUND) || (i2 != wxNOT_FOUND))
        {
            mnemonic_strings_.Add(str.Left(i1));
            strtemp = str.Mid(i1 + 1,(i2 - i1 - 1));
            if (!strtemp.IsEmpty())
                mnemonic_strings_.Add(strtemp);
        }
        else
            mnemonic_strings_.Add(str);
}


void MnemonicItem::SetOpCodeArgumentPos(uint argpos)
{
    if (argument_opcode_postion_ == 0)        // we want the first argument position
        argument_opcode_postion_ = static_cast<byte>(argpos);
}

unsigned int MnemonicItem::GetOpCodeSize()
{
    uint ret;
    ret = static_cast<uint>(opcode_size_);
    ret = ret & 0xFF;
    return ret;
}


ByteCode *MnemonicItem::GetOpCode()
{
    return &opcode_;
}


uint MnemonicItem::GetOpCode(uint opcode)
{
    uint ret;
    ret = static_cast<uint>(opcode_[opcode]);
    ret = ret & 0xFF;
    return ret;
}

uint MnemonicItem::GetArgumentSize()
{
    unsigned int ret;
    ret = static_cast<uint>(argument_size_);
    ret = ret & 0xFF;
    return ret;
}

uint MnemonicItem::GetArgumentPosition()
{
    unsigned int ret;
    ret = static_cast<uint>(argument_opcode_postion_);
    ret = ret & 0xFF;
    return ret;
}


void MnemonicItem::SetBranchType(enum BranchType branchtype)
{
    branch_type_ = branchtype;
}



void MnemonicItem::SetInstructionType(enum InstructionTypes itype)
{
    instruction_type_ = itype;
}


bool MnemonicItem::SetInstructionType(uint itype)
{
    bool ret = true;
    switch (itype)
    {
        case 0:
                SetInstructionType(IT_NONE);
                break;
        case 1:
                SetInstructionType(IT_RST);
                break;
        case 2:
                SetInstructionType(IT_CALL);
                break;
        case 3:
                SetInstructionType(IT_RET);
                break;
        case 4:
                SetInstructionType(IT_JUMP);
                break;
        case 5:
                SetInstructionType(IT_LOAD);
                break;
        default:
                ret = false;
    }
    return ret;
}



void MnemonicItem::SetInstructionDetail(enum InstructionDetails iinfo)
{
    instruction_detail_ = iinfo;
}


bool MnemonicItem::SetInstructionDetail(uint iinfo)
{
    bool ret = true;
    switch (iinfo)
    {
        case 0:
                SetInstructionDetail(II_NONE);
                break;
        case 1:
                SetInstructionDetail(II_RST_00);
                break;
        case 2:
                SetInstructionDetail(II_RST_08);
                break;
        case 3:
                SetInstructionDetail(II_RST_10);
                break;
        case 4:
                SetInstructionDetail(II_RST_18);
                break;
        case 5:
                SetInstructionDetail(II_RST_20);
                break;
        case 6:
                SetInstructionDetail(II_RST_28);
                break;
        case 7:
                SetInstructionDetail(II_RST_30);
                break;
        case 8:
                SetInstructionDetail(II_RST_38);
                break;
        case 9:
                SetInstructionDetail(II_CONDITIONAL);
                break;
        case 10:
                SetInstructionDetail(II_LD_A_N);
                break;
        case 11:
                SetInstructionDetail(II_LD_B_N);
                break;
        case 12:
                SetInstructionDetail(II_LD_C_N);
                break;
        case 13:
                SetInstructionDetail(II_LD_D_N);
                break;
        case 14:
                SetInstructionDetail(II_LD_E_N);
                break;
        case 15:
                SetInstructionDetail(II_LD_H_N);
                break;
        case 16:
                SetInstructionDetail(II_LD_L_N);
                break;
        case 17:
                SetInstructionDetail(II_LD_HL_N);
                break;
        case 18:
                SetInstructionDetail(II_LD_DE_N);
                break;
        case 19:
                SetInstructionDetail(II_LD_BC_N);
                break;
        case 20:
                SetInstructionDetail(II_LD_SP_N);
                break;
        case 21:
                SetInstructionDetail(II_LD_IX_N);
                break;
        case 22:
                SetInstructionDetail(II_LD_IY_N);
                break;
        default:
                ret = false;
                break;
    }
    return ret;
}



enum InstructionTypes MnemonicItem::GetInstructionType()
{
    return instruction_type_;
}


enum InstructionDetails MnemonicItem::GetInstructionDetail()
{
    return instruction_detail_;
}



