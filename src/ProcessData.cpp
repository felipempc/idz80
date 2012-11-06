﻿/****************************************************************
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
    constant_labels = new LabelListCtrl(m_main_frame, wxID_ANY, wxDefaultPosition, wxSize(170, 170));

    sys_calls =  0;
    sys_vars = 0;
    sys_io = 0;
    sys_const = 0;

    m_gauge = NULL;
    m_log = 0;
    m_modulename = "PROCESS: ";
}


ProcessData::~ProcessData()
{
    delete m_disassembler;
    if (sys_const != 0)
        delete sys_const;
	delete sys_calls;
	delete sys_vars;
	delete sys_io;
    delete m_CodeViewLine;
    delete constant_labels;
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
bool ProcessData::SetupSystemLabels()
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
            sys_const = new SystemLabelList("BDOSFUNCTIONS");
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
	if (sys_const != 0)
        sys_const->SetLog(m_log);

    return ret;
}






void ProcessData::DisassembleFirst(bool simulateexecution)
{
    m_Dasm->Clear();

    if (simulateexecution)
    {
        LogIt("Disassemble by simulating execution of code.\n");
        m_disassembler->FirstDisassemble(this);
    }
    else
    {
        LogIt("Full disassembling.\n");
        m_disassembler->FullDisassemble(this);
    }
}





void ProcessData::DisassembleItems(RangeItems &r)
{
    m_disassembler->DisassembleItems(r);
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
        offset = de->GetOffset();
        length = de->GetLength();
        m_Dasm->DelDasm(i);  //de
        for (j = 0; j < length; j++)
        {
            de = new DAsmElement(Program);
            de->SetLength(1);
            de->SetOffset(offset + j);
            de->SetMnemonic(0);
            de->SetType(et_Data);
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
    enum ArgumentTypes argtype;
    ArgStyle	style;

    if (m_Dasm->IsLoaded())
    {
        i = 0;
        nargsVar = nargsIO = nargsProg = 0;
        while (i < m_Dasm->GetCount())
        {
            dasmtemp = m_Dasm->GetData(i);
            if (dasmtemp->isInstruction())
            {
                argtype = dasmtemp->GetArgumentType(0);
                switch (argtype)
                {
                    case ARG_VARIABLE:
										style = dasmtemp->GetStyle();
                                        style.hasArgumentLabel = 1;
                                        dasmtemp->SetStyle(style);
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
										style = dasmtemp->GetStyle();
                                        style.hasArgumentLabel = 1;
                                        dasmtemp->SetStyle(style);
                                        break;
                    case ARG_IO_ADDR:
                                        addr = dasmtemp->getArgument(0, 0);
										style = dasmtemp->GetStyle();
                                        style.hasArgumentLabel = 1;
                                        dasmtemp->SetStyle(style);
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

