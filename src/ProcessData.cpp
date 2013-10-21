/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Process all data
 **************************************************************/

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
    CodeViewLines = new CodeViewLine(Disassembled, this);
    disassembler_ = 0;
    smart_disassembler_ = 0;

    var_labels = new LabelListCtrl(parent, VAR_LIST,"VAR", logparent);
    prog_labels = new LabelListCtrl(parent, PRG_LIST,"", logparent);
    io_labels = new LabelListCtrl(parent, IO_LIST,"PORT", logparent);
    constant_labels = new LabelListCtrl(parent, CONST_LIST,"", logparent);

    sys_calls =  0;
    sys_vars = 0;
    sys_io = 0;
    sys_const = 0;

    gauge_ = NULL;
    window_log_ = logparent;
    ModuleName = "PROCESS";
}


ProcessData::~ProcessData()
{
    if (disassembler_)
        delete disassembler_;
    if (smart_disassembler_)
        delete smart_disassembler_;
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
    if (smart_disassembler_)
        smart_disassembler_->Clear();
}


void ProcessData::SetGauge(wxGauge *g)
{
   gauge_ = g;
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
        sys_io = new SystemLabelList("IOMAP", window_log_);

        if (Program->isCOM())
        {
            sys_calls =  new SystemLabelList("MSXDOSCALLS", window_log_);
            sys_vars = new SystemLabelList("MSXDOS1VAR", window_log_);
            sys_const = new SystemLabelList("BDOSFUNCTIONS", window_log_);
        }
        else
        {
            sys_calls =  new SystemLabelList("BIOSCALLS", window_log_);
            sys_vars = new SystemLabelList("BIOSVARS", window_log_);
        }
    }

    return ret;
}






void ProcessData::DisassembleFirst(bool simulateexecution)
{
    Disassembled->Clear();


    if (simulateexecution)
    {
        if (smart_disassembler_ == 0)
            smart_disassembler_ = new SmartDecoder(this, window_log_);

        LogIt("Disassemble by simulating execution of code.");
        smart_disassembler_->FullDisassemble(this);
    }
    else
    {
        if (disassembler_ == 0)
            disassembler_ = new Decoder(this, window_log_);

        LogIt("Full disassembling.");
        disassembler_->FullDisassemble(this);
    }
}





void ProcessData::DisassembleItems(RangeItems &r)
{
    if (disassembler_)
        disassembler_->DisassembleItems(r);
    if (smart_disassembler_)
        smart_disassembler_->DisassembleItems(r);
}



void ProcessData::MakeData(RangeItems &r)
{
    uint		i, j, k, f, offset, length;
    DisassembledItem	*de;

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
            de = new DisassembledItem(Program);
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
    DisassembledItem *dasmtemp;
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
            if (dasmtemp->IsInstruction())
            {
                argtype = dasmtemp->GetArgumentType(0);
                switch (argtype)
                {
                    case ARG_VARIABLE:
										style = dasmtemp->GetStyle();
                                        style.hasArgumentLabel = 1;
                                        dasmtemp->SetStyle(style);
                                        addr = dasmtemp->GetArgument(0, 0);
                                        str = sys_vars->Find(addr);
                                        var_labels->AddLabel(addr, str, i);
                                        break;
                    case ARG_ABS_ADDR:
                    case ARG_REL_ADDR:
                                        addr = dasmtemp->GetArgument(0, Disassembled->GetBaseAddress(i));
                                        str = sys_calls->Find(addr);
                                        prog_labels->AddLabel(addr, str, i);
										style = dasmtemp->GetStyle();
                                        style.hasArgumentLabel = 1;
                                        dasmtemp->SetStyle(style);
                                        break;
                    case ARG_IO_ADDR:
                                        addr = dasmtemp->GetArgument(0, 0);
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

/*
 * Insert Labels of LabelListCtrl in the sourcecode
 */
void ProcessData::ProcessLabel(LabelListCtrl *label)
{
    LabelItem *lbl;
    int line_index = -1;
    LabelIndex counter = 0;
    CodeViewItem    *cvi;

    while (counter < label->GetCount())
    {
        lbl = label->GetData(counter);
        if (lbl) //&& (!CodeViewLines->getLineOfAddress(lbl->Address, line_index)))
        {
            line_index = CodeViewLines->getLineOfAddress(lbl->Address);
            cvi = CodeViewLines->getData(line_index - 1);
            if (cvi && !(cvi->LabelProgAddr || cvi->LabelVarAddr) && (line_index >= 0))
            {
                if (label->GetTypeList() == PRG_LIST)
                    CodeViewLines->InsertProgLabel(lbl->Address, "aaa", line_index);
                else
                    if (label->GetTypeList() == VAR_LIST)
                        CodeViewLines->InsertVarLabel(lbl->Address, "aaa", line_index);
            }
        }
        counter++;
    }
}


void ProcessData::InsertLineLabelsInSourceCode()
{
    ProcessLabel(prog_labels);
    ProcessLabel(var_labels);
}



void ProcessData::RemoveFromLabelUserList(DisassembledItem *de, const uint dasmitem)
{
    if (de->GetArgumentCount() > 0)
        switch(de->GetArgumentType(0))
        {
            case ARG_VARIABLE:
                var_labels->DelLabelUser(de->GetArgument(0, Program->ExecAddress), dasmitem);
            case ARG_IO_ADDR:
                io_labels->DelLabelUser(de->GetArgument(0, Program->ExecAddress), dasmitem);
            case ARG_REL_ADDR:
            case ARG_ABS_ADDR:
                prog_labels->DelLabelUser(de->GetArgument(0, Program->ExecAddress), dasmitem);
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
    int 			newLineCount, lineIndex, lineLast,
					oldLineCount, varindex;
    uint            i, line_count;
    wxArrayInt		cvlines, proglabels, varlabels;

    if ((selected.firstInstruction <= selected.lastInstruction) &&
        (selected.lastInstruction < static_cast<int>(Disassembled->GetCount())))
    {
        if (selected.firstLine > 0)
            cvi = CodeViewLines->getData(selected.firstLine - 1);

        lineIndex = selected.firstLine;
        lineLast = selected.lastLine;
        line_count = lineLast - lineIndex + 1;

        dasmed_items.Index = selected.firstInstruction;
        dasmed_items.Count = selected.lastInstruction - selected.firstInstruction + 1;
        oldLineCount = dasmed_items.Count;

        var_labels->GetLabelsBetweenRangeAddress(selected.firstAddress, selected.lastAddress, &varlabels);

        MakeData(dasmed_items);

        for (i = 0; i < line_count; i++)
            RemoveLineAndProgLabels(lineIndex);

//* TODO:REWRITE THIS
        CodeViewLines->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

        newLineCount = dasmed_items.Count - oldLineCount;

        CodeViewLines->UpdateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        for (i = 0; i < varlabels.GetCount(); i++)
        {
            varindex = CodeViewLines->getLineOfAddress(lineIndex, (line_count + newLineCount), varlabels.Item(i));
            if ((cvi) && (cvi->LabelVarAddr->Address != static_cast<ProgramAddress>(varlabels[i])))
            {
                LogIt(wxString::Format("Found var address 0x%X, line %d\n", varlabels.Item(i), varindex));
                CodeViewLines->InsertVarLabel(varlabels[i], "", varindex);
            }
        }
//*/
        selected.cursorLastPosition = lineIndex;
        selected.cursorPosition = lineIndex + line_count + newLineCount - 1;
    }
}




void ProcessData::DisassembleData(SelectedItemInfo &selected)
{
    RangeItems		dasmed_items;
    CodeViewItem	*cvi;
    int				newLineCount, oldLineCount, varindex;
    int				lineIndex, lineLast, lineCount;
    wxArrayInt		cvlines, varlabels;
    uint            i;

	if (!FilterInstructions(cvlines, selected))
		return;

	if (cvlines.GetCount() > 0)
	{
        lineIndex = cvlines[0];
        lineLast = cvlines[1];
        lineCount = lineLast - lineIndex + 1;
        if (lineCount < 0)
            return;

		cvi = CodeViewLines->getData(lineIndex);
		dasmed_items.Index = cvi->Dasmitem;
		cvi = CodeViewLines->getData(lineLast);
		dasmed_items.Count = cvi->Dasmitem - dasmed_items.Index + 1;
		oldLineCount = dasmed_items.Count;

        var_labels->GetLabelsBetweenRangeAddress(selected.firstAddress, selected.lastAddress, &varlabels);

		for (i = 0; i < static_cast<uint>(lineCount); i++)
            RemoveLineAndVarLabels(lineIndex);

		DisassembleItems(dasmed_items);

		CodeViewLines->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

		newLineCount = dasmed_items.Count - oldLineCount;

		CodeViewLines->UpdateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        if (lineIndex > 0)
            cvi = CodeViewLines->getData(lineIndex - 1);

        for (i = 0; i < varlabels.GetCount(); i++)
        {
            varindex = CodeViewLines->getLineOfAddress(lineIndex, (lineCount + newLineCount), varlabels.Item(i));
            if ((cvi) && (cvi->LabelVarAddr->Address != static_cast<ProgramAddress>(varlabels[i])))
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
    DisassembledItem
			*de;


    foundindex = false;
    last_i = 0;
    for (i = selected.firstLine; i <= selected.lastLine; i++)
    {
        cvi = CodeViewLines->getData(i);
        if (cvi->Dasmitem >= 0)
        {
            de = Disassembled->GetData(cvi->Dasmitem);
            if (de->IsInstruction() ||
                de->IsData())
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
    if (cvi && cvi->LabelProgAddr)
    {
        RemoveLabelUsers(cvi->LabelProgAddr->LabelUsers);
        prog_labels->DelLabel(cvi->LabelProgAddr->Address);
    }
    CodeViewLines->Del(index);

}


void ProcessData::RemoveLineAndVarLabels(const int index)
{
    CodeViewItem *cvi;

    cvi = CodeViewLines->getData(index);
    if (cvi && cvi->LabelVarAddr)
    {
        RemoveLabelUsers(cvi->LabelVarAddr->LabelUsers);
        var_labels->DelLabel(cvi->LabelVarAddr->Address);
    }
    CodeViewLines->Del(index);
}





void ProcessData::RemoveLabelUsers(wxArrayInt *users)
{
    uint i;
    DisassembledItem *de;
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



