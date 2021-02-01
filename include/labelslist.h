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
#include <wx/dynarray.h>

#include "idz80_base.h"
#include "disassembled_container.h"
#include "logbase.h"
#include "labelitem.h"

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



class LabelListCtrl : public wxListCtrl, public LogBase
{
    public:
        LabelListCtrl(wxWindow* parent, TypeLabelList label_type, const wxString default_name, LogWindow *logparent);
        ~LabelListCtrl();

        int AddLabel(AbsoluteAddress addr, wxString name, DisassembledIndex dasmitem = NO_DASM_ITEM);
        int AddLabel(AbsoluteAddress addr, wxString name, wxArrayInt &labelusers);
        bool DelLabel(AbsoluteAddress addr);
        void DelLabelUser(AbsoluteAddress addr, DisassembledIndex dasmitem);
        void EditLabel(LabelIndex listitem, wxString strlabel);
		bool EditLabelDialog(AbsoluteAddress addr);
        int GetLabel(AbsoluteAddress addr, wxString& str);
        int GetLabelIndex(AbsoluteAddress addr);
        wxString GetLabel(LabelIndex idx);
        wxString GetAddress(LabelIndex idx);
        LabelItem *GetData(LabelIndex index);
        LabelItem *GetDatabyAddress(AbsoluteAddress addr);
        wxArrayInt *GetLabelUsers(const LabelIndex index);
        TypeLabelList GetTypeList();

        void GetLabelsBetweenRangeAddress(AbsoluteAddress first_address, AbsoluteAddress last_address, wxArrayInt *address_list);
        uint GetCount();
        bool IsEmpty();
        void Clear();


        void SortAddress(bool crescent = true);
        // TODO: improve sorting Strings
        void SortLabelStr(bool crescent = true);


    protected:
    private:
        static const DisassembledIndex NO_DASM_ITEM = 0xFFFFFFFF;
        volatile int selected_item_;
        wxString default_label_name_;
        volatile int label_index_;
        wxWindow *main_window_;
        TypeLabelList type_label_list_;


        LabelItem *FindByAddress(AbsoluteAddress addr, LabelIndex &label_index);
        wxString CreateDefaultName(const AbsoluteAddress addr);
        void DeleteLabelData(LabelItem *label);

        void OnMouseRightDown(wxListEvent& event);
        void OnMouseDblLeft(wxListEvent& event);
        void OnMenuPopUpAdd(wxCommandEvent& event);
        void OnMenuPopUpDel(wxCommandEvent& event);
        void OnMenuPopUpEdit(wxCommandEvent& event);
        void OnColumnClick(wxListEvent& event);
};


int wxCALLBACK CompareAddress(long long item1, long long item2, long long data);
int wxCALLBACK CompareLabelStr(long long item1, long long item2, long long data);
#endif // LABELSLIST_H
