/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Dialog for edit labels on list labels
 **************************************************************/


#ifndef EDITLABELDLG_H
#define EDITLABELDLG_H


#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>

#include "IDZ80Base.h"


class EditLabelDlg: public wxDialog
{
	public:

		EditLabelDlg(wxWindow* parent,bool editmode=false,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);

		wxButton* B_OK;
		wxTextCtrl* TC_Label;
		wxStaticText* StaticText2;
		wxPanel* Panel1;
		wxStaticText* StaticText1;
		wxTextCtrl* TC_Address;
		wxButton* B_Cancel;


        uint GetAddress();
        wxString GetLabel();
        void SetAddress(uint addr);
        void SetLabel(wxString lbl);

	protected:

		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_TEXTCTRL2;
		static const long ID_PANEL1;


	private:

		void OnAddressTextUpdate(wxCommandEvent& event);

        volatile int base;

};

bool IsHexadecimal(wxString str);

#endif
