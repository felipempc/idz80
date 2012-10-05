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
 * Define Mnemonic Array of Object
 */
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(MnemonicArray);



/*
 *  MnemonicDataBase implementation
 */

const int MnemonicDataBase::MIN_ARRAY_ITEMS;




MnemonicDataBase::MnemonicDataBase()
{
    m_totalAllocated = 0;
    m_MnemonicList = new MnemonicArray();
    m_modulename = "MNDB:";
}

MnemonicDataBase::~MnemonicDataBase()
{
    Clear();
    delete m_MnemonicList;
}




void MnemonicDataBase::Clear()
{
    m_totalAllocated = 0;
    m_MnemonicList->Clear();
}





MnemonicItem *MnemonicDataBase::GetData(uint index)
{
    int f = m_MnemonicList->GetCount();

    if (index >= f)
        index = f - 1;
    return m_MnemonicList->Item(index);
}



bool MnemonicDataBase::IsLoaded()
{
    return (GetCount() > 0);
}


uint MnemonicDataBase::GetCount()
{
    return m_MnemonicList->GetCount();
}



bool MnemonicDataBase::SetupArgument(MnemonicItem *mnemonic, wxString &strline)
{
    wxChar      argtype;
    bool        ret = true;

    if (strline.Find('$') != wxNOT_FOUND)
    {
        argtype = strline[1];
        switch (argtype) {
                case 'o':  mnemonic->addArgument(ARG_OFFSET);
                           mnemonic->addOpCode(ARG_OFFSET);
                           break;
                case 'n':  mnemonic->addArgument(ARG_LITERAL);
                           mnemonic->addOpCode(ARG_LITERAL);
                           break;
                case 'v':  mnemonic->addArgument(ARG_VARIABLE);
                           mnemonic->addOpCode(ARG_VARIABLE);
                           break;
                case 'c':  mnemonic->addArgument(ARG_ABS_ADDR);
                           mnemonic->addOpCode(ARG_ABS_ADDR);
                           mnemonic->setBranchType(BR_CALL);
                           break;
                case 's':  mnemonic->setBranchType(BR_JUMP);
                           mnemonic->addArgument(ARG_ABS_ADDR);
                           mnemonic->addOpCode(ARG_ABS_ADDR);
                           break;
                case 'j':  mnemonic->addArgument(ARG_REL_ADDR);
                           mnemonic->addOpCode(ARG_REL_ADDR);
                           mnemonic->setBranchType(BR_JUMP);
                           break;
                case 'g':  mnemonic->addArgument(ARG_ABS_ADDR);
                           mnemonic->addOpCode(ARG_ABS_ADDR);
                           mnemonic->setBranchType(BR_CALL_CND);
                           break;
                case 'x':  mnemonic->addArgument(ARG_ABS_ADDR);
                           mnemonic->addOpCode(ARG_ABS_ADDR);
                           mnemonic->setBranchType(BR_JUMP_CND);
                           break;
                case 'k':  mnemonic->addArgument(ARG_REL_ADDR);
                           mnemonic->addOpCode(ARG_REL_ADDR);
                           mnemonic->setBranchType(BR_JUMP_CND);
                           break;
                case 'p':  mnemonic->addArgument(ARG_IO_ADDR);
                           mnemonic->addOpCode(ARG_IO_ADDR);
                           break;

                default:
                            mnemonic->addArgument(ARG_NONE);
                            mnemonic->addOpCode(ARG_NONE);
                            mnemonic->setBranchType(BR_NONE);
                            break;
        } // switch
    } // endif
    else
        ret = false;

    return ret;
}



/*
 *  Setup branch info of RETurn instructions
 *  Compatibility routine
 */
void MnemonicDataBase::SetupBranch(MnemonicItem *mnemonic)
{
    if (mnemonic->GetInstructionType() == IT_RET)
        switch (mnemonic->GetInstructionInfo())
        {
            case II_NONE:
                        mnemonic->setBranchType(BR_RETURN);
                        break;
            case II_CONDITIONAL:
                        mnemonic->setBranchType(BR_RETURN_CND);
                        break;
            default:
                        mnemonic->setBranchType(BR_NONE);
                        break;
        }
}





bool MnemonicDataBase::addData(wxArrayString& arraystr, int currentSection, int line)
{
    bool    ret = false;
    int     i,
            x = 0;
    uint    num_arguments;
    long    templong;

    wxString    temp_str,
                ret_str;
    //wxChar      argtype;

    MnemonicItem    *mnemonic = 0;



    if ((arraystr.GetCount() >= MIN_ARRAY_ITEMS) && (currentSection > 0))
    {
        mnemonic = new MnemonicItem();
        x = sizeof(MnemonicItem);

        for (i = 0; i < currentSection; i++)
        {
            temp_str = arraystr[i];

            if (SetupArgument(mnemonic, temp_str))
            {
                mnemonic->setOpCodeArgPos(i);
                continue;                   // we've found an argument, so go to the next item...
            }

            if (temp_str.ToLong(&templong, 16))
                mnemonic->addOpCode((byte) templong);
            else
            {
                #ifdef IDZ80DEBUG
                LogIt(wxString::Format("Error processing opcode in line %d ! Dropped line.\n", line));
                #endif
                delete mnemonic;
                mnemonic = 0;
                break;
            }
        }


        /*
         *  Get the number of arguments
         */
        if (mnemonic != 0)
        {
            temp_str = arraystr[i++];
            if (!temp_str.ToLong(&templong, 16))
            {
                #ifdef IDZ80DEBUG
                LogIt(wxString::Format("Error processing num of arguments in line %d ! Dropped line.\n", line));
                #endif
                delete mnemonic;
                mnemonic = 0;
            }
        }

        /*
         * Check if num arguments found is equal to file num of arguments
         */
        if (mnemonic != 0)
        {
            num_arguments = (int)templong;
            if (mnemonic->getArgNo() != num_arguments)
            {
                delete mnemonic;
                mnemonic = 0;
                #ifdef IDZ80DEBUG
                LogIt(wxString::Format("Error, num of arguments doesnt match in line %d ! Dropped line.\n", line));
                #endif
            }
        }


        /*
         *  Get Instruction type
         */
        if (mnemonic != 0)
        {
            temp_str = arraystr[i++];
            if (temp_str.ToLong(&templong, 16))
                mnemonic->setInstructionType(templong);
            else
            {
                delete mnemonic;
                mnemonic = 0;
                #ifdef IDZ80DEBUG
                LogIt(wxString::Format("Error converting instruction type in line %d ! Dropped line.\n", line));
                #endif
            }
        }

        /*
         *  Get Instruction info
         */
        if (mnemonic != 0)
        {
            temp_str = arraystr[i++];
            if (temp_str.ToLong(&templong, 16))
                mnemonic->setInstructionInfo(templong);
            else
            {
                delete mnemonic;
                mnemonic = 0;
                #ifdef IDZ80DEBUG
                LogIt(wxString::Format("Error converting instruction info in line %d ! Dropped line.\n", line));
                #endif
            }
        }

        /*
         *  Setup Instruction String,
         *  branch information and
         *  add object to the list
         */
        if (mnemonic != 0)
        {
            temp_str = arraystr[i];
            mnemonic->setMnemonicStr(temp_str);
            SetupBranch(mnemonic);
            m_MnemonicList->Add(mnemonic);
            ret = true;
            m_totalAllocated += x;
        }
    }
    return ret;
}




bool MnemonicDataBase::doReadData(wxTextFile& tf)
{
    wxString        str;
    wxArrayString   arraystr;

    m_totalAllocated = 0;

    int currentSection = -1,
        currentline = 0;

    for (str = tf.GetFirstLine(); !tf.Eof(); str = tf.GetNextLine())
    {
        currentline++;

        TrimComment(str);
        if (str.IsEmpty())
            continue;
        if (str.First('[') >= 0)
        {
            currentSection = GetSection(str);
            continue;
        }
        if ((currentSection > 0) && (str.First('"') > 0))
        {
            ParseString(str, arraystr);
            addData(arraystr, currentSection, currentline);
        }
    }
    if (m_MnemonicList->IsEmpty())
        return false;
    else
        return true;
}




bool MnemonicDataBase::Open(wxString& opcf)
{
    wxTextFile mnfile;

    if (mnfile.Open(opcf,wxConvLocal))
    {
        if (IsLoaded())
            Clear();
        if (doReadData(mnfile))
            return true;
    }
    return false;
}


void MnemonicDataBase::FindItems(wxArrayInt& arrayint, byte opcode, uint scanoffset)
{
    uint			i, f, index;
    MnemonicItem	*mnemonic;
    wxArrayInt		arrtemp;
    bool			arg_detected;
    byte			opcodetest;

    if (scanoffset == 0)
    {
        arrayint.Clear();
        f = m_MnemonicList->GetCount(); //Data.GetCount();
    }
    else
        f = arrayint.GetCount();

    arg_detected = true;
    if (f > 1)
    {
        arrtemp.Clear();
        for (i = 0; i < f; i++)
        {

            if (scanoffset == 0)
                index = i;
            else
                index = arrayint[i];
            mnemonic = m_MnemonicList->Item(index);

            /* In four-byte instructions of Z80, there are opcodes that
               have its argument in the middle of it (DD CB's and FD CB's series)
               so we must shift right the index to take the code
               The test below detects this condition
             */
            if ((!(mnemonic->getArgPos() == scanoffset)) || (mnemonic->getArgPos() == 0))
            {
                arg_detected = false;
                opcodetest = mnemonic->getOpCode(scanoffset);
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
        if (memcmp(ret->getOpCode(), &code, MAX_OPCODE_SIZE) != 0)
            ret = 0;
        else
            break;
    }

    return ret;
}



uint MnemonicDataBase::GetAllocated()
{
    return m_totalAllocated;
}


void MnemonicDataBase::DebugVodoo()
{
    uint    i,
            f,
            x,
            b,
            idebug;

    wxString        str;
    MnemonicItem    *mnemonic;

    f = m_MnemonicList->GetCount();

    if ((m_log == 0) || (f == 0))
        return;

    for (i = 0; i < f; i++)
    {
        str.Clear();
        mnemonic = m_MnemonicList->Item(i);
        /*
        for (x = 0; x < mnemonic->getBytesNo(); x++)
        {
            b = mnemonic->getOpCode(x);
            str += str.Format("[%.2X] ", b);
        }
        */
        str += str.Format("nArgs = %d ", mnemonic->getArgNo());
        str += str.Format("ArgSize = %d ", mnemonic->getArgSize());
        str += str.Format("BranchType = %d ", mnemonic->getBranchType());
        //str += str.Format("ArgPos = %d [", mnemonic->getArgPos());
        for (idebug = 0; idebug < mnemonic->MnemonicString.GetCount();
            idebug++)
        {
            str << mnemonic->MnemonicString[idebug];
        }
        str << "] \n";
        LogIt(str);
    }
    str.Clear();
    str.Printf("Total allocated: %d bytes\n", m_totalAllocated);
    LogIt(str);
}

