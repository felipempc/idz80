/***************************************************************
 * Name:      IDZ80.cpp
 * Purpose:   Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



#include "IDZ80.h"

#include "version.h"
#include "FileTypeDialog.h"
#include "ShowFileInfo.h"

#include <wx/filefn.h>
#include <wx/filename.h>

//(*InternalHeaders(IDZ80)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(IDZ80)
const long IDZ80::ID_TEXTCTRL1 = wxNewId();
const long IDZ80::idMenuFileOpenProject = wxNewId();
const long IDZ80::idMenuFileOpenArchive = wxNewId();
const long IDZ80::idMenuFileOpen = wxNewId();
const long IDZ80::idMenuFileSave = wxNewId();
const long IDZ80::idMenuFileClose = wxNewId();
const long IDZ80::idMenuFileInfo = wxNewId();
const long IDZ80::idMenuFileQuit = wxNewId();
const long IDZ80::idMenuViewDasmWindow = wxNewId();
const long IDZ80::idMenuViewProgLabels = wxNewId();
const long IDZ80::idMenuViewVarLabels = wxNewId();
const long IDZ80::idMenuViewIOLabels = wxNewId();
const long IDZ80::idMenuToolsDasmAll = wxNewId();
const long IDZ80::idMenuToolsAutoLabel = wxNewId();
const long IDZ80::idMenuToolsGenCode = wxNewId();
const long IDZ80::idMenuMnemLoad = wxNewId();
const long IDZ80::idMenuMnemInfo = wxNewId();
const long IDZ80::idMenuHelpContents = wxNewId();
const long IDZ80::IdMenuHelpAbout = wxNewId();
const long IDZ80::ID_STATUSBAR1 = wxNewId();
//*)


const long IDZ80::ID_VARLABELPANE=wxNewId();

BEGIN_EVENT_TABLE(IDZ80,wxFrame)
	//(*EventTable(IDZ80)
	//*)
END_EVENT_TABLE()

IDZ80::IDZ80(wxWindow* parent, wxArrayString &arraystr, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize s;
    wxPoint pt;
    int i;


	//(*Initialize(IDZ80)
	wxMenuItem* MenuItem11;
	wxMenuItem* MenuItem10;
	wxMenuItem* MenuItem12;
	wxMenuItem* MenuItem9;

	Create(parent, id, _("Interactive Disassembler Z80"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
	SetClientSize(wxSize(600,465));
	Move(wxDefaultPosition);
	AuiManager1 = new wxAuiManager(this, wxAUI_MGR_DEFAULT);
	PanelLog = new wxTextCtrl(this, ID_TEXTCTRL1, _("Text"), wxPoint(114,460), wxDefaultSize, wxTE_MULTILINE|wxTE_RICH, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	wxFont PanelLogFont(8,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	PanelLog->SetFont(PanelLogFont);
	AuiManager1->AddPane(PanelLog, wxAuiPaneInfo().Name(_T("MessageLog")).Caption(_("Message Log")).CaptionVisible().Bottom());
	AuiManager1->Update();
	MenuBar1 = new wxMenuBar();
	Menu1 = new wxMenu();
	MenuItem1 = new wxMenu();
	MenuItem14 = new wxMenuItem(MenuItem1, idMenuFileOpenProject, _("Project\tAlt+r"), wxEmptyString, wxITEM_NORMAL);
	MenuItem1->Append(MenuItem14);
	MenuItem15 = new wxMenuItem(MenuItem1, idMenuFileOpenArchive, _("Archive\tAlt+l"), wxEmptyString, wxITEM_NORMAL);
	MenuItem1->Append(MenuItem15);
	Menu1->Append(idMenuFileOpen, _("&Open"), MenuItem1, wxEmptyString);
	MenuItem13 = new wxMenuItem(Menu1, idMenuFileSave, _("&Save\tAlt+s"), wxEmptyString, wxITEM_NORMAL);
	Menu1->Append(MenuItem13);
	MenuItem17 = new wxMenuItem(Menu1, idMenuFileClose, _("Close"), wxEmptyString, wxITEM_NORMAL);
	Menu1->Append(MenuItem17);
	Menu1->AppendSeparator();
	MenuItem16 = new wxMenuItem(Menu1, idMenuFileInfo, _("&Info\tAlt+I"), wxEmptyString, wxITEM_NORMAL);
	Menu1->Append(MenuItem16);
	Menu1->AppendSeparator();
	MenuItem2 = new wxMenuItem(Menu1, idMenuFileQuit, _("Quit\tAlt+q"), wxEmptyString, wxITEM_NORMAL);
	Menu1->Append(MenuItem2);
	MenuBar1->Append(Menu1, _("&File"));
	Menu3 = new wxMenu();
	MenuItem4 = new wxMenuItem(Menu3, idMenuViewDasmWindow, _("Disassembly Window"), wxEmptyString, wxITEM_CHECK);
	Menu3->Append(MenuItem4);
	MenuItem9 = new wxMenuItem(Menu3, idMenuViewProgLabels, _("Program Labels"), wxEmptyString, wxITEM_CHECK);
	Menu3->Append(MenuItem9);
	MenuItem10 = new wxMenuItem(Menu3, idMenuViewVarLabels, _("Var Labels"), wxEmptyString, wxITEM_CHECK);
	Menu3->Append(MenuItem10);
	MenuItem11 = new wxMenuItem(Menu3, idMenuViewIOLabels, _("I/O Labels"), wxEmptyString, wxITEM_CHECK);
	Menu3->Append(MenuItem11);
	MenuBar1->Append(Menu3, _("&View"));
	Menu2 = new wxMenu();
	MenuItem3 = new wxMenuItem(Menu2, idMenuToolsDasmAll, _("Disassemble all\tCTRL+SHIFT+d"), wxEmptyString, wxITEM_NORMAL);
	Menu2->Append(MenuItem3);
	MenuItem12 = new wxMenuItem(Menu2, idMenuToolsAutoLabel, _("Auto Label"), wxEmptyString, wxITEM_NORMAL);
	Menu2->Append(MenuItem12);
	Menu2->AppendSeparator();
	MenuItem18 = new wxMenuItem(Menu2, idMenuToolsGenCode, _("Generate Code"), wxEmptyString, wxITEM_NORMAL);
	Menu2->Append(MenuItem18);
	MenuBar1->Append(Menu2, _("&Tools"));
	Menu4 = new wxMenu();
	MenuItem5 = new wxMenuItem(Menu4, idMenuMnemLoad, _("&Load"), wxEmptyString, wxITEM_NORMAL);
	Menu4->Append(MenuItem5);
	MenuItem6 = new wxMenuItem(Menu4, idMenuMnemInfo, _("&Info\tAlt+i"), wxEmptyString, wxITEM_NORMAL);
	Menu4->Append(MenuItem6);
	MenuBar1->Append(Menu4, _("&Mnemonics"));
	Menu5 = new wxMenu();
	MenuItem7 = new wxMenuItem(Menu5, idMenuHelpContents, _("&Contents\tAlt+c"), wxEmptyString, wxITEM_NORMAL);
	Menu5->Append(MenuItem7);
	Menu5->AppendSeparator();
	MenuItem8 = new wxMenuItem(Menu5, IdMenuHelpAbout, _("About\tAlt+b"), wxEmptyString, wxITEM_NORMAL);
	Menu5->Append(MenuItem8);
	MenuBar1->Append(Menu5, _("&Help"));
	SetMenuBar(MenuBar1);
	StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
	int __wxStatusBarWidths_1[1] = { -10 };
	int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
	StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
	StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
	SetStatusBar(StatusBar1);

	AuiManager1->Connect(wxEVT_AUI_PANE_CLOSE,(wxObjectEventFunction)&IDZ80::OnAuiPaneClose,0,this);
	Connect(idMenuFileOpenProject,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuFileOpen);
	Connect(idMenuFileOpenArchive,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuFileOpen);
	Connect(idMenuFileOpen,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuFileOpen);
	Connect(idMenuFileSave,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuFileSaveProject);
	Connect(idMenuFileClose,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuFileClose);
	Connect(idMenuFileInfo,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuFileInfo);
	Connect(idMenuFileQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuFileQuit);
	Connect(idMenuViewDasmWindow,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuViewDisassemblyWindow);
	Connect(idMenuViewProgLabels,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuViewProgramLabels);
	Connect(idMenuViewVarLabels,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuViewVarLabels);
	Connect(idMenuViewIOLabels,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuViewIOLabels);
	Connect(idMenuToolsDasmAll,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuToolsDisAsm);
	Connect(idMenuToolsAutoLabel,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuToolAutoLabel);
	Connect(idMenuToolsGenCode,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuToolsGenCode);
	Connect(idMenuMnemLoad,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuMnemonicsLoad);
	Connect(idMenuMnemInfo,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuMnemonicsInfo);
	Connect(IdMenuHelpAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDZ80::OnMenuHelpAbout);
	//*)
	Connect(wxEVT_IDLE,(wxObjectEventFunction)&IDZ80::OnFirstIdle);

    /*
     *  Get stored configuration
     */
    config = new wxConfig(_("IDZ80"));
    m_currentDir = ::wxGetCwd();

    process = new ProcessData(this);
    codeview = new CodeView(this,process);
    m_project = new ProjectManager(process,codeview);

    AuiManager1->AddPane(codeview, wxAuiPaneInfo().Name(_T("MainWindow")).Caption(_("Disassembly Window")).CaptionVisible().CenterPane().MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());
    AuiManager1->AddPane(process->var_labels,
    wxAuiPaneInfo().Name(_T("VarLabels")).Caption(_("Var Labels")).CaptionVisible().Right().TopDockable(false).BottomDockable(false).MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());
    AuiManager1->AddPane(process->prog_labels,
    wxAuiPaneInfo().Name(_T("ProgLabels")).Caption(_("Program Labels")).CaptionVisible().Right().TopDockable(false).BottomDockable(false).MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());
    AuiManager1->AddPane(process->io_labels,
    wxAuiPaneInfo().Name(_T("IOLabels")).Caption(_("IO Labels")).CaptionVisible().Right().TopDockable(false).BottomDockable(false).MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());

    wxString cfg;
    if (config->Read(_("/AUI_Perspective"),&cfg))
        AuiManager1->LoadPerspective(cfg,true);
    else
        AuiManager1->Update();

    if (!config->Read(_("/Lastdir"),&m_lastDir))
            m_lastDir = m_currentDir;

    Maximize();

    LoadMnemonicsDB();

    // Enable/Check submenus
    wxMenuBar *mb;
    mb = GetMenuBar();
    mb->Enable(idMenuToolsDasmAll,false);
    mb->Enable(idMenuFileInfo,false);
    mb->Enable(idMenuToolsAutoLabel,false);
    mb->Check(idMenuViewDasmWindow,true);
    mb->Enable(idMenuFileSave,false);
    mb->Enable(idMenuFileClose,false);
    mb->Enable(idMenuToolsGenCode,false);

    if (AuiManager1->GetPane(_("VarLabels")).IsShown())
        mb->Check(idMenuViewVarLabels,true);
    else
        mb->Check(idMenuViewVarLabels,false);

    if (AuiManager1->GetPane(_("ProgLabels")).IsShown())
        mb->Check(idMenuViewProgLabels,true);
    else
        mb->Check(idMenuViewProgLabels,false);

    if (AuiManager1->GetPane(_("IOLabels")).IsShown())
        mb->Check(idMenuViewIOLabels,true);
    else
        mb->Check(idMenuViewIOLabels,false);


    wxString path = m_currentDir + _("/HardwareChip.ico");
    icons = new wxIconBundle(path,wxBITMAP_TYPE_ICO);
    SetIcon(icons->GetIcon(wxSize(-1,-1)));

    process->SetLog(PanelLog);
    m_project->SetLog(PanelLog);
    codeview->DebugLog(PanelLog);
    codeview->Enable(false);

    m_commandline = arraystr;
    if (!m_commandline.IsEmpty())
	{
		PanelLog->AppendText(_("Command line arguments :\n"));
		for (i = 0; i < m_commandline.GetCount(); i++)
			PanelLog->AppendText(wxString::Format(_("-> %s <%s>\n"), m_commandline[i].c_str(), wxPathOnly(m_commandline[i])));

	}

}




IDZ80::~IDZ80()
{
    wxString cfg;
    cfg = AuiManager1->SavePerspective();
    config->Write(_("/AUI_Perspective"),cfg);
    config->Write(_("/Lastdir"),m_lastDir);
    AuiManager1->UnInit();
    delete m_project;
    delete config;
    delete icons;
    delete codeview;
    delete process;
	//(*Destroy(IDZ80)
	//*)
}







/*
 *	Process things after main window shows up
 */
void IDZ80::OnFirstIdle(wxIdleEvent &event)
{
	bool result;
	// this is needed to stop cathing this event all the time
	result = Disconnect(wxEVT_IDLE,(wxObjectEventFunction)&IDZ80::OnFirstIdle);

	#ifdef IDZ80DEBUG
	if (!result)
		PanelLog->AppendText(_T("*** First Idle Event failed to remove !\n\n"));
	#endif

    StatusBar1->SetStatusText(m_currentDir);

	if (m_commandline.GetCount() > 1)
	{
		OpenProgramFile(m_commandline[1]);
	}
}





bool IDZ80::LoadMnemonicsDB()
{
    wxString s;
    bool ret;

    s = m_currentDir + _("/Z80_OpCode_AlmostFull.txt");

    PanelLog->SetDefaultStyle(wxTextAttr(*wxBLACK));
    PanelLog->AppendText(_T("Opening mnemonic file:\n"));
    PanelLog->SetDefaultStyle(wxTextAttr(*wxRED));
    PanelLog->AppendText(s);
    PanelLog->AppendText(_T("\n"));

    ret = process->Mnemonics->Open(s);
    if (ret)
    {
        PanelLog->SetDefaultStyle(wxTextAttr(*wxBLACK));
        PanelLog->AppendText(_T("Mnemonics OK !\n"));
    }

    else
    {
        PanelLog->SetDefaultStyle(wxTextAttr(*wxRED));
        PanelLog->AppendText(_T("Mnemonics FAILED !\n"));
    }

    return ret;
}





bool IDZ80::OpenProgramFile(const wxString filename)
{
	bool ret;
	wxString info, caption;
	FileTypeDialog config(this);

	ret = process->Program->Open(filename);
	if (!ret)
	{
		caption.Printf(_("Could not open '%s' !"), filename.c_str());
		wxMessageBox(caption,_("Error..."));
	}
	else
	{
		wxMenuBar *mb;
		mb = GetMenuBar();
		mb->Enable(idMenuToolsDasmAll, true);
		mb->Enable(idMenuFileInfo, true);
		mb->Enable(idMenuFileClose, true);
		mb->Enable(idMenuToolsGenCode, false);
		mb->Enable(idMenuToolsAutoLabel, false);

		wxFileName::SplitPath(filename, 0, 0, &caption, 0, 0);
		UpdateTitle(caption);

		PanelLog->SetDefaultStyle(wxTextAttr(*wxBLACK));
		PanelLog->AppendText(_T("Opened file:\n"));
		PanelLog->SetDefaultStyle(wxTextAttr(*wxRED));
		PanelLog->AppendText(filename);
		PanelLog->AppendText(_T("\n"));
		PanelLog->SetDefaultStyle(wxTextAttr(*wxBLACK));
		PanelLog->AppendText(_T("File size: "));
		PanelLog->SetDefaultStyle(wxTextAttr(*wxRED));
		info.Printf(_("%d bytes\n"),process->Program->GetBufferSize());
		PanelLog->AppendText(info);
		config.SetData(*process->Program);
		config.ShowModal();
		process->Program->StartAddress = config.GetStartAddress();
		process->Program->ExecAddress = config.GetExecAddress();
		process->Program->EndAddress = config.GetEndAddress();
		process->m_Dasm->BaseAddress = config.GetStartAddress();

		Clear_all();
		m_project->New();
		if (config.cb_autodisassemble->IsChecked())
		{
			wxCommandEvent ev_dasm(wxEVT_COMMAND_MENU_SELECTED, idMenuToolsDasmAll);
			AddPendingEvent(ev_dasm);
		}
		if (config.cb_autolabel->IsChecked())
		{
			wxCommandEvent ev_label(wxEVT_COMMAND_MENU_SELECTED, idMenuToolsAutoLabel);
			AddPendingEvent(ev_label);
		}
	}

}


bool IDZ80::OpenProjectFile(const wxString filename)
{
	bool ret;

	ret = m_project->Open(filename);
	if (ret)
	{
		codeview->Enable(true);
		codeview->Plot();
		wxMenuBar *mb;
		mb = GetMenuBar();
		mb->Enable(idMenuToolsDasmAll, true);
		mb->Enable(idMenuFileInfo, true);
		mb->Enable(idMenuFileSave, true);
		mb->Enable(idMenuFileClose, true);
		mb->Enable(idMenuToolsGenCode, true);
		mb->Enable(idMenuToolsAutoLabel, true);
	}
	return ret;
}



void IDZ80::OnMenuFileOpen(wxCommandEvent& event)
{
    wxString fname, caption, wildcard,
            defaultFilename = wxEmptyString;
    bool unknown = false,
         project = false;


    if (event.GetId() == idMenuFileOpenArchive)
    {
        caption = _("Choose a file");
        wildcard = _("Program files (*.ROM, *.COM, *.BIN)|*.rom;*.com;*.bin|All files (*.*)|*.*");
    }
    else
        if (event.GetId() == idMenuFileOpenProject)
        {
            caption = _("Choose a project");
            wildcard = _("Project files (*.dap)|*.dap|All files (*.*)|*.*");
            project = true;
        }
        else
            unknown = true;

    if (!unknown)
    {
        wxFileDialog dialog(this, caption, m_lastDir, defaultFilename,wildcard, wxFD_OPEN);

        // Era : if (dialog.ShowModal() == wxID_OK)
		//DEBUG STUFF
		int dlgret = dialog.ShowModal();
		PanelLog->AppendText(wxString::Format(_("Dialog returns: %d\n"), dlgret));
        if (dlgret == wxID_OK)
        {
            fname = dialog.GetPath();
            m_lastDir = dialog.GetDirectory();
            StatusBar1->SetStatusText(fname,0);
            // Open a project
            if (project)
            {
                // load the project
                Clear_all();
                OpenProjectFile(fname);

            }
            else    //load a file
				OpenProgramFile(fname);

		}
	}
        else
            StatusBar1->SetStatusText(_("Cancelled by user !"));
}



void IDZ80::OnMenuFileQuit(wxCommandEvent& event)
{
    Close();
}




void IDZ80::OnMenuMnemonicsLoad(wxCommandEvent& event)
{
    wxString s;
    if (!LoadMnemonicsDB())
    {
        s=_("Could not open the file !");
        wxMessageBox(s,_("Error.."));
    }
}

void IDZ80::OnMenuMnemonicsInfo(wxCommandEvent& event)
{
    wxString str;
    str.Printf(_("Num of Mnemonics: %d\nMemory allocated: %d bytes        "),process->Mnemonics->GetCount(),
               process->Mnemonics->GetAllocated());
    wxMessageBox(str,_("Mnemonic Info"));
}



void IDZ80::OnMenuHelpAbout(wxCommandEvent& event)
{
    using namespace AutoVersion;
    wxString msg,status(STATUS,wxConvUTF8);
    msg.Printf(_("Interactive Disassembler for\nZ80 processors.\n2009 by Felipe"
                 "\nVersion: %d.%d Build %d\nStatus: "),MAJOR,MINOR,BUILD,REVISION);
    msg << status;
    wxMessageBox(msg, _("About"));
}




void IDZ80::OnMenuToolsDisAsm(wxCommandEvent& event)
{
    wxSize psize,ldsize;
    wxPoint ldpos;
    int w,h,x,y;

    codeview->Enable(false);

    psize=GetClientSize();
    w=psize.GetWidth()/2;
    x=w - w/2 - 5;
    h=w/15;
    y=psize.GetHeight()/2 - h/2 - 5;
    ldsize.Set(w,h);
    ldpos.x=x;
    ldpos.y=y;
    wxGauge *GaugeLd = new wxGauge(this, wxID_ANY, 100, ldpos, ldsize, 0, wxDefaultValidator, _T("wxID_ANY"));

    process->SetGauge(GaugeLd);
    process->DisassembleFirst();
    process->InitData();

    codeview->Enable(true);

    // Debug Area
    wxString stemp;
    stemp.Printf(_("Used memory (dasmed)= %d bytes\n"),process->m_Dasm->GetUsedMem());
    PanelLog->AppendText(stemp);

    delete GaugeLd;
    codeview->Plot();

    wxMenuBar *mb;
    mb=GetMenuBar();
    mb->Enable(idMenuToolsAutoLabel, true);
    mb->Enable(idMenuFileSave, true);
    mb->Enable(idMenuToolsGenCode, true);
}



void IDZ80::OnMenuViewDisassemblyWindow(wxCommandEvent& event)
{
    wxMenuBar *mb;
    bool test=false;
    mb = GetMenuBar();
    test = mb->IsChecked(idMenuViewDasmWindow);
    if (test)
    {
        mb->Check(idMenuViewDasmWindow,true);
        AuiManager1->GetPane(_("MainWindow")).Show();
    }
    else
    {
        mb->Check(idMenuViewDasmWindow,false);
        AuiManager1->GetPane(_("MainWindow")).Hide();
    }
    AuiManager1->Update();
}

void IDZ80::OnAuiPaneClose(wxAuiManagerEvent& event)
{
    wxMenuBar *mb;
    mb = GetMenuBar();
    if (event.pane->name == _("MainWindow"))
        mb->Check(idMenuViewDasmWindow,false);
    if (event.pane->name == _("VarLabels"))
        mb->Check(idMenuViewVarLabels,false);
    if (event.pane->name == _("ProgLabels"))
        mb->Check(idMenuViewProgLabels,false);
    if (event.pane->name == _("IOLabels"))
        mb->Check(idMenuViewIOLabels,false);
}

void IDZ80::OnMenuViewProgramLabels(wxCommandEvent& event)
{
    wxMenuBar *mb;
    bool test=false;
    mb = GetMenuBar();
    test = mb->IsChecked(idMenuViewProgLabels);
    if (test)
    {
        mb->Check(idMenuViewProgLabels,true);
        AuiManager1->GetPane(_("ProgLabels")).Show();
    }
    else
    {
        mb->Check(idMenuViewProgLabels,false);
        AuiManager1->GetPane(_("ProgLabels")).Hide();
    }
    AuiManager1->Update();
}

void IDZ80::OnMenuViewVarLabels(wxCommandEvent& event)
{
    wxMenuBar *mb;
    bool test=false;
    mb = GetMenuBar();
    test = mb->IsChecked(idMenuViewVarLabels);
    if (test)
    {
        mb->Check(idMenuViewVarLabels,true);
        AuiManager1->GetPane(_("VarLabels")).Show();
    }
    else
    {
        mb->Check(idMenuViewVarLabels,false);
        AuiManager1->GetPane(_("VarLabels")).Hide();
    }
    AuiManager1->Update();
}

void IDZ80::OnMenuViewIOLabels(wxCommandEvent& event)
{
    wxMenuBar *mb;
    bool test=false;
    mb = GetMenuBar();
    test = mb->IsChecked(idMenuViewIOLabels);
    if (test)
    {
        mb->Check(idMenuViewIOLabels,true);
        AuiManager1->GetPane(_("IOLabels")).Show();
    }
    else
    {
        mb->Check(idMenuViewIOLabels,false);
        AuiManager1->GetPane(_("IOLabels")).Hide();
    }
    AuiManager1->Update();
}




void IDZ80::OnMenuToolAutoLabel(wxCommandEvent& event)
{
    process->AutoLabel();
    process->processLabel();
    codeview->ClearSelection();
    codeview->Refresh();
}


void IDZ80::SaveAs()
{
    wxString fname, caption, wildcard,
            defaultFilename = wxEmptyString;

    caption = _("Save project as");
    wildcard = _("Project files (*.dap)|*.dap|All files (*.*)|*.*");
    wxFileDialog dialog(this, caption, m_lastDir, defaultFilename,wildcard,
                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    if (dialog.ShowModal() == wxID_OK)
    {
        fname=dialog.GetPath();
        m_project->Save(fname,true);
    }

}


void IDZ80::Clear_all()
{
    codeview->Clear();
    codeview->Enable(false);
    process->io_labels->Clear();
    process->var_labels->Clear();
    process->prog_labels->Clear();
}


void IDZ80::OnMenuFileSaveProject(wxCommandEvent& event)
{
    if (m_project->HasName())
    {
        if (!m_project->Save(true))
            PanelLog->AppendText(_("Project NOT saved !\n"));
        else
            PanelLog->AppendText(_("Project saved !\n"));
    }
    else
        SaveAs();
}




void IDZ80::OnMenuFileInfo(wxCommandEvent& event)
{
    ShowFileInfo dialog(this);
    dialog.SendInfo(process);
    dialog.ShowModal();

}





void IDZ80::OnMenuFileClose(wxCommandEvent& event)
{
    wxMenuBar *mb;
    mb = GetMenuBar();
    mb->Enable(idMenuToolsDasmAll, false);
    mb->Enable(idMenuFileInfo, false);
    mb->Enable(idMenuFileSave, false);
    mb->Enable(idMenuFileClose, false);
    mb->Enable(idMenuToolsGenCode, false);

    UpdateTitle(_(""));

    Clear_all();
}





void IDZ80::OnMenuToolsGenCode(wxCommandEvent& event)
{
    wxString fname, caption, wildcard,
            defaultFilename;

    wxDialog *dasmwin = new wxDialog(0, wxID_ANY, _T("Disassembled Code"), wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU, _T("TextCodeBox"));
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl *textCode = new wxTextCtrl(dasmwin, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    codeGenerator *bakeCode;
    topsizer->Add(textCode, 1, wxEXPAND | wxALL,10);

    dasmwin->SetSizer(topsizer);

    defaultFilename = m_project->GetFilename() << _T(".mac");

    caption = _("Save source code as");
    wildcard = _("Source code files (*.mac)|*.mac|All files (*.*)|*.*");
    wxFileDialog dialog(this, caption, m_lastDir, defaultFilename,wildcard,
                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    if (dialog.ShowModal() == wxID_OK)
    {
        fname = dialog.GetPath();
        bakeCode = new codeGenerator(process);
        //TODO: If viewing the final code is necessary. Replace with scintilla.
        //Since wxTextCtrl doesnt handle more than 32Kb, this is used only for debugging.
        textCode->AppendText(bakeCode->GenerateCode(fname, cfM80));
        dasmwin->ShowModal();

        delete bakeCode;
    }
    dasmwin->Destroy();
}






void IDZ80::UpdateTitle(const wxString str)
{
	wxString info = GetTitle();
	info = info.BeforeFirst('-');
	info.Trim();
	if (!str.IsEmpty())
		info << _(" - ") << str;
	SetTitle(info);
}



