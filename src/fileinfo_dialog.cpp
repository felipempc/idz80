/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Window that shows information about a loaded program
 **************************************************************/

#include "fileinfo_dialog.h"
#include <wx/intl.h>
#include <wx/string.h>
#include "rawdata.h"



const long ShowFileInfo::ID_BUTTON1 = wxNewId();
const long ShowFileInfo::ID_STATICBOX1 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT1 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT2 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT3 = wxNewId();
const long ShowFileInfo::ID_STATICBOX2 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT4 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT5 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT6 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT7 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT8 = wxNewId();
const long ShowFileInfo::ID_STATICTEXT9 = wxNewId();

const long ShowFileInfo::ID_PANEL1 = wxNewId();


ShowFileInfo::ShowFileInfo(IDZ80MainBase *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
	Create(parent, wxID_ANY, "File Information", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, "wxID_ANY");
	SetClientSize(wxSize(400,249));
	SetFocus();
	Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(400,248), wxTAB_TRAVERSAL, "ID_PANEL1");
	B_OK = new wxButton(Panel1, ID_BUTTON1, "OK", wxPoint(160,208), wxDefaultSize, 0, wxDefaultValidator, "ID_BUTTON1");
	B_OK->SetFocus();
	StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, "File", wxPoint(8,8), wxSize(384,72), 0, "ID_STATICBOX1");
	StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, "Name:", wxPoint(24,24), wxDefaultSize, 0, "ID_STATICTEXT1");
	StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, "Size:", wxPoint(24,40), wxDefaultSize, 0, "ID_STATICTEXT2");
	StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, "Type:", wxPoint(24,56), wxDefaultSize, 0, "ID_STATICTEXT3");
	StaticBox2 = new wxStaticBox(Panel1, ID_STATICBOX2, "Structure", wxPoint(8,88), wxSize(384,96), 0, "ID_STATICBOX2");


	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ShowFileInfo::OnB_OKClick, this, ID_BUTTON1);

}


void ShowFileInfo::OnB_OKClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

/*
void ShowFileInfo::SendInfo(ProcessData *process)
{
    const CartHeader  *cart;
    wxString    str;

    m_process = process;

    str = StaticText1->GetLabel();
    str << " " << m_process->Program->GetFileName();
    StaticText1->SetLabel(str);

    str = StaticText2->GetLabel();
    str << wxString::Format(" %d bytes", m_process->Program->GetFileSize());
    StaticText2->SetLabel(str);

    str = StaticText3->GetLabel();
    str << " ";
    if (m_process->Program->isCartridge())
        str << "Cartridge ";
    str << m_process->Program->GetFileTypeStr();
    StaticText3->SetLabel(str);

    if (m_process->Program->isCartridge())
    {
        cart = m_process->Program->GetCartridgeHeader();
        if (cart != 0)
        {
            StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, "Init:", wxPoint(200, 112), wxDefaultSize, 0, "ID_STATICTEXT4");
            StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, "Statement:", wxPoint(200, 128), wxDefaultSize, 0, "ID_STATICTEXT5");
            StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, "Device:", wxPoint(200, 144), wxDefaultSize, 0, "ID_STATICTEXT6");
            StaticText7 = new wxStaticText(Panel1, ID_STATICTEXT7, "Text:", wxPoint(200, 160), wxDefaultSize, 0, "ID_STATICTEXT7");
            StaticText8 = new wxStaticText(Panel1, ID_STATICTEXT8, "End Address:", wxPoint(24,128), wxDefaultSize, 0, "ID_STATICTEXT8");
            StaticText9 = new wxStaticText(Panel1, ID_STATICTEXT9, "Start Address:", wxPoint(24,112), wxDefaultSize, 0, "ID_STATICTEXT9");

            str = StaticText4->GetLabel();
            str << wxString::Format(" 0x%.4X", cart->init);
            StaticText4->SetLabel(str);

            str = StaticText5->GetLabel();
            str << wxString::Format(" 0x%.4X", cart->statement);
            StaticText5->SetLabel(str);

            str = StaticText6->GetLabel();
            str << wxString::Format(" 0x%.4X", cart->device);
            StaticText6->SetLabel(str);

            str = StaticText7->GetLabel();
            str << wxString::Format(" 0x%.4X", cart->text);
            StaticText7->SetLabel(str);

            str = StaticText8->GetLabel();
            str << wxString::Format(" 0x%.4X", m_process->Program->EndAddress);
            StaticText8->SetLabel(str);

            str = StaticText9->GetLabel();
            str << wxString::Format(" 0x%.4X", m_process->Program->StartAddress);
            StaticText9->SetLabel(str);
        }
        else
            StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, "Error ! It's not a cartridge !", wxPoint(24,112), wxDefaultSize, 0, "ID_STATICTEXT4");
    }
    else
    {
        StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, "Start Address:", wxPoint(24,112), wxDefaultSize, 0, "ID_STATICTEXT4");
        StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, "Execution Address:", wxPoint(24,128), wxDefaultSize, 0, "ID_STATICTEXT5");
        StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, "End Address:", wxPoint(24,144), wxDefaultSize, 0, "ID_STATICTEXT6");
        str = StaticText4->GetLabel();
        str << wxString::Format(" 0x%X", m_process->Program->StartAddress);
        StaticText4->SetLabel(str);

        str = StaticText5->GetLabel();
        str << wxString::Format(" 0x%X", m_process->Program->ExecAddress);
        StaticText5->SetLabel(str);

        str = StaticText6->GetLabel();
        str << wxString::Format(" 0x%X", m_process->Program->EndAddress);
        StaticText6->SetLabel(str);
    }
}
*/

void ShowFileInfo::OnInit(wxInitDialogEvent& event)
{
//    m_process = 0;
}
