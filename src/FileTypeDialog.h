/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/




#ifndef FILETYPEDIALOG_H
#define FILETYPEDIALOG_H

//(*Headers(FileTypeDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/dialog.h>
//*)

#include <wx/checkbox.h>
#include "rawdata.h"

class FileTypeDialog: public wxDialog
{
	public:

		FileTypeDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~FileTypeDialog();

		//(*Declarations(FileTypeDialog)
		wxTextCtrl		*Txt_ExecAddress;
		wxStaticText	*StaticText2;
		wxPanel			*Panel1;
		wxPanel			*Panel2;
		wxPanel			*Panel3;
		wxPanel			*Panel4;
		wxStaticText	*StaticText1;
		wxStaticText	*StaticText3;
		wxTextCtrl		*Txt_EndAddress;
		wxRadioBox		*RadioBox1;
		wxTextCtrl		*Txt_StartAddress;
		wxCheckBox		*cb_autodisassemble;
		wxCheckBox		*cb_autolabel;
		//*)

		void SetData(RawData& program);
		uint GetStartAddress();
		uint GetExecAddress();
		uint GetEndAddress();

	protected:

		//(*Identifiers(FileTypeDialog)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_PANEL2;
		static const long ID_RADIOBOX1;
		static const long ID_PANEL1;
		static const long ID_PANEL3;
		static const long ID_PANEL4;
		static const long ID_CHKBOX1;
		static const long ID_CHKBOX2;

		//*)

	private:

		//(*Handlers(FileTypeDialog)
		//*)
        uint StartAddress, ExecAddress,EndAddress;
        RawData *m_program;
        void SyncAddress();

        void OnRadioBoxSelect(wxCommandEvent &event);

		DECLARE_EVENT_TABLE()
};

#endif
