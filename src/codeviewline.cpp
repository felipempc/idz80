/****************************************************************
 * Name:      IDZ80
 * Purpose:   Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module holds one line in CodeView
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



CodeViewItem *CodeViewLine::getData(uint index)
{
    if (index >= m_CodeLine.GetCount())
		return (CodeViewItem *)0;	//index = m_CodeLine.GetCount() - 1;
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
        if (!(cvi == 0))
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


int CodeViewLine::AddDasm(const int dasmitem, const wxString &comment)
{
    return (AddNewItem(dasmitem, -1, -1, -1, comment));
}


int CodeViewLine::AddProgLabel(const int labeladdr, const wxString &comment)
{
    if (labeladdr >= 0)
        return (AddNewItem(-1, labeladdr, -1, -1, comment));
    return -1;
}


int CodeViewLine::AddVarLabel(const int labeladdr, const wxString &comment)
{
    if (labeladdr >= 0)
        return (AddNewItem(-1, -1, labeladdr, -1, comment));
    return -1;
}


int CodeViewLine::AddOrg(const int org, const wxString &comment)
{
    return (AddNewItem(-1, -1, -1, org, comment));
}

int CodeViewLine::Add(const wxString &comment)
{
    return (AddNewItem(-1, -1, -1, -1, comment));
}


void CodeViewLine::DelItem(CodeViewItem *cvi)
{
    if (cvi != 0)
    {
        if (cvi->Comment != 0)
            delete cvi->Comment;
        if (cvi->RectArg1 != 0)
            delete cvi->RectArg1;
        if (cvi->RectArg2 != 0)
            delete cvi->RectArg2;
        delete cvi;
    }
    m_CodeLine.Remove(cvi);
    m_itemcount--;
}


void CodeViewLine::DelComment(CodeViewItem *cvi)
{
    if (cvi->Comment != 0)
    {
        delete cvi->Comment;
        cvi->Comment = 0;
    }
}


void CodeViewLine::Del(const int idx)
{
    if ((idx >= 0) && (idx < m_CodeLine.GetCount()))
    {
        DelItem(getData(idx));
    }
}



void CodeViewLine::EditItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
							const int org, const wxString &comment, const int pos)
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

void CodeViewLine::EditDasm(const int dasmitem, const wxString &comment, int pos)
{
    if ((dasmitem >= 0) && (pos >= 0) && (pos < m_CodeLine.GetCount()))
        EditItem(dasmitem, -1, -1, -1, comment, pos);
}


void CodeViewLine::EditProgLabel(const int labeladdr, const wxString &comment, int pos)
{
    if ((labeladdr >= 0) && (pos >= 0) && (pos < m_CodeLine.GetCount()))
        EditItem(-1, labeladdr, -1, -1, comment, pos);
}


void CodeViewLine::EditVarLabel(const int labeladdr, const wxString &comment, int pos)
{
    if ((labeladdr >= 0) && (pos >= 0) && (pos < m_CodeLine.GetCount()))
        EditItem(-1, -1, labeladdr, -1, comment, pos);
}


void CodeViewLine::EditOrg(const int org, const wxString &comment, int pos)
{
    if ((pos >= 0) && (pos < m_CodeLine.GetCount()))
        EditItem(-1, -1, -1, org, comment, pos);
}



void CodeViewLine::Edit(const wxString &comment,const int pos)
{
    EditItem(-1, -1, -1, -1, comment, pos);
}


int CodeViewLine::InsertNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
								const int org, const wxString &comment, int pos)
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



int CodeViewLine::InsertDasm(const int dasmitem, const wxString &comment, int pos)
{
    if ((dasmitem >= 0) && (pos >= 0))
    {
		if (pos < m_CodeLine.GetCount())
			return (InsertNewItem(dasmitem, -1, -1, -1, comment, pos));
		else
			return (AddDasm(dasmitem, comment));
    }
    return -1;
}



int CodeViewLine::InsertProgLabel(const int labeladdr, const wxString &comment, int pos)
{
    if ((labeladdr >= 0) && (pos >= 0) && (pos < m_CodeLine.GetCount()))
        return (InsertNewItem(-1, labeladdr, -1, -1, comment, pos));
    return -1;
}

int CodeViewLine::InsertVarLabel(const int labeladdr, const wxString &comment, int pos)
{
    if ((labeladdr >= 0) && (pos >= 0) && (pos < m_CodeLine.GetCount()))
        return (InsertNewItem(-1, -1, labeladdr, -1, comment, pos));
    return -1;
}


int CodeViewLine::InsertOrg(const int org, const wxString &comment, int pos)
{
    if ((pos >= 0) && (pos < m_CodeLine.GetCount()))
        return (InsertNewItem(-1, -1, -1, org, comment, pos));
    return -1;
}



//create a new line with comment
int CodeViewLine::Insert(const wxString &comment, const int pos)
{
    if ((pos >= 0) && (pos < m_CodeLine.GetCount()))
        return (InsertNewItem(-1, -1, -1, -1, comment, pos));
    return -1;
}


// append a comment to an existing line
int CodeViewLine::AppendComment(const wxString &comment, const int pos)
{
    CodeViewItem *cvi;
    int i;
    i = -1;
    if ((pos >= 0) && (pos < m_CodeLine.GetCount()))
    {
        cvi = getData(pos);
        if (cvi != 0)
        {
            if (!comment.IsEmpty())
            {
                cvi->Comment = new CommentItem;
                cvi->Comment->CommentStr = comment;
            }
            else
                if (cvi->Comment != 0)
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


bool CodeViewLine::getDataLineAddress(uint addr, int &index)
{
    int		i;
    uint	a;
    bool	found,
			labelexists = false;
    CodeViewItem	*cvi;
    DAsmElement		*de;

    i = 0;
    a = 0;
    found = false;
    index = -1;
    while ((i < m_CodeLine.GetCount()) && (!found))
    {
        cvi = getData(i);
        if (cvi != 0)
		{
			if ((cvi->LabelProgAddr == addr) || (cvi->LabelVarAddr == addr))
				labelexists = true;

            if (cvi->Dasmitem >= 0)
            {
                de = m_dasm->GetData(cvi->Dasmitem);
                a = m_dasm->GetBaseAddress() + de->Offset;
                if (a == addr)
                {
                    found = true;
                    index = i;
                }
			}
		}
        if (!found)
            i++;
    }
	return labelexists;
}

void CodeViewLine::UpdateDasmIndex(const int index, const int delta)
{
    int i;
    CodeViewItem *cvi;

    i = index;
    if (index >= 0)
        while (i < GetCount())
        {
            cvi = getData(i);
            if (cvi->Dasmitem >= 0)
                cvi->Dasmitem += delta;
            i++;
        }
}



void CodeViewLine::linkData(int indexdasm, int indexline, int countdasm)
{
	wxString	str = _("");
	DAsmElement	*de;
	int			address, labadress;

    while (countdasm-- > 0)
	{
		labadress = getData(indexline)->LabelProgAddr;
		if (labadress >= 0)
		{
			de = m_dasm->GetData(indexdasm);
			address = m_dasm->GetBaseAddress() + de->Offset;
			if (address >= labadress)
				indexline++;

		}

		InsertDasm(indexdasm++, str, indexline++);
	}
}



