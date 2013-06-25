/****************************************************************
 * Name:      IDZ80
 * Purpose:   Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module holds one mnemonic item
 **************************************************************/

#include "MnemonicItem.h"



void MnemonicItem::Clear()
{
    int i;
    OpcodeSize = 0;
    ArgumentCount = 0;
    ArgumentSize = 0;
    ArgumentOpcodePostion = 0;
    m_branchType = BR_NONE;
    memset(Opcode, 0, sizeof(ByteCode));
    MnemonicString.Clear();
    for (i = 0; i < MAX_NUM_ARG; i++)
        ArgumentTypeList[i] = ARG_NONE;
}


MnemonicItem::MnemonicItem()
{
    Clear();
}

void MnemonicItem::addArgument(enum ArgumentTypes argument)
{
    if (ArgumentCount < MAX_NUM_ARG)
    {
        ArgumentTypeList[ArgumentCount] = argument;
        if (ArgumentCount > 0)
        {
            if (ArgumentTypeList[ArgumentCount - 1] == argument)
                ArgumentSize++;
            else
                ArgumentCount++;
        }
        else
        {
            ArgumentCount = 1;
            ArgumentSize = 1;
        }
    }
}


void MnemonicItem::addOpCode(byte opcode)
{
    Opcode[OpcodeSize++] = opcode;
}

uint MnemonicItem::getArgNo()
{
    return static_cast<uint>(ArgumentCount) & 0xFF;
}


ArgumentTypes MnemonicItem::getArgType(unsigned int argno)
{
    enum ArgumentTypes arg = ARG_NONE;
    if (argno < ArgumentCount)
        arg = ArgumentTypeList[argno];
    return arg;
}

BranchType MnemonicItem::getBranchType()
{
    return m_branchType;
}



bool MnemonicItem::isUnconditionalJump()
{
	return ((InstructionType == IT_JUMP) && (InstructionDetail == II_NONE));
}

bool MnemonicItem::isConditionalJump()
{
	return ((InstructionType == IT_JUMP) && (InstructionDetail == II_CONDITIONAL));
}

bool MnemonicItem::isJump()
{
	return (InstructionType == IT_JUMP);
}

bool MnemonicItem::isCall()
{
	return (InstructionType == IT_CALL);
}

bool MnemonicItem::isReturn()
{
	return (InstructionType == IT_RET);
}

bool MnemonicItem::isConditionalReturn()
{
    return ((InstructionType == IT_RET) && (InstructionDetail == II_CONDITIONAL));
}


bool MnemonicItem::hasArgument()
{
    return (ArgumentCount > 0);
}


void MnemonicItem::setMnemonicStr(wxString str)
{
    int i1,i2;
    wxString strtemp;
    i1 = str.Find(ARG_POSITION_CHAR, false);
    i2 = str.Find(ARG_POSITION_CHAR, true);
    if ((i1 == i2) && (!(i1 == wxNOT_FOUND)))
    {
        MnemonicString.Add(str.Left(i1));
        strtemp = str.Right(str.Len() - i1 - 1);
        MnemonicString.Add(strtemp);
    }
    else
        if ((i1 != wxNOT_FOUND) || (i2 != wxNOT_FOUND))
        {
            MnemonicString.Add(str.Left(i1));
            strtemp = str.Mid(i1 + 1,(i2 - i1 - 1));
            if (!strtemp.IsEmpty())
                MnemonicString.Add(strtemp);
        }
        else
            MnemonicString.Add(str);
}


void MnemonicItem::setOpCodeArgPos(uint argpos)
{
    if (ArgumentOpcodePostion == 0)        // we want the first argument position
        ArgumentOpcodePostion = static_cast<byte>(argpos);
}

unsigned int MnemonicItem::getBytesNo()
{
    uint ret;
    ret = static_cast<uint>(OpcodeSize);
    ret = ret & 0xFF;
    return ret;
}


ByteCode *MnemonicItem::getOpCode()
{
    return &Opcode;
}


uint MnemonicItem::getOpCode(uint opcode)
{
    uint ret;
    ret = static_cast<uint>(Opcode[opcode]);
    ret = ret & 0xFF;
    return ret;
}

uint MnemonicItem::getArgSize()
{
    unsigned int ret;
    ret = static_cast<uint>(ArgumentSize);
    ret = ret & 0xFF;
    return ret;
}

uint MnemonicItem::getArgPos()
{
    unsigned int ret;
    ret = static_cast<uint>(ArgumentOpcodePostion);
    ret = ret & 0xFF;
    return ret;
}


void MnemonicItem::setBranchType(enum BranchType branchtype)
{
    m_branchType = branchtype;
}



void MnemonicItem::setInstructionType(enum InstructionTypes itype)
{
    InstructionType = itype;
}


bool MnemonicItem::setInstructionType(uint itype)
{
    bool ret = true;
    switch (itype)
    {
        case 0:
                setInstructionType(IT_NONE);
                break;
        case 1:
                setInstructionType(IT_RST);
                break;
        case 2:
                setInstructionType(IT_CALL);
                break;
        case 3:
                setInstructionType(IT_RET);
                break;
        case 4:
                setInstructionType(IT_JUMP);
                break;
        case 5:
                setInstructionType(IT_LOAD);
                break;
        default:
                ret = false;
    }
    return ret;
}



void MnemonicItem::setInstructionDetail(enum InstructionDetails iinfo)
{
    InstructionDetail = iinfo;
}


bool MnemonicItem::setInstructionDetail(uint iinfo)
{
    bool ret = true;
    switch (iinfo)
    {
        case 0:
                setInstructionDetail(II_NONE);
                break;
        case 1:
                setInstructionDetail(II_RST_00);
                break;
        case 2:
                setInstructionDetail(II_RST_08);
                break;
        case 3:
                setInstructionDetail(II_RST_10);
                break;
        case 4:
                setInstructionDetail(II_RST_18);
                break;
        case 5:
                setInstructionDetail(II_RST_20);
                break;
        case 6:
                setInstructionDetail(II_RST_28);
                break;
        case 7:
                setInstructionDetail(II_RST_30);
                break;
        case 8:
                setInstructionDetail(II_RST_38);
                break;
        case 9:
                setInstructionDetail(II_CONDITIONAL);
                break;
        case 10:
                setInstructionDetail(II_LD_A_N);
                break;
        case 11:
                setInstructionDetail(II_LD_B_N);
                break;
        case 12:
                setInstructionDetail(II_LD_C_N);
                break;
        case 13:
                setInstructionDetail(II_LD_D_N);
                break;
        case 14:
                setInstructionDetail(II_LD_E_N);
                break;
        case 15:
                setInstructionDetail(II_LD_H_N);
                break;
        case 16:
                setInstructionDetail(II_LD_L_N);
                break;
        case 17:
                setInstructionDetail(II_LD_HL_N);
                break;
        case 18:
                setInstructionDetail(II_LD_DE_N);
                break;
        case 19:
                setInstructionDetail(II_LD_BC_N);
                break;
        case 20:
                setInstructionDetail(II_LD_SP_N);
                break;
        case 21:
                setInstructionDetail(II_LD_IX_N);
                break;
        case 22:
                setInstructionDetail(II_LD_IY_N);
                break;
        default:
                ret = false;
                break;
    }
    return ret;
}



enum InstructionTypes MnemonicItem::GetInstructionType()
{
    return InstructionType;
}


enum InstructionDetails MnemonicItem::GetInstructionDetail()
{
    return InstructionDetail;
}



