/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 ****************************************************************
 * This module stores source code lines
 ****************************************************************/


#include "sourcecodelines.h"


SourceCodeLines::SourceCodeLines(DisassembledContainer *dasm, LabelManager *labels)
{
    itemcount_ = -1;
    first_intruction_line = -1;
    disassembled_ = dasm;
    labels_ = labels;
}




SourceCodeLines::~SourceCodeLines()
{
    SourceCodeLines::Clear();
}




void SourceCodeLines::Clear()
{
    SourceCodeContainer::Clear();
    itemcount_ = -1;
}




int SourceCodeLines::AddNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
							 const int org, const wxString &comment)
{
    CodeViewItem *cvi;

    cvi = new CodeViewItem;
    cvi->Comment = 0;

    if (!comment.IsEmpty())
    {
        cvi->Comment = new wxString(comment);
    }

    cvi->Dasmitem = dasmitem;
    cvi->LabelProgAddr = labels_->prog_labels->GetDatabyAddress(labelprogaddr);
    cvi->LabelVarAddr = labels_->var_labels->GetDatabyAddress(labelvaraddr);
    cvi->Org = org;
    cvi->RectArg1 = 0;
    cvi->RectArg2 = 0;
    Add(cvi);
   return ++itemcount_;
}




int SourceCodeLines::AddDasm(const DisassembledIndex dasmitem, const wxString &comment)
{
    return (AddNewItem(dasmitem, -1, -1, -1, comment));
}




int SourceCodeLines::AddProgLabel(const AbsoluteAddress labeladdr, const wxString &comment)
{
    if (labeladdr >= 0)
        return (AddNewItem(-1, labeladdr, -1, -1, comment));
    return -1;
}




int SourceCodeLines::AddVarLabel(const AbsoluteAddress labeladdr, const wxString &comment)
{
    if (labeladdr >= 0)
        return (AddNewItem(-1, -1, labeladdr, -1, comment));
    return -1;
}




int SourceCodeLines::AddOrg(const AbsoluteAddress org, const wxString &comment)
{
    return (AddNewItem(-1, -1, -1, org, comment));
}




int SourceCodeLines::AddComment(const wxString &comment)
{
    return (AddNewItem(-1, -1, -1, -1, comment));
}




// TODO: Very slow, fix it !
void SourceCodeLines::DelItem(CodeViewItem *cvi)
{
    LineNumber pos;

    try
    {
        pos  = Find(cvi);
    }
    catch (uint e)
    {
        if (e == LINE_NOT_FOUND)
            return;
    }

    if ((first_intruction_line >= 0) && (pos < first_intruction_line))
        first_intruction_line--;

    RemoveLine(pos);

    itemcount_--;
}




void SourceCodeLines::DelComment(CodeViewItem *cvi)
{
    if (cvi->Comment)
    {
        delete cvi->Comment;
        cvi->Comment = 0;
    }
}




void SourceCodeLines::Del(const LineNumber idx)
{
    if (idx < GetCount())
    {
        if ((first_intruction_line >= 0) && (idx < first_intruction_line))
            first_intruction_line--;
        RemoveLine(idx);
    }
}




void SourceCodeLines::EditItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
							const int org, const wxString &comment, const LineNumber pos)
{
    CodeViewItem *cvi;
    cvi = Line(pos);
    if (comment.IsEmpty())
        DelComment(cvi);
    else
    {
        if (cvi->Comment == 0)
            cvi->Comment = new wxString();
        cvi->Comment->assign(comment);
    }

    if (dasmitem >= 0)
        cvi->Dasmitem = dasmitem;
    if (org >= 0)
        cvi->Org = org;
    if (labelprogaddr >= 0)
        cvi->LabelProgAddr = labels_->prog_labels->GetDatabyAddress(labelprogaddr);
    if (labelvaraddr >= 0)
        cvi->LabelVarAddr = labels_->var_labels->GetDatabyAddress(labelvaraddr);
}




void SourceCodeLines::EditDasm(const DisassembledIndex dasmitem, const wxString &comment, LineNumber pos)
{
    if (pos < GetCount())
        EditItem(dasmitem, -1, -1, -1, comment, pos);
}




void SourceCodeLines::EditProgLabel(const AbsoluteAddress labeladdr, const wxString &comment, LineNumber pos)
{
    if (pos < GetCount())
        EditItem(-1, labeladdr, -1, -1, comment, pos);
}




void SourceCodeLines::EditVarLabel(const AbsoluteAddress labeladdr, const wxString &comment, LineNumber pos)
{
    if (pos < GetCount())
        EditItem(-1, -1, labeladdr, -1, comment, pos);
}




void SourceCodeLines::EditOrg(const AbsoluteAddress org, const wxString &comment, LineNumber pos)
{
    if (pos < GetCount())
        EditItem(-1, -1, -1, org, comment, pos);
}




void SourceCodeLines::EditComment(const wxString &comment,const LineNumber pos)
{
    EditItem(-1, -1, -1, -1, comment, pos);
}




int SourceCodeLines::InsertNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
								const int org, const wxString &comment, LineNumber pos)
{
    CodeViewItem *cvi;

    cvi = new CodeViewItem;
    cvi->Comment = 0;

    if (!comment.IsEmpty())
    {
        cvi->Comment = new wxString(comment);
    }

    if ((first_intruction_line >= 0) && (pos < first_intruction_line))
        first_intruction_line++;


    cvi->Dasmitem = dasmitem;
    cvi->LabelProgAddr = labels_->prog_labels->GetDatabyAddress(labelprogaddr);
    cvi->LabelVarAddr = labels_->var_labels->GetDatabyAddress(labelvaraddr);
    cvi->Org = org;
    cvi->RectArg1 = 0;
    cvi->RectArg2 = 0;

    Insert(cvi, pos);
    itemcount_++;

    return pos;
}




int SourceCodeLines::InsertDasm(const DisassembledIndex dasmitem, const wxString &comment, LineNumber pos)
{
    if (pos < GetCount())
        return (InsertNewItem(dasmitem, -1, -1, -1, comment, pos));
    else
        return (AddDasm(dasmitem, comment));

    return -1;
}




int SourceCodeLines::InsertProgLabel(const AbsoluteAddress labeladdr, const wxString &comment, LineNumber pos)
{
    CodeViewItem *cvi;

    if (pos < GetCount())
    {
        pos = InsertNewItem(-1, labeladdr, -1, -1, comment, pos);
        cvi = Line(pos);
        if (cvi && cvi->LabelProgAddr)
            return (pos);

        if (cvi)
            RemoveLine(pos);
    }
    return -1;
}




int SourceCodeLines::InsertVarLabel(const AbsoluteAddress labeladdr, const wxString &comment, LineNumber pos)
{
    CodeViewItem *cvi;

    if (pos < GetCount())
    {
        pos = InsertNewItem(-1, -1, labeladdr, -1, comment, pos);
        cvi = Line(pos);
        if (cvi && cvi->LabelVarAddr)
            return (pos);

        if (cvi)
            RemoveLine(pos);
    }
    return -1;
}




int SourceCodeLines::InsertOrg(const AbsoluteAddress org, const wxString &comment, LineNumber pos)
{
    if (pos < GetCount())
        return (InsertNewItem(-1, -1, -1, org, comment, pos));
    return -1;
}




//create a new line with comment
int SourceCodeLines::InsertComment(const wxString &comment, const LineNumber pos)
{
    if (pos < GetCount())
        return (InsertNewItem(-1, -1, -1, -1, comment, pos));
    return -1;
}




// append a comment to an existing line
int SourceCodeLines::AppendComment(const wxString &comment, const LineNumber pos)
{
    CodeViewItem *cvi;
    int i;
    i = -1;
    if (pos < GetCount())
    {
        cvi = Line(pos);
        if (cvi)
        {
            if (comment.IsEmpty())
                DelComment(cvi);
            else
            {
                if (cvi->Comment == 0)
                    cvi->Comment = new wxString(comment);
                else
                    cvi->Comment->assign(comment);
            }

            i = pos;
        }
    }
    return i;
}




bool SourceCodeLines::IsEmpty()
{
    return (GetCount() == 0);
}




/*
 * Search for an instruction line which address is "addr". Returns the line number, if found.
 * TODO: Find a better solution.
 */
int SourceCodeLines::getLineOfAddress(LineNumber line_index, uint line_count, AbsoluteAddress addr)
{
    uint    item_count = 0;
    bool    found_item = false;
    uint    line_address = 0;
    CodeViewItem	*cvi;
    DisassembledItem    *de;
    int index = -1;

    while ((item_count < line_count) && !found_item)
    {
        cvi = Line(line_index);
        if (cvi && (cvi->Dasmitem >= 0))
        {
            de = disassembled_->GetData(cvi->Dasmitem);
            line_address = disassembled_->GetBaseAddress(cvi->Dasmitem) + de->GetOffsetInFile();
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




int SourceCodeLines::getLineOfAddress(AbsoluteAddress addr)
{
    return getLineOfAddress(0, GetCount(), addr);
}




int SourceCodeLines::getFirstInstructionLine()
{
    return first_intruction_line;
}




void SourceCodeLines::SetFirstInstructionLine(int fstiline)
{
    if (fstiline < GetCount())
        first_intruction_line = fstiline;
}




void SourceCodeLines::UpdateDasmIndex(LineNumber index, const int delta)
{
    CodeViewItem *cvi;

    while (index < GetCount())
    {
        cvi = Line(index);
        if (cvi->Dasmitem >= 0)
            cvi->Dasmitem += delta;
        index++;
    }
}




void SourceCodeLines::linkData(DisassembledIndex indexdasm, LineNumber indexline, uint countdasm)
{
	wxString        str;
	DisassembledItem     *de;
	AbsoluteAddress  address;
	CodeViewItem    *cvi;

    while (countdasm-- > 0)
	{
	    cvi = Line(indexline);
	    if (cvi)
        {
            if (cvi->LabelProgAddr)
            {
                de = disassembled_->GetData(indexdasm);
                address = disassembled_->GetBaseAddress(indexdasm) + de->GetOffsetInFile();
                if (address >= cvi->LabelProgAddr->Address)
                    indexline++;
            }
        }
		InsertDasm(indexdasm++, str, indexline++);
	}
}



