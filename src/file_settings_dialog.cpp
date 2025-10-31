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

	//Bind(wxEVT_RADIOBOX, &FileSettingsDialog::OnRadioBoxSelect, this, ID_RADIOBOX1);
    Bind(wxEVT_BUTTON, &FileSettingsDialog::OnOKPressed, this, wxID_OK);
    Bind(wxEVT_BUTTON, &FileSettingsDialog::OnCancelPressed, this, wxID_CANCEL);

    Bind(wxEVT_TEXT, &FileSettingsDialog::OnKeypressStartAddress, this, ID_TXTCTRL_START);
    Bind(wxEVT_TEXT, &FileSettingsDialog::OnKeypressExecutionAddress, this, ID_TXTCTRL_EXECUTION);
    Bind(wxEVT_TEXT, &FileSettingsDialog::OnKeypressEndAddress, this, ID_TXTCTRL_END);

    Bind(wxEVT_TEXT_ENTER, &FileSettingsDialog::OnAddressFieldEnterPress, this, ID_TXTCTRL_START);
    Bind(wxEVT_TEXT_ENTER, &FileSettingsDialog::OnAddressFieldEnterPress, this, ID_TXTCTRL_EXECUTION);
    Bind(wxEVT_TEXT_ENTER, &FileSettingsDialog::OnAddressFieldEnterPress, this, ID_TXTCTRL_END);

    SetMinSize(wxSize(250, 250));
    SetData();

	Txt_StartAddress->SetFocus();
}





FileSettingsDialog::~FileSettingsDialog()
{
    //delete main_panel;
}


void FileSettingsDialog::BuildDialog()
{
	main_panel_ = new wxPanel(this);
    wxPanel *panel_settings = new wxPanel(main_panel_);
    wxPanel *yesno_panel = new wxPanel(main_panel_);

    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *panel_settings_sizer = new wxBoxSizer(wxHORIZONTAL);
    //main_panel_->SetSizerAndFit(main_sizer);
    panel_settings->SetSizer(panel_settings_sizer);

    SetupRadioTypeChooser(panel_settings);
    SetupAddressBox(panel_settings);
    main_sizer->Add(panel_settings, wxSizerFlags(0).Border(wxALL, 10));
    SetupOkCancelButtons(yesno_panel);
    main_sizer->Add(yesno_panel,  wxSizerFlags(0).Expand());
    main_panel_->SetSizer(main_sizer);
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
	RadioFileTypeBox = new wxRadioBox(panel_radio, ID_RADIOBOX1, "File type", wxPoint(8,8), wxDefaultSize, 4, file_type_list, 1, 0, wxDefaultValidator, "ID_RADIOBOX1");
    radio_sizer->Add(RadioFileTypeBox);
    radio_sizer->Fit(panel_radio);
    panel->GetSizer()->Add(panel_radio); //, wxSizerFlags(0).Left().Top().Border(wxALL, 5).FixedMinSize());
    panel->GetSizer()->AddSpacer(10);
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

    panel_address = new wxPanel(panel); //, ID_PANEL2, wxDefaultPosition, wxSize(200, 200), wxTAB_TRAVERSAL, "ID_PANEL2");
    panel_address_start = new wxPanel(panel_address);
    address_start_sizer = new wxBoxSizer(wxVERTICAL);
    text = new wxStaticText(panel_address_start, ID_TXT_START, "Start", wxDefaultPosition, wxDefaultSize, 0, "ID_TXT_START");
    address_start_sizer->Add(text, wxSizerFlags(0).Left());
    Txt_StartAddress = new wxTextCtrl(panel_address_start, ID_TXTCTRL_START, wxEmptyString, wxDefaultPosition, wxSize(80,20), wxTE_PROCESS_ENTER, wxDefaultValidator, "START_ADDRESS");
    address_start_sizer->Add(Txt_StartAddress, wxSizerFlags(0).Left().Expand());
    panel_address_start->SetSizerAndFit(address_start_sizer);

    panel_address_exec = new wxPanel(panel_address);
    address_exec_sizer = new wxBoxSizer(wxVERTICAL);
    text = new wxStaticText(panel_address_exec, ID_TXT_EXECUTION, "Execution", wxDefaultPosition, wxDefaultSize, 0, "ID_TXT_EXECUTION");
    address_exec_sizer->Add(text, wxSizerFlags(0).Left());
    Txt_ExecAddress = new wxTextCtrl(panel_address_exec, ID_TXTCTRL_EXECUTION, wxEmptyString, wxDefaultPosition, wxSize(80,20), wxTE_PROCESS_ENTER, wxDefaultValidator, "EXECUTION_ADDRESS");
    address_exec_sizer->Add(Txt_ExecAddress, wxSizerFlags(0).Left());
    panel_address_exec->SetSizer(address_exec_sizer);

    panel_address_end = new wxPanel(panel_address);
    address_end_sizer = new wxBoxSizer(wxVERTICAL);
    text = new wxStaticText(panel_address_end, ID_TXT_END, "End", wxDefaultPosition, wxDefaultSize, 0, "ID_TXT_END");
    address_end_sizer->Add(text, wxSizerFlags(0).Left());
    Txt_EndAddress = new wxTextCtrl(panel_address_end, ID_TXTCTRL_END, wxEmptyString, wxDefaultPosition, wxSize(80,20), wxTE_PROCESS_ENTER, wxDefaultValidator, "END_ADDRESS");
    address_end_sizer->Add(Txt_EndAddress, wxSizerFlags(0).Left());
    panel_address_end->SetSizer(address_end_sizer);

    address_sizer = new wxStaticBoxSizer(wxVERTICAL, panel_address, "Address");
    address_sizer->Add(panel_address_start, wxSizerFlags(1).Left().Border(wxALL, 5));
    address_sizer->Add(panel_address_exec, wxSizerFlags(1).Left().Border(wxALL, 5));
    address_sizer->Add(panel_address_end, wxSizerFlags(1).Left().Border(wxALL, 5));
    panel_address->SetSizer(address_sizer);

    panel->GetSizer()->Add(panel_address); //, wxSizerFlags(0).Left().Top().Border(wxALL, 5).Expand());
}



void FileSettingsDialog::SetupOkCancelButtons(wxPanel *panel)
{
    wxBoxSizer *yesno_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton *yes_button = new wxButton(panel, wxID_OK, "OK");
    yesno_sizer->Add(yes_button, wxSizerFlags(0).Border(wxALL, 10));
    yesno_sizer->AddSpacer(60);
    wxButton *no_button = new wxButton(panel, wxID_CANCEL, "Cancel");
    yesno_sizer->Add(no_button, wxSizerFlags(0).Border(wxALL, 10));
    panel->SetSizer(yesno_sizer);
    //yesno_panel->SetOwnBackgroundColour(*wxBLACK);
}



void FileSettingsDialog::ChangeStartAddress()
{
    LogIt("Start address modified.");
}



void FileSettingsDialog::ChangeExecutionAddress()
{
    LogIt("Execution address modified.");
}



void FileSettingsDialog::ChangeEndAddress()
{
    LogIt("End address modified.");
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
            RadioFileTypeBox->SetSelection(3);
            break;
        case COM:
            RadioFileTypeBox->SetSelection(2);
            break;
        case ROM:
            cartrom = m_program->isCartridge();

			if (cartrom)
                RadioFileTypeBox->SetSelection(1);
            else
                RadioFileTypeBox->SetSelection(0);
            break;
        case UNKNOWN:
            break;

    }
}


uint FileSettingsDialog::GetStartAddress()
{
    return StartAddress;
}


uint FileSettingsDialog::GetExecAddress()
{
    return ExecAddress;
}


uint FileSettingsDialog::GetEndAddress()
{
    return EndAddress;
}



void FileSettingsDialog::UpdateFormAddress()
{
    wxString str;
    str.Printf("%.4X", StartAddress);
    Txt_StartAddress->ChangeValue(str);
    str.Printf("%.4X", EndAddress);
    Txt_EndAddress->ChangeValue(str);
    str.Printf("%.4X", ExecAddress);
    Txt_ExecAddress->ChangeValue(str);
}



void FileSettingsDialog::ResetOriginalAddresses()
{
    StartAddress = m_program->StartAddress;
    EndAddress = m_program->EndAddress;
    ExecAddress = m_program->ExecAddress;
}



void FileSettingsDialog::CheckAddressSanity()
{
    if (StartAddress > EndAddress)
        EndAddress = StartAddress + (m_program->EndAddress - m_program->StartAddress + 1);

    if (StartAddress > ExecAddress)
        ExecAddress = StartAddress;

    if (ExecAddress > EndAddress)
        ExecAddress = StartAddress;
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
            //m_program->SetFileType(ROM);
            //m_program->ForceNoCartridge();
            m_filetype = ROM;
            m_cartridge = false;
            break;
    case 1: //ROM Cartridge
            /*
            m_program->SetFileType(ROM);
            if (!m_program->isCartridge())
            {
                RadioFileTypeBox->SetSelection(0);
                RadioFileTypeBox->Enable(1, false);
            }
            */
            m_filetype = ROM;
            m_cartridge = true;
            break;
    case 2: //COM
            //m_program->SetFileType(COM);
            m_filetype = COM;
            break;
    case 3: //BIN
            //m_program->SetFileType(BIN);
            m_filetype = BIN;
            break;
    }

    ResetOriginalAddresses();
    UpdateFormAddress();
}



void FileSettingsDialog::OnOKPressed(wxCommandEvent &event)
{
    // TODO: Save modifications, if modified
    EndModal(wxID_OK);
}



void FileSettingsDialog::OnCancelPressed(wxCommandEvent &event)
{
    EndModal(wxID_CANCEL);
}



// OBSOLETE
void FileSettingsDialog::OnAddressFieldsKeypress(wxCommandEvent &event)
{
    // Processa cada tecla no TextCrl, filtra caracteres não correspondentes a um valor hexadecimal. Não verifica limites.
    long    conv;
    wxString    str;
    uint    *modified_address_field;
    int id;
    wxTextCtrl  *edited_field;

    id = event.GetId();
    if (id == ID_TXTCTRL_START)
    {
        str = Txt_StartAddress->GetValue();
        LogIt(wxString::Format("Changed START: %s", str));
        edited_field = Txt_StartAddress;
        modified_address_field = &StartAddress;
    }

    if (id == ID_TXTCTRL_EXECUTION)
    {
        str = Txt_ExecAddress->GetValue();
        LogIt(wxString::Format("Changed EXECUTION: %s", str));
        edited_field = Txt_ExecAddress;
        modified_address_field = &ExecAddress;
    }

    if (id == ID_TXTCTRL_END)
    {
        str = Txt_EndAddress->GetValue();
        LogIt(wxString::Format("Changed END: %s", str));
        edited_field = Txt_EndAddress;
        modified_address_field = &EndAddress;
    }

    if (!str.IsEmpty())
    {
        if (!str.ToLong(&conv, 16))
        {
            wxMessageBox("Only hexadecimal numbers allowed !", "Error ...");
            edited_field->Clear();
        }
        else
        {
            *modified_address_field = static_cast<uint>(conv);
            CheckAddressSanity();
        }
    }
    UpdateFormAddress();
}

void FileSettingsDialog::OnAddressFieldEnterPress(wxCommandEvent &event)
{
    // Processa ao teclar enter em algum campo endereço. Deve calcular os limites do endereçamento.
}



void FileSettingsDialog::OnKeypressStartAddress(wxCommandEvent &event)
{
    wxString str_number;
    long number;
    bool error_detected = true;

    str_number = Txt_StartAddress->GetValue();
    if (CheckHexadecimal(str_number, number))
        if (CheckIfZ80Addressable(number)) {
            error_detected = false;
            StartAddress = number;
        }
    
    if(error_detected)
        Txt_StartAddress->SelectAll();
    
    //LogIt("START field pressed.");
}

void FileSettingsDialog::OnKeypressExecutionAddress(wxCommandEvent &event)
{
    wxString str_number;
    long number;
    bool error_detected = true;

    str_number = Txt_ExecAddress->GetValue();
    if (CheckHexadecimal(str_number, number))
        if (CheckIfZ80Addressable(number)) {
            error_detected = false;
            ExecAddress = number;
        }

    if(error_detected)
        Txt_ExecAddress->SelectAll();
    //LogIt("EXECUTION field pressed.");
}

void FileSettingsDialog::OnKeypressEndAddress(wxCommandEvent &event)
{
    wxString str_number;
    long number;
    bool error_detected = true;

    str_number = Txt_EndAddress->GetValue();
    if (CheckHexadecimal(str_number, number))
        if (CheckIfZ80Addressable(number)) {
            error_detected = false;
            EndAddress = number;
        }
    
    if(error_detected)
        Txt_EndAddress->SelectAll();
    //LogIt("END field pressed.");
}
