/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   2009-11-09
 * License:   GPL
 *
 * This module shows/controls list labels
 **************************************************************/

#include <wx/menu.h>
#include "labelslist.h"
#include "EditLabelDlg.h"



const DisassembledIndex LabelListCtrl::NO_DASM_ITEM;

/*
 *      Label List Control Contructor/Destructor
 */
LabelListCtrl::LabelListCtrl(wxWindow* parent, const wxString default_name, LogWindow *logparent)
                    : wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL)
{
    selected_item_ = -1;
    label_index_ = -1;
    main_window_ = parent;

    InsertColumn(0, "Address");
    InsertColumn(1, "Label");
    SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
    SetColumnWidth(1, 100);

    if (default_name.IsEmpty())
        default_label_name_ = "LABEL";
    else
        default_label_name_ = default_name;

    Bind(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, &LabelListCtrl::OnMouseRightDown, this);
    Bind(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, &LabelListCtrl::OnMouseDblLeft, this);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &LabelListCtrl::OnMenuPopUpAdd, this, idMENU_POPUP_ADD);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &LabelListCtrl::OnMenuPopUpEdit, this, idMENU_POPUP_EDIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &LabelListCtrl::OnMenuPopUpDel, this, idMENU_POPUP_DEL);
    Bind(wxEVT_COMMAND_LIST_COL_CLICK, &LabelListCtrl::OnColumnClick, this);

    SetTextLog(logparent);
    ModuleName = "Label";
}

LabelListCtrl::~LabelListCtrl()
{
    Clear();
}


void LabelListCtrl::Clear()
{
    LabelIndex i;
    for (i = 0; i < GetItemCount(); i++)
        DeleteLabelData(GetData(i));
    label_index_ = -1;
    DeleteAllItems();
}


/*
 *     Label List Control Implementation
 */

int LabelListCtrl::AddLabel(ProgramAddress addr, wxString name, DisassembledIndex dasmitem)
{
    wxString buf;
    wxListItem li;
    LabelItem *lbl;
    long itemfound, index;

    lbl = 0;
    index = -1;
    buf.Printf("%X", addr);
    itemfound = FindItem(0, buf);
    if (itemfound < 0)
    {
		lbl = new LabelItem;
		lbl->Address = addr;
		if (name.IsEmpty())
            name = CreateDefaultName(addr);
		lbl->LabelStr = name;

		if (dasmitem >= 0)
		{
			lbl->LabelUsers = new wxArrayInt();
			lbl->LabelUsers->Add(dasmitem);
		}
		else
			lbl->LabelUsers = 0;

        li.SetText(buf);
        li.SetImage(-1);
        li.SetAlign(wxLIST_FORMAT_LEFT);
        li.SetData(lbl);
        li.SetId(++label_index_);
        index = InsertItem(li);
        SetItem(index, 1, name);
        LogIt(wxString::Format("[%X] Label Inserted %d", li.m_itemId, index));
        itemfound = index;
    }
    else
    {
        lbl = GetData(itemfound);
        if (lbl && lbl->LabelUsers)
            if (lbl->LabelUsers->Index(dasmitem) == wxNOT_FOUND)
                lbl->LabelUsers->Add(dasmitem);
    }
    return itemfound;
}



int LabelListCtrl::AddLabel(ProgramAddress addr, wxString name, wxArrayInt &labelusers)
{
    wxString	buf;
    wxListItem	li;
    LabelItem	*lbl;
    long		itemfound, index;
    uint        counter;

    lbl = 0;
    index = -1;
    buf.Printf("%X", addr);
    itemfound = FindItem(0, buf);
    if (itemfound < 0)
    {
		lbl = new LabelItem;
		lbl->Address = addr;
        if (name.IsEmpty())
            name = CreateDefaultName(addr);
        lbl->LabelStr = name;

		if (labelusers.IsEmpty())
			lbl->LabelUsers = 0;
		else
		{
			lbl->LabelUsers = new wxArrayInt();
			for(counter = 0; counter < labelusers.GetCount(); counter++)
                lbl->LabelUsers->Add(labelusers.Item(counter));
		}
        li.SetText(buf);
        li.SetImage(-1);
        li.SetAlign(wxLIST_FORMAT_LEFT);
        li.SetData(lbl);
        li.SetId(++label_index_);
        index = InsertItem(li);
        SetItem(index, 1, name);
        itemfound = index;
    }
    else
    {	// Overwrite old list of users, if exists
        lbl = GetData(itemfound);
        if ((lbl) && (!labelusers.IsEmpty()))
		{
            if (lbl->LabelUsers)
			{
                lbl->LabelUsers->Clear();
                for(counter = 0; counter < labelusers.GetCount(); counter++)
                    lbl->LabelUsers->Add(labelusers.Item(counter));
			}
			else
			{
				lbl->LabelUsers = new wxArrayInt();
				for(counter = 0; counter < labelusers.GetCount(); counter++)
                    lbl->LabelUsers->Add(labelusers.Item(counter));
			}
		}

    }
    return itemfound;
}



bool LabelListCtrl::DelLabel(ProgramAddress addr)
{
    uint		label;
    LabelItem	*lbl;
    bool		ret = false;

    lbl = FindByAddress(addr, label);

    if (lbl)
    {
        DeleteLabelData(lbl);
        ret = true;
        DeleteItem(label);
        #ifdef IDZ80DEBUG
        LogIt(wxString::Format("Removed item = %d", label));
        #endif
        label_index_--;
    }
    return ret;
}


void LabelListCtrl::DelLabelUser(ProgramAddress addr, DisassembledIndex dasmitem)
{
    int         i;
    uint        label;
    LabelItem	*lbl;


    lbl = FindByAddress(addr, label);

    if (lbl && lbl->LabelUsers)
    {
        i = lbl->LabelUsers->Index(dasmitem);
        if (i != wxNOT_FOUND)
            lbl->LabelUsers->RemoveAt(i);
        if (lbl->LabelUsers->IsEmpty())
        {
            delete lbl->LabelUsers;
            lbl->LabelUsers = 0;
            delete lbl;
            DeleteItem(label);
            label_index_--;
            #ifdef IDZ80DEBUG
            LogIt(wxString::Format("Removed item by userlabel = %d", label));
            #endif
        }
    }
}




//TODO: MODIFY or Remove....
int LabelListCtrl::GetLabel(ProgramAddress addr, wxString& str)
{
    int i;
    wxListItem item;

    str.Printf("%X", addr);
    i = FindItem(-1, str);
    if (i >= 0)
    {
        item.m_itemId = i;
        item.m_col = 1;
        item.m_mask = wxLIST_MASK_TEXT;
        GetItem(item);
        str = item.m_text;
    }
    return i;
}


wxString LabelListCtrl::GetLabel(LabelIndex idx)
{
    uint i;
    wxString str_ret;
    //wxListItem item;
    LabelItem *lbl;

    i = GetCount();
    if (i && (idx < i))
    {
        /*
        item.m_itemId = idx;
        item.m_col = 1;
        item.m_mask = wxLIST_MASK_TEXT;
        GetItem(item);
        str_ret = item.m_text;
        */

        lbl = GetData(idx);

        if (lbl)
            str_ret = lbl->LabelStr;

        LogIt(wxString::Format("Index %d  <%s>", idx, str_ret));

    }
    return str_ret;
}


int LabelListCtrl::GetLabelIndex(ProgramAddress addr)
{
    return (FindItem(-1, wxString::Format("%X", addr)));
}



wxString LabelListCtrl::GetAddress(uint idx)
{
    uint i;
    wxString str_ret;
    //wxListItem item;
    LabelItem *lbl;

    i = GetCount();
    if (i && (idx < i))
    {
        /*
        item.m_itemId = idx;
        item.m_col = 0;
        item.m_mask = wxLIST_MASK_TEXT;
        GetItem(item);
        str_ret = item.m_text;
        */
        lbl = GetData(idx);
        if (lbl)
            str_ret = wxString::Format("%.4X", lbl->Address);
    }
    return str_ret;
}


LabelItem *LabelListCtrl::FindByAddress(ProgramAddress addr, LabelIndex &label_index)
{
    int index;
    LabelItem *lbl;

    lbl = 0;

    index = GetLabelIndex(addr);


    if (index != wxNOT_FOUND)
    {
        lbl = GetData(index);
        label_index = index;
    }

    return lbl;
}


wxString LabelListCtrl::CreateDefaultName(const ProgramAddress addr)
{
    return (wxString::Format("%s_%.4X", default_label_name_, addr));
}


void LabelListCtrl::OnMouseDblLeft(wxListEvent& event)
{
    wxCommandEvent evt;

    selected_item_ = event.GetIndex();

    if (selected_item_ < 0)
        OnMenuPopUpAdd(evt);
    else
        OnMenuPopUpEdit(evt);
}


void LabelListCtrl::OnMouseRightDown(wxListEvent& event)
{
    wxMenu popup;
    #ifdef IDZ80DEBUG
    LabelItem *selected_label;
    uint i;
    wxString str;
    #endif

    selected_item_ = event.GetIndex();

    #ifdef IDZ80DEBUG
    selected_label = GetData(selected_item_);
    if (selected_label->LabelUsers)
    {
        for(i = 0; i < selected_label->LabelUsers->GetCount(); i++)
        {
            str << wxString::Format(" %x", selected_label->LabelUsers->Item(i));
        }
        str.Trim(false);
        LogIt(wxString::Format("[%X] <%s> Label users->%s", selected_label->Address, selected_label->LabelStr, str));
    }
    else
        LogIt("No users for this label.\n");
    #endif

    popup.Append(idMENU_POPUP_ADD, "Add Label");
    popup.Append(idMENU_POPUP_EDIT, "Edit Label");
    popup.AppendSeparator();
    popup.Append(idMENU_POPUP_DEL, "Del Label");
    PopupMenu(&popup);
}


void LabelListCtrl::OnMenuPopUpAdd(wxCommandEvent& event)
{
    EditLabelDlg adlab(this);
    LabelItem *lbl;
    int i;


    if (adlab.ShowModal() == wxID_OK)
    {
        i = AddLabel(adlab.GetAddress(), adlab.GetLabel());
        lbl = GetData(i);
        if (lbl)
            lbl->LabelStr = adlab.GetLabel();
        SortAddress();
    }
}

void LabelListCtrl::OnMenuPopUpDel(wxCommandEvent& event)
{
    DeleteLabelData(GetData(selected_item_));
    DeleteItem(selected_item_);
    label_index_--;
    selected_item_ = -1;
    main_window_->Refresh();
}


void LabelListCtrl::OnMenuPopUpEdit(wxCommandEvent& event)
{
    LabelItem *lbl;

    lbl = GetData(selected_item_);
	EditLabelDialog(lbl->Address);
}



bool LabelListCtrl::EditLabelDialog(ProgramAddress addr)
{
    EditLabelDlg adlab(this,true);
    LabelItem *lbl;
    wxString str;
	int idx;
	bool ret = false;


    adlab.SetAddress(addr);
	idx = GetLabel(addr, str);
	if (idx >= 0)
	{
		adlab.SetLabel(str);
		lbl = GetData(idx);

		if (adlab.ShowModal() == wxID_OK)
		{
			EditLabel(idx, adlab.GetLabel());
			if (lbl)
			{
				lbl->LabelStr = adlab.GetLabel();
				ret = true;
			}
		}
	}
	return ret;
}


bool LabelListCtrl::IsEmpty()
{
    return (GetItemCount() < 1);
}

uint LabelListCtrl::GetCount()
{
    return static_cast<uint>(GetItemCount());
}

void LabelListCtrl::EditLabel(uint listitem, wxString strlabel)
{
    wxListItem item;

    item.m_itemId = listitem;
    item.m_col = 1;
    item.m_mask = wxLIST_MASK_TEXT;
    item.m_text = strlabel;
    SetItem(item);
    SortAddress();

}



void LabelListCtrl::GetLabelsBetweenRangeAddress(ProgramAddress first_address, ProgramAddress last_address, wxArrayInt *address_list)
{
    LabelItem   *lbl;
    uint    loopcount;

    if (address_list)
    {
        for(loopcount = 0; loopcount < GetCount(); loopcount++)
        {
            lbl = GetData(loopcount);
            if (lbl && (lbl->Address >= first_address) && (lbl->Address <= last_address))
                address_list->Add(lbl->Address);
        }
    }
}




void LabelListCtrl::SortAddress(bool crescent)
{
    if (!IsEmpty())
    {
        if (crescent)
            SortItems(CompareAddress, 0);
        else
            SortItems(CompareAddress, 1);
    }
}


void LabelListCtrl::SortLabelStr(bool crescent)
{
    if (!IsEmpty())
    {
        if (crescent)
            SortItems(CompareLabelStr, 0);
        else
            SortItems(CompareLabelStr, 1);
    }
}



void LabelListCtrl::OnColumnClick(wxListEvent& event)
{
    static bool sort_direction = true;

    if (event.GetColumn() == 0)
    {
        if (sort_direction)
            sort_direction = false;
        else
            sort_direction = true;

        SortAddress(sort_direction);
    }

    if (event.GetColumn() == 1)
    {
        if (sort_direction)
            sort_direction = false;
        else
            sort_direction = true;

        SortLabelStr(sort_direction);
    }
}



/*
 *     Call back wxListCtrl implementation
 */

int wxCALLBACK CompareAddress(long item1, long item2, long data)
{
    LabelItem *lbl_1, *lbl_2;

    lbl_1 = reinterpret_cast<LabelItem *>(item1);
    lbl_2 = reinterpret_cast<LabelItem *>(item2);

    if ((lbl_1 == 0) || lbl_2 == 0)
        return 0;

    if (data == 0)
    {
        if (lbl_1->Address < lbl_2->Address)
            return -1;

        if (lbl_1->Address > lbl_2->Address)
            return 1;
    }
    else
    {
        if (lbl_1->Address < lbl_2->Address)
            return 1;

        if (lbl_1->Address > lbl_2->Address)
            return -1;
    }

    return 0;
}


int wxCALLBACK CompareLabelStr(long item1, long item2, long data)
{
    LabelItem *lbl_1, *lbl_2;
    int ret = 0;

    lbl_1 = reinterpret_cast<LabelItem *>(item1);
    lbl_2 = reinterpret_cast<LabelItem *>(item2);

    if ((lbl_1 == 0) || lbl_2 == 0)
        return 0;

    if (data == 0)
        ret = lbl_1->LabelStr.Cmp(lbl_2->LabelStr);
    else
        ret = lbl_2->LabelStr.Cmp(lbl_1->LabelStr);

    return ret;
}


wxArrayInt *LabelListCtrl::GetLabelUsers(const LabelIndex index)
{
    return GetData(index)->LabelUsers;
}




LabelItem *LabelListCtrl::GetData(LabelIndex index)
{
    return reinterpret_cast<LabelItem *>(GetItemData(index));
}




void LabelListCtrl::DeleteLabelData(LabelItem *label)
{
    if (label)
    {
        if (label->LabelUsers)
        {
            label->LabelUsers->Clear();
            delete label->LabelUsers;
            label->LabelUsers = 0;
        }
        delete label;
        label = 0;
    }
}
