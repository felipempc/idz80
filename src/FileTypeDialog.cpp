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
//*)

BEGIN_EVENT_TABLE(FileTypeDialog,wxDialog)
	//(*EventTable(FileTypeDialog)
	//*)
END_EVENT_TABLE()

FileTypeDialog::FileTypeDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(FileTypeDialog)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, id, _("Configuration"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxSize(263,257), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	Panel2 = new wxPanel(Panel1, ID_PANEL2, wxDefaultPosition, wxSize(139,145), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	StaticText1 = new wxStaticText(Panel2, ID_STATICTEXT1, _("Start"), wxPoint(8,8), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	Txt_StartAddress = new wxTextCtrl(Panel2, ID_TEXTCTRL1, wxEmptyString, wxPoint(8,24), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticText2 = new wxStaticText(Panel2, ID_STATICTEXT2, _("Execution"), wxPoint(8,56), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	Txt_EndAddress = new wxTextCtrl(Panel2, ID_TEXTCTRL2, wxEmptyString, wxPoint(8,120), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText3 = new wxStaticText(Panel2, ID_STATICTEXT3, _("End"), wxPoint(8,104), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	Txt_ExecAddress = new wxTextCtrl(Panel2, ID_TEXTCTRL3, wxEmptyString, wxPoint(8,72), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	BoxSizer3->Add(Panel2, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	wxString __wxRadioBoxChoices_1[3] =
	{
		_("ROM"),
		_("COM"),
		_("BIN")
	};
	RadioBox1 = new wxRadioBox(Panel1, ID_RADIOBOX1, _("File type"), wxDefaultPosition, wxSize(149,79), 3, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, _T("ID_RADIOBOX1"));
	BoxSizer2->Add(RadioBox1, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	Panel1->SetSizer(BoxSizer2);
	BoxSizer2->SetSizeHints(Panel1);
	BoxSizer1->Add(Panel1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	BoxSizer1->Add(StdDialogButtonSizer1, 1, wxALL|wxFIXED_MINSIZE|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)

	Connect(ID_RADIOBOX1,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&FileTypeDialog::OnRadioBoxSelect);
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

    m_program=&program;

    SyncAddress();

    t=program.GetFileType();
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
    StartAddress=m_program->StartAddress;
    EndAddress=m_program->EndAddress;
    ExecAddress=m_program->ExecAddress;
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
