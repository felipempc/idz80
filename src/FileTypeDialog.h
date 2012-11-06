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

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/dialog.h>


#include <wx/checkbox.h>
#include "rawdata.h"

class FileTypeDialog: public wxDialog
{
	public:

		FileTypeDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~FileTypeDialog();


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
		wxCheckBox		*cb_simulateexecution;
		wxCheckBox		*cb_cartridge;


		void SetData(RawData& program);
		uint GetStartAddress();
		uint GetExecAddress();
		uint GetEndAddress();

	protected:

		static const long ID_TXT_START;
		static const long ID_TXTCTRL_START;
		static const long ID_TXT_EXECUTION;
		static const long ID_TXTCTRL_EXECUTION;
		static const long ID_TXT_END;
		static const long ID_TXTCTRL_END;
		static const long ID_PANEL2;
		static const long ID_RADIOBOX1;
		static const long ID_PANEL1;
		static const long ID_PANEL3;
		static const long ID_PANEL4;
		static const long ID_CHKBOX1;
		static const long ID_CHKBOX2;
		static const long ID_CHKBOX_CARTRIDGE;

	private:

        uint	StartAddress,
				ExecAddress,
				EndAddress;
        RawData *m_program;

		void SyncAddress();
        void OnRadioBoxSelect(wxCommandEvent &event);
        void OnChkBoxCartridge(wxCommandEvent &event);

		DECLARE_EVENT_TABLE()
};

#endif
