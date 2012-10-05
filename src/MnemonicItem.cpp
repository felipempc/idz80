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
    m_bytesNo = 0;
    m_argNo = 0;
    m_argSize = 0;
    m_argPos = 0;
    m_argCount = 0;
    m_lastArg = ARG_NONE;
    m_branchType = BR_NONE;
    memset(m_opCode, 0, sizeof(ByteCode));
    MnemonicString.Clear();
    for (i = 0; i < MAX_NUM_ARG; i++)
        m_argType[i] = ARG_NONE;
}


MnemonicItem::MnemonicItem()
{
    Clear();
}

void MnemonicItem::addArgument(enum ArgType argument)
{
    if (m_argCount < MAX_NUM_ARG)
    {
        m_argType[m_argCount] = argument;
        if (m_argCount > 0)
        {
            if (m_lastArg == argument)
            {
                m_argNo = m_argCount;
                m_argSize = m_argCount + 1;
            }
            else
            {
                m_argNo = m_argCount + 1;
                m_argSize = m_argCount;
            }
        }
        else
        {
            m_argNo = 1;
            m_argSize = 1;
            m_lastArg = argument;
        }
        m_argCount++;
    }
}

void MnemonicItem::addOpCode(byte opcode)
{
    m_opCode[m_bytesNo++] = opcode;
}

unsigned int MnemonicItem::getArgNo()
{
    return (uint)m_argNo;
}


ArgType MnemonicItem::getArgType(unsigned int argno)
{
    return m_argType[argno];
}

BranchType MnemonicItem::getBranchType()
{
    return m_branchType;
}

bool MnemonicItem::isUnconditionalJump()
{
	return (m_branchType == BR_JUMP);
}

bool MnemonicItem::isConditionalJump()
{
	return (m_branchType == BR_JUMP_CND);
}


bool MnemonicItem::isJump()
{
	return ((m_branchType == BR_JUMP_CND) || (m_branchType == BR_JUMP));
}


bool MnemonicItem::isCall()
{
	return ((m_branchType == BR_CALL_CND) || (m_branchType == BR_CALL));
}



bool MnemonicItem::isReturn()
{
	return (m_branchType == BR_RETURN);
}



bool MnemonicItem::hasArgument()
{
    return (m_argNo > 0);
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


void MnemonicItem::setOpCodeArgPos(unsigned int argpos)
{
    if (m_argPos == 0)        // we want the first argument position
        m_argPos = (unsigned char)argpos;
}

unsigned int MnemonicItem::getBytesNo()
{
    uint ret;
    ret = (uint) m_bytesNo;
    ret = ret & 0xFF;
    return ret;
}


ByteCode *MnemonicItem::getOpCode()
{
    return &m_opCode;
}


unsigned int MnemonicItem::getOpCode(unsigned int opcode)
{
    unsigned int ret;
    ret = (unsigned int) m_opCode[opcode];
    ret = ret & 0xFF;
    return ret;
}

unsigned int MnemonicItem::getArgSize()
{
    unsigned int ret;
    ret = (unsigned int) m_argSize;
    ret = ret & 0xFF;
    return ret;
}

unsigned int MnemonicItem::getArgPos()
{
    unsigned int ret;
    ret = (unsigned int) m_argPos;
    ret = ret & 0xFF;
    return ret;
}


void MnemonicItem::setBranchType(enum BranchType branchtype)
{
    m_branchType = branchtype;
}



void MnemonicItem::setInstructionType(enum InstruType itype)
{
    m_type = itype;
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



void MnemonicItem::setInstructionInfo(enum InstruInfo iinfo)
{
    m_info = iinfo;
}


bool MnemonicItem::setInstructionInfo(uint iinfo)
{
    bool ret = true;
    switch (iinfo)
    {
        case 0:
                setInstructionInfo(II_NONE);
                break;
        case 1:
                setInstructionInfo(II_RST_00);
                break;
        case 2:
                setInstructionInfo(II_RST_08);
                break;
        case 3:
                setInstructionInfo(II_RST_10);
                break;
        case 4:
                setInstructionInfo(II_RST_18);
                break;
        case 5:
                setInstructionInfo(II_RST_20);
                break;
        case 6:
                setInstructionInfo(II_RST_28);
                break;
        case 7:
                setInstructionInfo(II_RST_30);
                break;
        case 8:
                setInstructionInfo(II_RST_38);
                break;
        case 9:
                setInstructionInfo(II_CONDITIONAL);
                break;
        case 10:
                setInstructionInfo(II_LD_A_N);
                break;
        case 11:
                setInstructionInfo(II_LD_B_N);
                break;
        case 12:
                setInstructionInfo(II_LD_C_N);
                break;
        case 13:
                setInstructionInfo(II_LD_D_N);
                break;
        case 14:
                setInstructionInfo(II_LD_E_N);
                break;
        case 15:
                setInstructionInfo(II_LD_H_N);
                break;
        case 16:
                setInstructionInfo(II_LD_L_N);
                break;
        case 17:
                setInstructionInfo(II_LD_HL_N);
                break;
        case 18:
                setInstructionInfo(II_LD_DE_N);
                break;
        case 19:
                setInstructionInfo(II_LD_BC_N);
                break;
        case 20:
                setInstructionInfo(II_LD_SP_N);
                break;
        case 21:
                setInstructionInfo(II_LD_IX_N);
                break;
        case 22:
                setInstructionInfo(II_LD_IY_N);
                break;
        default:
                ret = false;
                break;
    }
    return ret;
}



enum InstruType MnemonicItem::GetInstructionType()
{
    return m_type;
}


enum InstruInfo MnemonicItem::GetInstructionInfo()
{
    return m_info;
}



