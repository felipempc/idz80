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
        m_disassembler->clear();
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
    m_disassembler->fullDisassemble();

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
void ProcessData::initSourceCode(const unsigned int t_index)
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


/// @brief Take selected lines of code and convert to lines of data\
/// @brief Delete program labels; Keeps var labels, comments and org directives.
/// @param t_index Selects the source code to be processed.
/// @param t_selected 
void ProcessData::transformToData(const unsigned int t_index, SelectedItemInfo &t_selected)
{
    RangeItems		dasmed_items;
    SourceCodeLine	*sc_line = 0;
    SourceCode      *source_code = 0;
    int 			line_index, new_line_count, old_line_count,
                    address_line;
    unsigned int    counter, line_count, deleted_labels;
    AddressVector   varlabels;
    DisassembledContainer *disassembled;

    disassembled = m_disassembled_mgr->Index(t_index);
    source_code = m_sourcecode_mgr->index(t_index);
    if ((t_selected.firstInstruction <= t_selected.lastInstruction) &&
        (t_selected.lastInstruction < static_cast<int>(disassembled->GetCount())))
    {
        if (t_selected.firstLine > 0)
            sc_line = source_code->line(t_selected.firstLine - 1);

        deleted_labels = 0;
        line_index = t_selected.firstLine;
        line_count = t_selected.lastLine - line_index + 1;

        dasmed_items.Index = t_selected.firstInstruction;
        dasmed_items.Count = t_selected.lastInstruction - t_selected.firstInstruction + 1;
        old_line_count = dasmed_items.Count;

        //maps existing variables in this range.
        m_labels->var_labels->getLabelsBetweenRangeAddress(t_selected.firstAddress, t_selected.lastAddress, varlabels);

        //Delete disassembled instruction and transform to data
        makeData(t_index, dasmed_items);
        //Deletes lines of code from sourcecode and program label, if exists in this range
        for (counter = 0; counter < line_count; ++counter)
            if (removeLineAndProgLabels(t_index, line_index))
                ++deleted_labels;

//* TODO:REWRITE THIS
        //Fills source code with data
        source_code->linkData(dasmed_items.Index, line_index, dasmed_items.Count);

        new_line_count = dasmed_items.Count - old_line_count;

        source_code->updateDasmIndex((line_index + dasmed_items.Count), new_line_count);

        if (sc_line)
        {
            for(auto &address : varlabels)
            {
                address_line = source_code->getLineOfAddress(line_index, (line_count + new_line_count), address);
                if (!(sc_line->labelVarAddress) || (sc_line->labelVarAddress->address != static_cast<AbsoluteAddress>(address)))
                {
                    LogIt(wxString::Format("Found var address 0x%X, line %d\n", address, address_line));
                    source_code->insertVarLabel(address, "", address_line);
                }
            }
        }
//*/
        t_selected.cursorLastPosition = line_index;
        t_selected.cursorPosition = line_index + line_count - deleted_labels + new_line_count - 1;
    }
}



/// @brief Disassembles data into instructions in range.
/// @param t_index Selects the source code to be processed.
/// @param t_selected Lines in range of the source code
void ProcessData::disassembleData(const unsigned int t_index, SelectedItemInfo &t_selected)
{
    RangeItems		dasmed_items;
    SourceCodeLine	*sc_line;
    SourceCode      *source_code = 0;
    int				newLineCount, oldLineCount,
                    address_line;

    int				lineIndex, lineLast, deleted_labels;
    unsigned int    lineCount;
    IntArray		instruction_lines;
    AddressVector   proglabels;

	if (!filterInstructions(t_index, t_selected, instruction_lines)) {
		return;
    }   

    source_code = m_sourcecode_mgr->index(t_index);
    deleted_labels = 0;

	if (instruction_lines.size() > 0)
	{
        lineIndex = instruction_lines[0];
        lineLast = instruction_lines[1];
        lineCount = lineLast - lineIndex + 1;
        if (lineCount > source_code->getCount())
            return;

		sc_line = source_code->line(lineIndex);
		dasmed_items.Index = sc_line->dasmedItem;
		sc_line = source_code->line(lineLast);
		dasmed_items.Count = sc_line->dasmedItem - dasmed_items.Index + 1;
		oldLineCount = dasmed_items.Count;

        m_labels->prog_labels->getLabelsBetweenRangeAddress(t_selected.firstAddress, t_selected.lastAddress, proglabels);

		for (unsigned int i = 0; i < lineCount; ++i)
            if (removeLineAndVarLabels(t_index, lineIndex))
                ++deleted_labels;


		disassembleItems(t_index, dasmed_items);

		source_code->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);

		newLineCount = dasmed_items.Count - oldLineCount;

		source_code->updateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);

        if (lineIndex > 0)
            sc_line = source_code->line(lineIndex - 1);

        if (sc_line)
        {
            //for (i = 0; i < proglabels.size(); ++i)
            for (auto &address : proglabels) {
                //progitem = proglabels[i];
                address_line = source_code->getLineOfAddress(lineIndex, (lineCount + newLineCount), address);
                if (!(sc_line->labelProgAddress) || (sc_line->labelProgAddress->address != static_cast<AbsoluteAddress>(address)))
                {
                    LogIt(wxString::Format("Found program address 0x%X, line %d\n", address, address_line));
                    source_code->insertProgramLabel(address, "", address_line);
                }
            }
        }

		t_selected.cursorLastPosition = lineIndex;
        t_selected.cursorPosition = lineIndex + lineCount - deleted_labels + newLineCount - 1;
	}
}


/// @brief Scans a range of lines, selects the first line and the last line of instruction. Saves them in the array.\
/// @brief TODO: Improve filterIstructions
/// @param t_index Selects the source code to be processed.
/// @param t_range Array where will be saved the start and the end lines of code that was found.
/// @param t_selected Array where the results are saved.
/// @return True if it found any instruction.
bool ProcessData::filterInstructions(const unsigned int t_index,  const SelectedItemInfo &t_selected, IntArray &t_range)
{
    bool	foundindex;
    //int		i, last_i;
    SourceCodeLine      *sc_line;
    SourceCode          *source_code = 0;
    DisassembledContainer *Disassembled;

    //Disassembled = m_disassembled_mgr->Index(t_index);
    source_code = m_sourcecode_mgr->index(t_index);

    foundindex = false;
    //last_i = 0;
    for (unsigned int i = t_selected.firstLine; i <= t_selected.lastLine; ++i) {
        sc_line = source_code->line(i);
        if (sc_line->dasmedItem >= 0) {
            if (!foundindex)
            {
                t_range.push_back(i);
                t_range.push_back(t_selected.lastLine);
                foundindex = true;
                break;
            }
            //last_i = i;
        }
    }
    //if found, t_range will have only two elements, first instruction and last instruction. So we must test if size is equal 2.
	if (t_range.size() > 0)
		return true;
	else
		return false;
}



/// @brief Removes t_line of source code and, if exists, the program label.
/// @param t_index Selects the source code to be processed.
/// @param t_line The line of source code
/// @return True if a program label was found in this line.
bool ProcessData::removeLineAndProgLabels(const unsigned int t_index, const int t_line)
//TODO: Replace t_index with source_code
{
    SourceCodeLine  *sc_line;
    SourceCode      *source_code = 0;
    DisassembledContainer *disassembled;
    bool ret = false;

    source_code = m_sourcecode_mgr->index(t_index);
    disassembled = m_disassembled_mgr->Index(t_index);
    sc_line = source_code->line(t_line);
    if (sc_line && sc_line->labelProgAddress)
    {
        resetStyleFromUsers(disassembled, sc_line->labelProgAddress->labelUsers);
        m_labels->prog_labels->delLabel(sc_line->labelProgAddress->address);
        ret = true;
    }
    source_code->delLine(t_line);
    return ret;
}



/// @brief Removes t_line of source code and, if exists, the variable label.
/// @param t_index Selects the source code to be processed.
/// @param t_line The line of source code
/// @return True if a variable label was found in this line.
bool ProcessData::removeLineAndVarLabels(const unsigned int t_index,const int t_line)
{
    SourceCodeLine *sc_line;
    SourceCode      *source_code = 0;
    DisassembledContainer *disassembled;
    bool ret = false;

    source_code = m_sourcecode_mgr->index(t_index);
    disassembled = m_disassembled_mgr->Index(t_index);
    sc_line = source_code->line(t_line);
    if (sc_line && sc_line->labelVarAddress)
    {
        resetStyleFromUsers(disassembled, sc_line->labelVarAddress->labelUsers);
        m_labels->var_labels->delLabel(sc_line->labelVarAddress->address);
        ret = true;
    }
    source_code->delLine(t_line);
    return ret;
}




/// @brief Reset style from users(of label)
/// @param DisassembledContainer List of disassembled items
/// @param users List of users of the label
void ProcessData::resetStyleFromUsers(DisassembledContainer *t_disassembled, IntArray *t_users) {
    DisassembledItem        *disassembled_item;
    ArgumentStyleOptions    style;

    if (t_users && t_disassembled) {
        for (int index : *t_users) {
            disassembled_item = t_disassembled->GetData(t_users->at(index));
            if (disassembled_item) {
                style = STYLE_NONE;
                disassembled_item->SetArgumentStyle(0, style);
            }
        }
    }
}



/*
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
*/



/// @brief Searches for a value within the argument(var type), in an instruction.
/// @param t_di Disassembled Item
/// @param t_argument Value to be searched.
/// @return True if found.
bool ProcessData::findInArgumentVariables(DisassembledItem *t_di, unsigned int t_argument)
{
    if ((t_di->GetMnemonic()->GetArgument(0).type == OT_VARIABLE) && (t_di->GetArgumentValue(0, 0) == t_argument))
        return true;

    return false;
 }



/// @brief Searches for a value within the argument(literal type), in an instruction.
/// @param t_di Disassembled Item.
/// @param t_argument Value to be searched.
/// @return True if found.
bool ProcessData::findInArgumentLiteral(DisassembledItem *t_di, unsigned int t_argument)
{
    uint argument_index = 0;

    while (argument_index < t_di->GetMnemonic()->GetArgumentCount())
    {
        if ((t_di->GetMnemonic()->GetArgument(argument_index).type == OT_DATA) && (t_di->GetArgumentValue(0, 0) == t_argument))
            return true;

        ++argument_index;
    }

    return false;
}



/// @brief Searches for a value within the argument(Absolute address type), in an instruction.
/// @param t_di Disassembled Item.
/// @param t_argument Value to be searched.
/// @return True if found.
bool ProcessData::findInArgumentJumpsCalls(DisassembledItem *t_di, unsigned int t_argument)
{
    if ((t_di->GetMnemonic()->GetArgument(0).type == OT_ABSOLUTE_ADDRESS) && (t_di->GetArgumentValue(0, 0) == t_argument)) {
        return true;
    }
    return false;
}

