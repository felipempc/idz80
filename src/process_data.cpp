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
    disassembled->clear();


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



void ProcessData::disassembleItems(SourceCode *t_sourcecode, RangeItems &t_range)
{
    m_disassembler->DisassembleItems(t_sourcecode, t_range);
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
    if (f > Disassembled->getCount())
        f = Disassembled->getCount();

    t_range.Count = 0;    // prepare the new item's count
    k = t_range.Index;
    for (i = t_range.Index; i < f; ++i)
    {
        de = Disassembled->getData(i);
        offset = de->getOffsetInFile();
        length = de->getOpcodeSize();
        removeFromLabelUserList(de, i);
        Disassembled->del(i);
        for (j = 0; j < length; ++j)
        {
            de = new DisassembledItem(Program);
            de->setupDataItem(offset + j);
            Disassembled->insert(de, k++);
        }
        i += length - 1;
        f += length - 1;
        t_range.Count += length;
    }
}


/// @brief Converts an instruction to data
/// @param t_index Selects the source code to be processed.
/// @param t_range Range lines of source code.
void ProcessData::makeData(SourceCode *t_sourcecode, RangeItems &t_range)
{
    uint    index_in_range, last_in_range, file_offset, dasm_length;
    DisassembledItem	*dasm_item;
    DisassembledContainer *disassembled;
    RawData  *program;

    disassembled = t_sourcecode->getDisassembled();
    if (disassembled->getCount() > 0) {
        program = disassembled->getData(0)->getProgram();
    }
    else {
        return;
    }
    last_in_range = t_range.Index + t_range.Count;
    if (last_in_range > disassembled->getCount())
        last_in_range = disassembled->getCount();

    t_range.Count = 0;    // prepare the new item's count
    index_in_range = t_range.Index;
    for (unsigned int index = t_range.Index; index < last_in_range; ++index) {
        dasm_item = disassembled->getData(index);
        file_offset = dasm_item->getOffsetInFile();
        dasm_length = dasm_item->getOpcodeSize();
        removeFromLabelUserList(dasm_item, index);
        disassembled->del(index);
        for (unsigned int length_count = 0; length_count < dasm_length; ++length_count) {
            dasm_item = new DisassembledItem(program);
            dasm_item->setupDataItem(file_offset + length_count);
            disassembled->insert(dasm_item, index_in_range++);
        }
        index += dasm_length - 1;
        last_in_range += dasm_length - 1;
        t_range.Count += dasm_length;
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

    source_code->addOrigin(disassembled->getBaseAddress(0), "");
    i = 0;
    // points to the last line of actual source_code
    fstil = source_code->getCount();

    while (i < disassembled->getCount())
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
    while (counter < static_cast<int>(label->getCount()))
    {
        lbl = label->getData(counter);
        if (lbl) {
            line_index = source_code->getLineOfAddress(lbl->address);
            sc_line = source_code->line(line_index - 1);
            if (sc_line
                && !(sc_line->labelProgAddress || sc_line->labelVarAddress)
                && (line_index >= 0)) {
                if (label->getTypeList() == PRG_LIST) {
                    source_code->insertProgramLabel(lbl->address, "", line_index);
                }
                else {
                    if (label->getTypeList() == VAR_LIST) {
                        source_code->insertVarLabel(lbl->address, "", line_index);
                    }
                } //else
            }
        }
        ++counter;
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
void ProcessData::removeFromLabelUserList(DisassembledItem *t_dasmitem, const uint t_dasmindex)
{
    if (t_dasmitem->getMnemonic()->GetArgumentCount() > 0) {
        switch(t_dasmitem->getMnemonic()->GetArgument(0).type)
        {
            case OT_VARIABLE:
                m_labels->var_labels->delLabelUser(t_dasmitem->getArgumentValue(0, t_dasmitem->getProgram()->ExecAddress), t_dasmindex);
                [[fallthrough]];
            case OT_IO_ADDRESS:
                m_labels->io_labels->delLabelUser(t_dasmitem->getArgumentValue(0, t_dasmitem->getProgram()->ExecAddress), t_dasmindex);
                [[fallthrough]];
            case OT_RELATIVE_ADDRESS:
                [[fallthrough]];
            case OT_ABSOLUTE_ADDRESS:
                m_labels->prog_labels->delLabelUser(t_dasmitem->getArgumentValue(0, t_dasmitem->getProgram()->ExecAddress), t_dasmindex);
                break;
            case OT_NONE:
                break;
        }
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


/// @brief Takes selected lines of code and convert to lines of data\
/// @brief Delete program labels; Keeps var labels, comments and org directives.
/// @param t_index Selects the source code to be processed.
/// @param t_selected 
void ProcessData::transformToData(const unsigned int t_index, SelectedItemInfo &t_selected)
{
    //TODO: Verify if this function is still neeeded
    SourceCode      *source_code = m_sourcecode_mgr->index(t_index);
    transformToData(source_code, t_selected);
}



/// @brief Takes selected lines of code and convert to lines of data\
/// @brief Delete program labels; Keeps var labels, comments and org directives.
/// @param t_sourcecode The source code
/// @param t_selected Range info about the lines selected
void ProcessData::transformToData(SourceCode *t_sourcecode, SelectedItemInfo &t_selected)
{
    RangeItems		dasmed_items;
    SourceCodeLine	*sc_line = 0;
    int 			line_index, new_line_count, old_line_count,
                    address_line;
    unsigned int    counter, line_count, deleted_labels;
    AddressVector   varlabels;
    DisassembledContainer *disassembled;

    disassembled = t_sourcecode->getDisassembled();
    if ((t_selected.firstInstruction <= t_selected.lastInstruction) &&
        (t_selected.lastInstruction < static_cast<int>(disassembled->getCount())))
    {
        if (t_selected.firstLine > 0)
            sc_line = t_sourcecode->line(t_selected.firstLine - 1);

        deleted_labels = 0;
        line_index = t_selected.firstLine;
        line_count = t_selected.lastLine - line_index + 1;

        dasmed_items.Index = t_selected.firstInstruction;
        dasmed_items.Count = t_selected.lastInstruction - t_selected.firstInstruction + 1;
        old_line_count = dasmed_items.Count;

        //maps existing variables in this range.
        m_labels->var_labels->getLabelsBetweenRangeAddress(t_selected.firstAddress, t_selected.lastAddress, varlabels);

        //Delete disassembled instruction and transform to data
        makeData(t_sourcecode, dasmed_items);
        //Deletes lines of code from sourcecode and program label, if exists in this range
        for (counter = 0; counter < line_count; ++counter)
            if (removeLineAndProgLabels(t_sourcecode, line_index))
                ++deleted_labels;

//* TODO:REWRITE THIS
        //Fills source code with data
        t_sourcecode->linkData(dasmed_items.Index, line_index, dasmed_items.Count);

        new_line_count = dasmed_items.Count - old_line_count;

        t_sourcecode->updateDasmIndex((line_index + dasmed_items.Count), new_line_count);

        if (sc_line)
        {
            for(auto &address : varlabels)
            {
                address_line = t_sourcecode->getLineOfAddress(line_index, (line_count + new_line_count), address);
                if (!(sc_line->labelVarAddress) || (sc_line->labelVarAddress->address != static_cast<AbsoluteAddress>(address)))
                {
                    LogIt(wxString::Format("Found var address 0x%X, line %d\n", address, address_line));
                    t_sourcecode->insertVarLabel(address, "", address_line);
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
    //TODO: Verify it disassembleData(unsigned int t_index) is needed
    disassembleData(m_sourcecode_mgr->index(t_index), t_selected);
}



/// @brief Disassembles data into instructions in range.
/// @param t_sourcecode The source code
/// @param t_selected Lines in range of the source code
void ProcessData::disassembleData(SourceCode *t_sourcecode, SelectedItemInfo &t_selected)
{
    RangeItems		dasmed_items;
    SourceCodeLine	*sc_line;
    int				newLineCount, oldLineCount,
                    address_line;
    int				lineIndex, lineLast, deleted_labels;
    unsigned int    lineCount;
    IntArray		instruction_lines;
    AddressVector   proglabels;

	if (!filterInstructions(t_sourcecode, t_selected, instruction_lines)) {
		return;
    }   

    deleted_labels = 0;

	if (instruction_lines.size() > 0)
	{
        lineIndex = instruction_lines[0];
        lineLast = instruction_lines[1];
        lineCount = lineLast - lineIndex + 1;
        if (lineCount > t_sourcecode->getCount())
            return;

		sc_line = t_sourcecode->line(lineIndex);
		dasmed_items.Index = sc_line->dasmedItem;
		sc_line = t_sourcecode->line(lineLast);
		dasmed_items.Count = sc_line->dasmedItem - dasmed_items.Index + 1;
		oldLineCount = dasmed_items.Count;

        m_labels->prog_labels->getLabelsBetweenRangeAddress(t_selected.firstAddress, t_selected.lastAddress, proglabels);

		for (unsigned int i = 0; i < lineCount; ++i) {
            if (removeLineAndVarLabels(t_sourcecode, lineIndex)) {
                ++deleted_labels;
            }
        }
		disassembleItems(t_sourcecode, dasmed_items);
		t_sourcecode->linkData(dasmed_items.Index, lineIndex, dasmed_items.Count);
		newLineCount = dasmed_items.Count - oldLineCount;
		t_sourcecode->updateDasmIndex((lineIndex + dasmed_items.Count), newLineCount);
        if (lineIndex > 0)
            sc_line = t_sourcecode->line(lineIndex - 1);

        if (sc_line)
        {
            //for (i = 0; i < proglabels.size(); ++i)
            for (auto &address : proglabels) {
                //progitem = proglabels[i];
                address_line = t_sourcecode->getLineOfAddress(lineIndex, (lineCount + newLineCount), address);
                if (!(sc_line->labelProgAddress) || (sc_line->labelProgAddress->address != static_cast<AbsoluteAddress>(address)))
                {
                    LogIt(wxString::Format("Found program address 0x%X, line %d\n", address, address_line));
                    t_sourcecode->insertProgramLabel(address, "", address_line);
                }
            }
        }

		t_selected.cursorLastPosition = lineIndex;
        t_selected.cursorPosition = lineIndex + lineCount - deleted_labels + newLineCount - 1;
	}
}



/// @brief Scans a range of lines, selects the first line and the last line of instruction. Saves them in the array.\
/// @brief TODO: Verify if it's still needed
/// @param t_index Selects the source code to be processed.
/// @param t_range Array where will be saved the start and the end lines of code that was found.
/// @param t_selected Array where the results are saved.
/// @return True if it found any instruction.
bool ProcessData::filterInstructions(const unsigned int t_index,  const SelectedItemInfo &t_selected, IntArray &t_range)
{
    SourceCode          *source_code = 0;

    source_code = m_sourcecode_mgr->index(t_index);
    return filterInstructions(source_code, t_selected, t_range);
}



/// @brief Scans a range of lines, selects the first line and the last line of instruction. Saves them in the array.
/// @param t_sourcecode The source code
/// @param t_selected Array where the results are saved.
/// @param t_range Array where will be saved the start and the end lines of code that was found.
/// @return True if it found any instruction.
bool ProcessData::filterInstructions(SourceCode *t_sourcecode, const SelectedItemInfo &t_selected, IntArray &t_range)
{
    SourceCodeLine      *sc_line;

    for (int scanline = t_selected.firstLine; scanline <= t_selected.lastLine; ++scanline) {
        sc_line = t_sourcecode->line(scanline);
        if (sc_line->dasmedItem >= 0) {
            t_range.push_back(scanline);
            t_range.push_back(t_selected.lastLine);
            break;
        }
    }
    //if found, t_range will have only two elements, first instruction and last instruction. So we must test if it's not empty.
	if (!t_range.empty()) {
		return true;
    }

	return false;
}



/// @brief Removes t_line of source code and, if exists, the program label.
/// @param t_sourcecode The source code
/// @param t_line The line of source code
/// @return True if a program label was found in this line.
bool ProcessData::removeLineAndProgLabels(SourceCode *t_sourcecode, const int t_line)
{
    SourceCodeLine  *sc_line;
    DisassembledContainer *disassembled;
    bool ret = false;

    disassembled = t_sourcecode->getDisassembled();
    sc_line = t_sourcecode->line(t_line);
    if (sc_line && sc_line->labelProgAddress)
    {
        resetStyleFromUsers(disassembled, sc_line->labelProgAddress->labelUsers);
        m_labels->prog_labels->delLabel(sc_line->labelProgAddress->address);
        ret = true;
    }
    t_sourcecode->delLine(t_line);
    return ret;
}



/// @brief Removes t_line of source code and, if exists, the variable label.
/// @param t_index Selects the source code to be processed.
/// @param t_line The line of source code
/// @return True if a variable label was found in this line.
bool ProcessData::removeLineAndVarLabels(const unsigned int t_index,const int t_line)
{
    //TODO: Do we need this?
    return removeLineAndVarLabels(m_sourcecode_mgr->index(t_index), t_line);
}



/// @brief Removes t_line of source code and, if exists, the variable label.
/// @param t_sourcecode The source code
/// @param t_line The line of source code
/// @return True if a variable label was found in this line.
bool ProcessData::removeLineAndVarLabels(SourceCode *t_sourcecode, const int t_line)
{
    SourceCodeLine *sc_line;
    DisassembledContainer *disassembled;
    bool ret = false;

    disassembled = t_sourcecode->getDisassembled();
    sc_line = t_sourcecode->line(t_line);
    if (sc_line && sc_line->labelVarAddress) {
        resetStyleFromUsers(disassembled, sc_line->labelVarAddress->labelUsers);
        m_labels->var_labels->delLabel(sc_line->labelVarAddress->address);
        ret = true;
    }
    t_sourcecode->delLine(t_line);
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
            disassembled_item = t_disassembled->getData(t_users->at(index));
            if (disassembled_item) {
                style = STYLE_NONE;
                disassembled_item->setArgumentStyle(0, style);
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
bool ProcessData::findInArgumentVariables(DisassembledItem *t_di, int t_argument)
{
    if ((t_di->getMnemonic()->GetArgument(0).type == OT_VARIABLE)
        && (t_di->getArgumentValue(0, 0) == t_argument)) {
        return true;
    }

    return false;
 }



/// @brief Searches for a value within the argument(literal type), in an instruction.
/// @param t_di Disassembled Item.
/// @param t_argument Value to be searched.
/// @return True if found.
bool ProcessData::findInArgumentLiteral(DisassembledItem *t_di, int t_argument)
{
    uint argument_index = 0;

    while (argument_index < t_di->getMnemonic()->GetArgumentCount())
    {
        if ((t_di->getMnemonic()->GetArgument(argument_index).type == OT_DATA)
            && (t_di->getArgumentValue(0, 0) == t_argument)) {
            return true;
        }
        ++argument_index;
    }

    return false;
}



/// @brief Searches for a value within the argument(Absolute address type), in an instruction.
/// @param t_di Disassembled Item.
/// @param t_argument Value to be searched.
/// @return True if found.
bool ProcessData::findInArgumentJumpsCalls(DisassembledItem *t_di, int t_argument)
{
    if ((t_di->getMnemonic()->GetArgument(0).type == OT_ABSOLUTE_ADDRESS) && (t_di->getArgumentValue(0, 0) == t_argument)) {
        return true;
    }
    return false;
}

