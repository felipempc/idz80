/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-12-14
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/




#ifndef DIALOGFILESETTINGS_H
#define DIALOGFILESETTINGS_H

#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/dialog.h>

#include <wx/checkbox.h>
#include "rawdata.h"

class FileSettingsDialog: public wxDialog
{
	public:

		FileSettingsDialog();
		virtual ~FileSettingsDialog();


		wxTextCtrl		*Txt_ExecAddress;
		wxTextCtrl		*Txt_EndAddress;
		wxTextCtrl		*Txt_StartAddress;
		wxRadioBox		*RadioBox1;
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
        wxBookCtrlBase  *bookCtrl;
        wxPanel         *main_panel;

        uint	StartAddress,
				ExecAddress,
				EndAddress;
        RawData *m_program;

		void SyncAddress();
        void OnRadioBoxSelect(wxCommandEvent &event);
        void OnChkBoxCartridge(wxCommandEvent &event);

        wxPanel *SetupProgramSettings();

};

#endif
