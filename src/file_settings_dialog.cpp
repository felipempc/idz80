/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   14-12-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Dialog for program file setting
 **************************************************************/



#include "file_settings_dialog.h"


#include <wx/intl.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/msgdlg.h>


const long FileSettingsDialog::ID_TXT_START = wxNewId();
const long FileSettingsDialog::ID_TXTCTRL_START = wxNewId();
const long FileSettingsDialog::ID_TXT_EXECUTION = wxNewId();
const long FileSettingsDialog::ID_TXTCTRL_EXECUTION = wxNewId();
const long FileSettingsDialog::ID_TXT_END = wxNewId();
const long FileSettingsDialog::ID_TXTCTRL_END = wxNewId();
const long FileSettingsDialog::ID_RADIOBOX1 = wxNewId();
const long FileSettingsDialog::ID_PANEL1 = wxNewId();
const long FileSettingsDialog::ID_PANEL2 = wxNewId();



FileSettingsDialog::FileSettingsDialog(IDZ80MainBase *parent)
{
	Create(0, wxID_ANY, "Configuration", wxPoint(100, 100), wxSize(300, 300), wxDEFAULT_DIALOG_STYLE, "id");

    ModuleName = "FileSettings";
    SetTextLog(parent->GetTextLog());
    m_program = parent->Programs_->Current();

    BuildDialog();

	Bind(wxEVT_RADIOBOX, &FileSettingsDialog::OnRadioBoxSelect, this, ID_RADIOBOX1);
    Bind(wxEVT_BUTTON, &FileSettingsDialog::OnOKPressed, this, wxID_OK);
    Bind(wxEVT_BUTTON, &FileSettingsDialog::OnCancelPressed, this, wxID_CANCEL);
    Bind(wxEVT_BUTTON, &FileSettingsDialog::OnResetPressed, this, wxID_RESET);

    Bind(wxEVT_TEXT, &FileSettingsDialog::OnKeypressCheckAddress, this, ID_TXTCTRL_START);
    Bind(wxEVT_TEXT, &FileSettingsDialog::OnKeypressCheckAddress, this, ID_TXTCTRL_EXECUTION);
    Bind(wxEVT_TEXT, &FileSettingsDialog::OnKeypressCheckAddress, this, ID_TXTCTRL_END);

    Bind(wxEVT_TEXT_ENTER, &FileSettingsDialog::OnAddressFieldEnterPress, this, ID_TXTCTRL_START);
    Bind(wxEVT_TEXT_ENTER, &FileSettingsDialog::OnAddressFieldEnterPress, this, ID_TXTCTRL_EXECUTION);
    Bind(wxEVT_TEXT_ENTER, &FileSettingsDialog::OnAddressFieldEnterPress, this, ID_TXTCTRL_END);

    m_txtctrl_start_address->Bind(wxEVT_KILL_FOCUS, &FileSettingsDialog::OnTextCtrlLostFocus, this);
    m_txtctrl_exec_address->Bind(wxEVT_KILL_FOCUS, &FileSettingsDialog::OnTextCtrlLostFocus, this);
    m_txtctrl_end_address->Bind(wxEVT_KILL_FOCUS, &FileSettingsDialog::OnTextCtrlLostFocus, this);

    SetMinSize(wxSize(250, 250));
    SetData();

    m_changed = false;

	m_txtctrl_start_address->SetFocus();
    m_reset_button->Disable();
}



void FileSettingsDialog::BuildDialog()
{
	m_main_panel = new wxPanel(this);
    wxPanel *panel_settings = new wxPanel(m_main_panel);
    wxPanel *yesno_panel = new wxPanel(m_main_panel);

    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *panel_settings_sizer = new wxBoxSizer(wxHORIZONTAL);

    panel_settings->SetSizer(panel_settings_sizer);

    SetupRadioTypeChooser(panel_settings);
    SetupAddressBox(panel_settings);
    main_sizer->Add(panel_settings, wxSizerFlags(0).Border(wxALL, 10));
    SetupButtons(yesno_panel);
    main_sizer->Add(yesno_panel,  wxSizerFlags(0).Expand());
    m_main_panel->SetSizer(main_sizer);
    main_sizer->Fit(this);
}



void FileSettingsDialog::SetupRadioTypeChooser(wxPanel *panel)
{
    wxPanel *panel_radio = new wxPanel(panel, ID_PANEL1, wxDefaultPosition, wxSize(115,115), wxTAB_TRAVERSAL, "ID_PANEL1");
    wxBoxSizer *radio_sizer = new wxBoxSizer(wxHORIZONTAL);
    panel_radio->SetSizer(radio_sizer);

	wxString file_type_list[4] =
	{
		"ROM",
		"ROM (Cartridge)",
		"COM",
		"BIN"
	};
	m_radio_filetype_box = new wxRadioBox(panel_radio, ID_RADIOBOX1, "File type", wxPoint(8,8), wxDefaultSize, 4, file_type_list, 1, 0, wxDefaultValidator, "ID_RADIOBOX1");
    radio_sizer->Add(m_radio_filetype_box);
    radio_sizer->Fit(panel_radio);
    panel->GetSizer()->Add(panel_radio);
    panel->GetSizer()->AddSpacer(10);

    m_radio_filetype_box->Enable(1, false); // Disable ROM (Cartridge). Its not possible to fill Cartridge header.
}



void FileSettingsDialog::SetupAddressBox(wxPanel *panel)
{
    wxPanel *panel_address,
            *panel_address_start,
            *panel_address_exec,
            *panel_address_end;

    wxStaticBoxSizer  *address_sizer;
    wxBoxSizer  *address_start_sizer,
                *address_exec_sizer,
                *address_end_sizer;

    wxStaticText    *text;

    panel_address = new wxPanel(panel);
    panel_address_start = new wxPanel(panel_address);
    address_start_sizer = new wxBoxSizer(wxVERTICAL);
    text = new wxStaticText(panel_address_start, ID_TXT_START, "Start", wxDefaultPosition, wxDefaultSize, 0, "ID_TXT_START");
    address_start_sizer->Add(text, wxSizerFlags(0).Left());
    m_txtctrl_start_address = new wxTextCtrl(panel_address_start, ID_TXTCTRL_START, wxEmptyString, wxDefaultPosition, wxSize(80,20), wxTE_PROCESS_ENTER, wxDefaultValidator, "START_ADDRESS");
    address_start_sizer->Add(m_txtctrl_start_address, wxSizerFlags(0).Left().Expand());
    panel_address_start->SetSizerAndFit(address_start_sizer);

    panel_address_exec = new wxPanel(panel_address);
    address_exec_sizer = new wxBoxSizer(wxVERTICAL);
    text = new wxStaticText(panel_address_exec, ID_TXT_EXECUTION, "Execution", wxDefaultPosition, wxDefaultSize, 0, "ID_TXT_EXECUTION");
    address_exec_sizer->Add(text, wxSizerFlags(0).Left());
    m_txtctrl_exec_address = new wxTextCtrl(panel_address_exec, ID_TXTCTRL_EXECUTION, wxEmptyString, wxDefaultPosition, wxSize(80,20), wxTE_PROCESS_ENTER, wxDefaultValidator, "EXECUTION_ADDRESS");
    address_exec_sizer->Add(m_txtctrl_exec_address, wxSizerFlags(0).Left());
    panel_address_exec->SetSizer(address_exec_sizer);

    panel_address_end = new wxPanel(panel_address);
    address_end_sizer = new wxBoxSizer(wxVERTICAL);
    text = new wxStaticText(panel_address_end, ID_TXT_END, "End", wxDefaultPosition, wxDefaultSize, 0, "ID_TXT_END");
    address_end_sizer->Add(text, wxSizerFlags(0).Left());
    m_txtctrl_end_address = new wxTextCtrl(panel_address_end, ID_TXTCTRL_END, wxEmptyString, wxDefaultPosition, wxSize(80,20), wxTE_PROCESS_ENTER, wxDefaultValidator, "END_ADDRESS");
    address_end_sizer->Add(m_txtctrl_end_address, wxSizerFlags(0).Left());
    panel_address_end->SetSizer(address_end_sizer);

    address_sizer = new wxStaticBoxSizer(wxVERTICAL, panel_address, "Address");
    address_sizer->Add(panel_address_start, wxSizerFlags(1).Left().Border(wxALL, 5));
    address_sizer->Add(panel_address_exec, wxSizerFlags(1).Left().Border(wxALL, 5));
    address_sizer->Add(panel_address_end, wxSizerFlags(1).Left().Border(wxALL, 5));
    panel_address->SetSizer(address_sizer);

    panel->GetSizer()->Add(panel_address);
}



void FileSettingsDialog::SetupButtons(wxPanel *panel)
{
    wxBoxSizer *yesno_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton *yes_button = new wxButton(panel, wxID_OK, "OK");
    yesno_sizer->Add(yes_button, wxSizerFlags(0).Border(wxALL, 10));

    m_reset_button = new wxButton(panel, wxID_RESET, "Reset");
    yesno_sizer->Add(m_reset_button, wxSizerFlags(0).Border(wxALL, 10));

    wxButton *no_button = new wxButton(panel, wxID_CANCEL, "Cancel");
    yesno_sizer->Add(no_button, wxSizerFlags(0).Border(wxALL, 10));

    panel->SetSizer(yesno_sizer);
}



void FileSettingsDialog::SetData()
{
    wxString str;
    FileType t;
    bool    cartrom;

    ResetOriginalAddresses();
    UpdateFormAddress();

    t = m_program->GetFileType();
    m_filetype = t;
    switch (t)
    {
        case BIN:
            m_radio_filetype_box->SetSelection(3);
            break;
        case COM:
            m_radio_filetype_box->SetSelection(2);
            break;
        case ROM:
            cartrom = m_program->isCartridge();

			if (cartrom)
                m_radio_filetype_box->SetSelection(1);
            else
                m_radio_filetype_box->SetSelection(0);
            break;
        case UNKNOWN:
            break;

    }
}



void FileSettingsDialog::UpdateFormAddress()
{
    wxString str;
    str.Printf("%.4X", m_start_address);
    m_txtctrl_start_address->ChangeValue(str);
    str.Printf("%.4X", m_end_address);
    m_txtctrl_end_address->ChangeValue(str);
    str.Printf("%.4X", m_exec_address);
    m_txtctrl_exec_address->ChangeValue(str);
}



void FileSettingsDialog::ResetOriginalAddresses()
{
    m_start_address = m_program->StartAddress;
    m_end_address = m_program->EndAddress;
    m_exec_address = m_program->ExecAddress;
}



void FileSettingsDialog::CheckAddressSanity()
{
    if ((m_end_address - m_start_address) > (m_program->GetSize() - 1))
        m_end_address = m_start_address + m_program->GetSize() - 1;

    if (m_start_address >= m_end_address)
        m_end_address = m_start_address + m_program->GetSize() - 1;

    if (m_start_address > m_exec_address)
        m_exec_address = m_start_address;

    if (m_exec_address >= m_end_address)
        m_exec_address = m_start_address;
}



bool FileSettingsDialog::CheckHexadecimal(wxString &str_number, long &convert)
{
    if (str_number.IsEmpty())
        return false;
    
    
    if (!str_number.ToLong(&convert, 16)) {
        wxMessageBox("Only hexadecimal numbers allowed !", "Error ...");
        return false;
    }
    return true;
}



bool FileSettingsDialog::CheckIfZ80Addressable(long &number)
{
    bool is_addressable = number <= 0xFFFF;

    if (!is_addressable)
        wxMessageBox(wxString::Format("Value 0x%X is greater than Z80 can address!", number), "Error ...");
    
    return is_addressable;
}



void FileSettingsDialog::OnRadioBoxSelect(wxCommandEvent &event)
{
    int selection = event.GetSelection();

    switch (selection)
    {
    case 0: //ROM
            m_filetype = ROM;
            m_cartridge = false;
            break;
    case 1: //ROM Cartridge
            m_filetype = ROM;
            m_cartridge = true;
            break;
    case 2: //COM
            m_filetype = COM;
            break;
    case 3: //BIN
            m_filetype = BIN;
            break;
    }
    m_reset_button->Enable();
    m_changed = true;
    ResetOriginalAddresses();
    UpdateFormAddress();
}



void FileSettingsDialog::OnOKPressed(wxCommandEvent &event)
{
    if (m_changed) {
        CheckAddressSanity();
        m_program->SetFileType(m_filetype);
        if ((m_filetype == ROM) && !m_cartridge)
            m_program->ForceNoCartridge();
        m_program->SetNewAddresses(m_start_address, m_exec_address, m_end_address);
    }

    EndModal(wxID_OK);
}



void FileSettingsDialog::OnResetPressed(wxCommandEvent &event)
{
    m_reset_button->Disable();
    SetData();
}



void FileSettingsDialog::OnCancelPressed(wxCommandEvent &event)
{
    EndModal(wxID_CANCEL);
}



void FileSettingsDialog::OnAddressFieldEnterPress(wxCommandEvent &event)
{
    CheckAddressSanity();
    UpdateFormAddress();
}



void FileSettingsDialog::OnKeypressCheckAddress(wxCommandEvent &event)
{
    wxString str_number;
    long number;
    uint *targetAddress = 0;
    wxTextCtrl *targetTextCtrl = 0;
    bool error_detected = true;

    if (event.GetId() == ID_TXTCTRL_START) {
        targetAddress = &m_start_address;
        targetTextCtrl = m_txtctrl_start_address;
    }

    if (event.GetId() == ID_TXTCTRL_EXECUTION) {
        targetAddress = &m_exec_address;
        targetTextCtrl = m_txtctrl_exec_address;
    }

    if (event.GetId() == ID_TXTCTRL_END) {
        targetAddress = &m_end_address;
        targetTextCtrl = m_txtctrl_end_address;
    }

    if(targetTextCtrl == 0)
        return;

    //LogIt(wxString::Format("%s field have a key pressed.", targetTextCtrl->GetName()));

    str_number = targetTextCtrl->GetValue();
    if (CheckHexadecimal(str_number, number))
        if (CheckIfZ80Addressable(number)) {
            error_detected = false;
            *targetAddress = number;
        }
    
    if(error_detected)
        targetTextCtrl->SelectAll();
    
    m_reset_button->Enable();
    m_changed = true;
}



void FileSettingsDialog::OnTextCtrlLostFocus(wxFocusEvent &event)
{
    CheckAddressSanity();
    UpdateFormAddress();
    event.Skip();
}


