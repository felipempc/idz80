/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



#include "DebugView.h"

//(*InternalHeaders(DebugView)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(DebugView)
const long DebugView::ID_BUTTON1 = wxNewId();
const long DebugView::ID_TEXTCTRL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(DebugView,wxDialog)
	//(*EventTable(DebugView)
	//*)
END_EVENT_TABLE()

DebugView::DebugView(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(DebugView)
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, id, _("Debug View"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX, _T("id"));
	SetClientSize(wxSize(321,286));
	Move(wxDefaultPosition);
	FlexGridSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
	B_Limpa = new wxButton(this, ID_BUTTON1, _("Limpa"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	FlexGridSizer1->Add(B_Limpa, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TC_Log = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(311,248), wxTE_AUTO_SCROLL|wxTE_MULTILINE|wxTE_RICH, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	FlexGridSizer1->Add(TC_Log, 1, wxALL|wxSHAPED|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugView::OnB_LimpaClick1);
	Connect(wxEVT_SIZE,(wxObjectEventFunction)&DebugView::OnResize);
	//*)
    TC_Log->SetMaxSize(wxSize(1023,767));
}

DebugView::~DebugView()
{
	//(*Destroy(DebugView)
	//*)
}


void DebugView::DoResize(void)
{
    wxSize s;


    s=GetClientSize();
    s.DecBy(20,40);
    TC_Log->SetSize(s);
}


void DebugView::OnResize(wxSizeEvent& event)
{
    /*
    wxFlexGridSizer *x;
    wxString str;
    wxSize sz1;

    sz1=GetClientSize();

    x=(wxFlexGridSizer *)GetSizer();
    x->SetDimension(0,0,sz1.GetWidth(),sz1.GetHeight());
    str.Printf(_T("Sizer(%d,%d)\n"),x->GetSize().GetWidth(),x->GetSize().GetHeight());
    TC_Log->AppendText(str);
    TC_Log->SetSize(x->ComputeFittingClientSize(this));
    */
}


void DebugView::OnB_LimpaClick1(wxCommandEvent& event)
{
    TC_Log->Clear();
}

void DebugView::BlackPen(void)
{
    TC_Log->SetDefaultStyle(wxTextAttr(*wxBLACK));
}
void DebugView::RedPen(void)
{
    TC_Log->SetDefaultStyle(wxTextAttr(*wxRED));
}



