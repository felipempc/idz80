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

	protected:
		static const long ID_TXTCTRL_START;
		static const long ID_TXTCTRL_EXECUTION;
		static const long ID_TXTCTRL_END;


	private:
		wxPanel 	*m_main_panel;
		wxRadioBox	*m_radio_filetype_box;

        uint		m_start_address,
					m_exec_address,
					m_end_address;
		FileType	m_filetype;
		bool		m_cartridge;
		bool		m_changed;

		wxButton	*m_reset_button;
		
        RawData		*m_program;

		wxTextCtrl	*m_txtctrl_exec_address;
		wxTextCtrl	*m_txtctrl_end_address;
		wxTextCtrl	*m_txtctrl_start_address;


		void SetData();
		void UpdateFormAddress();
		void ResetOriginalAddresses();

        void OnRadioBoxSelect(wxCommandEvent &event);
		void OnOKPressed(wxCommandEvent &event);
		void OnResetPressed(wxCommandEvent &event);
		void OnCancelPressed(wxCommandEvent &event);
		void OnAddressFieldEnterPress(wxCommandEvent &event);
		void OnKeypressCheckAddress(wxCommandEvent &event);
		void OnTextCtrlLostFocus(wxFocusEvent& event);

		void BuildDialog();
		void SetupRadioTypeChooser(wxPanel *panel);
		void SetupAddressBox(wxPanel *panel);
		void SetupButtons(wxPanel *panel);
        void CheckAddressSanity();
		bool CheckHexadecimal(wxString &str_number, long &convert);
		bool CheckIfZ80Addressable(long &number);
};

#endif
