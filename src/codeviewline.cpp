/****************************************************************
 * Name:      IDZ80
 * Purpose:   Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module manages lines in CodeView
 **************************************************************/

#include "codeviewline.h"

CodeViewLine::CodeViewLine(DAsmData *dasm)
{
    m_itemcount = -1;
    m_dasm = dasm;
}

CodeViewLine::~CodeViewLine()
{
    Clear();
}



CodeViewItem *CodeViewLine::getData(LineNumber index)
{
    if (index >= m_CodeLine.GetCount())
		return (CodeViewItem *)0;
    return (CodeViewItem *)m_CodeLine[index];
}

void CodeViewLine::setData(CodeViewItem *cvi)
{
    m_CodeLine.Add(cvi);
}



void CodeViewLine::Clear()
{
    uint i;
    CodeViewItem *cvi;

    for (i=0; i < m_CodeLine.GetCount(); i++)
    {
        cvi = getData(i);
        if (cvi != 0)
            if (cvi->Comment != 0)
                delete cvi->Comment;
            if (cvi->RectArg1 != 0)
                delete cvi->RectArg1;
            if (cvi->RectArg2 != 0)
                delete cvi->RectArg2;
            delete cvi;
    }
    m_CodeLine.Clear();
    m_itemcount = -1;
}


int CodeViewLine::AddNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
							 const int org, const wxString &comment)
{
    CodeViewItem *cvi;

    cvi = new CodeViewItem;
    if (!comment.IsEmpty())
    {
        cvi->Comment = new CommentItem;
        cvi->Comment->CommentStr = comment;
    }
    else
        cvi->Comment = 0;
    cvi->Dasmitem = dasmitem;
    cvi->LabelProgAddr = labelprogaddr;
    cvi->LabelVarAddr = labelvaraddr;
    cvi->Org = org;
    cvi->RectArg1 = 0;
    cvi->RectArg2 = 0;
    m_CodeLine.Add(cvi);
    return ++m_itemcount;
}


int CodeViewLine::AddDasm(const DisassembledIndex dasmitem, const wxString &comment)
{
    return (AddNewItem(dasmitem, -1, -1, -1, comment));
}


int CodeViewLine::AddProgLabel(const ProgramAddress labeladdr, const wxString &comment)
{
    if (labeladdr >= 0)
        return (AddNewItem(-1, labeladdr, -1, -1, comment));
    return -1;
}


int CodeViewLine::AddVarLabel(const ProgramAddress labeladdr, const wxString &comment)
{
    if (labeladdr >= 0)
        return (AddNewItem(-1, -1, labeladdr, -1, comment));
    return -1;
}


int CodeViewLine::AddOrg(const ProgramAddress org, const wxString &comment)
{
    return (AddNewItem(-1, -1, -1, org, comment));
}

int CodeViewLine::Add(const wxString &comment)
{
    return (AddNewItem(-1, -1, -1, -1, comment));
}


void CodeViewLine::DelItem(CodeViewItem *cvi)
{
    if (cvi)
    {
        if (cvi->Comment)
            delete cvi->Comment;
        if (cvi->RectArg1)
            delete cvi->RectArg1;
        if (cvi->RectArg2)
            delete cvi->RectArg2;
        delete cvi;
    }
    m_CodeLine.Remove(cvi);
    m_itemcount--;
}


void CodeViewLine::DelComment(CodeViewItem *cvi)
{
    if (cvi->Comment)
    {
        delete cvi->Comment;
        cvi->Comment = 0;
    }
}


void CodeViewLine::Del(const LineNumber idx)
{
    if ((idx >= 0) && (idx < m_CodeLine.GetCount()))
    {
        DelItem(getData(idx));
    }
}



void CodeViewLine::EditItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
							const int org, const wxString &comment, const LineNumber pos)
{
    CodeViewItem *cvi;
    cvi = getData(pos);
    if (!comment.IsEmpty())
        if (cvi->Comment == 0)
        {
            cvi->Comment = new CommentItem;
            cvi->Comment->CommentStr = comment;
        }
        else
            cvi->Comment->CommentStr = comment;
    else
    {
        if (cvi->Comment != 0)
        {
            delete cvi->Comment;
            cvi->Comment = 0;
        }
    }
    if (dasmitem >= 0)
        cvi->Dasmitem = dasmitem;
    if (org >= 0)
        cvi->Org = org;
    if (labelprogaddr >= 0)
        cvi->LabelProgAddr = labelprogaddr;
    if (labelvaraddr >= 0)
        cvi->LabelVarAddr = labelvaraddr;
}

void CodeViewLine::EditDasm(const DisassembledIndex dasmitem, const wxString &comment, LineNumber pos)
{
    if (pos < m_CodeLine.GetCount())
        EditItem(dasmitem, -1, -1, -1, comment, pos);
}


void CodeViewLine::EditProgLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos)
{
    if (pos < m_CodeLine.GetCount())
        EditItem(-1, labeladdr, -1, -1, comment, pos);
}


void CodeViewLine::EditVarLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos)
{
    if (pos < m_CodeLine.GetCount())
        EditItem(-1, -1, labeladdr, -1, comment, pos);
}


void CodeViewLine::EditOrg(const ProgramAddress org, const wxString &comment, LineNumber pos)
{
    if (pos < m_CodeLine.GetCount())
        EditItem(-1, -1, -1, org, comment, pos);
}



void CodeViewLine::Edit(const wxString &comment,const LineNumber pos)
{
    EditItem(-1, -1, -1, -1, comment, pos);
}


int CodeViewLine::InsertNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
								const int org, const wxString &comment, LineNumber pos)
{
    CodeViewItem *cvi;
    cvi = new CodeViewItem;
    if (!comment.IsEmpty())
    {
        cvi->Comment = new CommentItem;
        cvi->Comment->CommentStr = comment;
    }
    else
        cvi->Comment = 0;
    cvi->Dasmitem = dasmitem;
    cvi->LabelProgAddr = labelprogaddr;
    cvi->LabelVarAddr = labelvaraddr;
    cvi->Org = org;
    cvi->RectArg1 = 0;
    cvi->RectArg2 = 0;
    m_CodeLine.Insert(cvi, pos);
    m_itemcount++;
    return pos;
}



int CodeViewLine::InsertDasm(const DisassembledIndex dasmitem, const wxString &comment, LineNumber pos)
{
    if (pos < m_CodeLine.GetCount())
        return (InsertNewItem(dasmitem, -1, -1, -1, comment, pos));
    else
        return (AddDasm(dasmitem, comment));

    return -1;
}



int CodeViewLine::InsertProgLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos)
{
    if (pos < m_CodeLine.GetCount())
        return (InsertNewItem(-1, labeladdr, -1, -1, comment, pos));
    return -1;
}

int CodeViewLine::InsertVarLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos)
{
    if (pos < m_CodeLine.GetCount())
        return (InsertNewItem(-1, -1, labeladdr, -1, comment, pos));
    return -1;
}


int CodeViewLine::InsertOrg(const ProgramAddress org, const wxString &comment, LineNumber pos)
{
    if (pos < m_CodeLine.GetCount())
        return (InsertNewItem(-1, -1, -1, org, comment, pos));
    return -1;
}



//create a new line with comment
int CodeViewLine::Insert(const wxString &comment, const LineNumber pos)
{
    if (pos < m_CodeLine.GetCount())
        return (InsertNewItem(-1, -1, -1, -1, comment, pos));
    return -1;
}


// append a comment to an existing line

int CodeViewLine::AppendComment(const wxString &comment, const LineNumber pos)
{
    CodeViewItem *cvi;
    int i;
    i = -1;
    if (pos < m_CodeLine.GetCount())
    {
        cvi = getData(pos);
        if (cvi)
        {
            if (!comment.IsEmpty())
            {
                cvi->Comment = new CommentItem;
                cvi->Comment->CommentStr = comment;
            }
            else
                if (cvi->Comment)
                {
                    delete cvi->Comment;
                    cvi->Comment = 0;
                }
            i = pos;
        }
    }
    return i;
}


bool CodeViewLine::IsEmpty()
{
    return (GetCount() == 0);
}


uint CodeViewLine::GetCount()
{
    return (m_CodeLine.GetCount());
}

/*
 * If found, store the line which holds this address in "index"
 * Return true if a line pointing to a label with this address already exists.
 * TODO: Find a better solution.
 */
bool CodeViewLine::getLineOfAddress(LineNumber line_index, uint line_count, ProgramAddress addr, int &index)
{
    uint    item_count = 0;
    bool    found_item = false,
            line_label_exists = false;
    uint    line_address = 0;
    CodeViewItem	*cvi;
    DisassembledItem		*de;

    index = -1;
    while ((item_count < line_count) && !found_item)
    {
        cvi = getData(line_index);
        if (cvi)
		{
			if ((cvi->LabelProgAddr == addr) || (cvi->LabelVarAddr == addr))
				line_label_exists = true;

            if (cvi->Dasmitem >= 0)
            {
                de = m_dasm->GetData(cvi->Dasmitem);
                line_address = m_dasm->GetBaseAddress(cvi->Dasmitem) + de->GetOffset();
                if (line_address == addr)
                {
                    found_item = true;
                    index = line_index;
                }
			}
		}
        item_count++;
        line_index++;
    }

    return line_label_exists;
}

bool CodeViewLine::getLineOfAddress(ProgramAddress addr, int &index)
{
    return getLineOfAddress(0, GetCount(), addr, index);
}



void CodeViewLine::UpdateDasmIndex(LineNumber index, const int delta)
{
    CodeViewItem *cvi;

    while (index < GetCount())
    {
        cvi = getData(index);
        if (cvi->Dasmitem >= 0)
            cvi->Dasmitem += delta;
        index++;
    }
}



void CodeViewLine::linkData(DisassembledIndex indexdasm, LineNumber indexline, uint countdasm)
{
	wxString        str;
	DisassembledItem     *de;
	ProgramAddress  address;
	CodeViewItem    *cvi;

    while (countdasm-- > 0)
	{
	    cvi = getData(indexline);
	    if (cvi)
        {
            if (cvi->LabelProgAddr >= 0)
            {
                de = m_dasm->GetData(indexdasm);
                address = m_dasm->GetBaseAddress(indexdasm) + de->GetOffset();
                if (address >= static_cast<ProgramAddress>(cvi->LabelProgAddr))
                    indexline++;
            }
        }
		InsertDasm(indexdasm++, str, indexline++);
	}
}



