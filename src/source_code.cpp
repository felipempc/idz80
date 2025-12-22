/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 ****************************************************************
 * This module stores source code lines
 ****************************************************************/


#include "source_code.hpp"


SourceCode::SourceCode(const ProjectBase *t_parent)
{
    m_itemcount = -1;
    m_first_instruction_line = -1;
    //TODO: Determine the source code object
    //m_disassembled = t_parent->m_disassembled_mgr->Index();
    m_labels = t_parent->m_labels;
}



SourceCode::~SourceCode()
{
    SourceCode::clear();
}



void SourceCode::clear()
{
    SourceCodeContainer::clear();
    m_itemcount = -1;
}



//TODO: REMOVE IT
int SourceCode::addNewItem(const int t_dasmindex, const int labelprogaddr, const int labelvaraddr,
							 const int org, const wxString &comment)
{
    SourceCodeLine *cvi;

    cvi = new SourceCodeLine;
    cvi->comment = 0;

    if (!comment.IsEmpty())
    {
        cvi->comment = new wxString(comment);
    }

    cvi->dasmedItem = t_dasmindex;
    cvi->labelProgAddress = m_labels->prog_labels->getDatabyAddress(labelprogaddr);
    cvi->labelVarAddress = m_labels->var_labels->getDatabyAddress(labelvaraddr);
    cvi->originAddress = org;
    cvi->rectArg1 = 0;
    cvi->rectArg2 = 0;
    addLine(cvi);
   return ++m_itemcount;
}



/// @brief Add a disassembled item index and comment to the source code
/// @param t_dasmindex An index to a dissassembled item
/// @param t_comment A comment string to the source code
/// @return index to the new line of code
int SourceCode::addDasmIndex(const DisassembledIndex t_dasmindex, const wxString &t_comment)
{
    SourceCodeLine *scline = new SourceCodeLine;

    if (!t_comment.IsEmpty())
        scline->comment = new wxString(t_comment);

    scline->dasmedItem = t_dasmindex;
    scline->labelProgAddress = 0;
    scline->labelVarAddress = 0;
    scline->originAddress = -1;
    scline->rectArg1 = 0;
    scline->rectArg2 = 0;
    addLine(scline);

    return ++m_itemcount;
}



/// @brief Add a Program Label and comment to the source code
/// @param t_labeladdr The address to a line
/// @param t_comment A comment string to the source code
/// @return index to the new line of code
int SourceCode::addProgramLabel(const AbsoluteAddress t_labeladdr, const wxString &t_comment)
{
    SourceCodeLine *scline = new SourceCodeLine;

    if (!t_comment.IsEmpty())
        scline->comment = new wxString(t_comment);

    scline->dasmedItem = -1;
    scline->labelProgAddress = m_labels->prog_labels->getDatabyAddress(t_labeladdr);
    scline->labelVarAddress = 0;
    scline->originAddress = -1;
    scline->rectArg1 = 0;
    scline->rectArg2 = 0;
    addLine(scline);

    return ++m_itemcount;
}



/// @brief Add a Var Label and comment to the source code
/// @param t_labeladdr The address of the label to a line
/// @param t_comment A comment string to the source code
/// @return index to the new line of code
int SourceCode::addVarLabel(const AbsoluteAddress t_labeladdr, const wxString &t_comment)
{
    SourceCodeLine *scline = new SourceCodeLine;

    if (!t_comment.IsEmpty())
        scline->comment = new wxString(t_comment);

    scline->dasmedItem = -1;
    scline->labelProgAddress = 0;
    scline->labelVarAddress = m_labels->var_labels->getDatabyAddress(t_labeladdr);
    scline->originAddress = -1;
    scline->rectArg1 = 0;
    scline->rectArg2 = 0;
    addLine(scline);

    return ++m_itemcount;
}



int SourceCode::addOrigin(const AbsoluteAddress t_org, const wxString &t_comment)
{
    SourceCodeLine *scline = new SourceCodeLine;

    if (!t_comment.IsEmpty())
        scline->comment = new wxString(t_comment);

    scline->dasmedItem = -1;
    scline->labelProgAddress = 0;
    scline->labelVarAddress = 0;
    scline->originAddress = t_org;
    scline->rectArg1 = 0;
    scline->rectArg2 = 0;
    addLine(scline);

    return ++m_itemcount;
}




int SourceCode::addComment(const wxString &t_comment)
{
    SourceCodeLine *scline = new SourceCodeLine;

    if (!t_comment.IsEmpty())
        scline->comment = new wxString(t_comment);

    scline->dasmedItem = -1;
    scline->labelProgAddress = 0;
    scline->labelVarAddress = 0;
    scline->originAddress = -1;
    scline->rectArg1 = 0;
    scline->rectArg2 = 0;
    addLine(scline);

    return ++m_itemcount;
}




// TODO: Very slow, fix it !
void SourceCode::delSCItem(SourceCodeLine *t_scline)
{
    LineNumber pos = 0;

    try
    {
        pos  = findLine(t_scline);
    }
    catch (uint e)
    {
        if (e == LINE_NOT_FOUND)
            return;
    }

    if ((m_first_instruction_line >= 0) && (pos < m_first_instruction_line))
        --m_first_instruction_line;

    removeLine(pos);

    --m_itemcount;
}




void SourceCode::delComment(SourceCodeLine *t_scline)
{
    if (t_scline->comment)
    {
        delete t_scline->comment;
        t_scline->comment = 0;
    }
}




void SourceCode::delLine(const LineNumber t_line)
{
    if (t_line < getCount())
    {
        if ((m_first_instruction_line >= 0) && (t_line < m_first_instruction_line))
            m_first_instruction_line--;
        removeLine(t_line);
    }
}



//TODO:REmove It
void SourceCode::editItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
							const int org, const wxString &comment, const LineNumber pos)
{
    SourceCodeLine *cvi;
    cvi = line(pos);
    if (comment.IsEmpty())
        delComment(cvi);
    else
    {
        if (cvi->comment == 0)
            cvi->comment = new wxString();
        cvi->comment->assign(comment);
    }

    if (dasmitem >= 0)
        cvi->dasmedItem = dasmitem;
    if (org >= 0)
        cvi->originAddress = org;
    if (labelprogaddr >= 0)
        cvi->labelProgAddress = m_labels->prog_labels->getDatabyAddress(labelprogaddr);
    if (labelvaraddr >= 0)
        cvi->labelVarAddress = m_labels->var_labels->getDatabyAddress(labelvaraddr);
}




void SourceCode::editDasmIndex(const DisassembledIndex t_dasmindex, const wxString &t_comment, LineNumber t_line)
{
    SourceCodeLine *scline;
    scline = line(t_line);

    if (t_comment.IsEmpty())
        delComment(scline);
    else
    {
        if (scline->comment == 0)
            scline->comment = new wxString();
        scline->comment->assign(t_comment);
    }

    if (t_dasmindex >= 0)
        scline->dasmedItem = t_dasmindex;

    scline->labelProgAddress = 0;
    scline->labelVarAddress = 0;
    scline->originAddress = -1;
    scline->rectArg1 = 0;
    scline->rectArg2 = 0;
}




/// @brief Select t_line of Source Code, edit t_labeladdr, if it has a label program. Change comment too.
/// @param t_labeladdr 
/// @param t_comment 
/// @param t_line 
void SourceCode::editProgramLabel(const AbsoluteAddress t_labeladdr, const wxString &t_comment, LineNumber t_line)
{
    SourceCodeLine *scline;

    if (t_line >= getCount())
        return;

    scline = line(t_line);

    if (t_comment.IsEmpty())
        delComment(scline);
    else
    {
        if (scline->comment == 0)
            scline->comment = new wxString();
        scline->comment->assign(t_comment);
    }

    scline->labelProgAddress = m_labels->prog_labels->getDatabyAddress(t_labeladdr);

}




void SourceCode::editVarLabel(const AbsoluteAddress t_labeladdr, const wxString &t_comment, LineNumber t_line)
{
    SourceCodeLine *scline;

    if (t_line >= getCount())
        return;

    scline = line(t_line);

    if (t_comment.IsEmpty())
        delComment(scline);
    else
    {
        if (scline->comment == 0)
            scline->comment = new wxString();
        scline->comment->assign(t_comment);
    }

    scline->labelVarAddress = m_labels->var_labels->getDatabyAddress(t_labeladdr);
}



/// @brief Select t_line, edit the origin
/// @param t_org 
/// @param t_comment 
/// @param t_line 
void SourceCode::editOrigin(const AbsoluteAddress t_org, const wxString &t_comment, LineNumber t_line)
{
    SourceCodeLine *scline;

    if (t_line >= getCount())
        return;

    scline = line(t_line);

    if (t_comment.IsEmpty())
        delComment(scline);
    else
    {
        if (scline->comment == 0)
            scline->comment = new wxString();
        scline->comment->assign(t_comment);
    }

    if (t_org >= 0)
        scline->originAddress = static_cast<int>(t_org);
}




void SourceCode::editComment(const wxString &comment,const LineNumber pos)
{
    editItem(-1, -1, -1, -1, comment, pos);
}




int SourceCode::insertNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
								const int org, const wxString &comment, LineNumber pos)
{
    SourceCodeLine *cvi;

    cvi = new SourceCodeLine;
    cvi->comment = 0;

    if (!comment.IsEmpty())
    {
        cvi->comment = new wxString(comment);
    }

    if ((m_first_instruction_line >= 0) && (pos < m_first_instruction_line))
        m_first_instruction_line++;


    cvi->dasmedItem = dasmitem;
    cvi->labelProgAddress = m_labels->prog_labels->getDatabyAddress(labelprogaddr);
    cvi->labelVarAddress = m_labels->var_labels->getDatabyAddress(labelvaraddr);
    cvi->originAddress = org;
    cvi->rectArg1 = 0;
    cvi->rectArg2 = 0;

    insertLine(cvi, pos);
    m_itemcount++;

    return pos;
}




int SourceCode::insertDasmIndex(const DisassembledIndex dasmitem, const wxString &comment, LineNumber pos)
{
    if (pos < getCount())
        return (insertNewItem(dasmitem, -1, -1, -1, comment, pos));
    else
        return (addDasmIndex(dasmitem, comment));

    return -1;
}




int SourceCode::insertProgramLabel(const AbsoluteAddress labeladdr, const wxString &comment, LineNumber pos)
{
    SourceCodeLine *cvi;

    if (pos < getCount())
    {
        pos = insertNewItem(-1, labeladdr, -1, -1, comment, pos);
        cvi = line(pos);
        if (cvi && cvi->labelProgAddress)
            return (pos);

        if (cvi)
            removeLine(pos);
    }
    return -1;
}




int SourceCode::insertVarLabel(const AbsoluteAddress labeladdr, const wxString &comment, LineNumber pos)
{
    SourceCodeLine *cvi;

    if (pos < getCount())
    {
        pos = insertNewItem(-1, -1, labeladdr, -1, comment, pos);
        cvi = line(pos);
        if (cvi && cvi->labelVarAddress)
            return (pos);

        if (cvi)
            removeLine(pos);
    }
    return -1;
}




int SourceCode::insertOrigin(const AbsoluteAddress org, const wxString &comment, LineNumber pos)
{
    if (pos < getCount())
        return (insertNewItem(-1, -1, -1, org, comment, pos));
    return -1;
}




//create a new line with comment
int SourceCode::insertComment(const wxString &comment, const LineNumber pos)
{
    if (pos < getCount())
        return (insertNewItem(-1, -1, -1, -1, comment, pos));
    return -1;
}




// append a comment to an existing line
int SourceCode::appendComment(const wxString &comment, const LineNumber pos)
{
    SourceCodeLine *cvi;
    int i;
    i = -1;
    if (pos < getCount())
    {
        cvi = line(pos);
        if (cvi)
        {
            if (comment.IsEmpty())
                delComment(cvi);
            else
            {
                if (cvi->comment == 0)
                    cvi->comment = new wxString(comment);
                else
                    cvi->comment->assign(comment);
            }

            i = pos;
        }
    }
    return i;
}




bool SourceCode::isEmpty()
{
    return (getCount() == 0);
}




/*
 * Search for an instruction line which address is "addr". Returns the line number, if found.
 * TODO: Find a better solution.
 */
int SourceCode::getLineOfAddress(LineNumber line_index, uint line_count, AbsoluteAddress addr)
{
    uint    item_count = 0;
    bool    found_item = false;
    uint    line_address = 0;
    SourceCodeLine	*cvi;
    DisassembledItem    *de;
    int index = -1;

    while ((item_count < line_count) && !found_item)
    {
        cvi = line(line_index);
        if (cvi && (cvi->dasmedItem >= 0))
        {
            de = m_disassembled->GetData(cvi->dasmedItem);
            line_address = m_disassembled->GetBaseAddress(cvi->dasmedItem) + de->GetOffsetInFile();
            if (line_address == addr)
            {
                found_item = true;
                index = line_index;
            }
        }
        item_count++;
        line_index++;
    }

    return index;
}




int SourceCode::getLineOfAddress(AbsoluteAddress addr)
{
    return getLineOfAddress(0, getCount(), addr);
}




int SourceCode::getFirstInstructionLine()
{
    return m_first_instruction_line;
}




void SourceCode::setFirstInstructionLine(int fstiline)
{
    if (fstiline < getCount())
        m_first_instruction_line = fstiline;
}




void SourceCode::updateDasmIndex(LineNumber index, const int delta)
{
    SourceCodeLine *cvi;

    while (index < getCount())
    {
        cvi = line(index);
        if (cvi->dasmedItem >= 0)
            cvi->dasmedItem += delta;
        index++;
    }
}




void SourceCode::linkData(DisassembledIndex indexdasm, LineNumber indexline, uint countdasm)
{
	wxString        str;
	DisassembledItem     *de;
	AbsoluteAddress  address;
	SourceCodeLine    *cvi;

    while (countdasm-- > 0)
	{
	    cvi = line(indexline);
	    if (cvi)
        {
            if (cvi->labelProgAddress)
            {
                de = m_disassembled->GetData(indexdasm);
                address = m_disassembled->GetBaseAddress(indexdasm) + de->GetOffsetInFile();
                if (address >= cvi->labelProgAddress->address)
                    indexline++;
            }
        }
		insertDasmIndex(indexdasm++, str, indexline++);
	}
}



