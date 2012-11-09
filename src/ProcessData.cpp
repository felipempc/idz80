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

ProcessData::ProcessData(wxWindow *parent, LogWindow *logparent)
{
    Mnemonics = new MnemonicDataBase(logparent);
    Program = new RawData();
    Disassembled = new DAsmData(logparent);
    CodeViewLines = new CodeViewLine(Disassembled);
    m_disassembler = new Decoder(this, logparent);

    var_labels = new LabelListCtrl(parent, logparent);
    prog_labels = new LabelListCtrl(parent, logparent);
    io_labels = new LabelListCtrl(parent, logparent);
    constant_labels = new LabelListCtrl(parent, logparent);

    sys_calls =  0;
    sys_vars = 0;
    sys_io = 0;
    sys_const = 0;

    m_gauge = NULL;
    WindowLog = logparent;
    ModuleName = "PROCESS";
}


ProcessData::~ProcessData()
{
    delete m_disassembler;
    if (sys_const != 0)
        delete sys_const;
	delete sys_calls;
	delete sys_vars;
	delete sys_io;
    delete CodeViewLines;
    delete constant_labels;
    delete io_labels;
    delete prog_labels;
    delete var_labels;
    delete Mnemonics;
    delete Program;
    delete Disassembled;
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
        sys_io = new SystemLabelList("IOMAP", WindowLog);

        if (Program->isCOM())
        {
            sys_calls =  new SystemLabelList("MSXDOSCALLS", WindowLog);
            sys_vars = new SystemLabelList("MSXDOS1VAR", WindowLog);
            sys_const = new SystemLabelList("BDOSFUNCTIONS", WindowLog);
        }
        else
        {
            sys_calls =  new SystemLabelList("BIOSCALLS", WindowLog);
            sys_vars = new SystemLabelList("BIOSVARS", WindowLog);
        }
    }

    return ret;
}






void ProcessData::DisassembleFirst(bool simulateexecution)
{
    Disassembled->Clear();

    if (simulateexecution)
    {
        LogIt("Disassemble by simulating execution of code.");
        m_disassembler->FirstDisassemble(this);
    }
    else
    {
        LogIt("Full disassembling.");
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
    if (f > Disassembled->GetCount())
        f = Disassembled->GetCount();

    r.Count = 0;    // prepare the new item's count
    k = r.Index;
    for (i = r.Index; i < f; i++)
    {
        de = Disassembled->GetData(i);
        offset = de->GetOffset();
        length = de->GetLength();
        Disassembled->DelDasm(i);  //de
        for (j = 0; j < length; j++)
        {
            de = new DAsmElement(Program);
            de->SetLength(1);
            de->SetOffset(offset + j);
            de->SetMnemonic(0);
            de->SetType(et_Data);
            Disassembled->InsertDasm(de, k++);
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

    if (Disassembled->IsLoaded())
    {
        i = 0;
        nargsVar = nargsIO = nargsProg = 0;
        while (i < Disassembled->GetCount())
        {
            dasmtemp = Disassembled->GetData(i);
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
                                        addr = dasmtemp->getArgument(0, Disassembled->GetBaseAddress(i));
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
    if (!CodeViewLines->IsEmpty())
         CodeViewLines->Clear();

    m_Comments.Add("; ------------------------");
    m_Comments.Add("; Disassembled with IDZ80");
    m_Comments.Add("; 2010 by Felipempc");
    m_Comments.Add("; ------------------------");
    m_Comments.Add("");

    i = 0;
    while (i < m_Comments.GetCount())
        CodeViewLines->Add(m_Comments[i++]);

    CodeViewLines->AddOrg(Disassembled->GetBaseAddress(0), "");
    i = 0;
    while (i < Disassembled->GetCount())
        CodeViewLines->AddDasm(i++, "");
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
        if ((lbl != 0) && (!CodeViewLines->getDataLineAddress(lbl->Address, a)))
        {
            if (a >= 0)
            {
                CodeViewLines->InsertProgLabel(lbl->Address, "", a);
            }
            else
                CodeViewLines->EditProgLabel(lbl->Address, "", a);
        }
        i++;
    }

	// Variables
    i = 0;
    a = -1;
    while (i <  var_labels->GetItemCount())
    {
        lbl = (LabelItem *)var_labels->GetItemData(i);
        if ((lbl != 0) && (!CodeViewLines->getDataLineAddress(lbl->Address, a)))
        {
            if (a >= 0)
            {
                CodeViewLines->InsertVarLabel(lbl->Address, "", a);
            }
            else
                CodeViewLines->EditVarLabel(lbl->Address, "", a);
        }
        i++;
    }


}



