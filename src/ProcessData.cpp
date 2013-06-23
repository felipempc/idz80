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
    Program = new RawData(logparent);
    Disassembled = new DAsmData(logparent);
    CodeViewLines = new CodeViewLine(Disassembled);
    m_disassembler = new Decoder(this, logparent);

    var_labels = new LabelListCtrl(parent, "VAR", logparent);
    prog_labels = new LabelListCtrl(parent, "", logparent);
    io_labels = new LabelListCtrl(parent, "PORT", logparent);
    constant_labels = new LabelListCtrl(parent, "", logparent);

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


void ProcessData::Clear()
{
    ClearUserLabels();
    delete sys_calls;
    delete sys_io;
    delete sys_const;
    delete sys_vars;
    sys_calls = 0;
    sys_io = 0;
    sys_const = 0;
    sys_vars = 0;

    Program->Clear();
    Disassembled->Clear();
    CodeViewLines->Clear();
    m_disassembler->Clear();
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
        RemoveFromLabelUserList(de, i);
        Disassembled->DelDasm(i);
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
    uint    	i;

    long		addr;
    wxString	str, strdebug;
    enum ArgumentTypes argtype;
    ArgStyle	style;

    if (Disassembled->IsLoaded())
    {
        i = 0;
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
                                        var_labels->AddLabel(addr, str, i);
                                        break;
                    case ARG_ABS_ADDR:
                    case ARG_REL_ADDR:
                                        addr = dasmtemp->getArgument(0, Disassembled->GetBaseAddress(i));
                                        str = sys_calls->Find(addr);
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
        io_labels->SortAddress();
        prog_labels->SortAddress();
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
        if ((lbl) && (!CodeViewLines->getLineOfAddress(lbl->Address, a)))
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
        if ((lbl) && (!CodeViewLines->getLineOfAddress(lbl->Address, a)))
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



void ProcessData::RemoveFromLabelUserList(DAsmElement *de, const uint dasmitem)
{
    if (de->GetNumArgs() > 0)
        switch(de->GetArgumentType(0))
        {
            case ARG_VARIABLE:
                var_labels->DelLabelUser(de->getArgument(0, Program->ExecAddress), dasmitem);
            case ARG_IO_ADDR:
                io_labels->DelLabelUser(de->getArgument(0, Program->ExecAddress), dasmitem);
            case ARG_REL_ADDR:
            case ARG_ABS_ADDR:
                prog_labels->DelLabelUser(de->getArgument(0, Program->ExecAddress), dasmitem);
        }
}




/*
 * Take selected lines of code and convert to lines
 * of data.
 * Delete program labels;
 * Keeps var labels;
 * Keeps comments;
 * keeps org directives;
 */
void ProcessData::TransformToData(SelectedItemInfo &selected)
{
    RangeItems		dasmed_items;
    CodeViewItem	*cvi;
    DAsmElement     *de;
    int 			i, newLineCount, lineIndex, lineLast, lineCount,
					oldLineCount, varindex;
    uint            first_address, last_address;
    wxArrayInt		cvlines, proglabels, varlabels;

    if ((selected.firstInstruction <= selected.lastInstruction) &&
        (selected.lastInstruction < Disassembled->GetCount()))
    {
        if (selected.firstLine > 0)
            cvi = CodeViewLines->getData(selected.firstLine - 1);

        lineIndex = selected.firstLine;
        lineLast = selected.lastLine;
        lineCount = lineLast - lineIndex + 1;

        dasmed_items.Index = selected.firstInstruction;
        dasmed_items.Count = selected.lastInstruction - selected.firstInstruction + 1;
        oldLineCount = dasmed_items.Count;

        var_labels->GetLabelsBetweenRangeAddress(selected.firstAddress, selected.lastAddress, &varlabels);

        MakeData(dasmed_items);

        for (i = 0; i < lineCount; i++)
            RemoveLineAndProgLabels(lineIndex);

//* TODO:REWRITE THIS
        CodeViewLines->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

        newLineCount = dasmed_items.Count - oldLineCount;

        CodeViewLines->UpdateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        for (i = 0; i < varlabels.GetCount(); i++)
        {
            CodeViewLines->getLineOfAddress(lineIndex, (lineCount + newLineCount), varlabels.Item(i), varindex);
            if ((cvi) && (cvi->LabelVarAddr != varlabels[i]))
            {
                LogIt(wxString::Format("Found var address 0x%X, line %d\n", varlabels.Item(i), varindex));
                CodeViewLines->InsertVarLabel(varlabels[i], "", varindex);
            }
        }
//*/
        selected.cursorLastPosition = lineIndex;
        selected.cursorPosition = lineIndex + lineCount + newLineCount - 1;
    }
}




void ProcessData::DisassembleData(SelectedItemInfo &selected)
{
    RangeItems		dasmed_items;
    CodeViewItem	*cvi;
    int				i, newLineCount, oldLineCount, varindex;
    int				lineIndex, lineLast, lineCount;
    wxArrayInt		cvlines, varlabels;

	if (!FilterInstructions(cvlines, selected))
		return;

	if (cvlines.GetCount() > 0)
	{
        lineIndex = cvlines[0];
        lineLast = cvlines[1];
        lineCount = lineLast - lineIndex + 1;

		cvi = CodeViewLines->getData(lineIndex);
		dasmed_items.Index = cvi->Dasmitem;
		cvi = CodeViewLines->getData(lineLast);
		dasmed_items.Count = cvi->Dasmitem - dasmed_items.Index + 1;
		oldLineCount = dasmed_items.Count;

        var_labels->GetLabelsBetweenRangeAddress(selected.firstAddress, selected.lastAddress, &varlabels);

		for (i = 0; i < lineCount; i++)
            RemoveLineAndVarLabels(lineIndex);

		DisassembleItems(dasmed_items);

		CodeViewLines->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

		newLineCount = dasmed_items.Count - oldLineCount;

		CodeViewLines->UpdateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        if (lineIndex > 0)
            cvi = CodeViewLines->getData(lineIndex - 1);

        for (i = 0; i < varlabels.GetCount(); i++)
        {
            CodeViewLines->getLineOfAddress(lineIndex, (lineCount + newLineCount), varlabels.Item(i), varindex);
            if ((cvi) && (cvi->LabelVarAddr != varlabels[i]))
            {
                LogIt(wxString::Format("Found var address 0x%X, line %d\n", varlabels.Item(i), varindex));
                CodeViewLines->InsertVarLabel(varlabels[i], "", varindex);
            }
        }

		selected.cursorLastPosition = lineIndex;
        selected.cursorPosition = lineIndex + lineCount + newLineCount - 1;
	}
}



/* Returns the first and the last line of instruction / data
 * Returns program labels
 */
bool ProcessData::FilterInstructions(wxArrayInt &range, SelectedItemInfo &selected)
{
    bool	foundindex;
    int		i, last_i;
    CodeViewItem
			*cvi;
    DAsmElement
			*de;


    foundindex = false;
    last_i = 0;
    for (i = selected.firstLine; i <= selected.lastLine; i++)
    {
        cvi = CodeViewLines->getData(i);
        if (cvi->Dasmitem >= 0)
        {
            de = Disassembled->GetData(cvi->Dasmitem);
            if (de->isInstruction() ||
                de->isData())
            {
                if (!foundindex)
                {
                    range.Add(i);
                    foundindex = true;
                }
                last_i = i;
            }
        }
    }
    if (foundindex)
        range.Add(last_i);

	if (range.GetCount() > 0)
		return true;
	else
		return false;
}


void ProcessData::RemoveLineAndProgLabels(const int index)
{
    CodeViewItem *cvi;

    cvi = CodeViewLines->getData(index);
    if (cvi && (cvi->LabelProgAddr >= 0))
    {
        RemoveLabelUsers(prog_labels->GetLabelUsers(prog_labels->GetLabelIndex(cvi->LabelProgAddr)));
        prog_labels->DelLabel(cvi->LabelProgAddr);
    }
    CodeViewLines->Del(index);

}


void ProcessData::RemoveLineAndVarLabels(const int index)
{
    CodeViewItem *cvi;

    cvi = CodeViewLines->getData(index);
    if (cvi && (cvi->LabelVarAddr >= 0))
    {
        RemoveLabelUsers(var_labels->GetLabelUsers(var_labels->GetLabelIndex(cvi->LabelVarAddr)));
        var_labels->DelLabel(cvi->LabelVarAddr);
    }
    CodeViewLines->Del(index);
}





void ProcessData::RemoveLabelUsers(wxArrayInt *users)
{
    int i;
    DAsmElement *de;
    ArgStyle    style;

    if (users)
    {
        for (i = 0; i < users->GetCount(); i++)
        {
            de = Disassembled->GetData(users->Item(i));
            if (de)
            {
                style = de->GetStyle();
                style.hasArgumentLabel = 0;
                de->SetStyle(style);
            }
        }
    }
}
