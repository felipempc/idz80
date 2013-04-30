/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



#include "FileTypeDialog.h"


#include <wx/intl.h>
#include <wx/button.h>
#include <wx/string.h>



const long FileTypeDialog::ID_TXT_START = wxNewId();
const long FileTypeDialog::ID_TXTCTRL_START = wxNewId();
const long FileTypeDialog::ID_TXT_EXECUTION = wxNewId();
const long FileTypeDialog::ID_TXTCTRL_EXECUTION = wxNewId();
const long FileTypeDialog::ID_TXT_END = wxNewId();
const long FileTypeDialog::ID_TXTCTRL_END = wxNewId();
const long FileTypeDialog::ID_PANEL2 = wxNewId();
const long FileTypeDialog::ID_RADIOBOX1 = wxNewId();
const long FileTypeDialog::ID_PANEL1 = wxNewId();
const long FileTypeDialog::ID_PANEL3 = wxNewId();
const long FileTypeDialog::ID_PANEL4 = wxNewId();
const long FileTypeDialog::ID_CHKBOX1 = wxNewId();
const long FileTypeDialog::ID_CHKBOX2 = wxNewId();
const long FileTypeDialog::ID_CHKBOX_CARTRIDGE = wxNewId();


BEGIN_EVENT_TABLE(FileTypeDialog,wxDialog)
END_EVENT_TABLE()



//TODO: Create events to modify addresses

FileTypeDialog::FileTypeDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	wxBoxSizer		*UpsideSizer;
	wxFlexGridSizer	*MainSizer;
	wxFlexGridSizer	*RightSizer;
	wxStdDialogButtonSizer	*OkCancelSizer;
	wxStaticBox		*AddressBox;
	int             addr_x, addr_y;

	Create(parent, id, "Configuration", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, "id");
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);

	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxSize(263,257), wxTAB_TRAVERSAL, "ID_PANEL1");
	Panel3 = new wxPanel(Panel1, ID_PANEL3, wxDefaultPosition, wxSize(139,145), wxTAB_TRAVERSAL, "ID_PANEL3");
	Panel4 = new wxPanel(this, ID_PANEL4, wxDefaultPosition, wxSize(339,145), wxTAB_TRAVERSAL, "ID_PANEL4");

	MainSizer = new wxFlexGridSizer(2,1,0,0);
	UpsideSizer = new wxBoxSizer(wxHORIZONTAL);
	RightSizer = new wxFlexGridSizer(2,1,0,0);

	AddressBox = new wxStaticBox(Panel1, wxID_ANY, "Address", wxDefaultPosition, wxDefaultSize, 0, "wxID_ANY");

    addr_x = 15;
    addr_y = 20;
	StaticText1 = new wxStaticText(AddressBox, ID_TXT_START, "Start", wxPoint(addr_x,addr_y), wxDefaultSize, 0, "ID_TXT_START");
	addr_y += 13;
	Txt_StartAddress = new wxTextCtrl(AddressBox, ID_TXTCTRL_START, wxEmptyString, wxPoint(addr_x,addr_y), wxDefaultSize, 0, wxDefaultValidator, "ID_TXTCTRL_START");
	addr_y += 32;
	StaticText2 = new wxStaticText(AddressBox, ID_TXT_EXECUTION, "Execution", wxPoint(addr_x,addr_y), wxDefaultSize, 0, "ID_TXT_EXECUTION");
	addr_y += 13;
	Txt_ExecAddress = new wxTextCtrl(AddressBox, ID_TXTCTRL_EXECUTION, wxEmptyString, wxPoint(addr_x,addr_y), wxDefaultSize, 0, wxDefaultValidator, "ID_TXTCTRL_EXECUTION");
	addr_y += 32;
	StaticText3 = new wxStaticText(AddressBox, ID_TXT_END, "End", wxPoint(addr_x,addr_y), wxDefaultSize, 0, "ID_TXT_END");
	addr_y += 13;
	Txt_EndAddress = new wxTextCtrl(AddressBox, ID_TXTCTRL_END, wxEmptyString, wxPoint(addr_x,addr_y), wxDefaultSize, 0, wxDefaultValidator, "ID_TXTCTRL_END");
	UpsideSizer->Add(AddressBox, 1, wxLEFT|wxEXPAND, 10);

	wxString __wxRadioBoxChoices_1[3] =
	{
		"ROM",
		"COM",
		"BIN"
	};

	RadioBox1 = new wxRadioBox(Panel1, ID_RADIOBOX1, "File type", wxPoint(8,8), wxDefaultSize, 3, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, "ID_RADIOBOX1");
	RightSizer->Add(RadioBox1, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 1);
	cb_autodisassemble = new wxCheckBox(Panel3, ID_CHKBOX1, "Auto disassemble", wxPoint(8,24), wxDefaultSize, 0, wxDefaultValidator, "ID_CHKBOX1");
	cb_simulateexecution = new wxCheckBox(Panel3, ID_CHKBOX2, "Simulate Execution", wxPoint(8,48), wxDefaultSize, 0, wxDefaultValidator, "ID_CHKBOX2");
	cb_cartridge = new wxCheckBox(Panel3, ID_CHKBOX_CARTRIDGE, "Cartridge", wxPoint(8, 72), wxDefaultSize, 0, wxDefaultValidator, "ID_CHKBOX_CARTRIDGE");
	RightSizer->Add(Panel3, 1, wxTOP|wxALL, 10);
	UpsideSizer->Add(RightSizer, 1, wxRIGHT|wxALL, 10);
	Panel1->SetSizer(UpsideSizer);
	UpsideSizer->SetSizeHints(Panel1);
	MainSizer->Add(Panel1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 100);

	OkCancelSizer = new wxStdDialogButtonSizer();
	OkCancelSizer->AddButton(new wxButton(Panel4, wxID_OK, wxEmptyString));
	OkCancelSizer->AddButton(new wxButton(Panel4, wxID_CANCEL, wxEmptyString));
	OkCancelSizer->Realize();
	Panel4->SetSizer(OkCancelSizer);
	OkCancelSizer->Fit(Panel4);
	OkCancelSizer->SetSizeHints(Panel4);
	MainSizer->Add(Panel4, 1, wxALL|wxALIGN_CENTER, 10);

	SetSizer(MainSizer);
	MainSizer->Fit(this);
	MainSizer->SetSizeHints(this);


	Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &FileTypeDialog::OnRadioBoxSelect, this, ID_RADIOBOX1);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &FileTypeDialog::OnChkBoxCartridge, this, ID_CHKBOX_CARTRIDGE);

	cb_autodisassemble->SetValue(true);
	cb_simulateexecution->SetValue(false);
	cb_cartridge->Enable(false);

	StartAddress = 0;
	ExecAddress = 0;
	EndAddress = 0;
}

FileTypeDialog::~FileTypeDialog()
{
}


void FileTypeDialog::SetData(RawData& program)
{
    wxString str;
    FileType t;
    bool    cartrom;

    m_program = &program;

    UpdateFormAddress();

    t = program.GetFileType();
    switch (t)
    {
        case BIN:
            RadioBox1->SetSelection(2);
            break;
        case COM:
            RadioBox1->SetSelection(1);
            break;
        case ROM:
            cartrom = m_program->isCartridge();
			cb_cartridge->Enable(cartrom);
			if (cartrom)
                cb_cartridge->SetValue(cartrom);
            RadioBox1->SetSelection(0);
            break;

    }
}


uint FileTypeDialog::GetStartAddress()
{
    return StartAddress;
}


uint FileTypeDialog::GetExecAddress()
{
    return ExecAddress;
}


uint FileTypeDialog::GetEndAddress()
{
    return EndAddress;
}

void FileTypeDialog::UpdateFormAddress()
{
    wxString str;
    StartAddress = m_program->StartAddress;
    EndAddress = m_program->EndAddress;
    ExecAddress = m_program->ExecAddress;
    str.Printf("%.4Xh",StartAddress);
    Txt_StartAddress->SetValue(str);
    str.Printf("%.4Xh",EndAddress);
    Txt_EndAddress->SetValue(str);
    str.Printf("%.4Xh",ExecAddress);
    Txt_ExecAddress->SetValue(str);
}

void FileTypeDialog::OnRadioBoxSelect(wxCommandEvent &event)
{
    wxString str;
    str = event.GetString();
    if (str == "COM")
    {
        m_program->SetFileType(COM);
		cb_cartridge->Enable(m_program->isCartridge());
	}
    else
        if (str == "ROM")
        {
            m_program->SetFileType(ROM);
            cb_cartridge->Enable(m_program->isCartridge());
		}
        else
        {
            m_program->SetFileType(BIN);
            cb_cartridge->Enable(m_program->isCartridge());
		}

    UpdateFormAddress();
}



void FileTypeDialog::OnChkBoxCartridge(wxCommandEvent &event)
{
	if (cb_cartridge->IsChecked())
	{
        if (!m_program->CheckCartridge())
            cb_cartridge->SetValue(false);
	}
	else
		m_program->ForceNoCartridge();
	UpdateFormAddress();
}



