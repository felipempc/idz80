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


#ifndef LABELSLIST_H
#define LABELSLIST_H

#include <wx/Panel.h>
#include <wx/String.h>
#include <wx/ListCtrl.h>
#include <wx/Sizer.h>
#include <wx/msgdlg.h>
#include <wx/dynarray.h>

#include "IDZ80Base.h"

#define NO_DASM_ITEM    -1


enum
{
    idMENU_POPUP_ADD=100,
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



class LabelListCtrl : public wxListCtrl
{
    public:
        LabelListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
                            const wxSize& size = wxDefaultSize);
        ~LabelListCtrl();

        void SetLog(wxTextCtrl *_lg);
        int AddLabel(uint addr, const wxString name, int dasmitem=NO_DASM_ITEM);
        int AddLabel(uint addr, const wxString name, wxArrayInt &labelusers);
        bool DelLabel(uint addr);
        void EditLabel(uint listitem,wxString strlabel);
		bool EditLabelDialog(uint addr);
        int GetLabel(uint addr, wxString& str);
        wxString GetLabel(uint idx);
        wxString GetAddress(uint idx);
        LabelItem *GetLabelItem(const int index);
        int GetCount();
        bool IsEmpty();
        void Clear();
        wxArrayInt *GetLabelUsers(const int index);

        void SortAddress(bool crescent = true);
        // TODO: improve sorting Strings
        void SortLabelStr(bool crescent = true);


    protected:
    private:
        wxTextCtrl *m_log;
        int m_item_selected;


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
