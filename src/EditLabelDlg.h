/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 *
 * This module Edit Labels on list labels
 **************************************************************/


#ifndef EDITLABELDLG_H
#define EDITLABELDLG_H

//(*Headers(EditLabelDlg)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)


#include "IDZ80Base.h"

class EditLabelDlg: public wxDialog
{
	public:

		EditLabelDlg(wxWindow* parent,bool editmode=false,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~EditLabelDlg();

		//(*Declarations(EditLabelDlg)
		wxButton* B_OK;
		wxTextCtrl* TC_Label;
		wxStaticText* StaticText2;
		wxPanel* Panel1;
		wxStaticText* StaticText1;
		wxTextCtrl* TC_Address;
		wxButton* B_Cancel;
		//*)

        uint GetAddress();
        wxString GetLabel();
        void SetAddress(uint addr);
        void SetLabel(wxString lbl);

	protected:

		//(*Identifiers(EditLabelDlg)
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_TEXTCTRL2;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(EditLabelDlg)
		void OnAddressTextUpdate(wxCommandEvent& event);
		//*)

        int base;

		DECLARE_EVENT_TABLE()
};

bool IsHexadecimal(wxString str);

#endif
