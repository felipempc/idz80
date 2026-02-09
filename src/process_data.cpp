/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Process all data
 **************************************************************/

#include "process_data.hpp"
#include <stack>


/*
 * ProcessData Implementation
 */


 // REWRITE IS WORK IN PROGRESS...

ProcessData::ProcessData(ProjectBase *t_parent)
{
    m_sourcecode_mgr = t_parent->m_sourcecode_mgr;
    m_disassembled_mgr = t_parent->m_disassembled_mgr;
    m_programs_mgr = t_parent->m_programs_mgr;
    m_mnemonics = t_parent->m_mnemonics;
    m_labels = t_parent->m_labels;

    //m_search_status = new SearchManager();
    m_disassembler = new SmartDecoder(t_parent);

    ModuleName = "ProcessData";
    SetTextLog(t_parent->GetTextLog());
}


ProcessData::~ProcessData()
{
    if (m_disassembler)
        delete m_disassembler;    
    m_labels->destroyAll();
}


void ProcessData::clear()
{
    m_labels->clearUserLabels();
    m_disassembled_mgr->Clear();
    //CodeViewLines->Clear();
    if (m_disassembler)
        m_disassembler->Clear();
}



/// @brief Creates the objects of Disassembled and SourceCode for each "file" opened.
void ProcessData::setupDisassembledAndSourceCode()
{
    unsigned int file_count = m_programs_mgr->Count();

    if (file_count > 0) {
        for (unsigned int x = 0; x < file_count; ++x) {
            DisassembledContainer *disassembled = new DisassembledContainer(this);
            m_disassembled_mgr->Add(disassembled);
            m_sourcecode_mgr->addSourceCode(new SourceCode(disassembled, m_labels));
        }
    }
}



/*  Load system labels (IOPorts, Variables,
 * system calls, etc) after program
 * has been loaded.
 */
bool ProcessData::setupSystemLabels()
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



void ProcessData::disassembleFirst(const unsigned int t_index)
{
    DisassembledContainer *disassembled = m_disassembled_mgr->Index(t_index);
    disassembled->Clear();


    LogIt("Disassemble by simulating execution of code.");
    m_disassembler->SetProgramIndex(t_index);
    m_disassembler->FullDisassemble();

}



void ProcessData::disassembleItems(const unsigned int t_index, RangeItems &t_range)
{
    m_disassembler->SetProgramIndex(t_index);
    m_disassembler->DisassembleItems(t_range);
}



void ProcessData::makeData(const unsigned int t_index, RangeItems &t_range)
{
    uint		i, j, k, f, offset, length;
    DisassembledItem	*de;
    DisassembledContainer *Disassembled;
    RawData  *Program;

    Disassembled = m_disassembled_mgr->Index(t_index);
    Program = m_programs_mgr->Index(t_index);
    f = t_range.Index + t_range.Count;
    if (f > Disassembled->GetCount())
        f = Disassembled->GetCount();

    t_range.Count = 0;    // prepare the new item's count
    k = t_range.Index;
    for (i = t_range.Index; i < f; ++i)
    {
        de = Disassembled->GetData(i);
        offset = de->GetOffsetInFile();
        length = de->GetLength();
        removeFromLabelUserList(de, i);
        Disassembled->Del(i);
        for (j = 0; j < length; ++j)
        {
            de = new DisassembledItem(Program);
            de->SetupDataItem(offset + j);
            Disassembled->Insert(de, k++);
        }
        i += length - 1;
        f += length - 1;
        t_range.Count += length;
    }
}



void ProcessData::initData(const unsigned int t_index)
{
    uint                    i, fstil;
    wxArrayString           m_Comments;
    DisassembledContainer   *Disassembled = 0;
    SourceCode  *source_code = 0;

    Disassembled = m_disassembled_mgr->Index(t_index);
    source_code = m_sourcecode_mgr->index(t_index);
    if (!source_code) {
        source_code = new SourceCode(Disassembled, m_labels);
        m_sourcecode_mgr->addSourceCode(source_code);
    }
    else
        source_code->clear();

    m_Comments.Add("; ------------------------");
    m_Comments.Add("; Disassembled with IDZ80");
    m_Comments.Add("; 2010 by Felipempc");
    m_Comments.Add("; ------------------------");
    m_Comments.Add("");

    i = 0;
    while (i < m_Comments.GetCount())
        source_code->addComment(m_Comments[i++]);

    source_code->addOrigin(Disassembled->GetBaseAddress(0), "");
    i = 0;
    fstil = source_code->GetCount();
    while (i < Disassembled->GetCount())
        source_code->AddDasm(i++, "");

    source_code->setFirstInstructionLine(fstil);
}



/*
 * Insert Labels of LabelListCtrl in the sourcecode
 */
void ProcessData::processLabel(LabelListCtrl *label)
{
    LabelItem *lbl;
    int line_index = -1;
    LabelIndex counter = 0;
    SourceCodeLine    *cvi;

    while (counter < label->getCount())
    {
        lbl = label->getData(counter);
        if (lbl)
        {
            line_index = CodeViewLines->getLineOfAddress(lbl->address);
            cvi = CodeViewLines->line(line_index - 1);
            if (cvi && !(cvi->labelProgAddress || cvi->labelVarAddress) && (line_index >= 0))
            {
                if (label->getTypeList() == PRG_LIST)
                    CodeViewLines->insertProgramLabel(lbl->address, "", line_index);
                else
                    if (label->getTypeList() == VAR_LIST)
                        CodeViewLines->insertVarLabel(lbl->address, "", line_index);
            }
        }
        counter++;
    }
}


void ProcessData::insertLineLabelsInSourceCode()
{
    processLabel(m_labels->prog_labels);
    processLabel(m_labels->var_labels);
}



void ProcessData::removeFromLabelUserList(DisassembledItem *t_de, const uint t_dasmitem)
{
    if (t_de->GetMnemonic()->GetArgumentCount() > 0)
        switch(t_de->GetMnemonic()->GetArgument(0).type)
        {
            case OT_VARIABLE:
                m_labels->var_labels->delLabelUser(t_de->GetArgumentValue(0, t_de->GetProgram()->ExecAddress), t_dasmitem);
            case OT_IO_ADDRESS:
                m_labels->io_labels->delLabelUser(t_de->GetArgumentValue(0, t_de->GetProgram()->ExecAddress), t_dasmitem);
            case OT_RELATIVE_ADDRESS:
            case OT_ABSOLUTE_ADDRESS:
                m_labels->prog_labels->delLabelUser(t_de->GetArgumentValue(0, t_de->GetProgram()->ExecAddress), t_dasmitem);
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
void ProcessData::transformToData(const unsigned int t_index, SelectedItemInfo &t_selected)
{
    RangeItems		dasmed_items;
    SourceCodeLine	*cvi;
    int 			newLineCount, lineIndex, oldLineCount, varindex,
                    varitem;
    uint            i, line_count, deleted_labels;
    IntArray		cvlines, proglabels, varlabels;
    DisassembledContainer *Disassembled;

    Disassembled = m_disassembled_mgr->Index(t_index);
    if ((t_selected.firstInstruction <= t_selected.lastInstruction) &&
        (t_selected.lastInstruction < static_cast<int>(Disassembled->GetCount())))
    {
        if (t_selected.firstLine > 0)
            cvi = CodeViewLines->line(t_selected.firstLine - 1);

        deleted_labels = 0;
        lineIndex = t_selected.firstLine;
        line_count = t_selected.lastLine - lineIndex + 1;

        dasmed_items.Index = t_selected.firstInstruction;
        dasmed_items.Count = t_selected.lastInstruction - t_selected.firstInstruction + 1;
        oldLineCount = dasmed_items.Count;

        m_labels->var_labels->getLabelsBetweenRangeAddress(t_selected.firstAddress, t_selected.lastAddress, &varlabels);

        makeData(t_index, dasmed_items);

        for (i = 0; i < line_count; ++i)
            if (removeLineAndProgLabels(lineIndex))
                deleted_labels++;

//* TODO:REWRITE THIS
        CodeViewLines->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

        newLineCount = dasmed_items.Count - oldLineCount;

        CodeViewLines->updateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        if (cvi)
        {
            for (i = 0; i < varlabels.size(); ++i)
            {
                varitem = varlabels[i];
                varindex = CodeViewLines->getLineOfAddress(lineIndex, (line_count + newLineCount), varitem);
                if (!(cvi->labelVarAddress) || (cvi->labelVarAddress->address != static_cast<AbsoluteAddress>(varitem)))
                {
                    LogIt(wxString::Format("Found var address 0x%X, line %d\n", varitem, varindex));
                    CodeViewLines->insertVarLabel(varitem, "", varindex);
                }
            }
        }
//*/
        t_selected.cursorLastPosition = lineIndex;
        t_selected.cursorPosition = lineIndex + line_count - deleted_labels + newLineCount - 1;
    }
}
 



void ProcessData::disassembleData(const unsigned int t_index, SelectedItemInfo &t_selected)
{
    RangeItems		dasmed_items;
    SourceCodeLine	*cvi;
    int				newLineCount, oldLineCount,
                    progindex, progitem;

    int				lineIndex, lineLast, lineCount,
                    deleted_labels;
    IntArray		cvlines, proglabels;
    uint            i;

	if (!filterInstructions(t_index, cvlines, t_selected))
		return;

    deleted_labels = 0;

	if (cvlines.size() > 0)
	{
        lineIndex = cvlines[0];
        lineLast = cvlines[1];
        lineCount = lineLast - lineIndex + 1;
        if (lineCount < 0)
            return;

		cvi = CodeViewLines->line(lineIndex);
		dasmed_items.Index = cvi->dasmedItem;
		cvi = CodeViewLines->line(lineLast);
		dasmed_items.Count = cvi->dasmedItem - dasmed_items.Index + 1;
		oldLineCount = dasmed_items.Count;

        m_labels->prog_labels->GetLabelsBetweenRangeAddress(t_selected.firstAddress, t_selected.lastAddress, &proglabels);

		for (i = 0; i < static_cast<uint>(lineCount); ++i)
            if (removeLineAndVarLabels(lineIndex))
                deleted_labels++;


		disassembleItems(t_index, dasmed_items);

		CodeViewLines->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

		newLineCount = dasmed_items.Count - oldLineCount;

		CodeViewLines->updateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        if (lineIndex > 0)
            cvi = CodeViewLines->line(lineIndex - 1);

        if (cvi)
        {
            for (i = 0; i < proglabels.size(); ++i)
            {
                progitem = proglabels[i];
                progindex = CodeViewLines->getLineOfAddress(lineIndex, (lineCount + newLineCount), progitem);
                if (!(cvi->labelProgAddress) || (cvi->labelProgAddress->address != static_cast<AbsoluteAddress>(progitem)))
                {
                    LogIt(wxString::Format("Found program address 0x%X, line %d\n", progitem, progindex));
                    CodeViewLines->insertProgramLabel(progitem, "", progindex);
                }
            }
        }

		t_selected.cursorLastPosition = lineIndex;
        t_selected.cursorPosition = lineIndex + lineCount - deleted_labels + newLineCount - 1;
	}
}



/* Returns the first and the last line of instruction / data
 * Returns program labels
 */
bool ProcessData::filterInstructions(const unsigned int t_index, IntArray &t_range, SelectedItemInfo &t_selected)
{
    bool	foundindex;
    int		i, last_i;
    SourceCodeLine
			*cvi;
    DisassembledItem
			*de;
    DisassembledContainer *Disassembled;

    Disassembled = m_disassembled_mgr->Index(t_index);
    foundindex = false;
    last_i = 0;
    for (i = t_selected.firstLine; i <= t_selected.lastLine; i++)
    {
        cvi = CodeViewLines->line(i);
        if (cvi->dasmedItem >= 0)
        {
            de = Disassembled->GetData(cvi->dasmedItem);

            if (!foundindex)
            {
                t_range.push_back(i);
                foundindex = true;
            }
            last_i = i;
        }
    }
    if (foundindex)
        t_range.push_back(last_i);

	if (t_range.size() > 0)
		return true;
	else
		return false;
}



// Return true if a prog label was found
bool ProcessData::removeLineAndProgLabels(const int t_line)
{
    SourceCodeLine *cvi;
    bool ret = false;

    cvi = CodeViewLines->line(t_line);
    if (cvi && cvi->labelProgAddress)
    {
        removeLabelUsers(cvi->dasmedItem, cvi->labelProgAddress->labelUsers);
        m_labels->prog_labels->DelLabel(cvi->labelProgAddress->address);
        ret = true;
    }
    CodeViewLines->delLine(t_line);
    return ret;
}



// Return true if a var label was found
bool ProcessData::removeLineAndVarLabels(const int t_line)
{
    SourceCodeLine *cvi;
    bool ret = false;

    cvi = CodeViewLines->line(t_line);
    if (cvi && cvi->labelVarAddress)
    {
        removeLabelUsers(cvi->dasmedItem, cvi->labelVarAddress->labelUsers);
        m_labels->var_labels->DelLabel(cvi->labelVarAddress->address);
        ret = true;
    }
    CodeViewLines->delLine(t_line);
    return ret;
}




void ProcessData::removeLabelUsers(const unsigned int t_index, IntArray *users)
{
    uint        i;
    DisassembledItem        *de;
    DisassembledContainer   *Disassembled;
    ArgumentStyleOptions    style;

    Disassembled = m_disassembled_mgr->Index(t_index);
    if (users)
    {
        for (i = 0; i < users->size(); ++i)
        {
            de = Disassembled->GetData(users->at(i));
            if (de)
            {
                style = STYLE_NONE;
                de->SetArgumentStyle(0, style);
            }
        }
    }
}




void ProcessData::searchInstructionArgument(word t_argument_value, uint t_search_config)
{
//    m_search_status->Setup(0, Disassembled->GetCount() - 1, search_config, argument_value);
}



bool ProcessData::searchInstructionArgumentContinue(AbsoluteAddress &t_address)
{
    DisassembledItem *de;
    DisassembledIndex dasm_count;
    bool    found = false,
            ended = false;


    while (!found || !ended)
    {
        try
        {
            dasm_count = m_search_status->Next();
        }
        catch (SearchException &e)
        {
            wxMessageBox(e.getErrorStr(), "Error ...");
            break;
        }

        de = Disassembled->GetData(dasm_count);
        t_address = Disassembled->GetBaseAddress(0) + de->GetOffsetInFile();
        if (m_search_status->SearchLiteral() && findInArgumentLiteral(de, m_search_status->Target()))
        {
            found = true;
            break;
        }

        if (m_search_status->SearchVariable() && findInArgumentVariables(de, m_search_status->Target()))
        {
            found = true;
            break;
        }

        if (m_search_status->SearchJumpsCalls() && findInArgumentJumpsCalls(de, m_search_status->Target()))
        {
            found = true;
            break;
        }
    }

    return found;
}



bool ProcessData::findInArgumentVariables(DisassembledItem *de, word argument)
{
    if ((de->GetMnemonic()->GetArgument(0).type == OT_VARIABLE) && (de->GetArgumentValue(0, 0) == argument))
        return true;

    return false;
 }



bool ProcessData::findInArgumentLiteral(DisassembledItem *de, word argument)
{
    uint argument_index = 0;

    while (argument_index < de->GetMnemonic()->GetArgumentCount())
    {
        if ((de->GetMnemonic()->GetArgument(argument_index).type == OT_DATA) && (de->GetArgumentValue(0, 0) == argument))
            return true;

        ++argument_index;
    }

    return false;
}


bool ProcessData::findInArgumentJumpsCalls(DisassembledItem *de, word argument)
{
    if ((de->GetMnemonic()->GetArgument(0).type == OT_ABSOLUTE_ADDRESS) && (de->GetArgumentValue(0, 0) == argument))
        return true;

    return false;
}

