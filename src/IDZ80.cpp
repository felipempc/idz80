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
#include "systemlabels.h"
#include "wx/dir.h"

#include <wx/filefn.h>
#include <wx/filename.h>

#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>

#include "dialog_file_settings.h"


const long IDZ80::ID_TEXTCTRL1 = wxNewId();
const long IDZ80::idMenuFileOpenProject = wxNewId();
const long IDZ80::idMenuFileOpenArchive = wxNewId();
const long IDZ80::idMenuFileOpen = wxNewId();
const long IDZ80::idMenuFileSave = wxNewId();
const long IDZ80::idMenuFileSaveAs = wxNewId();
const long IDZ80::idMenuFileClose = wxNewId();
const long IDZ80::idMenuFileInfo = wxNewId();
const long IDZ80::idMenuFileQuit = wxNewId();
const long IDZ80::idMenuViewDasmWindow = wxNewId();
const long IDZ80::idMenuViewProgLabels = wxNewId();
const long IDZ80::idMenuViewVarLabels = wxNewId();
const long IDZ80::idMenuViewIOLabels = wxNewId();
const long IDZ80::idMenuViewConstLabels = wxNewId();
const long IDZ80::idMenuViewLabels = wxNewId();
const long IDZ80::idMenuToolsDasmAll = wxNewId();
const long IDZ80::idMenuToolsAutoLabel = wxNewId();
const long IDZ80::idMenuToolsGenCode = wxNewId();
const long IDZ80::idMenuMnemLoad = wxNewId();
const long IDZ80::idMenuMnemInfo = wxNewId();
const long IDZ80::idMenuSettingsColors = wxNewId();
const long IDZ80::idMenuSettingsBlur = wxNewId();
const long IDZ80::idMenuHelpContents = wxNewId();
const long IDZ80::IdMenuHelpAbout = wxNewId();
const long IDZ80::ID_STATUSBAR1 = wxNewId();



const long IDZ80::ID_VARLABELPANE = wxNewId();


IDZ80::IDZ80(wxWindow* parent, wxArrayString &arraystr)
{
	wxMenu      *main_menu;
	wxMenu      *sub_menu;
	wxMenuItem  *menu_item;
	wxMenuBar   *main_menu_bar;

	MaximizeMainWindow = true;

	Create(parent, wxID_ANY, "Interactive Disassembler Z80", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, "id");

    Log = new LogWindow(this, "IDZ80 - Log Window");
    SetTextLog(Log);
    ModuleName = "Main";

	SetClientSize(wxSize(600,465));
	Move(wxDefaultPosition);

	AuiManager1 = new wxAuiManager(this, wxAUI_MGR_DEFAULT);

	PanelLog = new wxTextCtrl(this, ID_TEXTCTRL1, "Text", wxPoint(114,460), wxDefaultSize, wxTE_MULTILINE|wxTE_RICH, wxDefaultValidator, "ID_TEXTCTRL1");
	wxFont PanelLogFont(8,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false, "Courier New",wxFONTENCODING_DEFAULT);
	PanelLog->SetFont(PanelLogFont);
	AuiManager1->AddPane(PanelLog, wxAuiPaneInfo().Name("MessageLog").Caption("Message Log").CaptionVisible().Bottom());
	AuiManager1->Update();

	main_menu_bar = new wxMenuBar();
	main_menu = new wxMenu();
	sub_menu = new wxMenu();
	menu_item = new wxMenuItem(sub_menu, idMenuFileOpenProject, "Project\tAlt+r", wxEmptyString, wxITEM_NORMAL);
	sub_menu->Append(menu_item);
	menu_item = new wxMenuItem(sub_menu, idMenuFileOpenArchive, "Archive\tAlt+l", wxEmptyString, wxITEM_NORMAL);
	sub_menu->Append(menu_item);
	main_menu->Append(idMenuFileOpen, "&Open", sub_menu, wxEmptyString);

	menu_item = new wxMenuItem(main_menu, idMenuFileSave, "&Save\tAlt+s", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	menu_item = new wxMenuItem(main_menu, idMenuFileSaveAs, "&Save As...\tAlt+s", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	menu_item = new wxMenuItem(main_menu, idMenuFileClose, "Close", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	main_menu->AppendSeparator();
	menu_item = new wxMenuItem(main_menu, idMenuFileInfo, "&Info\tAlt+I", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	main_menu->AppendSeparator();
	menu_item = new wxMenuItem(main_menu, idMenuFileQuit, "Quit\tAlt+q", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	main_menu_bar->Append(main_menu, "&File");

	main_menu = new wxMenu();
	sub_menu = new wxMenu();
	menu_item = new wxMenuItem(main_menu, idMenuViewDasmWindow, "Disassembly Window", wxEmptyString, wxITEM_CHECK);
	main_menu->Append(menu_item);
	menu_item = new wxMenuItem(sub_menu, idMenuViewProgLabels, "Program", wxEmptyString, wxITEM_CHECK);
	sub_menu->Append(menu_item);
	menu_item = new wxMenuItem(sub_menu, idMenuViewVarLabels, "Variables", wxEmptyString, wxITEM_CHECK);
	sub_menu->Append(menu_item);
	menu_item = new wxMenuItem(sub_menu, idMenuViewIOLabels, "I/O", wxEmptyString, wxITEM_CHECK);
	sub_menu->Append(menu_item);
	menu_item = new wxMenuItem(sub_menu, idMenuViewConstLabels, "Constants", wxEmptyString, wxITEM_CHECK);
	sub_menu->Append(menu_item);
	main_menu->Append(idMenuViewLabels, "&Labels", sub_menu, wxEmptyString);
	main_menu_bar->Append(main_menu, "&View");

	main_menu = new wxMenu();
	menu_item = new wxMenuItem(main_menu, idMenuToolsDasmAll, "Disassemble all\tCTRL+SHIFT+d", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	menu_item = new wxMenuItem(main_menu, idMenuToolsAutoLabel, "Auto Label", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	main_menu->AppendSeparator();
	menu_item = new wxMenuItem(main_menu, idMenuToolsGenCode, "Generate Code", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	main_menu_bar->Append(main_menu, "&Tools");

	main_menu = new wxMenu();
	menu_item = new wxMenuItem(main_menu, idMenuSettingsColors, "Colors", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);

	menu_item = new wxMenuItem(main_menu, idMenuSettingsBlur, "Make Blur", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);


	main_menu_bar->Append(main_menu, "&Settings");



    #ifdef IDZ80DEBUG
	main_menu = new wxMenu();
	menu_item = new wxMenuItem(main_menu, idMenuMnemLoad, "&Load", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	menu_item = new wxMenuItem(main_menu, idMenuMnemInfo, "&Info\tAlt+i", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	main_menu_bar->Append(main_menu, "&Mnemonics");
    #endif

	main_menu = new wxMenu();
	menu_item = new wxMenuItem(main_menu, idMenuHelpContents, "&Contents\tAlt+c", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	main_menu->AppendSeparator();
	menu_item = new wxMenuItem(main_menu, IdMenuHelpAbout, "About\tAlt+b", wxEmptyString, wxITEM_NORMAL);
	main_menu->Append(menu_item);
	main_menu_bar->Append(main_menu, "&Help");

	SetMenuBar(main_menu_bar);

	StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, "ID_STATUSBAR1");
	int __wxStatusBarWidths_1[1] = { -10 };
	int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
	StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
	StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
	SetStatusBar(StatusBar1);

	Bind(wxEVT_IDLE, &IDZ80::OnFirstIdle, this);
}




IDZ80::~IDZ80()
{
    StoreConfiguration();
    AuiManager1->UnInit();
    delete m_project;
    delete config;
    delete icons;
    delete codeview;
    delete process;
}





void IDZ80::StoreConfiguration()
{
    config = new wxConfig("IDZ80");
    wxString cfg;
    int    x, y, width, height;

    cfg = AuiManager1->SavePerspective();
    config->SetPath("/AUI");
    config->Write("Perspective", cfg);

    config->SetPath("/Directory");
    config->Write("Last", m_lastDir);

    GetSize(&width, &height);
    GetPosition(&x, &y);
    config->SetPath("/MainWindow");
    config->Write("X", x);
    config->Write("Y", y);
    config->Write("WIDTH", width);
    config->Write("HEIGHT", height);

    Log->GetSize(&width, &height);
    Log->GetPosition(&x, &y);
    config->SetPath("/LogWindow");
    config->Write("X", x);
    config->Write("Y", y);
    config->Write("WIDTH", width);
    config->Write("HEIGHT", height);
}





/*
 *	Initialize after main window shows up
 */
void IDZ80::OnFirstIdle(wxIdleEvent &event)
{
	bool result;
	// this is needed to stop catching this event all the time
	result = Unbind(wxEVT_IDLE, &IDZ80::OnFirstIdle, this);
	#ifdef IDZ80DEBUG
	if (!result)
		PanelLog->AppendText("*** First Idle Event failed to unbind !\n\n");
	#endif

    m_currentDir = LocalPath;

    SetupIcon();
    SetupMenuItemStatus();

    process = new ProcessData(this, Log);
    codeview = new CodeView(this, process, Log);
    m_project = new ProjectManagerXML(process, codeview);

    SetupAUIPanes();
    ReadStoredConfiguration();
    LoadMnemonicsDB();

    StatusBar1->SetStatusText(m_currentDir);

	if (m_commandline.GetCount() > 1)
	{
		OpenProgramFile(m_commandline[1]);
	}

    codeview->Enable(false);

	Log->Show();
	//TODO: Implement Log in project class
	m_project->SetLog(PanelLog);

	SetupMenuEvents();

    if (MaximizeMainWindow)
        Maximize();


}





void IDZ80::SetupMenuItemStatus()
{
    wxMenuBar *mb;
    mb = GetMenuBar();
    mb->Enable(idMenuToolsDasmAll, false);
    mb->Enable(idMenuFileInfo, false);
    mb->Enable(idMenuToolsAutoLabel, false);
    mb->Check(idMenuViewDasmWindow, true);
    mb->Enable(idMenuFileSave, false);
    mb->Enable(idMenuFileSaveAs, false);
    mb->Enable(idMenuFileClose, false);
    mb->Enable(idMenuToolsGenCode, false);

    if (AuiManager1->GetPane("VarLabels").IsShown())
        mb->Check(idMenuViewVarLabels, true);
    else
        mb->Check(idMenuViewVarLabels, false);

    if (AuiManager1->GetPane("ProgLabels").IsShown())
        mb->Check(idMenuViewProgLabels, true);
    else
        mb->Check(idMenuViewProgLabels, false);

    if (AuiManager1->GetPane("IOLabels").IsShown())
        mb->Check(idMenuViewIOLabels, true);
    else
        mb->Check(idMenuViewIOLabels, false);

    if (AuiManager1->GetPane("ConstLabels").IsShown())
        mb->Check(idMenuViewConstLabels, true);
    else
        mb->Check(idMenuViewConstLabels, false);
}



bool IDZ80::LoadMnemonicsDB()
{
    wxString s;
    bool ret;

    s = m_currentDir + "/Z80_OpCode_AlmostFull.txt";

    PanelLog->SetDefaultStyle(wxTextAttr(*wxBLACK));
    PanelLog->AppendText("Opening mnemonic file:\n");
    PanelLog->SetDefaultStyle(wxTextAttr(*wxRED));
    PanelLog->AppendText(s);
    PanelLog->AppendText("\n");

    ret = process->Mnemonics->Open(s);
    if (ret)
    {
        PanelLog->SetDefaultStyle(wxTextAttr(*wxBLACK));
        PanelLog->AppendText("Mnemonics OK !\n");
        LogIt("Mnemonics OK !\n");
    }

    else
    {
        PanelLog->SetDefaultStyle(wxTextAttr(*wxRED));
        PanelLog->AppendText("Mnemonics FAILED !\n");
    }

    return ret;
}




void IDZ80::SetupIcon()
{
    wxString path = m_currentDir + "/HardwareChip.ico";
    icons = new wxIconBundle(path, wxBITMAP_TYPE_ICO);
    SetIcon(icons->GetIcon(wxSize(-1,-1)));
}



void IDZ80::SetupAUIPanes()
{
    if ((codeview != 0) && (process != 0))
    {
        AuiManager1->AddPane(codeview, wxAuiPaneInfo().Name("MainWindow").Caption("Disassembly Window").CaptionVisible().CenterPane().MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());
        AuiManager1->AddPane(process->var_labels,
        wxAuiPaneInfo().Name("VarLabels").Caption("Var Labels").CaptionVisible().Left().TopDockable(false).BottomDockable(false).MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());
        AuiManager1->AddPane(process->prog_labels,
        wxAuiPaneInfo().Name("ProgLabels").Caption("Program Labels").CaptionVisible().Left().TopDockable(false).BottomDockable(false).MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());
        AuiManager1->AddPane(process->io_labels,
        wxAuiPaneInfo().Name("IOLabels").Caption("IO Labels").CaptionVisible().Right().TopDockable(false).BottomDockable(false).MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());
        AuiManager1->AddPane(process->constant_labels,
        wxAuiPaneInfo().Name("ConstLabels").Caption("Constant Labels").CaptionVisible().Right().TopDockable(false).BottomDockable(false).MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());

        SetupAUIStoredConfiguration();
    }
}



void IDZ80::SetupAUIStoredConfiguration()
{
    config = new wxConfig("IDZ80");

    wxString cfg;

    config->SetPath("/AUI");
    if (config->Read("Perspective", &cfg))
        AuiManager1->LoadPerspective(cfg, true);
    else
        AuiManager1->Update();
}



void IDZ80::ReadStoredConfiguration()
{
    long x, y, width, height;

    config = new wxConfig("IDZ80");

    config->SetPath("/Directory");
    if (config->Read("Last", &m_lastDir))
    {
        wxDir dir;
        if (!dir.Exists(m_lastDir))
            m_lastDir = m_currentDir;
    }
    else
        m_lastDir = m_currentDir;

    config->SetPath("/MainWindow");
    if (config->Read("X", &x) &&
        config->Read("Y", &y) &&
        config->Read("WIDTH", &width) &&
        config->Read("HEIGHT", &height))
    {
        SetPosition(wxPoint(x, y));
        SetSize(width, height);
        MaximizeMainWindow = false;
    }

    config->SetPath("/LogWindow");
    if (config->Read("X", &x) &&
        config->Read("Y", &y) &&
        config->Read("WIDTH", &width) &&
        config->Read("HEIGHT", &height))
    {
        Log->SetPosition(wxPoint(x, y));
        Log->SetSize(width, height);
    }
}




void IDZ80::SetupMenuEvents()
{
	AuiManager1->Bind(wxEVT_AUI_PANE_CLOSE, &IDZ80::OnAuiPaneClose, this);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuFileOpen, this, idMenuFileOpenProject);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuFileOpen, this, idMenuFileOpenArchive);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuFileOpen, this, idMenuFileOpen);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuFileSaveProject, this, idMenuFileSave);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuFileSaveAsProject, this, idMenuFileSaveAs);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuFileClose, this, idMenuFileClose);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuFileInfo, this, idMenuFileInfo);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuFileQuit, this, idMenuFileQuit);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuViewDisassemblyWindow, this, idMenuViewDasmWindow);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuViewProgramLabels, this, idMenuViewProgLabels);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuViewVarLabels, this, idMenuViewVarLabels);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuViewIOLabels, this, idMenuViewIOLabels);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuViewConstLabels, this, idMenuViewConstLabels);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuToolsDisAsm, this, idMenuToolsDasmAll);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuToolAutoLabel, this, idMenuToolsAutoLabel);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuToolsGenCode, this, idMenuToolsGenCode);

	#ifdef IDZ80DEBUG
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuMnemonicsLoad, this, idMenuMnemLoad);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuMnemonicsInfo, this, idMenuMnemInfo);
	#endif

	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuSettingsColor, this, idMenuSettingsColors);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuSettingsBlur, this, idMenuSettingsBlur);

	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuHelpAbout, this, IdMenuHelpAbout);
}




//bool IDZ80::OpenProgramFile(const wxString filename)
void IDZ80::OpenProgramFile(wxString filename)
{
	static bool simulateexecution = false;
	wxString    info,
				caption;
	//FileTypeDialog config(this);
	FileSettingsDialog config(process->Program);

    if (filename.IsEmpty())
        filename = DialogLoadProgramFile();

    if ((!filename.IsEmpty()) && process->Program->Open(filename))
    {
		PanelLog->SetDefaultStyle(wxTextAttr(*wxBLACK));
		PanelLog->AppendText("Opened file:\n");
		PanelLog->SetDefaultStyle(wxTextAttr(*wxRED));
		PanelLog->AppendText(filename);
		PanelLog->AppendText("\n");
		PanelLog->SetDefaultStyle(wxTextAttr(*wxBLACK));
		PanelLog->AppendText("File size: ");
		PanelLog->SetDefaultStyle(wxTextAttr(*wxRED));
		info.Printf("%d bytes\n",process->Program->GetBufferSize());
		PanelLog->AppendText(info);

		config.SetData();
		if (config.ShowModal() == wxID_OK)
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

            if (process->SetupSystemLabels())
            {
                process->LoadSystemLabels("X:/idz80/Labels.txt");
            }

            /*FIXME: If user change the addresses, it won't appear here, because
             * they are not converted from textcntrl objects to integer
             * variables of FileTypeDialog.
             */
            process->Program->StartAddress = config.GetStartAddress();
            process->Program->ExecAddress = config.GetExecAddress();
            process->Program->EndAddress = config.GetEndAddress();

            process->Disassembled->AddOrgAddress(0, process->Program->StartAddress);

            m_project->New();
            if (config.WantsAutoDisassembly())
            {
                simulateexecution = config.WantsSimulateExecution();

                wxCommandEvent ev_dasm(wxEVT_COMMAND_MENU_SELECTED, idMenuToolsDasmAll);
                ev_dasm.SetClientData(&simulateexecution);
                AddPendingEvent(ev_dasm);
            }
        }
        else
        {
            process->Program->Close();
            StatusBar1->SetStatusText("Cancelled by user !");
        }
    }
	else
	{
		caption.Printf("Could not open '%s' !", filename.c_str());
		wxMessageBox(caption, "Error...");
	}
}



wxString IDZ80::DialogLoadProgramFile()
{
    wxString caption = "Choose a file";
    wxString wildcard = "Program files (*.ROM, *.COM, *.BIN)|*.rom;*.com;*.bin|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, m_lastDir, wxEmptyString, wildcard, wxFD_OPEN);
    wxString ret = "";

    if (dialog.ShowModal() == wxID_OK)
    {
        ret = dialog.GetPath();
        m_lastDir = dialog.GetDirectory();
    }

    return ret;
}




//bool IDZ80::OpenProjectFile(const wxString filename)
void IDZ80::OpenProjectFile()
{
	wxString filename;

	filename = DialogLoadProjectFile();
	if ((!filename.IsEmpty()) && m_project->Open(filename))
    {
		codeview->Enable(true);
		codeview->Plot();
		wxMenuBar *mb;
		mb = GetMenuBar();
		mb->Enable(idMenuToolsDasmAll, true);
		mb->Enable(idMenuFileInfo, true);
		mb->Enable(idMenuFileSave, true);
		mb->Enable(idMenuFileSaveAs, true);
		mb->Enable(idMenuFileClose, true);
		mb->Enable(idMenuToolsGenCode, true);
		mb->Enable(idMenuToolsAutoLabel, true);
	}
}



wxString IDZ80::DialogLoadProjectFile()
{
    wxString caption = "Choose a project";
    wxString wildcard = "Project files (*.dap)|*.dap|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, m_lastDir, wxEmptyString, wildcard, wxFD_OPEN);
    wxString ret = "";

    if (dialog.ShowModal() == wxID_OK)
    {
        ret = dialog.GetPath();
        m_lastDir = dialog.GetDirectory();
    }
    return ret;
}




void IDZ80::OnMenuFileOpen(wxCommandEvent& event)
{
    Clear_all();
    if (event.GetId() == idMenuFileOpenArchive)
        OpenProgramFile();
    else
        OpenProjectFile();
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
        s = "Could not open the file !";
        wxMessageBox(s, "Error..");
    }
}

void IDZ80::OnMenuMnemonicsInfo(wxCommandEvent& event)
{
    wxString str;
    str.Printf("Num of Mnemonics: %d\nMemory allocated: %d bytes\nSyscalls = %d\nSysVars = %d\nSysIO = %d ",
    process->Mnemonics->GetCount(), process->Mnemonics->GetAllocated(), process->sys_calls->GetCount(),
    process->sys_vars->GetCount(), process->sys_io->GetCount());
    wxMessageBox(str, "Mnemonic Info");
}



void IDZ80::OnMenuHelpAbout(wxCommandEvent& event)
{
    using namespace AutoVersion;
    wxString msg,status(STATUS,wxConvUTF8);
    msg.Printf("Interactive Disassembler for\nZ80 processors.\n2009 by Felipe"
                 "\nVersion: %d.%d Build %d\nStatus: ",MAJOR,MINOR,BUILD,REVISION);
    msg << status;
    wxMessageBox(msg, "About");
}




void IDZ80::OnMenuToolsDisAsm(wxCommandEvent& event)
{
    wxSize  psize,ldsize;
    wxPoint ldpos;
    int     w,h,x,y;
    SortedIntArray  entries(CompareSortedInt);
    bool *simulateexecution;

    simulateexecution = (bool *)event.GetClientData();
    codeview->Enable(false);

    psize = GetClientSize();
    w = psize.GetWidth() / 2;
    x = w - w / 2 - 5;
    h = w / 15;
    y = psize.GetHeight() / 2 - h / 2 - 5;
    ldsize.Set(w, h);
    ldpos.x = x;
    ldpos.y = y;
    wxGauge *GaugeLd = new wxGauge(this, wxID_ANY, 100, ldpos, ldsize, 0, wxDefaultValidator, "wxID_ANY");

    process->SetGauge(GaugeLd);
    process->DisassembleFirst(*simulateexecution);
    process->InitData();
    process->processLabel();
    process->prog_labels->SortAddress(true);
    process->io_labels->SortAddress(true);
    process->var_labels->SortAddress(true);
    process->constant_labels->SortAddress(true);

    codeview->Enable(true);

    #ifdef IDZ80DEBUG
    wxString stemp;
    stemp.Printf("Used memory (dasmed)= %d bytes\n", process->Disassembled->GetUsedMem());
    PanelLog->AppendText(stemp);
    #endif

    delete GaugeLd;
    codeview->Plot();

    wxMenuBar *mb;
    mb = GetMenuBar();
    mb->Enable(idMenuToolsAutoLabel, true);
    mb->Enable(idMenuFileSave, true);
    mb->Enable(idMenuFileSaveAs, true);
    mb->Enable(idMenuToolsGenCode, true);

    process->Program->GetEntries(entries);

    if (!entries.IsEmpty())
        codeview->CenterAddress(entries[0]);
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
        AuiManager1->GetPane("MainWindow").Show();
    }
    else
    {
        mb->Check(idMenuViewDasmWindow,false);
        AuiManager1->GetPane("MainWindow").Hide();
    }
    AuiManager1->Update();
}





void IDZ80::OnAuiPaneClose(wxAuiManagerEvent& event)
{
    wxMenuBar *mb;
    mb = GetMenuBar();
    if (event.pane->name == "MainWindow")
        mb->Check(idMenuViewDasmWindow, false);
    if (event.pane->name == "VarLabels")
        mb->Check(idMenuViewVarLabels, false);
    if (event.pane->name == "ProgLabels")
        mb->Check(idMenuViewProgLabels, false);
    if (event.pane->name == "IOLabels")
        mb->Check(idMenuViewIOLabels, false);
    if (event.pane->name == "ConstLabels")
        mb->Check(idMenuViewConstLabels, false);
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
        AuiManager1->GetPane("ProgLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewProgLabels, false);
        AuiManager1->GetPane("ProgLabels").Hide();
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
        AuiManager1->GetPane("VarLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewVarLabels,false);
        AuiManager1->GetPane("VarLabels").Hide();
    }
    AuiManager1->Update();
}




void IDZ80::OnMenuViewIOLabels(wxCommandEvent& event)
{
    wxMenuBar *mb;
    bool test = false;
    mb = GetMenuBar();
    test = mb->IsChecked(idMenuViewIOLabels);
    if (test)
    {
        mb->Check(idMenuViewIOLabels,true);
        AuiManager1->GetPane("IOLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewIOLabels,false);
        AuiManager1->GetPane("IOLabels").Hide();
    }
    AuiManager1->Update();
}




void IDZ80::OnMenuViewConstLabels(wxCommandEvent& event)
{
    wxMenuBar *mb;
    bool test=false;
    mb = GetMenuBar();
    test = mb->IsChecked(idMenuViewConstLabels);
    if (test)
    {
        mb->Check(idMenuViewConstLabels,true);
        AuiManager1->GetPane("ConstLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewConstLabels,false);
        AuiManager1->GetPane("ConstLabels").Hide();
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



void IDZ80::OnMenuSettingsColor(wxCommandEvent& event)
{
    LogIt("Change colors !");
}


void IDZ80::OnMenuSettingsBlur(wxCommandEvent& event)
{
    LogIt("Event: Make Blur...");
    codeview->TestBlur();
}



bool IDZ80::SaveAs()
{
    wxString fname, caption, wildcard,
            defaultFilename = wxEmptyString;
    bool    ret;

    ret = false;
    caption = "Save project as";
    wildcard = "Project files (*.dap)|*.dap|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, m_lastDir, defaultFilename,wildcard,
                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    if (dialog.ShowModal() == wxID_OK)
    {
        fname = dialog.GetPath();
        if (m_project->Save(fname))
            ret = true;
    }
    return ret;
}





void IDZ80::Clear_all()
{
    codeview->Clear();
    codeview->Enable(false);
    process->Clear();
}




void IDZ80::OnMenuFileSaveProject(wxCommandEvent& event)
{
    if (m_project->HasName())
    {
        if (!m_project->Save())
            PanelLog->AppendText("Project NOT saved !\n");
        else
            PanelLog->AppendText("Project saved !\n");
    }
    else
        if (SaveAs())
        {
            UpdateTitle(m_project->GetFilename());
        }

}




void IDZ80::OnMenuFileSaveAsProject(wxCommandEvent& event)
{
    if (SaveAs())
    {
        UpdateTitle(m_project->GetFilename());
    }
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
    mb->Enable(idMenuFileSaveAs, false);
    mb->Enable(idMenuFileClose, false);
    mb->Enable(idMenuToolsGenCode, false);

    UpdateTitle("");

    Clear_all();
}





void IDZ80::OnMenuToolsGenCode(wxCommandEvent& event)
{
    wxString fname, caption, wildcard,
            defaultFilename;

    wxDialog *dasmwin = new wxDialog(0, wxID_ANY, "Disassembled Code", wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU, "TextCodeBox");
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl *textCode = new wxTextCtrl(dasmwin, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    codeGenerator *bakeCode;
    topsizer->Add(textCode, 1, wxEXPAND | wxALL, 10);

    dasmwin->SetSizer(topsizer);

    defaultFilename = m_project->GetFilename() << ".mac";

    caption = "Save source code as";
    wildcard = "Source code files (*.mac)|*.mac|All files (*.*)|*.*";
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
		info << " - " << str;
	SetTitle(info);
}



