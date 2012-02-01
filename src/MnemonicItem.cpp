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

void MnemonicItem::addOpCode(unsigned char opcode)
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


bool MnemonicItem::hasArgument()
{
    return (m_argNo > 0);
}


void MnemonicItem::setMnemonicStr(wxString str)
{
    int i1,i2;
    wxString strtemp;
    i1 = str.Find(ARG_POSITION_CHAR,false);
    i2 = str.Find(ARG_POSITION_CHAR,true);
    if ((i1 == i2) && (!(i1 == wxNOT_FOUND)))
    {
        MnemonicString.Add(str.Left(i1));
        strtemp = str.Right(str.Len() - i1 - 1);
        MnemonicString.Add(strtemp);
    }
    else
        if (!((i1 == wxNOT_FOUND) || (i2 == wxNOT_FOUND)))
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
    unsigned int ret;
    ret = (unsigned int) m_bytesNo;
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

