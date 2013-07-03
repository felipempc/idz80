/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   2009-11-09
 * License:   GPL
 *
 * This module shows/controls list labels
 **************************************************************/


#ifndef LABELSLIST_H
#define LABELSLIST_H

#include <wx/Panel.h>
#include <wx/String.h>
#include <wx/ListCtrl.h>
#include <wx/Sizer.h>
#include <wx/msgdlg.h>
#include <wx/dynarray.h>

#include "IDZ80Base.h"
#include "logbase.h"

enum
{
    idMENU_POPUP_ADD = 100,
    idMENU_POPUP_EDIT,
    idMENU_POPUP_DEL
};


struct st_label
{
    uint Address;
    wxArrayInt *LabelUsers;
    wxString LabelStr;
};

typedef struct st_label LabelItem;



class LabelListCtrl : public wxListCtrl, public LogBase
{
    public:
        LabelListCtrl(wxWindow* parent, const wxString default_name, LogWindow *logparent);
        ~LabelListCtrl();

        int AddLabel(ProgramAddress addr, wxString name, DisassembledIndex dasmitem = NO_DASM_ITEM);
        int AddLabel(ProgramAddress addr, wxString name, wxArrayInt &labelusers);
        bool DelLabel(ProgramAddress addr);
        void DelLabelUser(ProgramAddress addr, DisassembledIndex dasmitem);
        void EditLabel(LabelIndex listitem, wxString strlabel);
		bool EditLabelDialog(ProgramAddress addr);
        int GetLabel(ProgramAddress addr, wxString& str);
        int GetLabelIndex(ProgramAddress addr);
        wxString GetLabel(LabelIndex idx);
        wxString GetAddress(LabelIndex idx);
        LabelItem *GetData(LabelIndex index);
        wxArrayInt *GetLabelUsers(const LabelIndex index);

        void GetLabelsBetweenRangeAddress(ProgramAddress first_address, ProgramAddress last_address, wxArrayInt *address_list);
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

        LabelItem *FindByAddress(ProgramAddress addr, LabelIndex &label_index);
        wxString CreateDefaultName(const ProgramAddress addr);
        void DeleteLabelData(LabelItem *label);

        void OnMouseRightDown(wxListEvent& event);
        void OnMouseDblLeft(wxListEvent& event);
        void OnMenuPopUpAdd(wxCommandEvent& event);
        void OnMenuPopUpDel(wxCommandEvent& event);
        void OnMenuPopUpEdit(wxCommandEvent& event);
        void OnColumnClick(wxListEvent& event);
};


int wxCALLBACK CompareAddress(long item1, long item2, long data);
int wxCALLBACK CompareLabelStr(long item1, long item2, long data);
#endif // LABELSLIST_H
