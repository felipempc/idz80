/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 *
 * This module shows/controls list labels
 **************************************************************/

#include <wx/menu.h>
#include "labelslist.h"
#include "EditLabelDlg.h"


/*
 *      Label List Control Contructor/Destructor
 */
LabelListCtrl::LabelListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos,const wxSize& size)
                    : wxListCtrl(parent,id,pos,size,wxLC_REPORT | wxLC_SINGLE_SEL)
{
    m_log=0;
    m_item_selected=-1;

    InsertColumn(0,_("Address"));
    InsertColumn(1,_("Label"));
    SetColumnWidth(0,wxLIST_AUTOSIZE_USEHEADER);
    SetColumnWidth(1,100);

    Connect(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,wxListEventHandler(LabelListCtrl::OnMouseRightDown));
    Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,wxListEventHandler(LabelListCtrl::OnMouseDblLeft));
    Connect(idMENU_POPUP_ADD,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&LabelListCtrl::OnMenuPopUpAdd);
    Connect(idMENU_POPUP_EDIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&LabelListCtrl::OnMenuPopUpEdit);
    Connect(idMENU_POPUP_DEL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&LabelListCtrl::OnMenuPopUpDel);
    Connect(wxEVT_COMMAND_LIST_COL_CLICK,wxListEventHandler(LabelListCtrl::OnColumnClick));

}

LabelListCtrl::~LabelListCtrl()
{
    Clear();
}


void LabelListCtrl::Clear()
{
    int i;
    LabelItem *lbl=0;
    for (i=0;i<GetItemCount();i++)
    {
        lbl=(LabelItem *)GetItemData(i);
        if (!(lbl==0))
        {
            lbl->LabelUsers->Clear();
            lbl->Address=0;
            delete lbl->LabelUsers;
            delete lbl;
        }
    }
    DeleteAllItems();
}


/*
 *     Label List Control Implementation
 */

int LabelListCtrl::AddLabel(uint addr, const wxString name, int dasmitem)
{
    wxString buf;
    wxListItem li;
    LabelItem *lbl;
    long itemfound,p;

    lbl = 0;
    p = -1;
    buf.Printf(_T("%X"),addr);
    itemfound = FindItem(0, buf);
    if (itemfound < 0)    // item not found
    {
		lbl = new LabelItem;
		lbl->Address = addr;
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
        p = InsertItem(li);
        SetItem(p, 1, name);
        itemfound = p;
    }
    else
    {
        lbl = (LabelItem *)GetItemData(itemfound);
        if (!(lbl == 0))
            if (lbl->LabelUsers->Index(dasmitem) == wxNOT_FOUND)
                lbl->LabelUsers->Add(dasmitem);
    }
    return itemfound;
}



int LabelListCtrl::AddLabel(uint addr, const wxString name, wxArrayInt &labelusers)
{
    wxString	buf;
    wxListItem	li;
    LabelItem	*lbl;
    long		itemfound, p;

    lbl = 0;
    p = -1;
    buf.Printf(_T("%X"),addr);
    itemfound = FindItem(0, buf);
    if (itemfound < 0)    // item not found
    {
		lbl = new LabelItem;
		lbl->Address = addr;
		if (labelusers.IsEmpty())
			lbl->LabelUsers = 0;
		else
		{
			lbl->LabelUsers = new wxArrayInt();
			for(p = 0; p < labelusers.GetCount(); p++)
                lbl->LabelUsers->Add(labelusers.Item(p));
			lbl->LabelStr = name;
		}
        li.SetText(buf);
        li.SetImage(-1);
        li.SetAlign(wxLIST_FORMAT_LEFT);
        li.SetData(lbl);
        p = InsertItem(li);
        SetItem(p, 1, name);
        itemfound = p;
    }
    else
    {	// Overwrite old list of users, if exists
        lbl = (LabelItem *)GetItemData(itemfound);
        if ((lbl != 0) && (!labelusers.IsEmpty()))
		{
            if (lbl->LabelUsers != 0)
			{
                lbl->LabelUsers->Clear();
                for(p = 0; p < labelusers.GetCount(); p++)
                    lbl->LabelUsers->Add(labelusers.Item(p));
			}
			else
			{
				lbl->LabelUsers = new wxArrayInt();
				for(p = 0; p < labelusers.GetCount(); p++)
                    lbl->LabelUsers->Add(labelusers.Item(p));
			}
		}

    }
    return itemfound;
}



void LabelListCtrl::DelLabel(uint addr)
{
    int i;
    wxListItem item;
    LabelItem *lbl;
    wxString str;

    str.Printf(_T("%X"),addr);
    i=FindItem(-1,str);
    if (i>=0)
    {
        lbl=(LabelItem *)GetItemData(i);
        if (!(lbl==0))
        {
            lbl->LabelUsers->Clear();
            delete lbl->LabelUsers;
            delete lbl;
        }
        DeleteItem(i);
    }
}



void LabelListCtrl::SetLog(wxTextCtrl *_lg)
{
    m_log=_lg;

}


bool LabelListCtrl::GetLabel(uint addr, wxString& str)
{
    int i;
    wxListItem item;

    str.Printf(_T("%X"),addr);
    i = FindItem(-1,str);
    if (i >= 0)
    {
        item.m_itemId = i;
        item.m_col = 1;
        item.m_mask = wxLIST_MASK_TEXT;
        GetItem(item);
        str=item.m_text;
        return true;
    }
    return false;
}



wxString LabelListCtrl::GetLabel(uint idx)
{
    uint i;
    wxString str_ret;
    wxListItem item;

    i = GetCount();
    if (i != 0)
    {
        if (idx >= i)
            idx = i - 1;

        item.m_itemId = idx;
        item.m_col = 1;
        item.m_mask = wxLIST_MASK_TEXT;
        GetItem(item);
        str_ret = item.m_text;
    }
    return str_ret;
}


wxString LabelListCtrl::GetAddress(uint idx)
{
    uint i;
    wxString str_ret;
    wxListItem item;

    i = GetCount();
    if (i != 0)
    {
        if (idx >= i)
            idx = i - 1;

        item.m_itemId = idx;
        item.m_col = 0;
        item.m_mask = wxLIST_MASK_TEXT;
        GetItem(item);
        str_ret = item.m_text;
    }
    return str_ret;

}



void LabelListCtrl::OnMouseDblLeft(wxListEvent& event)
{
    wxCommandEvent evt;

    m_item_selected=event.GetIndex();

    if (m_item_selected<0)
        OnMenuPopUpAdd(evt);
    else
        OnMenuPopUpEdit(evt);
}


void LabelListCtrl::OnMouseRightDown(wxListEvent& event)
{
    wxMenu popup;

    m_item_selected = event.GetIndex();

    popup.Append(idMENU_POPUP_ADD,_("Add Label"));
    popup.Append(idMENU_POPUP_EDIT,_("Edit Label"));
    popup.AppendSeparator();
    popup.Append(idMENU_POPUP_DEL,_("Del Label"));
    PopupMenu(&popup);
}


void LabelListCtrl::OnMenuPopUpAdd(wxCommandEvent& event)
{
    EditLabelDlg adlab(this);
    LabelItem *lbl;
    int i;


    if (adlab.ShowModal() == wxID_OK)
    {
        i=AddLabel(adlab.GetAddress(),adlab.GetLabel());
        lbl=(LabelItem *)GetItemData(i);
        if (lbl!=0)
            lbl->LabelStr=adlab.GetLabel();
        SortAddress();
    }
}

void LabelListCtrl::OnMenuPopUpDel(wxCommandEvent& event)
{
    DeleteItem(m_item_selected);
    m_item_selected=-1;
}


void LabelListCtrl::OnMenuPopUpEdit(wxCommandEvent& event)
{
    EditLabelDlg adlab(this,true);
    LabelItem *lbl;
    wxString str;
    wxListItem item;


    lbl=(LabelItem *)GetItemData(m_item_selected);
    adlab.SetAddress(lbl->Address);

    item.m_itemId = m_item_selected;
    item.m_col = 1;
    item.m_mask = wxLIST_MASK_TEXT;
    GetItem(item);
    adlab.SetLabel(item.m_text);


    if (adlab.ShowModal() == wxID_OK)
    {
        EditLabel(m_item_selected,adlab.GetLabel());
        if (lbl!=0)
            lbl->LabelStr=adlab.GetLabel();
    }
}


bool LabelListCtrl::IsEmpty()
{
    return (GetItemCount()<1);
}

int LabelListCtrl::GetCount()
{
    return GetItemCount();
}

void LabelListCtrl::EditLabel(uint listitem,wxString strlabel)
{
    wxListItem item;

    item.m_itemId = listitem;
    item.m_col = 1;
    item.m_mask = wxLIST_MASK_TEXT;
    item.m_text=strlabel;
    SetItem(item);
    SortAddress();

}


LabelItem *LabelListCtrl::GetLabelItem(const int index)
{
    LabelItem *lbl;
    lbl=(LabelItem *)GetItemData(index);
    return lbl;
}




void LabelListCtrl::SortAddress(bool crescent)
{
    if (crescent)
        SortItems(CompareAddress,0);
    else
        SortItems(CompareAddress,1);
}


void LabelListCtrl::SortLabelStr(bool crescent)
{
    if (crescent)
        SortItems(CompareLabelStr,0);
    else
        SortItems(CompareLabelStr,1);
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

    lbl_1 = (LabelItem *)item1;
    lbl_2 = (LabelItem *)item2;

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

    lbl_1 = (LabelItem *)item1;
    lbl_2 = (LabelItem *)item2;

    if ((lbl_1 == 0) || lbl_2 == 0)
        return 0;

    if (data == 0)
        ret = lbl_1->LabelStr.Cmp(lbl_2->LabelStr);
    else
        ret = lbl_2->LabelStr.Cmp(lbl_1->LabelStr);

    return ret;
}


wxArrayInt *LabelListCtrl::GetLabelUsers(const int index)
{
    return GetLabelItem(index)->LabelUsers;
}




