/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Shows and controls list labels
 **************************************************************/


#include <wx/menu.h>
#include "labelslist.h"
//#include "editlabel_dialog.h"



const DisassembledIndex LabelListCtrl::NO_DASM_ITEM;

/*
 *      Label List Control Contructor/Destructor
 */

LabelListCtrl::LabelListCtrl(wxWindow *t_parent, const TypeLabelList t_label_type, const wxString &t_default_name, DebugLogWindow *t_logparent)
                    : wxListCtrl(t_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL)
{
    m_selected_item = -1;
    m_label_index = -1;
    m_main_window = t_parent;
    m_type_label_list = t_label_type;

    InsertColumn(0, "Address");
    InsertColumn(1, "Label");
    SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
    SetColumnWidth(1, 100);

    if (t_default_name.IsEmpty())
        m_default_label_name = "LABEL";
    else
        m_default_label_name = t_default_name;

    Bind(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, &LabelListCtrl::onMouseRightDown, this);
    Bind(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, &LabelListCtrl::onMouseDblLeft, this);
    Bind(wxEVT_MENU, &LabelListCtrl::onMenuPopUpAdd, this, idMENU_POPUP_ADD);
    Bind(wxEVT_MENU, &LabelListCtrl::onMenuPopUpEdit, this, idMENU_POPUP_EDIT);
    Bind(wxEVT_MENU, &LabelListCtrl::onMenuPopUpDel, this, idMENU_POPUP_DEL);
    Bind(wxEVT_COMMAND_LIST_COL_CLICK, &LabelListCtrl::onColumnClick, this);

    SetTextLog(t_logparent);
    ModuleName = "Label";
}



LabelListCtrl::~LabelListCtrl()
{
    clear();
}



/// @brief Add a label to the list (wxListCtrl) and add the user of it.
/// @param t_address of that the labels represents
/// @param t_name of the label
/// @param t_dasmitem user index (DisassembledItem) of this label
/// @return the index of the label in the list (wxListCtrl)
int LabelListCtrl::addLabel(const AbsoluteAddress t_address, const wxString &t_name, DisassembledIndex t_dasmitem)
{
    wxString buf;
    wxListItem li;
    LabelItem *lbl;
    LabelIndex itemfound, index;

    lbl = 0;
    index = -1;
    buf.Printf("%X", t_address);
    itemfound = static_cast<LabelIndex>(FindItem(0, buf));
    if (itemfound < 0)
    {
		lbl = new LabelItem;
		lbl->Address = t_address;
		if (t_name.IsEmpty())
            lbl->LabelStr = createDefaultName(t_address);
        else
		    lbl->LabelStr = t_name;

		if (t_dasmitem >= 0)
		{
			lbl->LabelUsers = new IndexVector;
			lbl->LabelUsers->push_back(t_dasmitem);
		}
		else
			lbl->LabelUsers = 0;

        li.SetText(buf);
        li.SetImage(-1);
        li.SetAlign(wxLIST_FORMAT_LEFT);
        li.SetData(lbl);
        li.SetId(++m_label_index);
        index = static_cast<LabelIndex>(InsertItem(li));
        SetItem(index, 1, lbl->LabelStr);
        itemfound = index;
    }
    else
    {
        lbl = getData(itemfound);
        if (lbl && lbl->LabelUsers)
            if (!findDataIn(t_dasmitem, lbl->LabelUsers))
                lbl->LabelUsers->push_back(t_dasmitem);
    }
    return itemfound;
}



/// @brief Add a label to the list. Test if already exists and if it does, overwrite label users vector.
/// @param t_address of that the labels represents
/// @param t_name of the label
/// @param t_labelusers List of DisassembledItems indexes that use it.
/// @return itemfound
int LabelListCtrl::addLabel(const AbsoluteAddress t_address, const wxString &t_name, const IndexVector &t_labelusers)
{
    wxString	buf;
    wxListItem	li;
    LabelItem	*lbl;
    LabelIndex  itemfound, index;
    uint        counter;

    lbl = 0;
    index = -1;
    buf.Printf("%X", t_address);
    itemfound = static_cast<LabelIndex>(FindItem(0, buf));
    if (itemfound < 0)
    {
		lbl = new LabelItem;
		lbl->Address = t_address;
        if (t_name.IsEmpty())
            lbl->LabelStr = createDefaultName(t_address);
        else
            lbl->LabelStr = t_name;

		if (t_labelusers.empty())
			lbl->LabelUsers = 0;
		else
		{
			lbl->LabelUsers = new IndexVector;
            for (int labelitem : t_labelusers)
			    lbl->LabelUsers->push_back(labelitem);
		}
        li.SetText(buf);
        li.SetImage(-1);
        li.SetAlign(wxLIST_FORMAT_LEFT);
        li.SetData(lbl);
        li.SetId(++m_label_index);
        index = static_cast<LabelIndex>(InsertItem(li));
        SetItem(index, 1, lbl->LabelStr);
        itemfound = index;
    }
    else
    {	// Overwrite old list of users, if exists
        lbl = getData(itemfound);
        if ((lbl) && (!t_labelusers.empty()))
		{
            if (lbl->LabelUsers)
                lbl->LabelUsers->clear();
            else
                lbl->LabelUsers = new IndexVector;
            
            for(int labelitem : t_labelusers)
                lbl->LabelUsers->push_back(labelitem);
		}

    }
    return itemfound;
}



/// @brief Delete the label that represents t_address
/// @param t_address 
/// @return true if succeed
bool LabelListCtrl::delLabel(const AbsoluteAddress t_address)
{
    uint		label;
    LabelItem	*lbl;
    bool		ret = false;

    lbl = findByAddress(t_address, label);

    if (lbl)
    {
        deleteLabelData(lbl);
        ret = true;
        DeleteItem(label);
        --m_label_index;
    }
    return ret;
}




/// @brief Remove the user t_dasmitem of the label (represented by t_address) from the label users vector
/// @param t_address represented by label 
/// @param t_dasmitem user index (DisassembledItem)
void LabelListCtrl::delLabelUser(AbsoluteAddress t_address, DisassembledIndex t_dasmitem)
{
    int         i;
    uint        label;
    LabelItem	*lbl;


    lbl = findByAddress(t_address, label);

    if (lbl && lbl->LabelUsers)
    {
        i = findDataIndex(t_dasmitem, lbl->LabelUsers);
        if (i >= 0)
            lbl->LabelUsers->erase(lbl->LabelUsers->begin() + i);
        if (lbl->LabelUsers->empty())
        {
            delete lbl->LabelUsers;
            lbl->LabelUsers = 0;
            delete lbl;
            DeleteItem(label);
            --m_label_index;
#ifdef IDZ80_DEBUG_LBL_LIST
            LogIt(wxString::Format("Removed item by userlabel = %d", label));
#endif
        }
    }
}



/// @brief Rename the label at t_listitem index to t_strlabel
/// @param t_listitem index of the label in wxListCtrl
/// @param t_strlabel New name of the label
void LabelListCtrl::editLabel(const LabelIndex t_listitem, const wxString &t_strlabel)
{
    wxListItem item;

    item.m_itemId = t_listitem;
    item.m_col = 1;
    item.m_mask = wxLIST_MASK_TEXT;
    item.m_text = t_strlabel;
    SetItem(item);
    //SortAddress();
}



/// @brief To be defined
/// @param t_address 
/// @return 
bool LabelListCtrl::editLabelDialog(const AbsoluteAddress t_address)
{
//   EditLabelDlg adlab(this,true);
    LabelItem *lbl;
    wxString str;
	int idx;
	bool ret = false;

/*
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
	} */
	return ret;
}



//TODO: MODIFY or Remove....
int LabelListCtrl::getLabel(const AbsoluteAddress t_address, const wxString &str)
{
    /*
    int i;
    wxListItem item;

    str =  .Printf("%X", t_address);
    i = FindItem(-1, str);
    if (i >= 0)
    {
        item.m_itemId = i;
        item.m_col = 1;
        item.m_mask = wxLIST_MASK_TEXT;
        GetItem(item);
        str = item.m_text;
    }
*/
    return 0;   //i;
}



/// @brief Return the label index that represents t_address
/// @param t_address 
/// @return index of label in wxListCtrl
LabelIndex LabelListCtrl::getLabelIndex(const AbsoluteAddress t_address)
{
    return (FindItem(-1, wxString::Format("%X", t_address)));
}



/// @brief Get the label string from wxListCtrl at position t_index
/// @param t_index of the label
/// @return label
wxString LabelListCtrl::getLabel(const LabelIndex t_index)
{
    uint i;
    wxString str_ret;
    LabelItem *lbl;

    i = getCount();
    if (i && (t_index < i))
    {
        lbl = getData(t_index);

        if (lbl)
            str_ret = lbl->LabelStr;
#ifdef IDZ80_DEBUG_LBL_LIST
        LogIt(wxString::Format("Index %d  <%s>", t_index, str_ret));
#endif
    }

    return str_ret;
}



/// @brief Return the address, as hexadecimal string, from wxListCtrl at position t_index
/// @param t_index of the label
/// @return label
wxString LabelListCtrl::getAddress(const LabelIndex t_index)
{
    uint i;
    wxString str_ret;
    LabelItem *lbl;

    i = getCount();
    if (i && (t_index < i))
    {
        lbl = getData(t_index);
        if (lbl)
            str_ret = wxString::Format("%.4X", lbl->Address);
    }

    return str_ret;
}



/// @brief Get LabelItem* from wxListCtrl at position t_index
/// @param t_index position of the data
/// @return LabelItem*
LabelItem *LabelListCtrl::getData(const LabelIndex t_index)
{
    return reinterpret_cast<LabelItem *>(GetItemData(t_index));
}



LabelItem *LabelListCtrl::getDatabyAddress(const AbsoluteAddress t_address)
{
    LabelIndex li;

    return findByAddress(t_address, li);
}



/// @brief Get the label users vector from wxListCtrl at t_index
/// @param t_index 
/// @return 
IntArray *LabelListCtrl::getLabelUsers(const LabelIndex t_index)
{
    return getData(t_index)->LabelUsers;
}




TypeLabelList LabelListCtrl::getTypeList()
{
    return m_type_label_list;
}



/// @brief Get addresses between t_first_address and t_last_address and add to t_address_list
/// @param t_first_address First address
/// @param t_last_address Last address
/// @param address_list list to be filled by addresses found
void LabelListCtrl::getLabelsBetweenRangeAddress(const AbsoluteAddress t_first_address, const AbsoluteAddress t_last_address, AddressVector *t_address_list)
{
    LabelItem   *lbl;
    LabelIndex  loopcount;

    if (t_address_list)
    {
        for(loopcount = 0; loopcount < getCount(); loopcount++)
        {
            lbl = getData(loopcount);
            if (lbl && (lbl->Address >= t_first_address) && (lbl->Address <= t_last_address))
                t_address_list->push_back(lbl->Address);
        }
    }
}



/// @brief Get how many elements in wxListCtrl
/// @return 
uint LabelListCtrl::getCount()
{
    return static_cast<uint>(GetItemCount());
}



/// @brief Checks if wxListCtrl is empty
/// @return true if it's empty
bool LabelListCtrl::isEmpty()
{
        return (GetItemCount() < 1);
}



/// @brief Return the LabelItem* from wxListCtrl based on t_address
/// @param t_address that LabelItem* represents
/// @param t_label_index HAVE NO IDEA WHY 
/// @return LabelItem*
LabelItem *LabelListCtrl::findByAddress(AbsoluteAddress t_address, const LabelIndex t_label_index)
{
    LabelIndex index;
    LabelItem *lbl;

    lbl = 0;

    index = getLabelIndex(t_address);
    if (index != wxNOT_FOUND)
    {
        lbl = getData(t_label_index);

        //label_index = t_label_index;
    }

    return lbl;
}



/// @brief Create a name to the label of t_address
/// @param t_address
/// @return a string with the name of the label
wxString LabelListCtrl::createDefaultName(const AbsoluteAddress t_address)
{
    return (wxString::Format("%s_%.4X", m_default_label_name, t_address));
}



/// @brief Delete t_label data
/// @param t_label pointer of a label data to be deleted
void LabelListCtrl::deleteLabelData(LabelItem *t_label)
{
    if (t_label)
    {
        if (t_label->LabelUsers)
        {
            t_label->LabelUsers->clear();
            delete t_label->LabelUsers;
            t_label->LabelUsers = 0;
        }
        delete t_label;
        t_label = 0;
    }
}



void LabelListCtrl::onMouseDblLeft(wxListEvent& event)
{
    wxCommandEvent evt;

    m_selected_item = event.GetIndex();

    if (m_selected_item < 0)
        onMenuPopUpAdd(evt);
    else
        onMenuPopUpEdit(evt);
}


void LabelListCtrl::onMouseRightDown(wxListEvent& event)
{
    wxMenu popup;
#ifdef IDZ80_DEBUG_LBL_LIST
    LabelItem *selected_label;
    uint i;
    wxString str;
#endif

    m_selected_item = event.GetIndex();

#ifdef IDZ80_DEBUG_LBL_LIST
    selected_label = getData(m_selected_item);
    if (selected_label->LabelUsers)
    {
        for(int users : *selected_label->LabelUsers)
            str << wxString::Format(" %x", users);

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



void LabelListCtrl::onMenuPopUpAdd(wxCommandEvent& event)
{
//    EditLabelDlg adlab(this);
    LabelItem *lbl;
    int i;

/*
    if (adlab.ShowModal() == wxID_OK)
    {
        i = AddLabel(adlab.GetAddress(), adlab.GetLabel());
        lbl = GetData(i);
        if (lbl)
            lbl->LabelStr = adlab.GetLabel();
        SortAddress();
    } */
}



void LabelListCtrl::onMenuPopUpDel(wxCommandEvent& event)
{
    deleteLabelData(getData(m_selected_item));
    DeleteItem(m_selected_item);
    --m_label_index;
    m_selected_item = -1;
    m_main_window->Refresh();
}


void LabelListCtrl::onMenuPopUpEdit(wxCommandEvent& event)
{
    LabelItem *lbl;

    lbl = getData(m_selected_item);
	editLabelDialog(lbl->Address);
}



void LabelListCtrl::clear()
{
    LabelIndex i;
    for (i = 0; i < static_cast<LabelIndex>(GetItemCount()); ++i)
        deleteLabelData(getData(i));
    m_label_index = -1;
    DeleteAllItems();
}



/// @brief Search for t_data (int) in t_data_list (vector(int)).
/// @param t_data item to be found
/// @param t_data_list list to search in
/// @return true if found, false otherwise
bool LabelListCtrl::findDataIn(const int t_data, const IndexVector *t_data_list)
{
    bool item_found = false;

    for (int data : *t_data_list)
        if (t_data == data) {
            item_found = true;
            break;
        }
    return item_found;
}



/// @brief Search for t_data(int) in t_data_list(vector(int)) and return the index of it.
/// @param t_data item to be searched
/// @param t_data_list list to search in
/// @return index of t_data in t_data_list if found, -1 otherwise
int LabelListCtrl::findDataIndex(const int t_data, const IndexVector *t_data_list)
{
    LabelIndex  ret_index = -1,
                index = -1;

    for (int data : *t_data_list) {
        ++index;
        if (t_data == data) {
            ret_index = index;
            break;
        }
    }

    return ret_index;
}



void LabelListCtrl::sortAddress(const bool t_crescent)
{
    if (!IsEmpty())
    {
        if (t_crescent)
            SortItems(CompareAddress, 0);
        else
            SortItems(CompareAddress, 1);
    }
}



void LabelListCtrl::sortLabelStr(const bool t_crescent)
{
    if (!IsEmpty())
    {
        if (t_crescent)
            SortItems(CompareLabelStr, 0);
        else
            SortItems(CompareLabelStr, 1);
    }
}



void LabelListCtrl::onColumnClick(wxListEvent& t_event)
{
    static bool sort_direction = true;

    if (t_event.GetColumn() == 0)
    {
        if (sort_direction)
            sort_direction = false;
        else
            sort_direction = true;

        sortAddress(sort_direction);
    }

    if (t_event.GetColumn() == 1)
    {
        if (sort_direction)
            sort_direction = false;
        else
            sort_direction = true;

        sortLabelStr(sort_direction);
    }
}



/*
 *     Call back wxListCtrl implementation
 */

int wxCALLBACK CompareAddress(wxIntPtr t_item1, wxIntPtr t_item2, wxIntPtr t_data)
{
    LabelItem *lbl_1, *lbl_2;

    lbl_1 = reinterpret_cast<LabelItem *>(t_item1);
    lbl_2 = reinterpret_cast<LabelItem *>(t_item2);

    if ((lbl_1 == 0) || lbl_2 == 0)
        return 0;

    if (t_data == 0)
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


int wxCALLBACK CompareLabelStr(wxIntPtr t_item1, wxIntPtr t_item2, wxIntPtr t_data)
{
    LabelItem *lbl_1, *lbl_2;
    int ret = 0;

    lbl_1 = reinterpret_cast<LabelItem *>(t_item1);
    lbl_2 = reinterpret_cast<LabelItem *>(t_item2);

    if ((lbl_1 == 0) || lbl_2 == 0)
        return 0;

    if (t_data == 0)
        ret = lbl_1->LabelStr.Cmp(lbl_2->LabelStr);
    else
        ret = lbl_2->LabelStr.Cmp(lbl_1->LabelStr);

    return ret;
}
