/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Process all data
 **************************************************************/

#include "process_data.h"
#include <stack>


/*
 * ProcessData Implementation
 */


 // MUST BE COMPLETELY REWRITTEN !!!!

ProcessData::ProcessData(ProjectBase *parent)
{
    m_programs_mgr = parent->m_programs_mgr;
    m_mnemonics = parent->m_mnemonics;
    m_labels = parent->m_labels;

    m_disassembled_mgr = new DisassembledManager(parent);
    parent->m_disassembled_mgr = m_disassembled_mgr;

    //CodeViewLines = new SourceCodeLines(m_disassembled_mgr, this);
    search_status_ = new SearchManager();
    m_disassembler = new SmartDecoder(this);

    ModuleName = "ProcessData";
    SetTextLog(parent->GetTextLog());
}


ProcessData::~ProcessData()
{
    if (m_disassembler)
        delete m_disassembler;
    if (m_disassembled_mgr)
        delete m_disassembled_mgr;
    
    m_labels->DestroyAll();
}


void ProcessData::Clear()
{
    m_labels->ClearUserLabels();
    m_disassembled_mgr->Clear();
    //CodeViewLines->Clear();
    if (m_disassembler)
        m_disassembler->Clear();
}



/*  Load system labels (IOPorts, Variables,
 * system calls, etc) after program
 * has been loaded.
 */
bool ProcessData::SetupSystemLabels()
{
    bool ret = false;
    RawData *program = m_programs_mgr->First();

    if (m_programs_mgr->isLoaded())
    {
        ret = true;
        m_labels->sys_io = new SystemLabelList("IOMAP", this);

        if (program->isCOM())
        {
            m_labels->sys_calls =  new SystemLabelList("MSXDOSCALLS", this);
            m_labels->sys_vars = new SystemLabelList("MSXDOS1VAR", this);
            m_labels->sys_const = new SystemLabelList("BDOSFUNCTIONS", this);
        }
        else
        {
            m_labels->sys_calls =  new SystemLabelList("BIOSCALLS", this);
            m_labels->sys_vars = new SystemLabelList("BIOSVARS", this);
        }
    }

    return ret;
}






void ProcessData::DisassembleFirst(bool simulateexecution)
{
    Disassembled->Clear();


    if (m_disassembler == 0)
        m_disassembler = new SmartDecoder(this, window_log_);

    LogIt("Disassemble by simulating execution of code.");
    m_disassembler->FullDisassemble(this);

}





void ProcessData::DisassembleItems(RangeItems &r)
{
    if (m_disassembler)
        m_disassembler->DisassembleItems(r);
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
/*
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
*/


void ProcessData::InitData()
{
    uint i, fstil;
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
        CodeViewLines->AddComment(m_Comments[i++]);

    CodeViewLines->AddOrg(Disassembled->GetBaseAddress(0), "");
    i = 0;
    fstil = CodeViewLines->GetCount();
    while (i < Disassembled->GetCount())
        CodeViewLines->AddDasm(i++, "");

    CodeViewLines->SetFirstInstructionLine(fstil);
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
        if (lbl)
        {
            line_index = CodeViewLines->getLineOfAddress(lbl->Address);
            cvi = CodeViewLines->Line(line_index - 1);
            if (cvi && !(cvi->LabelProgAddr || cvi->LabelVarAddr) && (line_index >= 0))
            {
                if (label->GetTypeList() == PRG_LIST)
                    CodeViewLines->InsertProgLabel(lbl->Address, "", line_index);
                else
                    if (label->GetTypeList() == VAR_LIST)
                        CodeViewLines->InsertVarLabel(lbl->Address, "", line_index);
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
    int 			newLineCount, lineIndex, oldLineCount, varindex,
                    varitem;
    uint            i, line_count, deleted_labels;
    wxArrayInt		cvlines, proglabels, varlabels;

    if ((selected.firstInstruction <= selected.lastInstruction) &&
        (selected.lastInstruction < static_cast<int>(Disassembled->GetCount())))
    {
        if (selected.firstLine > 0)
            cvi = CodeViewLines->Line(selected.firstLine - 1);

        deleted_labels = 0;
        lineIndex = selected.firstLine;
        line_count = selected.lastLine - lineIndex + 1;

        dasmed_items.Index = selected.firstInstruction;
        dasmed_items.Count = selected.lastInstruction - selected.firstInstruction + 1;
        oldLineCount = dasmed_items.Count;

        var_labels->GetLabelsBetweenRangeAddress(selected.firstAddress, selected.lastAddress, &varlabels);

        MakeData(dasmed_items);

        for (i = 0; i < line_count; i++)
            if (RemoveLineAndProgLabels(lineIndex))
                deleted_labels++;

//* TODO:REWRITE THIS
        CodeViewLines->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

        newLineCount = dasmed_items.Count - oldLineCount;

        CodeViewLines->UpdateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        if (cvi)
        {
            for (i = 0; i < varlabels.GetCount(); i++)
            {
                varitem = varlabels[i];
                varindex = CodeViewLines->getLineOfAddress(lineIndex, (line_count + newLineCount), varitem);
                if (!(cvi->LabelVarAddr) || (cvi->LabelVarAddr->Address != static_cast<AbsoluteAddress>(varitem)))
                {
                    LogIt(wxString::Format("Found var address 0x%X, line %d\n", varitem, varindex));
                    CodeViewLines->InsertVarLabel(varitem, "", varindex);
                }
            }
        }
//*/
        selected.cursorLastPosition = lineIndex;
        selected.cursorPosition = lineIndex + line_count - deleted_labels + newLineCount - 1;
    }
}




void ProcessData::DisassembleData(SelectedItemInfo &selected)
{
    RangeItems		dasmed_items;
    CodeViewItem	*cvi;
    int				newLineCount, oldLineCount,
                    progindex, progitem;

    int				lineIndex, lineLast, lineCount,
                    deleted_labels;
    wxArrayInt		cvlines, proglabels;
    uint            i;

	if (!FilterInstructions(cvlines, selected))
		return;

    deleted_labels = 0;

	if (cvlines.GetCount() > 0)
	{
        lineIndex = cvlines[0];
        lineLast = cvlines[1];
        lineCount = lineLast - lineIndex + 1;
        if (lineCount < 0)
            return;

		cvi = CodeViewLines->Line(lineIndex);
		dasmed_items.Index = cvi->Dasmitem;
		cvi = CodeViewLines->Line(lineLast);
		dasmed_items.Count = cvi->Dasmitem - dasmed_items.Index + 1;
		oldLineCount = dasmed_items.Count;

        prog_labels->GetLabelsBetweenRangeAddress(selected.firstAddress, selected.lastAddress, &proglabels);

		for (i = 0; i < static_cast<uint>(lineCount); i++)
            if (RemoveLineAndVarLabels(lineIndex))
                deleted_labels++;


		DisassembleItems(dasmed_items);

		CodeViewLines->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

		newLineCount = dasmed_items.Count - oldLineCount;

		CodeViewLines->UpdateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        if (lineIndex > 0)
            cvi = CodeViewLines->Line(lineIndex - 1);

        if (cvi)
        {
            for (i = 0; i < proglabels.GetCount(); i++)
            {
                progitem = proglabels[i];
                progindex = CodeViewLines->getLineOfAddress(lineIndex, (lineCount + newLineCount), progitem);
                if (!(cvi->LabelProgAddr) || (cvi->LabelProgAddr->Address != static_cast<AbsoluteAddress>(progitem)))
                {
                    LogIt(wxString::Format("Found program address 0x%X, line %d\n", progitem, progindex));
                    CodeViewLines->InsertProgLabel(progitem, "", progindex);
                }
            }
        }

		selected.cursorLastPosition = lineIndex;
        selected.cursorPosition = lineIndex + lineCount - deleted_labels + newLineCount - 1;
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
        cvi = CodeViewLines->Line(i);
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

// Return true if a prog label was found
bool ProcessData::RemoveLineAndProgLabels(const int index)
{
    CodeViewItem *cvi;
    bool ret = false;

    cvi = CodeViewLines->Line(index);
    if (cvi && cvi->LabelProgAddr)
    {
        RemoveLabelUsers(cvi->LabelProgAddr->LabelUsers);
        prog_labels->DelLabel(cvi->LabelProgAddr->Address);
        ret = true;
    }
    CodeViewLines->Del(index);
    return ret;
}

// Return true if a var label was found
bool ProcessData::RemoveLineAndVarLabels(const int index)
{
    CodeViewItem *cvi;
    bool ret = false;

    cvi = CodeViewLines->Line(index);
    if (cvi && cvi->LabelVarAddr)
    {
        RemoveLabelUsers(cvi->LabelVarAddr->LabelUsers);
        var_labels->DelLabel(cvi->LabelVarAddr->Address);
        ret = true;
    }
    CodeViewLines->Del(index);
    return ret;
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




void ProcessData::SearchInstructionArgument(word argument_value, uint search_config)
{
    search_status_->Setup(0, Disassembled->GetCount() - 1, search_config, argument_value);
}



bool ProcessData::SearchInstructionArgumentContinue(AbsoluteAddress &address)
{
    DisassembledItem *de;
    DisassembledIndex dasm_count;
    bool    found = false,
            ended = false;


    while (!found || !ended)
    {
        try
        {
            dasm_count = search_status_->Next();
        }
        catch (SearchException &e)
        {
            wxMessageBox(e.getErrorStr(), "Error ...");
            break;
        }

        de = Disassembled->GetData(dasm_count);
        address = Disassembled->GetBaseAddress(0) + de->GetOffset();
        if (search_status_->SearchLiteral() && FindInArgumentLiteral(de, search_status_->Target()))
        {
            found = true;
            break;
        }

        if (search_status_->SearchVariable() && FindInArgumentVariables(de, search_status_->Target()))
        {
            found = true;
            break;
        }

        if (search_status_->SearchJumpsCalls() && FindInArgumentJumpsCalls(de, search_status_->Target()))
        {
            found = true;
            break;
        }
    }

    return found;
}



bool ProcessData::FindInArgumentVariables(DisassembledItem *de, word argument)
{
    if ((de->GetArgumentType(0) == ARG_VARIABLE) && (de->GetArgument(0, 0) == argument))
        return true;

    return false;
 }

bool ProcessData::FindInArgumentLiteral(DisassembledItem *de, word argument)
{
    uint argument_index = 0;

    while (argument_index < de->GetArgumentCount())
    {
        if ((de->GetArgumentType(argument_index) == ARG_LITERAL) && (de->GetArgument(0, 0) == argument))
            return true;

        argument_index++;
    }

    return false;
}


bool ProcessData::FindInArgumentJumpsCalls(DisassembledItem *de, word argument)
{
    if ((de->GetArgumentType(0) == ARG_ABS_ADDR) && (de->GetArgument(0, 0) == argument))
        return true;

    return false;
}

