/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Dialog for edit labels on list labels
 **************************************************************/


#include "editlabel_dialog.h"
#include <wx/msgdlg.h>


const long EditLabelDlg::ID_STATICTEXT2 = wxNewId();
const long EditLabelDlg::ID_STATICTEXT1 = wxNewId();
const long EditLabelDlg::ID_TEXTCTRL1 = wxNewId();
const long EditLabelDlg::ID_TEXTCTRL2 = wxNewId();
const long EditLabelDlg::ID_PANEL1 = wxNewId();


EditLabelDlg::EditLabelDlg(wxWindow* parent,bool editmode,wxWindowID id,const wxPoint& pos,const wxSize& size)
{

	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, id, _("Edit Label"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(408,298));
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer1 = new wxFlexGridSizer(2, 2, 0, 0);
	StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Address"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer1->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Label"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TC_Address = new wxTextCtrl(Panel1, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	FlexGridSizer1->Add(TC_Address, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TC_Label = new wxTextCtrl(Panel1, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	FlexGridSizer1->Add(TC_Label, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	B_OK = new wxButton(Panel1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer4->Add(B_OK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	B_Cancel = new wxButton(Panel1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer4->Add(B_Cancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer4, 1, wxALL|wxEXPAND|wxSHAPED|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	Panel1->SetSizer(BoxSizer2);
	BoxSizer2->Fit(Panel1);
	BoxSizer2->SetSizeHints(Panel1);
	BoxSizer1->Add(Panel1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);

	Bind(wxEVT_TEXT, &EditLabelDlg::OnAddressTextUpdate, this, ID_TEXTCTRL1);

	if (editmode)
        TC_Address->Enable(false);

	base = 10;
}


uint EditLabelDlg::GetAddress()
{
    wxString str;
    long lg;
    str=TC_Address->GetValue();
    str.MakeUpper();
    if (!(str.ToLong(&lg,base)))
        lg=0;
    return ((uint)lg);
}

wxString EditLabelDlg::GetLabel()
{
    return TC_Label->GetValue();
}

void EditLabelDlg::SetAddress(uint addr)
{
    wxString str;
    str.Printf(_("0x%X"),addr);
    TC_Address->ChangeValue(str);
}

void EditLabelDlg::SetLabel(wxString lbl)
{
    TC_Label->ChangeValue(lbl);
}



void EditLabelDlg::OnAddressTextUpdate(wxCommandEvent& event)
{
    long i;
    wxString str,test1,hexsign;
    str=TC_Address->GetValue();

    if (str.ToLong(&i))
    {
        base=10;
    }
    else
    if (!str.IsEmpty())
    {
        str.MakeUpper();
        hexsign.Clear();
        test1=_("Z");
        if (str.Len() == 1)
        {
            test1=_("0");
            hexsign=str;
        }
        if (str.Len() >= 2)
        {
            test1=_("0X");
            hexsign=str.Left(2);
        }
        if (hexsign == test1)
        {
            if (str.Len()>2)
            {
                test1=str.Right(str.Len()-2);
                if (IsHexadecimal(test1))
                    base=16;
                else
                {
                    base=10;
                    wxMessageBox(_("Only hexadecimal numbers allowed !"),_("Error ..."));
                    TC_Address->SetSelection(-1,-1);
                }
            }
        }
        else
        {
            wxMessageBox(_("Only decimal or hexadecimal (with 0x or &h prefix)\n numbers allowed !"),_("Error ..."));
            TC_Address->SetSelection(-1,-1);
        }
    }
}



bool IsHexadecimal(wxString str)
{
    wxString test;
    bool isHex=false;
    uint i,j;
    test=_("0123456789ABCDEF");

    for (i=0;i<str.Len();i++)
    {
        for (j=0;j<test.Len();j++)
        {
            if (str.GetChar(i) == test.GetChar(j))
            {
                isHex=true;
                break;
            }
            else
                isHex=false;
        }
        if (!isHex)
            break;
    }
    return isHex;
}
