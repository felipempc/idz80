/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 ****************************************************************
 * This module stores source code lines
 ****************************************************************/

#include "codeviewline.h"

CodeViewLine::CodeViewLine(DAsmData *dasm, LabelManager *labels)
{
    itemcount_ = -1;
    disassembled_ = dasm;
    labels_ = labels;
}

CodeViewLine::~CodeViewLine()
{
    Clear();
}



CodeViewItem *CodeViewLine::getData(LineNumber index)
{
    return code_line_.GetLine(index);
}




void CodeViewLine::setData(CodeViewItem *cvi)
{
    code_line_.Add(cvi);
}



void CodeViewLine::Clear()
{
    code_line_.Clear();
    itemcount_ = -1;
}


int CodeViewLine::AddNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
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
    code_line_.Add(cvi);
   return ++itemcount_;
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
    code_line_.Remove(cvi);

    itemcount_--;
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
    if (idx < code_line_.GetCount())
    {
        DelItem(getData(idx));
    }
}



void CodeViewLine::EditItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
							const int org, const wxString &comment, const LineNumber pos)
{
    CodeViewItem *cvi;
    cvi = getData(pos);
    if (comment.IsEmpty())
    {
        if (cvi->Comment)
        {
            delete cvi->Comment;
            cvi->Comment = 0;
        }
    }
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
    cvi->LabelProgAddr = labels_->prog_labels->GetDatabyAddress(labelprogaddr);
    cvi->LabelVarAddr = labels_->var_labels->GetDatabyAddress(labelvaraddr);
}

void CodeViewLine::EditDasm(const DisassembledIndex dasmitem, const wxString &comment, LineNumber pos)
{
    if (pos < code_line_.GetCount())
        EditItem(dasmitem, -1, -1, -1, comment, pos);
}


void CodeViewLine::EditProgLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos)
{
    if (pos < code_line_.GetCount())
        EditItem(-1, labeladdr, -1, -1, comment, pos);
}


void CodeViewLine::EditVarLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos)
{
    if (pos < code_line_.GetCount())
        EditItem(-1, -1, labeladdr, -1, comment, pos);
}


void CodeViewLine::EditOrg(const ProgramAddress org, const wxString &comment, LineNumber pos)
{
    if (pos < code_line_.GetCount())
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

    code_line_.Insert(cvi, pos);

    itemcount_++;
    return pos;
}



int CodeViewLine::InsertDasm(const DisassembledIndex dasmitem, const wxString &comment, LineNumber pos)
{
    if (pos < code_line_.GetCount())
        return (InsertNewItem(dasmitem, -1, -1, -1, comment, pos));
    else
        return (AddDasm(dasmitem, comment));

    return -1;
}



int CodeViewLine::InsertProgLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos)
{
    if (pos < code_line_.GetCount())
        return (InsertNewItem(-1, labeladdr, -1, -1, comment, pos));
    return -1;
}

int CodeViewLine::InsertVarLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos)
{
    if (pos < code_line_.GetCount())
        return (InsertNewItem(-1, -1, labeladdr, -1, comment, pos));
    return -1;
}


int CodeViewLine::InsertOrg(const ProgramAddress org, const wxString &comment, LineNumber pos)
{
    if (pos < code_line_.GetCount())
        return (InsertNewItem(-1, -1, -1, org, comment, pos));
    return -1;
}



//create a new line with comment
int CodeViewLine::Insert(const wxString &comment, const LineNumber pos)
{
    if (pos < code_line_.GetCount())
        return (InsertNewItem(-1, -1, -1, -1, comment, pos));
    return -1;
}


// append a comment to an existing line

int CodeViewLine::AppendComment(const wxString &comment, const LineNumber pos)
{
    CodeViewItem *cvi;
    int i;
    i = -1;
    if (pos < code_line_.GetCount())
    {
        cvi = getData(pos);
        if (cvi)
        {
            if (comment.IsEmpty())
            {
                if (cvi->Comment)
                    delete cvi->Comment;
                cvi->Comment = 0;
            }
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


bool CodeViewLine::IsEmpty()
{
    return (GetCount() == 0);
}


uint CodeViewLine::GetCount()
{
    return (code_line_.GetCount());
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
			if ((cvi->LabelProgAddr) || (cvi->LabelVarAddr))
				line_label_exists = true;

            if (cvi->Dasmitem >= 0)
            {
                de = disassembled_->GetData(cvi->Dasmitem);
                line_address = disassembled_->GetBaseAddress(cvi->Dasmitem) + de->GetOffset();
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
                de = disassembled_->GetData(indexdasm);
                address = disassembled_->GetBaseAddress(indexdasm) + de->GetOffset();
                if (address >= cvi->LabelProgAddr->Address)
                    indexline++;
            }
        }
		InsertDasm(indexdasm++, str, indexline++);
	}
}



