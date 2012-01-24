/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



#include "FileTypeDialog.h"

//(*InternalHeaders(FileTypeDialog)
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/string.h>
//*)

//(*IdInit(FileTypeDialog)
const long FileTypeDialog::ID_STATICTEXT1 = wxNewId();
const long FileTypeDialog::ID_TEXTCTRL1 = wxNewId();
const long FileTypeDialog::ID_STATICTEXT2 = wxNewId();
const long FileTypeDialog::ID_TEXTCTRL2 = wxNewId();
const long FileTypeDialog::ID_STATICTEXT3 = wxNewId();
const long FileTypeDialog::ID_TEXTCTRL3 = wxNewId();
const long FileTypeDialog::ID_PANEL2 = wxNewId();
const long FileTypeDialog::ID_RADIOBOX1 = wxNewId();
const long FileTypeDialog::ID_PANEL1 = wxNewId();
const long FileTypeDialog::ID_PANEL3 = wxNewId();
const long FileTypeDialog::ID_PANEL4 = wxNewId();
const long FileTypeDialog::ID_CHKBOX1 = wxNewId();
const long FileTypeDialog::ID_CHKBOX2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(FileTypeDialog,wxDialog)
	//(*EventTable(FileTypeDialog)
	//*)
END_EVENT_TABLE()

FileTypeDialog::FileTypeDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	wxBoxSizer		*UpsideSizer;
	wxFlexGridSizer	*MainSizer;
	wxFlexGridSizer	*RightSizer;
	wxStdDialogButtonSizer	*OkCancelSizer;
	wxStaticBox		*AddressBox;
	int             addr_x, addr_y;

	Create(parent, id, _("Configuration"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);

	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxSize(263,257), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel3 = new wxPanel(Panel1, ID_PANEL3, wxDefaultPosition, wxSize(139,145), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	Panel4 = new wxPanel(this, ID_PANEL4, wxDefaultPosition, wxSize(339,145), wxTAB_TRAVERSAL, _T("ID_PANEL4"));

	MainSizer = new wxFlexGridSizer(2,1,0,0);
	UpsideSizer = new wxBoxSizer(wxHORIZONTAL);
	RightSizer = new wxFlexGridSizer(2,1,0,0);

	AddressBox = new wxStaticBox(Panel1, wxID_ANY, _("Address"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));

    addr_x = 15;
    addr_y = 20;
	StaticText1 = new wxStaticText(AddressBox, ID_STATICTEXT1, _("Start"), wxPoint(addr_x,addr_y), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	addr_y += 13;
	Txt_StartAddress = new wxTextCtrl(AddressBox, ID_TEXTCTRL1, wxEmptyString, wxPoint(addr_x,addr_y), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	addr_y += 32;
	StaticText2 = new wxStaticText(AddressBox, ID_STATICTEXT2, _("Execution"), wxPoint(addr_x,addr_y), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	addr_y += 13;
	Txt_ExecAddress = new wxTextCtrl(AddressBox, ID_TEXTCTRL2, wxEmptyString, wxPoint(addr_x,addr_y), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	addr_y += 32;
	StaticText3 = new wxStaticText(AddressBox, ID_STATICTEXT3, _("End"), wxPoint(addr_x,addr_y), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	addr_y += 13;
	Txt_EndAddress = new wxTextCtrl(AddressBox, ID_TEXTCTRL3, wxEmptyString, wxPoint(addr_x,addr_y), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	UpsideSizer->Add(AddressBox, 1, wxLEFT|wxEXPAND, 10);

	wxString __wxRadioBoxChoices_1[3] =
	{
		_("ROM"),
		_("COM"),
		_("BIN")
	};

	RadioBox1 = new wxRadioBox(Panel1, ID_RADIOBOX1, _("File type"), wxPoint(8,8), wxDefaultSize, 3, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, _T("ID_RADIOBOX1"));
	RightSizer->Add(RadioBox1, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 1);
	cb_autodisassemble = new wxCheckBox(Panel3, ID_CHKBOX1, _("Auto disassemble"), wxPoint(8,24), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHKBOX1"));
	cb_autolabel = new wxCheckBox(Panel3, ID_CHKBOX2, _("Auto label"), wxPoint(8,48), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHKBOX2"));
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


	Connect(ID_RADIOBOX1,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&FileTypeDialog::OnRadioBoxSelect);
	cb_autodisassemble->SetValue(true);
	cb_autolabel->SetValue(true);
}

FileTypeDialog::~FileTypeDialog()
{
	//(*Destroy(FileTypeDialog)
	//*)
}


void FileTypeDialog::SetData(RawData& program)
{
    wxString str;
    FileType t;

    m_program = &program;

    SyncAddress();

    t = program.GetFileType();
    switch (t)
    {
        case BIN:
            RadioBox1->SetSelection(2);
            break;
        case COM:
            RadioBox1->SetSelection(1);
            break;
        default:
        case ROM:
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

void FileTypeDialog::SyncAddress()
{
    wxString str;
    StartAddress = m_program->StartAddress;
    EndAddress = m_program->EndAddress;
    ExecAddress = m_program->ExecAddress;
    str.Printf(_T("%.4Xh"),StartAddress);
    Txt_StartAddress->SetValue(str);
    str.Printf(_T("%.4Xh"),EndAddress);
    Txt_EndAddress->SetValue(str);
    str.Printf(_T("%.4Xh"),ExecAddress);
    Txt_ExecAddress->SetValue(str);
}

void FileTypeDialog::OnRadioBoxSelect(wxCommandEvent &event)
{
    wxString str;
    str=event.GetString();
    if (str == _T("COM"))
        m_program->SetFileType(COM);
    else
        if (str == _T("ROM"))
            m_program->SetFileType(ROM);
        else
            m_program->SetFileType(BIN);

    SyncAddress();
}
