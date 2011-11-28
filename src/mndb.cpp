/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



/*
 *     Mnemonic DataBase
 */

#include "mndb.h"
#include "mndb_tools.h"



/*
 *  MnemonicDataBase implementation
 */


void MnemonicDataBase::DebugVodoo(wxTextCtrl& log)
{
    uint i,f,x,b,idebug;
    wxString str;
    MnemonicItem *mtemp;

    f=Data.GetCount();

    if ((&log==NULL) && (f==0))
        return;
    log.Clear();

    for (i=0;i<f;i++)
    {
        str.Clear();
        mtemp=(MnemonicItem *)Data[i];
        for (x=0;x<mtemp->getBytesNo();x++)
        {
            b=mtemp->getOpCode(x);
            str += str.Format(_("[%.2X] "),b);
        }
        str += str.Format(_("nArgs = %d "),mtemp->getArgNo());
        str += str.Format(_("ArgSize = %d "),mtemp->getArgSize());
        str += str.Format(_("ArgPos = %d ["),mtemp->getArgPos());
        for (idebug=0;idebug<mtemp->MnemonicString.GetCount();
            idebug++)
        {
            str << mtemp->MnemonicString[idebug];
        }
        str << _("] \n");
        log.AppendText(str);
    }
    str.Clear();
    str.Printf(_("Total allocated: %d bytes\n"),totalAllocated);
    log.AppendText(str);
}

MnemonicItem *MnemonicDataBase::GetData(uint index)
{
    if (index>=Data.GetCount())
        index = Data.GetCount()-1;
    return (MnemonicItem *)Data[index];
}

bool MnemonicDataBase::IsLoaded()
{
    return (GetCount() > 0);
}

uint MnemonicDataBase::GetCount()
{
    return Data.GetCount();
}



void MnemonicDataBase::Clear()
{
    totalAllocated=0;
    int f = Data.GetCount();
    if (f>0)
    {
        MnemonicItem *e;
        for(int i=0; i<f;i++)
        {
            e=(MnemonicItem *)Data.Item(i);
            delete e;
        }
        Data.Clear();
    }
}



MnemonicDataBase::MnemonicDataBase()
{
    totalAllocated=0;
    Data.Clear();
}

MnemonicDataBase::~MnemonicDataBase()
{
    Clear();
}

bool MnemonicDataBase::addData(wxArrayString& arraystr, int currentSection)
{
    if ((arraystr.GetCount()<MIN_ARRAY_ITENS) && (currentSection<1))
        return FALSE;

    int i,x;
    uint f;
    long templong;
    wxString stemp;
    wxChar argtype,last_argtype;
    MnemonicItem *mtemp=0;

    mtemp = new MnemonicItem();
    x=sizeof(MnemonicItem);
    last_argtype=0;

    for (i=0;i<currentSection;i++)
    {
        stemp=arraystr[i];
        if (!(stemp.Find('$')==wxNOT_FOUND))
        {
            argtype=stemp[1];
            switch (argtype) {
                    case wxT('o'):  mtemp->addArgument(ARG_OFFSET);
                                    mtemp->addOpCode(ARG_OFFSET);
                                    break;
                    case wxT('n'):  mtemp->addArgument(ARG_LITERAL);
                                    mtemp->addOpCode(ARG_LITERAL);
                                    break;
                    case wxT('v'):  mtemp->addArgument(ARG_VARIABLE);
                                    mtemp->addOpCode(ARG_VARIABLE);
                                    break;
                    case wxT('c'):  mtemp->addArgument(ARG_ABS_ADDR);
                                    mtemp->addOpCode(ARG_ABS_ADDR);
                                    mtemp->setBranchType(BR_CALL);
                                    break;
                    case wxT('s'):  mtemp->setBranchType(BR_JUMP);
                                    mtemp->addArgument(ARG_ABS_ADDR);
                                    mtemp->addOpCode(ARG_ABS_ADDR);
                                    break;
                    case wxT('j'):  mtemp->addArgument(ARG_REL_ADDR);
                                    mtemp->addOpCode(ARG_REL_ADDR);
                                    mtemp->setBranchType(BR_JUMP);
                                    break;
                    case wxT('g'):
                    case wxT('x'):  mtemp->addArgument(ARG_ABS_ADDR);
                                    mtemp->addOpCode(ARG_ABS_ADDR);
                                    mtemp->setBranchType(BR_JUMP_CND);
                                    break;
                    case wxT('k'):  mtemp->addArgument(ARG_REL_ADDR);
                                    mtemp->addOpCode(ARG_REL_ADDR);
                                    mtemp->setBranchType(BR_JUMP_CND);
                                    break;
                    case wxT('p'):  mtemp->addArgument(ARG_IO_ADDR);
                                    mtemp->addOpCode(ARG_IO_ADDR);
                                    break;

                    default:        mtemp->addArgument(ARG_NONE);
                                    mtemp->addOpCode(ARG_NONE);
                                    break;
            }
            mtemp->setOpCodeArgPos(i);

            continue;  // we've found an argument, so go to the next item...
        }

        if (stemp.ToLong(&templong,16))
            mtemp->addOpCode((unsigned char) templong);
        else
        {
            delete mtemp;
            return FALSE;
        }
    }
    stemp=arraystr[i++];
    if (stemp.IsNumber())
        stemp.ToLong(&templong);
    f=(int)templong;
    // Check if the counted nArgs = file's num. of args
    if (!(mtemp->getArgNo()==f))
    {
        delete mtemp;
        return FALSE;
    }
    stemp=arraystr[i];
    mtemp->setMnemonicStr(stemp);
    Data.Add(mtemp);
    totalAllocated +=x;
    return TRUE;
}


bool MnemonicDataBase::doReadData(wxTextFile& tf)
{
    wxString str;
    wxArrayString arraystr;
    totalAllocated=0;

    int currentSection=-1;

    for (str = tf.GetFirstLine(); !tf.Eof(); str=tf.GetNextLine())
    {
        TrimComment(str);
        if (str.IsEmpty())
            continue;
        if (str.First('[')>=0)
        {
            currentSection=GetSection(str);
            continue;
        }
        if ((currentSection>0) && (str.First('"')>0))
        {
            ParseString(str,arraystr);
            addData(arraystr,currentSection);
        }
    }
    if (Data.IsEmpty())
        return FALSE;
    else
        return TRUE;
}


bool MnemonicDataBase::Open(wxString& opcf)
{
    wxTextFile mnfile;

    if (mnfile.Open(opcf,wxConvLocal))
    {
        if (IsLoaded())
            Clear();
        if (doReadData(mnfile))
            return TRUE;
    }
    return FALSE;
}


void MnemonicDataBase::FindItems(wxArrayInt& arrayint, byte opcode, uint scanoffset)
{
    uint			i, f, index;
    MnemonicItem	*mtemp;
    wxArrayInt		arrtemp;
    bool			arg_detected;
    byte			opcodetest;

    if (scanoffset == 0)
    {
        arrayint.Clear();
        f = Data.GetCount();
    }
    else
        f = arrayint.GetCount();

    arg_detected = TRUE;
    if (f > 1)
    {
        arrtemp.Clear();
        for (i = 0; i < f; i++)
        {

            if (scanoffset == 0)
                index = i;
            else
                index = arrayint[i];
            mtemp = (MnemonicItem *)Data[index];
            // In four-byte instructions of Z80, there are opcodes that
            // have its argument in the middle of it (DD CB's and FD CB's series)
            // so we must shift right the index to take the code
            // The test below detects this condition
            if ((!(mtemp->getArgPos() == scanoffset)) || (mtemp->getArgPos() == 0))
            {
                arg_detected = FALSE;
                opcodetest = mtemp->getOpCode(scanoffset);
                if (opcode == opcodetest)
                {
                    if (scanoffset == 0)
                        arrayint.Add(i);
                    else
                        arrtemp.Add(index);
                }
            }
        }
        if ((scanoffset > 0) && (!arg_detected))
        {
            arrayint.Clear();
            arrayint = arrtemp;
        }
    }
}



MnemonicItem *MnemonicDataBase::FindItem(const ByteCode& code)
{
    uint i, total;

    total = GetCount();
    MnemonicItem* ret = 0;

    for (i = 0; i < total; i++)
    {
        ret = GetData(i);
        if (memcmp(ret->getOpCode(),&code,MAX_OPCODE_SIZE) != 0)
            ret = 0;
        else
            i = total;
    }

    return ret;
}



uint MnemonicDataBase::GetAllocated()
{
    return totalAllocated;
}


