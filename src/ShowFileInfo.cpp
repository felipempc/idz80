/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module shows information about the program file
 **************************************************************/

#include "ShowFileInfo.h"

//(*InternalHeaders(ShowFileInfo)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ShowFileInfo)
const long ShowFileInfo::ID_BUTTON1 = wxNewId();
const long ShowFileInfo::ID_STATICBOX1 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT1 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT2 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT3 = wxNewId();
const long ShowFileInfo::ID_STATICBOX2 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT4 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT5 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT6 = wxNewId();
const long ShowFileInfo::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ShowFileInfo,wxDialog)
	//(*EventTable(ShowFileInfo)
	//*)
END_EVENT_TABLE()

ShowFileInfo::ShowFileInfo(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	Create(parent, wxID_ANY, _("File Information"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetClientSize(wxSize(400,249));
	SetFocus();
	Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(400,248), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	B_OK = new wxButton(Panel1, ID_BUTTON1, _("OK"), wxPoint(160,208), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	B_OK->SetFocus();
	StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, _("File"), wxPoint(8,8), wxSize(384,72), 0, _T("ID_STATICBOX1"));
	StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Name:"), wxPoint(24,24), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Size:"), wxPoint(24,40), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, _("Type:"), wxPoint(24,56), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticBox2 = new wxStaticBox(Panel1, ID_STATICBOX2, _("Structure"), wxPoint(8,88), wxSize(384,96), 0, _T("ID_STATICBOX2"));
	StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, _("Start Address:"), wxPoint(24,112), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, _("Execution Address:"), wxPoint(24,128), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, _("End Address:"), wxPoint(24,144), wxDefaultSize, 0, _T("ID_STATICTEXT6"));

	//Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ShowFileInfo::OnB_OKClick);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ShowFileInfo::OnB_OKClick, this, ID_BUTTON1);

}

ShowFileInfo::~ShowFileInfo()
{
	//(*Destroy(ShowFileInfo)
	//*)
}


void ShowFileInfo::OnB_OKClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}


void ShowFileInfo::SendInfo(ProcessData *process)
{
    wxString str;
    m_process = process;
    str = StaticText1->GetLabel();
    str << _(" ") << m_process->Program->GetFileName();
    StaticText1->SetLabel(str);

    str = StaticText2->GetLabel();
    str << wxString::Format(_(" %d bytes"),m_process->Program->GetFileSize());
    StaticText2->SetLabel(str);

    str = StaticText3->GetLabel();
    str << _(" ") << m_process->Program->GetFileTypeStr();
    StaticText3->SetLabel(str);

    str = StaticText4->GetLabel();
    str << wxString::Format(_(" 0x%X"),m_process->Program->StartAddress);
    StaticText4->SetLabel(str);

    str = StaticText5->GetLabel();
    str << wxString::Format(_(" 0x%X"),m_process->Program->ExecAddress);
    StaticText5->SetLabel(str);

    str = StaticText6->GetLabel();
    str << wxString::Format(_(" 0x%X"),m_process->Program->EndAddress);
    StaticText6->SetLabel(str);
}


void ShowFileInfo::OnInit(wxInitDialogEvent& event)
{
    m_process = 0;
}
