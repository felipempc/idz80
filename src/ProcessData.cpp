/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/




/*
 * Process data (disassemble & etc)
 */

#include "ProcessData.h"
#include <stack>





/*
 * ProcessData Implementation
 */

ProcessData::ProcessData(wxWindow *parent)
{
    Mnemonics = new MnemonicDataBase();
    Program = new RawData();
    m_Dasm = new DAsmData();
    m_CodeViewLine = new CodeViewLine(m_Dasm);
    m_main_frame = parent;
    m_disassembler = new Decoder(Program, m_Dasm, Mnemonics);

    var_labels = new LabelListCtrl(m_main_frame, wxID_ANY, wxDefaultPosition, wxSize(170, 170));
    prog_labels = new LabelListCtrl(m_main_frame, wxID_ANY, wxDefaultPosition, wxSize(170, 170));
    io_labels = new LabelListCtrl(m_main_frame, wxID_ANY, wxDefaultPosition, wxSize(170, 170));

    sys_calls =  0;
    sys_vars = 0;
    sys_io = 0;

    m_gauge = NULL;
    m_log = 0;
}


ProcessData::~ProcessData()
{
    delete m_disassembler;
	delete sys_calls;
	delete sys_vars;
	delete sys_io;
    delete m_CodeViewLine;
    delete io_labels;
    delete prog_labels;
    delete var_labels;
    delete Mnemonics;
    delete Program;
    delete m_Dasm;
}


void ProcessData::SetGauge(wxGauge *g)
{
   m_gauge = g;
}



/*  Load system labels (IOPorts, Variables,
 * system calls, etc) after program
 * has been loaded.
 */
bool ProcessData::LoadSysLabels()
{
    bool ret = false;

    if (Program->IsLoaded())
    {
        ret = true;
        sys_io = new SystemLabelList("IOMAP");

        if (Program->isCOM())
        {
            sys_calls =  new SystemLabelList("MSXDOSCALLS");
            sys_vars = new SystemLabelList("MSXDOS1VAR");
        }
        else
        {
            sys_calls =  new SystemLabelList("BIOSCALLS");
            sys_vars = new SystemLabelList("BIOSVARS");
        }
    }

    sys_io->SetLog(m_log);
	sys_calls->SetLog(m_log);
	sys_vars->SetLog(m_log);

    return ret;
}






/*
 *	Convert dasm data items to file offset
 */
void ProcessData::ConvertProgramAddress(RangeItems r, RangeData& d)
{
    DAsmElement* temp;

    temp = m_Dasm->GetData(r.Index);
    d.First = temp->Offset;
    temp = m_Dasm->GetData((r.Index + r.Count - 1));
    d.Count = temp->Offset + temp->Length;
}




/*
 * Search for an opcode pattern (from Program)
 * in mndb.
 */
uint ProcessData::MatchOpcode(const uint i, const uint max)
{
    uint 		j, nitems;
    wxArrayInt	foundItems;
    byte		scan;
    bool		quit = false;
    MnemonicItem	*mi;

    foundItems.Clear();
    j = 0;
    while ((j < MAX_OPCODE_SIZE) && ((j + i) < max) && (!quit))
    {
        scan = Program->GetData(i + j);
        Mnemonics->FindItems(foundItems, scan, j);
        j++;
        if (foundItems.GetCount() < 2)
			quit = true;
    }
    nitems = foundItems.GetCount();
    if (nitems == 1)
	{
		mi = Mnemonics->GetData(foundItems.Last());
		if (mi->getBytesNo() > (max - i))
			nitems = 0;
	}
    if ((nitems == 0) || (nitems > max))
        return (OPCODE_NOT_MATCHED);
    else
        return (foundItems.Last());
}


void ProcessData::addLabels()
{
    int         i,
                counter = 0,
                address;
    SortedIntArray *temp;
    wxString    str;


    temp = m_disassembler->GetVarList();
    for(i = 0; i < temp->GetCount(); i++)
    {
        address = temp->Item(i);
        str = sys_vars->Find(address);
        if (str.IsEmpty())
            str.Printf("VAR%d", counter++);
        var_labels->AddLabel(address, str);
    }

    counter = 0;
    temp = m_disassembler->GetEntryList();
    for(i = 0; i < temp->GetCount(); i++)
    {
        address = temp->Item(i);
        str = sys_calls->Find(address);
        if (str.IsEmpty())
            str.Printf("LABEL%d", counter++);
        prog_labels->AddLabel(address, str);
    }

    counter = 0;
    temp = m_disassembler->GetIOList();
    for(i = 0; i < temp->GetCount(); i++)
    {
        address = temp->Item(i);
        str = sys_io->Find(address);
        if (str.IsEmpty())
            str.Printf("PORT%d", counter++);
        io_labels->AddLabel(address, str);
    }
}






void ProcessData::DisassembleFirst()
{
    m_Dasm->Clear();
    m_disassembler->FirstDisassemble();
    addLabels();
    m_log->AppendText(m_disassembler->GetCodeSegmentStr());
    m_disassembler->OptimizeCodeSegment();
    m_log->AppendText(m_disassembler->GetCodeSegmentStr());
}





void ProcessData::DisassembleItems(RangeItems &r)
{
    uint 		i, f, x, j, totargs, item;
    DAsmElement	*de;
    RangeData	prange;
    bool		IsMiddle;


    f = r.Index + r.Count - 1;
    x = m_Dasm->GetCount();
    IsMiddle = TRUE;

    if ((r.Index < x) || (f < x))
    {
        ConvertProgramAddress(r,prange);
        m_Dasm->DelDasm(r.Index,r.Count);
        if (r.Index >= m_Dasm->GetCount())
            IsMiddle = FALSE;
        else
            IsMiddle = TRUE;
        x = r.Index;
        r.Count = 0;
        for (i = prange.First; i < prange.Count; i++)
        {
            item = MatchOpcode(i, prange.Count);
            if (item == OPCODE_NOT_MATCHED)
            {
                de = new DAsmElement(Program);
                memset(de->Args,'\0',sizeof(OpCodeArguments));
                memset(de->Code,'\0',sizeof(ByteCode));
				de->Style.hasArgumentLabel = 0; // false;
				de->Style.hasLabel = 0;   //false;
				de->Style.arg1 = ast_hex;
				de->Style.arg2 = ast_hex;
				de->Style.arg1styled = 0;
				de->Style.arg2styled = 0;
                de->Length = 1;
                de->Offset = i;
                de->MnItem = 0;
                de->ElType = et_Data;
                if (IsMiddle)
                    m_Dasm->InsertDasm(de, x++);
                else
                {
                    m_Dasm->AddDasm(de);
                    x++;
                }
            }
            else
            {
                de = new DAsmElement(Program);
				de->Style.hasArgumentLabel = 0; // false;
				de->Style.hasLabel = 0;   //false;
				de->Style.arg1 = ast_hex;
				de->Style.arg2 = ast_hex;
				de->Style.arg1styled = 0;
				de->Style.arg2styled = 0;

                de->MnItem = Mnemonics->GetData(item);
                for (j = 0; j < MAX_OPCODE_SIZE; j++)
                {
                    if (j < de->MnItem->getBytesNo())
                        de->Code[j] = de->MnItem->getOpCode(j);
                    else
                        de->Code[j] = 0;
                }
                de->Length = de->MnItem->getBytesNo();
                de->ElType = et_Instruction;
                de->Offset = i;
                totargs = de->MnItem->getArgNo() * de->MnItem->getArgSize();
                for (j=0;j<totargs;j++)
                    de->Args[j] = Program->GetData(i + j + de->MnItem->getArgPos());
                if (IsMiddle)
                    m_Dasm->InsertDasm(de, x++);
                else
                {
                    m_Dasm->AddDasm(de);
                    x++;
                }
            }
            i += (de->Length - 1);
            r.Count++;
        }
    }
}




void ProcessData::MakeData(RangeItems &r)
{
    uint		i, j, k, f, offset, length;
    DAsmElement	*de;

    f = r.Index + r.Count;
    if (f > m_Dasm->GetCount())
        f = m_Dasm->GetCount();

    r.Count = 0;    // prepare the new item's count
    k = r.Index;
    for (i = r.Index; i < f; i++)
    {
        de = m_Dasm->GetData(i);
        offset = de->Offset;
        length = de->Length;
        m_Dasm->DelDasm(de);
        for (j = 0; j < length; j++)
        {
            de = new DAsmElement(Program);
			de->Style.hasArgumentLabel = 0; // false;
			de->Style.hasLabel = 0;   //false;
			de->Style.arg1 = ast_hex;
			de->Style.arg2 = ast_hex;
			de->Style.arg1styled = 0;
			de->Style.arg2styled = 0;
            memset(de->Args,'\0',sizeof(OpCodeArguments));
            de->Length = 1;
            de->Offset = offset + j;
            de->MnItem = 0;
            de->ElType = et_Data;
            m_Dasm->InsertDasm(de, k++);
        }
        i += length - 1;
        f += length - 1;
        r.Count += length;
    }
}








//TODO: REmove it
void ProcessData::AutoLabel()
{
    DAsmElement *dasmtemp;
    uint    	i, nargsVar, nargsProg, nargsIO;

    long		addr;
    wxString	str, strdebug;
    enum ArgType argtype;

    if (m_Dasm->IsLoaded())
    {
        i = 0;
        nargsVar = nargsIO = nargsProg = 0;
        while (i < m_Dasm->GetCount())
        {
            dasmtemp = m_Dasm->GetData(i);
            if (dasmtemp->ElType == et_Instruction)
            {
                argtype = dasmtemp->MnItem->getArgType(0);
                switch (argtype)
                {
                    case ARG_VARIABLE:
                                        dasmtemp->Style.hasArgumentLabel = 1;
                                        addr = dasmtemp->getArgument(0, 0);
                                        str = sys_vars->Find(addr);
                                        if (str.IsEmpty())
											str.Printf("VAR%d", nargsVar++);
                                        var_labels->AddLabel(addr, str, i);
                                        break;
                    case ARG_ABS_ADDR:
                    case ARG_REL_ADDR:
                                        addr = dasmtemp->getArgument(0, m_Dasm->GetBaseAddress(i));
                                        str = sys_calls->Find(addr);
                                        if (str.IsEmpty())
											str.Printf("LABEL%d", nargsProg++);
                                        prog_labels->AddLabel(addr, str, i);
										dasmtemp->Style.hasArgumentLabel = 1;
                                        break;
                    case ARG_IO_ADDR:
                                        addr = dasmtemp->getArgument(0, 0);
                                        dasmtemp->Style.hasArgumentLabel = 1;
                                        str = sys_io->Find(addr);
                                        if (str.IsEmpty())
											str.Printf("PORT%d", nargsIO++);
                                        io_labels->AddLabel(addr, str, i);
                                        break;
                    case ARG_NONE:
                    case ARG_LITERAL:
                    case ARG_OFFSET:
                                        break;
                }
            }
            i++;
        }
        if (nargsIO > 0)
            io_labels->SortAddress();
        if (nargsProg > 0)
            prog_labels->SortAddress();
        if (nargsVar > 0)
            var_labels->SortAddress();
    }
}



void ProcessData::InitData()
{
    uint i;
    wxArrayString m_Comments;
    if (!m_CodeViewLine->IsEmpty())
         m_CodeViewLine->Clear();

    m_Comments.Add("; ------------------------");
    m_Comments.Add("; Disassembled with IDZ80");
    m_Comments.Add("; 2010 by Felipempc");
    m_Comments.Add("; ------------------------");
    m_Comments.Add("");

    i = 0;
    while (i < m_Comments.GetCount())
        m_CodeViewLine->Add(m_Comments[i++]);

    m_CodeViewLine->AddOrg(m_Dasm->GetBaseAddress(0), "");
    i = 0;
    while (i < m_Dasm->GetCount())
        m_CodeViewLine->AddDasm(i++, "");
}


void ProcessData::processLabel()
{
    LabelItem *lbl;
    int i,a;

	// Program label
    i = 0;
    a = -1;
    while (i < prog_labels->GetItemCount())
    {
        lbl = (LabelItem *)prog_labels->GetItemData(i);
        if ((lbl != 0) && (!m_CodeViewLine->getDataLineAddress(lbl->Address, a)))
        {
            if (a >= 0)
            {
                m_CodeViewLine->InsertProgLabel(lbl->Address, "", a);
            }
            else
                m_CodeViewLine->EditProgLabel(lbl->Address, "", a);
        }
        i++;
    }

	// Variables
    i = 0;
    a = -1;
    while (i <  var_labels->GetItemCount())
    {
        lbl = (LabelItem *)var_labels->GetItemData(i);
        if ((lbl != 0) && (!m_CodeViewLine->getDataLineAddress(lbl->Address, a)))
        {
            if (a >= 0)
            {
                m_CodeViewLine->InsertVarLabel(lbl->Address, "", a);
            }
            else
                m_CodeViewLine->EditVarLabel(lbl->Address, "", a);
        }
        i++;
    }


}








void ProcessData::SetLog(wxTextCtrl *_lg)
{
    m_log = _lg;
    m_Dasm->SetLog(_lg);
    io_labels->SetLog(_lg);
    var_labels->SetLog(_lg);
    prog_labels->SetLog(_lg);
    Program->DebugLog(_lg);
    Mnemonics->SetLog(_lg);
    m_disassembler->SetLog(_lg);
}

