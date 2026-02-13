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

//TODO: Rewrite clear method in ProcessData
/// @brief Clears user labels, disassembled lists, source codes, 
void ProcessData::clear()
{
    m_labels->clearUserLabels();
    m_disassembled_mgr->Clear();
    //source_code->Clear();
    if (m_disassembler)
        m_disassembler->Clear();
}



/// @brief Load system labels (IOPorts, Variables, system calls, etc) after program has been loaded
/// @return True if success
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



/// @brief Makes the first disassemble of data to source code
/// @param t_index Selects the source code to be processed.
void ProcessData::disassembleFirst(const unsigned int t_index)
{
    DisassembledContainer *disassembled = m_disassembled_mgr->Index(t_index);
    disassembled->Clear();


    LogIt("Disassemble by simulating execution of code.");
    m_disassembler->SetProgramIndex(t_index);
    m_disassembler->FullDisassemble();

}



/// @brief Converts a range of data to instructions code
/// @param t_index Selects the source code to be processed.
/// @param t_range Range lines of data.
void ProcessData::disassembleItems(const unsigned int t_index, RangeItems &t_range)
{
    m_disassembler->SetProgramIndex(t_index);
    m_disassembler->DisassembleItems(t_range);
}



/// @brief Converts an instruction to data
/// @param t_index Selects the source code to be processed.
/// @param t_range Range lines of source code.
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



/// @brief Inserts initial data in the source code
/// @param t_index Selects the source code to be processed.
void ProcessData::initData(const unsigned int t_index)
{
    uint                    i, fstil;
    wxArrayString           m_Comments;
    DisassembledContainer   *disassembled = 0;
    SourceCode  *source_code = 0;

    disassembled = m_disassembled_mgr->Index(t_index);
    source_code = m_sourcecode_mgr->index(t_index);
    source_code->clear();

    m_Comments.Add("; ------------------------");
    m_Comments.Add("; Disassembled with IDZ80");
    m_Comments.Add("; 2010 by Felipempc");
    m_Comments.Add("; ------------------------");
    m_Comments.Add("");

    i = 0;
    while (i < m_Comments.GetCount())
        source_code->addComment(m_Comments[i++]);

    source_code->addOrigin(disassembled->GetBaseAddress(0), "");
    i = 0;
    // points to the last line of actual source_code
    fstil = source_code->getCount();

    while (i < disassembled->GetCount())
        source_code->addDasmIndex(i++, "");

    source_code->setFirstInstructionLine(fstil);
}



/// @brief Insert Labels of LabelListCtrl in the sourcecode
/// @param t_index Selects the source code to be modified.
/// @param label Label list to be processed.
void ProcessData::processLabel(const unsigned int t_index, LabelListCtrl *label)
{
    LabelItem *lbl;
    int line_index = -1;
    LabelIndex  counter = 0;
    SourceCodeLine  *sc_line;
    SourceCode      *source_code = 0;

    source_code = m_sourcecode_mgr->index(t_index);
    while (counter < label->getCount())
    {
        lbl = label->getData(counter);
        if (lbl)
        {
            line_index = source_code->getLineOfAddress(lbl->address);
            sc_line = source_code->line(line_index - 1);
            if (sc_line && !(sc_line->labelProgAddress || sc_line->labelVarAddress) && (line_index >= 0))
            {
                if (label->getTypeList() == PRG_LIST) {
                    source_code->insertProgramLabel(lbl->address, "", line_index);
                }
                else
                    if (label->getTypeList() == VAR_LIST)
                        source_code->insertVarLabel(lbl->address, "", line_index);
            }
        }
        counter++;
    }
}


/// @brief Inserts variable and program labels in the source code
/// @param t_index Selects the source code to be processed.
void ProcessData::insertLineLabelsInSourceCode(const unsigned int t_index)
{
    processLabel(t_index, m_labels->prog_labels);
    processLabel(t_index, m_labels->var_labels);
}



/// @brief Removes the DisassembledItem from the list of the label users
/// @param t_de Pointer to the disassembled item
/// @param t_dasmindex Index of the disassembled item
void ProcessData::removeFromLabelUserList(DisassembledItem *t_de, const uint t_dasmindex)
{
    if (t_de->GetMnemonic()->GetArgumentCount() > 0)
        switch(t_de->GetMnemonic()->GetArgument(0).type)
        {
            case OT_VARIABLE:
                m_labels->var_labels->delLabelUser(t_de->GetArgumentValue(0, t_de->GetProgram()->ExecAddress), t_dasmindex);
            case OT_IO_ADDRESS:
                m_labels->io_labels->delLabelUser(t_de->GetArgumentValue(0, t_de->GetProgram()->ExecAddress), t_dasmindex);
            case OT_RELATIVE_ADDRESS:
            case OT_ABSOLUTE_ADDRESS:
                m_labels->prog_labels->delLabelUser(t_de->GetArgumentValue(0, t_de->GetProgram()->ExecAddress), t_dasmindex);
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
    SourceCodeLine	*sc_line;
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
            sc_line = source_code->line(t_selected.firstLine - 1);

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
        source_code->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

        newLineCount = dasmed_items.Count - oldLineCount;

        source_code->updateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        if (sc_line)
        {
            for (i = 0; i < varlabels.size(); ++i)
            {
                varitem = varlabels[i];
                varindex = source_code->getLineOfAddress(lineIndex, (line_count + newLineCount), varitem);
                if (!(sc_line->labelVarAddress) || (sc_line->labelVarAddress->address != static_cast<AbsoluteAddress>(varitem)))
                {
                    LogIt(wxString::Format("Found var address 0x%X, line %d\n", varitem, varindex));
                    source_code->insertVarLabel(varitem, "", varindex);
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
    SourceCodeLine	*sc_line;
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

		sc_line = source_code->line(lineIndex);
		dasmed_items.Index = sc_line->dasmedItem;
		sc_line = source_code->line(lineLast);
		dasmed_items.Count = sc_line->dasmedItem - dasmed_items.Index + 1;
		oldLineCount = dasmed_items.Count;

        m_labels->prog_labels->GetLabelsBetweenRangeAddress(t_selected.firstAddress, t_selected.lastAddress, &proglabels);

		for (i = 0; i < static_cast<uint>(lineCount); ++i)
            if (removeLineAndVarLabels(lineIndex))
                deleted_labels++;


		disassembleItems(t_index, dasmed_items);

		source_code->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

		newLineCount = dasmed_items.Count - oldLineCount;

		source_code->updateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        if (lineIndex > 0)
            sc_line = source_code->line(lineIndex - 1);

        if (sc_line)
        {
            for (i = 0; i < proglabels.size(); ++i)
            {
                progitem = proglabels[i];
                progindex = source_code->getLineOfAddress(lineIndex, (lineCount + newLineCount), progitem);
                if (!(sc_line->labelProgAddress) || (sc_line->labelProgAddress->address != static_cast<AbsoluteAddress>(progitem)))
                {
                    LogIt(wxString::Format("Found program address 0x%X, line %d\n", progitem, progindex));
                    source_code->insertProgramLabel(progitem, "", progindex);
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
			*sc_line;
    DisassembledItem
			*de;
    DisassembledContainer *Disassembled;

    Disassembled = m_disassembled_mgr->Index(t_index);
    foundindex = false;
    last_i = 0;
    for (i = t_selected.firstLine; i <= t_selected.lastLine; i++)
    {
        sc_line = source_code->line(i);
        if (sc_line->dasmedItem >= 0)
        {
            de = Disassembled->GetData(sc_line->dasmedItem);

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
    SourceCodeLine *sc_line;
    bool ret = false;

    sc_line = source_code->line(t_line);
    if (sc_line && sc_line->labelProgAddress)
    {
        removeLabelUsers(sc_line->dasmedItem, sc_line->labelProgAddress->labelUsers);
        m_labels->prog_labels->DelLabel(sc_line->labelProgAddress->address);
        ret = true;
    }
    source_code->delLine(t_line);
    return ret;
}



// Return true if a var label was found
bool ProcessData::removeLineAndVarLabels(const int t_line)
{
    SourceCodeLine *sc_line;
    bool ret = false;

    sc_line = source_code->line(t_line);
    if (sc_line && sc_line->labelVarAddress)
    {
        removeLabelUsers(sc_line->dasmedItem, sc_line->labelVarAddress->labelUsers);
        m_labels->var_labels->DelLabel(sc_line->labelVarAddress->address);
        ret = true;
    }
    source_code->delLine(t_line);
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

