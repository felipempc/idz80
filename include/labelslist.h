/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Shows and controls list labels
 **************************************************************/


#ifndef LABELSLIST_H
#define LABELSLIST_H

#include <wx/Panel.h>
#include <wx/String.h>
#include <wx/ListCtrl.h>
#include <wx/Sizer.h>
#include <wx/msgdlg.h>

#include "disassembled_container.h"
#include "debug_logbase.h"
#include "labelitem.h"


// DEBUG DEFINITION
#define IDZ80_DEBUG_LBL_LIST

enum
{
    idMENU_POPUP_ADD = 100,
    idMENU_POPUP_EDIT,
    idMENU_POPUP_DEL
};

typedef enum eTypeLabelList
{
    VAR_LIST,
    PRG_LIST,
    IO_LIST,
    CONST_LIST
} TypeLabelList;

typedef int LabelIndex;


class LabelListCtrl : public wxListCtrl, public DebugLogBase
{
    public:
        LabelListCtrl(wxWindow* t_parent, const TypeLabelList t_label_type, const wxString &t_default_name, DebugLogWindow *t_logparent);
        ~LabelListCtrl();

        int addLabel(const AbsoluteAddress t_address, const wxString &t_name, DisassembledIndex t_dasmitem = NO_DASM_ITEM);
        int addLabel(const AbsoluteAddress t_address, const wxString &t_name, const IndexVector &t_labelusers);
        bool delLabel(const AbsoluteAddress t_address);
        void delLabelUser(const AbsoluteAddress t_address, const DisassembledIndex t_dasmitem);
        void editLabel(const LabelIndex t_listitem, const wxString &t_strlabel);
		bool editLabelDialog(const AbsoluteAddress t_address);
        int getLabel(const AbsoluteAddress t_address, const wxString& str);
        LabelIndex getLabelIndex(const AbsoluteAddress t_address);
        wxString getLabel(const LabelIndex t_index);
        wxString getAddress(const LabelIndex t_index);
        LabelItem *getData(const LabelIndex t_index);
        LabelItem *getDatabyAddress(const AbsoluteAddress t_address);
        IntArray *getLabelUsers(const LabelIndex t_index);
        TypeLabelList getTypeList();

        void getLabelsBetweenRangeAddress(const AbsoluteAddress t_first_address, const AbsoluteAddress t_last_address, AddressVector *t_address_list);
        uint getCount();
        bool isEmpty();
        void clear();

        // Tools
        bool findDataIn(const int t_data, const IndexVector *t_data_list);
        int findDataIndex(const int t_data, const IndexVector *t_data_list);

        void sortAddress(const bool t_crescent = true);
        // TODO: improve sorting Strings
        void sortLabelStr(const bool t_crescent = true);


    protected:
    private:
        static const DisassembledIndex NO_DASM_ITEM = 0xFFFFFFFF;
        volatile LabelIndex m_selected_item;
        volatile LabelIndex m_label_index;
        wxString m_default_label_name;
        wxWindow *m_main_window;
        TypeLabelList m_type_label_list;


        LabelItem *findByAddress(const AbsoluteAddress t_address, const LabelIndex t_label_index);
        wxString createDefaultName(const AbsoluteAddress t_address);
        void deleteLabelData(LabelItem *t_label);

        void onMouseRightDown(wxListEvent& t_event);
        void onMouseDblLeft(wxListEvent& t_event);
        void onMenuPopUpAdd(wxCommandEvent& t_event);
        void onMenuPopUpDel(wxCommandEvent& t_event);
        void onMenuPopUpEdit(wxCommandEvent& t_event);
        void onColumnClick(wxListEvent& t_event);
};


int wxCALLBACK CompareAddress(wxIntPtr t_item1, wxIntPtr t_item2, wxIntPtr t_data);
int wxCALLBACK CompareLabelStr(wxIntPtr t_item1, wxIntPtr t_item2, wxIntPtr t_data);
#endif // LABELSLIST_H
