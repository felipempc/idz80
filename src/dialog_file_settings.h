/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   14-12-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Dialog for program file setting
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

		FileSettingsDialog(RawData *program);
		virtual ~FileSettingsDialog();


		wxTextCtrl		*Txt_ExecAddress;
		wxTextCtrl		*Txt_EndAddress;
		wxTextCtrl		*Txt_StartAddress;


		void SetData();
		uint GetStartAddress();
		uint GetExecAddress();
		uint GetEndAddress();
		bool WantsAutoDisassembly();
		bool WantsAutoLabel();
		bool WantsSimulateExecution();

	protected:

		static const long ID_TXT_START;
		static const long ID_TXTCTRL_START;
		static const long ID_TXT_EXECUTION;
		static const long ID_TXTCTRL_EXECUTION;
		static const long ID_TXT_END;
		static const long ID_TXTCTRL_END;
		static const long ID_RADIOBOX1;
		static const long ID_PANEL1;

	private:
        wxNotebook      *bookCtrl;
        wxPanel         *main_panel,
                        *yesno_panel;
        wxRadioBox      *RadioFileTypeBox;
        wxCheckBox      *AutoLabel_CheckBox,
                        *AutoDisassemble_CheckBox,
                        *SimulateExecution_CheckBox;

        uint	StartAddress,
				ExecAddress,
				EndAddress;
        RawData *m_program;

		void UpdateFormAddress();
        void OnRadioBoxSelect(wxCommandEvent &event);
        void OnSizeEvent(wxSizeEvent &event);
        void OnAddressFieldsKeypress(wxCommandEvent &event);

        void SetupProgramSettings(wxNotebook *book);
        void CreateAddressBox(wxStaticBoxSizer *boxsizer);
        void CheckAddressSanity();

        void SetupDAsmSettings(wxNotebook *book);
};

#endif
