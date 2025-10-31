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

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/dialog.h>


#include "main_dialog_base.h"
#include "rawdata.h"


class FileSettingsDialog: public wxDialog, public LogBase
{
	public:

		FileSettingsDialog(IDZ80MainBase *parent);
		virtual ~FileSettingsDialog();


		wxTextCtrl		*Txt_ExecAddress;
		wxTextCtrl		*Txt_EndAddress;
		wxTextCtrl		*Txt_StartAddress;


		void SetData();
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
		static const long ID_RADIOBOX1;
		static const long ID_PANEL1;
		static const long ID_PANEL2;

	private:
		wxPanel *main_panel_;
		wxRadioBox      *RadioFileTypeBox;

        uint	StartAddress,
				ExecAddress,
				EndAddress;
		FileType m_filetype;
		bool m_cartridge;
		
        RawData *m_program;

		void UpdateFormAddress();
		void ResetOriginalAddresses();

        void OnRadioBoxSelect(wxCommandEvent &event);
		void OnOKPressed(wxCommandEvent &event);
		void OnCancelPressed(wxCommandEvent &event);
        void OnAddressFieldsKeypress(wxCommandEvent &event); // OBSOLETE
		void OnAddressFieldEnterPress(wxCommandEvent &event);
		void OnKeypressStartAddress(wxCommandEvent &event);
		void OnKeypressExecutionAddress(wxCommandEvent &event);
		void OnKeypressEndAddress(wxCommandEvent &event);

		void BuildDialog();
		void SetupRadioTypeChooser(wxPanel *panel);
		void SetupAddressBox(wxPanel *panel);
		void SetupOkCancelButtons(wxPanel *panel);
		void ChangeStartAddress();
		void ChangeExecutionAddress();
		void ChangeEndAddress();
        void CheckAddressSanity();
		bool CheckHexadecimal(wxString &str_number, long &convert);
		bool CheckIfZ80Addressable(long &number);
};

#endif
