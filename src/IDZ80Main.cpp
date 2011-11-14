/***************************************************************
 * Name:      IDZ80Main.cpp
 * Purpose:   Code for Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/

#include "IDZ80Main.h"

#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include "FileTypeDialog.h"
#include <wx/dcclient.h>

//(*InternalHeaders(IDZ80Frame)
#include <wx/intl.h>
#include <wx/string.h>
//*)


//(*IdInit(IDZ80Frame)
const long IDZ80Frame::ID_BUTTON1 = wxNewId();
const long IDZ80Frame::ID_GAUGE1 = wxNewId();
const long IDZ80Frame::ID_PANEL1 = wxNewId();
const long IDZ80Frame::IdMenuOpen = wxNewId();
const long IDZ80Frame::idMenuQuit = wxNewId();
const long IDZ80Frame::ID_DebugView = wxNewId();
const long IDZ80Frame::ID_DebugListMneum = wxNewId();
const long IDZ80Frame::ID_MneumLoad = wxNewId();
const long IDZ80Frame::ID_MneumInfo = wxNewId();
const long IDZ80Frame::idMenuAbout = wxNewId();
const long IDZ80Frame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(IDZ80Frame,wxFrame)
    //(*EventTable(IDZ80Frame)
    //*)
END_EVENT_TABLE()

IDZ80Frame::IDZ80Frame(wxWindow* parent,wxWindowID id)
{
    wxSize s;
    wxPoint pt;

    //(*Initialize(IDZ80Frame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuItem* MenuItem3;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;
    wxMenu* Menu5;

    Create(parent, wxID_ANY, _("Interactive Disassembler"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(600,465));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(56,88), wxDefaultSize, 0, _T("ID_PANEL1"));
    B_DASM = new wxButton(Panel1, ID_BUTTON1, _("DASM"), wxPoint(520,8), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    Gauge1 = new wxGauge(Panel1, ID_GAUGE1, 100, wxPoint(16,368), wxSize(432,28), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu1, IdMenuOpen, _("Open\tAlt+o"), _("Open a file"), wxITEM_NORMAL);
    Menu1->Append(MenuItem3);
    Menu1->AppendSeparator();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu5 = new wxMenu();
    MenuItem5 = new wxMenuItem(Menu5, ID_DebugView, _("View\tAlt+d"), wxEmptyString, wxITEM_CHECK);
    Menu5->Append(MenuItem5);
    MenuItem6 = new wxMenuItem(Menu5, ID_DebugListMneum, _("List Mnemonics"), wxEmptyString, wxITEM_NORMAL);
    Menu5->Append(MenuItem6);
    MenuBar1->Append(Menu5, _("&Debug"));
    Menu3 = new wxMenu();
    Menu4 = new wxMenuItem(Menu3, ID_MneumLoad, _("&Load"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(Menu4);
    MenuItem4 = new wxMenuItem(Menu3, ID_MneumInfo, _("&Info"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem4);
    MenuBar1->Append(Menu3, _("&Mnemonics"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&IDZ80Frame::OnButtonDASMClick);
    Connect(IdMenuOpen,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80Frame::OnMenuOpen);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80Frame::OnQuit);
    Connect(ID_DebugView,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80Frame::OnMenuDebugView);
    Connect(ID_DebugListMneum,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80Frame::OnMenuDebugLstMnem);
    Connect(ID_MneumLoad,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80Frame::OnMneumLoadClick);
    Connect(ID_MneumInfo,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80Frame::OnMenuInfoSelected);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80Frame::OnAbout);
    Connect(wxEVT_SIZE,(wxObjectEventFunction)&IDZ80Frame::OnResize);
    //*)

    mnDb = new MnemonicDataBase();
    program = new RawData();
    dasmed = new DAsmData(program);
    process = new ProcessData(dasmed,program,mnDb);
    codeview = new CodeView(Panel1,wxID_ANY,*dasmed);
    debugview = new DebugView(this,wxID_ANY,wxDefaultPosition,wxSize(390,400));

    //Posicionando Janela Principal
    Move(1,1);
    LastSize=GetSize();

    // Posicionando CodeView
    codeview->Move(16,16);
    codeview->SetSize(432,336);

    // Posicionando Debug View
    s=GetSize();
    pt=GetScreenPosition();
    s.SetHeight(0);
    pt+=s;
    debugview->Move(pt);
    debugview->Show(true);

    codeview->DebugLog(debugview->TC_Log);

    LoadMnemonicsDB();
    B_DASM->Enable(false);

    wxMenuBar *mb;
    mb=GetMenuBar();
    mb->Check(ID_DebugView,true);
}

IDZ80Frame::~IDZ80Frame()
{
    delete process;
    delete mnDb;
    delete program;
    delete dasmed;
    delete codeview;
    delete debugview;
    //(*Destroy(IDZ80Frame)
    //*)
}


bool IDZ80Frame::LoadMnemonicsDB()
{
    wxString s;
    s=_("e:/Felipe/Coding/C_C++/Projetos/IDZ80/Z80_OpCode_AlmostFull.txt");
    debugview->BlackPen();
    debugview->TC_Log->AppendText(_T("Opening mneumonic file:\n"));
    debugview->RedPen();
    debugview->TC_Log->AppendText(s);
    debugview->TC_Log->AppendText(_T("\n"));
    return (mnDb->open(s));
}

void IDZ80Frame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void IDZ80Frame::OnAbout(wxCommandEvent& event)
{
    wxString msg = _T("Interactive Disassembler for\nZ80 processors.\n2009 by Felipe");
    wxMessageBox(msg, _("Welcome to..."));
}


void IDZ80Frame::OnMenuOpen(wxCommandEvent& event)
{
    FileTypeDialog config(this);
    wxString info;
    wxString fname;
    wxString caption=_("Choose a file");
    wxString wildcard=_("Program files (*.ROM, *.COM, *.BIN)|*.rom;*.com;*.bin|All files (*.*)|*.*");
    wxString defaultDir=_("C:/felipe/MSX");
    wxString defaultFilename=wxEmptyString;
    wxFileDialog dialog(this, caption, defaultDir, defaultFilename,wildcard, wxFD_OPEN);
    if (dialog.ShowModal() == wxID_OK)
    {
        Gauge1->SetValue(0);
        fname=dialog.GetPath();
        StatusBar1->SetStatusText(fname,0);
        if (!(program->Open(fname)))
        {
            caption.Printf(_("Could not open '%s' !"),fname.c_str());
            wxMessageBox(caption,_("Error..."));
        }
        else
        {
            B_DASM->Enable(true);
            debugview->BlackPen();
            debugview->TC_Log->AppendText(_T("Opened file:\n"));
            debugview->RedPen();
            debugview->TC_Log->AppendText(fname);
            debugview->TC_Log->AppendText(_T("\n"));
            debugview->BlackPen();
            debugview->TC_Log->AppendText(_T("File size: "));
            debugview->RedPen();
            info.Printf(_("%d bytes\n"),program->GetBufferSize());
            debugview->TC_Log->AppendText(info);
            config.SetData(*program);
            config.ShowModal();
            dasmed->SetBaseAddress(config.GetStartAddress());
        }
    }
    else
        StatusBar1->SetStatusText(_("Cancelled by user !"));
}


void IDZ80Frame::OnButtonDASMClick(wxCommandEvent& event)
{
    process->SetGauge(Gauge1);
    process->DisassembleFirst();

    // Debug Area
    wxString stemp;
    stemp.Printf(_("Used memory (dasmed)= %d bytes\n"),dasmed->GetUsedMem());
    debugview->TC_Log->AppendText(stemp);

    codeview->Plot();
}


void IDZ80Frame::OnMneumLoadClick(wxCommandEvent& event)
{
    wxString s;
    if (!LoadMnemonicsDB())
    {
        s=_("Could not open the file !");
        wxMessageBox(s,_("Error.."));
    }

}

void IDZ80Frame::OnMenuDebugView(wxCommandEvent& event)
{
    wxMenuBar *mb;

    mb=GetMenuBar();
    if (mb->IsChecked(ID_DebugView))
    {
        mb->Check(ID_DebugView,true);
        debugview->TC_Log->AppendText(wxT("View Enabled!\n"));
        debugview->Show(true);
    }
    else
    {
        mb->Check(ID_DebugView,false);
        debugview->TC_Log->AppendText(_T("View Disabled!\n"));
        debugview->Show(false);
    }
}

void IDZ80Frame::OnMenuDebugLstMnem(wxCommandEvent& event)
{
    mnDb->DebugVodoo(*debugview->TC_Log);
}

void IDZ80Frame::OnResize(wxSizeEvent& event)
{
    wxSize sz=event.GetSize();
    wxSize cvsize=codeview->GetSize();
    wxSize ggsize=Gauge1->GetSize();
    int x,y;
    x=sz.GetWidth()-LastSize.GetWidth();
    y=sz.GetHeight()-LastSize.GetHeight();
    cvsize.IncBy(x,y);
    Panel1->SetSize(sz);
    codeview->SetSize(cvsize);
    LastSize=GetSize();
    ggsize.IncBy(x,0);
    Gauge1->SetSize(ggsize);
    Panel1->Refresh();
}

void IDZ80Frame::OnMenuInfoSelected(wxCommandEvent& event)
{
    wxString str;
    str.Printf(_("Num of Mnemonics: %d\nMemory allocated: %d bytes        "),mnDb->GetCount(),
               mnDb->GetAllocated());
    wxMessageBox(str,_("Mnemonic Info"));
}
