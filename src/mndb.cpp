/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe MPC ()
 * License:   GPL
 * This module is a database of mnemonic items
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

const uint MnemonicDataBase::MIN_ARRAY_ITEMS;




MnemonicDataBase::MnemonicDataBase(LogWindow *logparent)
{
    memory_allocated_ = 0;
    mnemonic_list_ = new MnemonicArray();
    SetTextLog(logparent);
    ModuleName = "MNDB";
}

MnemonicDataBase::~MnemonicDataBase()
{
    Clear();
    delete mnemonic_list_;
}




void MnemonicDataBase::Clear()
{
    memory_allocated_ = 0;
    mnemonic_list_->Clear();
}





MnemonicItem *MnemonicDataBase::GetData(uint index)
{
    uint f = mnemonic_list_->GetCount();

    if (index >= f)
        index = f - 1;
    return mnemonic_list_->Item(index);
}



bool MnemonicDataBase::IsLoaded()
{
    return (GetCount() > 0);
}


uint MnemonicDataBase::GetCount()
{
    return mnemonic_list_->GetCount();
}



bool MnemonicDataBase::SetupArgument(MnemonicItem *mnemonic, wxString &strline)
{
    wxChar      argtype;
    bool        ret = true;

    if (strline.Find('$') != wxNOT_FOUND)
    {
        argtype = strline[1];
        switch (argtype) {
                case 'o':  mnemonic->AddArgument(ARG_OFFSET);
                           mnemonic->AddOpCode(ARG_OFFSET);
                           break;
                case 'n':  mnemonic->AddArgument(ARG_LITERAL);
                           mnemonic->AddOpCode(ARG_LITERAL);
                           break;
                case 'v':  mnemonic->AddArgument(ARG_VARIABLE);
                           mnemonic->AddOpCode(ARG_VARIABLE);
                           break;
                case 'c':  mnemonic->AddArgument(ARG_ABS_ADDR);
                           mnemonic->AddOpCode(ARG_ABS_ADDR);
                           mnemonic->SetBranchType(BR_CALL);

                           break;
                case 's':  mnemonic->SetBranchType(BR_JUMP);
                           mnemonic->AddArgument(ARG_ABS_ADDR);
                           mnemonic->AddOpCode(ARG_ABS_ADDR);
                           break;
                case 'j':  mnemonic->AddArgument(ARG_REL_ADDR);
                           mnemonic->AddOpCode(ARG_REL_ADDR);
                           mnemonic->SetBranchType(BR_JUMP);
                           break;
                case 'g':  mnemonic->AddArgument(ARG_ABS_ADDR);
                           mnemonic->AddOpCode(ARG_ABS_ADDR);
                           mnemonic->SetBranchType(BR_CALL_CND);
                           break;
                case 'x':  mnemonic->AddArgument(ARG_ABS_ADDR);
                           mnemonic->AddOpCode(ARG_ABS_ADDR);
                           mnemonic->SetBranchType(BR_JUMP_CND);
                           break;
                case 'k':  mnemonic->AddArgument(ARG_REL_ADDR);
                           mnemonic->AddOpCode(ARG_REL_ADDR);
                           mnemonic->SetBranchType(BR_JUMP_CND);
                           break;
                case 'p':  mnemonic->AddArgument(ARG_IO_ADDR);
                           mnemonic->AddOpCode(ARG_IO_ADDR);
                           break;

                default:
                            mnemonic->AddArgument(ARG_NONE);
                            mnemonic->AddOpCode(ARG_NONE);
                            mnemonic->SetBranchType(BR_NONE);
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
        switch (mnemonic->GetInstructionDetail())
        {
            case II_NONE:
                        mnemonic->SetBranchType(BR_RETURN);
                        break;
            case II_CONDITIONAL:
                        mnemonic->SetBranchType(BR_RETURN_CND);
                        break;
            default:
                        mnemonic->SetBranchType(BR_NONE);
                        break;
        }
}





bool MnemonicDataBase::ProcessAndStore(wxArrayString& mnemonic_line_items, int currentSection, int line)
{
    bool    ret = false;
    int     i,
            x = 0;
    uint    num_arguments;
    long    templong;

    wxString    temp_str,
                ret_str;

    MnemonicItem    *mnemonic = 0;



    if ((mnemonic_line_items.GetCount() >= MIN_ARRAY_ITEMS) && (currentSection > 0))
    {
        mnemonic = new MnemonicItem();
        x = sizeof(MnemonicItem);

        for (i = 0; i < currentSection; i++)
        {
            temp_str = mnemonic_line_items[i];

            if (SetupArgument(mnemonic, temp_str))
            {
                mnemonic->SetOpCodeArgumentPos(i);
                continue;                   // we've found an argument, so go to the next item...
            }

            if (temp_str.ToLong(&templong, 16))
                mnemonic->AddOpCode((byte) templong);
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
            temp_str = mnemonic_line_items[i++];
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
            if (mnemonic->GetArgumentCount() != num_arguments)
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
            temp_str = mnemonic_line_items[i++];
            if (temp_str.ToLong(&templong, 16))
                mnemonic->SetInstructionType(templong);
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
            temp_str = mnemonic_line_items[i++];
            if (temp_str.ToLong(&templong, 16))
                mnemonic->SetInstructionDetail(templong);
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
            temp_str = mnemonic_line_items[i];
            mnemonic->SetMnemonicStr(temp_str);
            SetupBranch(mnemonic);
            mnemonic_list_->Add(mnemonic);
            ret = true;
            memory_allocated_ += x;
        }
    }
    return ret;
}




bool MnemonicDataBase::ReadFileAndStore(wxTextFile& tf)
{
    wxString        str;
    wxArrayString   mnemonic_line;

    memory_allocated_ = 0;

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
            ParseString(str, mnemonic_line);
            ProcessAndStore(mnemonic_line, currentSection, currentline);
        }
    }
    if (mnemonic_list_->IsEmpty())
        return false;
    else
        return true;
}




bool MnemonicDataBase::Open(wxString& filename)
{
    wxTextFile mnfile;
    bool ret = false;

    if (mnfile.Open(filename, wxConvLocal))
    {
        if (IsLoaded())
            Clear();
        ret = ReadFileAndStore(mnfile);
    }
    return ret;
}

/*
 * Search for mnemonics in mnemonic_list
 *
*/
void MnemonicDataBase::Find(wxArrayInt& mnemonics_found_list, byte opcode, uint scanoffset)
{
    uint			i, f, index;
    MnemonicItem	*mnemonic;
    wxArrayInt		temporary_found;
    bool			arg_detected;
    byte			opcodetest;

    if (scanoffset == 0)
    {
        mnemonics_found_list.Clear();
        f = mnemonic_list_->GetCount();
    }
    else
        f = mnemonics_found_list.GetCount();

    arg_detected = true;
    if (f > 1)
    {
        temporary_found.Clear();
        for (i = 0; i < f; i++)
        {

            if (scanoffset == 0)
                index = i;
            else
                index = mnemonics_found_list[i];
            mnemonic = mnemonic_list_->Item(index);

            /* In four-byte instructions of Z80, there are opcodes that
               have its argument in the middle of it (DD CB's and FD CB's series)
               so we must shift right the index to take the code
               The test below detects this condition
             */
            if ((!(mnemonic->GetArgumentPosition() == scanoffset)) || (mnemonic->GetArgumentPosition() == 0))
            {
                arg_detected = false;
                opcodetest = mnemonic->GetOpCode(scanoffset);
                if (opcode == opcodetest)
                {
                    if (scanoffset == 0)
                        mnemonics_found_list.Add(i);
                    else
                        temporary_found.Add(index);
                }
            }
        }
        if ((scanoffset > 0) && (!arg_detected))
        {
            mnemonics_found_list.Clear();
            mnemonics_found_list = temporary_found;
        }
    }
}



MnemonicItem *MnemonicDataBase::FindByOpCode(const ByteCode& code)
{
    uint i, total;

    total = GetCount();
    MnemonicItem* ret = 0;

    for (i = 0; i < total; i++)
    {
        ret = GetData(i);
        if (memcmp(ret->GetOpCode(), &code, MAX_OPCODE_SIZE) != 0)
            ret = 0;
        else
            break;
    }

    return ret;
}



uint MnemonicDataBase::GetAllocated()
{
    return memory_allocated_;
}


