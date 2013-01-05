/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



#include "dialog_file_settings.h"


#include <wx/intl.h>
#include <wx/button.h>
#include <wx/string.h>



const long FileSettingsDialog::ID_TXT_START = wxNewId();
const long FileSettingsDialog::ID_TXTCTRL_START = wxNewId();
const long FileSettingsDialog::ID_TXT_EXECUTION = wxNewId();
const long FileSettingsDialog::ID_TXTCTRL_EXECUTION = wxNewId();
const long FileSettingsDialog::ID_TXT_END = wxNewId();
const long FileSettingsDialog::ID_TXTCTRL_END = wxNewId();
const long FileSettingsDialog::ID_RADIOBOX1 = wxNewId();
const long FileSettingsDialog::ID_PANEL1 = wxNewId();



FileSettingsDialog::FileSettingsDialog(RawData *program)
{
	Create(0, wxID_ANY, "Configuration", wxPoint(100, 100), wxSize(300, 200), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER, "id");

	main_panel = new wxPanel(this);
	yesno_panel = new wxPanel(main_panel);
	bookCtrl = new wxNotebook(main_panel, wxID_ANY, wxPoint(0,0), wxSize(300, 200), wxNB_TOP);
    SetupProgramSettings(bookCtrl);
    SetupDAsmSettings(bookCtrl);
    bookCtrl->AddPage(new wxPanel(bookCtrl), "Teste");

    wxBoxSizer *yesno_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *yes_button = new wxButton(yesno_panel, wxID_OK, "OK");
    yesno_sizer->Add(yes_button, wxSizerFlags(1).Left());
    yesno_sizer->AddStretchSpacer(1);
    wxButton *no_button = new wxButton(yesno_panel, wxID_CANCEL, "Cancel");
    yesno_sizer->Add(no_button, wxSizerFlags(1).Right());
    yesno_panel->SetSizer(yesno_sizer);

    wxBoxSizer *d_sizer = new wxBoxSizer(wxVERTICAL);
    d_sizer->Add(bookCtrl, wxSizerFlags(1).Left());
    d_sizer->Add(yesno_panel, wxSizerFlags(1).Bottom().Center());
    SetSizerAndFit(d_sizer);


	Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &FileSettingsDialog::OnRadioBoxSelect, this, ID_RADIOBOX1);
    Bind(wxEVT_SIZE, &FileSettingsDialog::OnSizeEvent, this);

	StartAddress = 0;
	ExecAddress = 0;
	EndAddress = 0;

	m_program = program;

	Txt_StartAddress->SetFocus();
}





FileSettingsDialog::~FileSettingsDialog()
{
    delete main_panel;
}





void FileSettingsDialog::SetupProgramSettings(wxNotebook *book)
{
    wxPanel *panel_file_type;
	wxBoxSizer  *MainSizer;
	wxStaticBoxSizer    *AddressBox;


    panel_file_type = new wxPanel(book, ID_PANEL1, wxDefaultPosition, wxSize(200,200), wxTAB_TRAVERSAL, "ID_PANEL1");
	MainSizer = new wxBoxSizer(wxHORIZONTAL);

	wxString file_type_list[4] =
	{
		"ROM",
		"ROM (Cartridge)",
		"COM",
		"BIN"
	};
	RadioFileTypeBox = new wxRadioBox(panel_file_type, ID_RADIOBOX1, "File type", wxPoint(8,8), wxDefaultSize, 4, file_type_list, 1, 0, wxDefaultValidator, "ID_RADIOBOX1");
    MainSizer->Add(RadioFileTypeBox, wxSizerFlags(0).Left().Top().Border(wxALL, 10).FixedMinSize());


	AddressBox = new wxStaticBoxSizer(wxVERTICAL, panel_file_type, "Address");
	CreateAddressBox(AddressBox);

    MainSizer->Add(AddressBox, wxSizerFlags(0).Left().Top().Border(wxALL, 10));


	panel_file_type->SetSizer(MainSizer);


	book->AddPage(panel_file_type, "Boundary", true);
}




void FileSettingsDialog::CreateAddressBox(wxStaticBoxSizer *boxsizer)
{
    wxPanel *panel_address_start,
            *panel_address_exec,
            *panel_address_end;

    wxBoxSizer  *address_start_sizer,
                *address_exec_sizer,
                *address_end_sizer;

    wxStaticText    *text;

    panel_address_start = new wxPanel(boxsizer->GetStaticBox());
    address_start_sizer = new wxBoxSizer(wxVERTICAL);
    text = new wxStaticText(panel_address_start, ID_TXT_START, "Start", wxDefaultPosition, wxDefaultSize, 0, "ID_TXT_START");
    address_start_sizer->Add(text, wxALIGN_LEFT);
    Txt_StartAddress = new wxTextCtrl(panel_address_start, ID_TXTCTRL_START, wxEmptyString, wxDefaultPosition, wxSize(80,20), 0, wxDefaultValidator, "START_ADDRESS");
    address_start_sizer->Add(Txt_StartAddress, wxALIGN_LEFT);
    panel_address_start->SetSizer(address_start_sizer);

    panel_address_exec = new wxPanel(boxsizer->GetStaticBox());
    address_exec_sizer = new wxBoxSizer(wxVERTICAL);
    text = new wxStaticText(panel_address_exec, ID_TXT_EXECUTION, "Execution", wxDefaultPosition, wxDefaultSize, 0, "ID_TXT_EXECUTION");
    address_exec_sizer->Add(text, wxALIGN_LEFT);
    Txt_ExecAddress = new wxTextCtrl(panel_address_exec, ID_TXTCTRL_EXECUTION, wxEmptyString, wxDefaultPosition, wxSize(80,20), 0, wxDefaultValidator, "EXECUTION_ADDRESS");
    address_exec_sizer->Add(Txt_ExecAddress, wxALIGN_LEFT);
    panel_address_exec->SetSizer(address_exec_sizer);

    panel_address_end = new wxPanel(boxsizer->GetStaticBox());
    address_end_sizer = new wxBoxSizer(wxVERTICAL);
    text = new wxStaticText(panel_address_end, ID_TXT_END, "End", wxDefaultPosition, wxDefaultSize, 0, "ID_TXT_END");
    address_end_sizer->Add(text, wxALIGN_LEFT);
    Txt_EndAddress = new wxTextCtrl(panel_address_end, ID_TXTCTRL_END, wxEmptyString, wxDefaultPosition, wxSize(80,20), 0, wxDefaultValidator, "END_ADDRESS");
    address_end_sizer->Add(Txt_EndAddress, wxALIGN_LEFT);
    panel_address_end->SetSizer(address_end_sizer);

    boxsizer->Add(panel_address_start, wxSizerFlags(1).Left().Border(wxALL, 5));
    boxsizer->Add(panel_address_exec, wxSizerFlags(1).Left().Border(wxALL, 5));
    boxsizer->Add(panel_address_end, wxSizerFlags(1).Left().Border(wxALL, 5));
}




void FileSettingsDialog::SetupDAsmSettings(wxNotebook *book)
{
    wxPanel *checkbox_panel, *dasm_box_panel;
    wxBoxSizer *chkbox_sizer;
    wxStaticBoxSizer *box_dasm;

    checkbox_panel = new wxPanel(book);
    dasm_box_panel = new wxPanel(checkbox_panel, wxID_ANY, wxDefaultPosition, wxSize(200, 200));
    box_dasm = new wxStaticBoxSizer(wxVERTICAL, dasm_box_panel, "Disassemble");

    chkbox_sizer = new wxBoxSizer(wxVERTICAL);

    AutoDisassemble_CheckBox = new wxCheckBox(box_dasm->GetStaticBox(), wxID_ANY, "Auto disassemble");
    box_dasm->Add(AutoDisassemble_CheckBox, wxSizerFlags(1).Left());

    AutoLabel_CheckBox = new wxCheckBox(box_dasm->GetStaticBox(), wxID_ANY, "Auto label");
    box_dasm->Add(AutoLabel_CheckBox, wxSizerFlags(1).Left());

    SimulateExecution_CheckBox = new wxCheckBox(box_dasm->GetStaticBox(), wxID_ANY, "Simulate execution");
    box_dasm->Add(SimulateExecution_CheckBox, wxSizerFlags(1).Left());

    chkbox_sizer->Add(box_dasm);
    checkbox_panel->SetSizer(chkbox_sizer);

    AutoDisassemble_CheckBox->SetValue(true);
    AutoLabel_CheckBox->SetValue(true);
    SimulateExecution_CheckBox->SetValue(false);

    book->AddPage(checkbox_panel, "Options");
}





void FileSettingsDialog::SetData()
{
    wxString str;
    FileType t;
    bool    cartrom;

    SyncAddress();

    t = m_program->GetFileType();
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


bool FileSettingsDialog::WantsAutoDisassembly()
{
    return AutoDisassemble_CheckBox->IsChecked();
}

bool FileSettingsDialog::WantsAutoLabel()
{
    return AutoLabel_CheckBox->IsChecked();
}

bool FileSettingsDialog::WantsSimulateExecution()
{
    return SimulateExecution_CheckBox->IsChecked();
}


void FileSettingsDialog::SyncAddress()
{
    wxString str;
    StartAddress = m_program->StartAddress;
    EndAddress = m_program->EndAddress;
    ExecAddress = m_program->ExecAddress;
    str.Printf("%.4X", StartAddress);
    Txt_StartAddress->SetValue(str);
    str.Printf("%.4X", EndAddress);
    Txt_EndAddress->SetValue(str);
    str.Printf("%.4X", ExecAddress);
    Txt_ExecAddress->SetValue(str);
}



void FileSettingsDialog::OnRadioBoxSelect(wxCommandEvent &event)
{
    int selection = event.GetSelection();
    Txt_StartAddress->SetValue(wxString::Format("DEBUG = %d",selection));

    switch (selection)
    {
    case 0: //ROM
            m_program->SetFileType(ROM);
            m_program->ForceNoCartridge();
            break;
    case 1: //ROM Cartridge
            m_program->SetFileType(ROM);
            m_program->CheckCartridge();
            break;
    case 2: //COM
            m_program->SetFileType(COM);
            break;
    case 3: //BIN
            m_program->SetFileType(BIN);
            break;
    }

    SyncAddress();
}



void FileSettingsDialog::OnSizeEvent(wxSizeEvent &event)
{
    main_panel->SetSize(event.GetSize());
    bookCtrl->SetSize(event.GetSize());
}


