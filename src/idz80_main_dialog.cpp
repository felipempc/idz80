/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Main module
 **************************************************************/

#include <wx/filedlg.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/icon.h>
#include <wx/string.h>
#include <wx/msgdlg.h>

#include "idz80_main_dialog.h"
#include "version.h"
#include "fileinfo_dialog.h"
#include "systemlabels.h"
#include "file_settings_dialog.h"
#include "mnemonicxmlfile.h"
#include "newproject_dialog.h"

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
const long IDZ80::IdMenuMnemonicXML = wxNewId();
const long IDZ80::ID_VARLABELPANE = wxNewId();


IDZ80::IDZ80(wxWindow* parent, wxArrayString &arraystr)
{
    aui_mgr_ = 0;
    panel_log_ = 0;
    status_bar_ = 0;
    log_window_ = 0;
//    process_ = 0;
//    codeview_ = 0;
    config_ = 0;
//    project_ = 0;
	maximize_main_window_ = true;
//	CodeViewLines_ = 0;
    Disassembled_ = 0;
    Programs_ = 0;
    Mnemonics_ = 0;
    Labels_ = 0;

	Create(parent, wxID_ANY, "Interactive Disassembler Z80", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, "id");

    log_window_ = new LogWindow(this, "IDZ80 - Log Window");
    SetTextLog(log_window_);
    ModuleName = "Main";

    SetupIcon();

	SetClientSize(wxSize(600,465));
	Move(wxDefaultPosition);

	SetupPanelLog();
	aui_mgr_ = new wxAuiManager(this, wxAUI_MGR_DEFAULT);
	aui_mgr_->AddPane(panel_log_, wxAuiPaneInfo().Name("MessageLog").Caption("Message Log").CaptionVisible().Bottom());
	aui_mgr_->Update();

    SetupMenu();
    SetupStatusBar();

	Bind(wxEVT_IDLE, &IDZ80::OnFirstIdle, this);

}




IDZ80::~IDZ80()
{
    StoreConfiguration();
    aui_mgr_->UnInit();
//    delete project_;
    delete config_;
//    delete codeview_;
//    delete process_;
}




void IDZ80::StoreConfiguration()
{
    config_ = new wxConfig("IDZ80");
    wxString cfg;
    int    x, y, width, height;

    cfg = aui_mgr_->SavePerspective();
    config_->SetPath("/AUI");
    config_->Write("Perspective", cfg);

    config_->SetPath("/Directory");
    config_->Write("Last", fileopen_last_dir_);

    GetSize(&width, &height);
    GetPosition(&x, &y);
    config_->SetPath("/MainWindow");
    config_->Write("X", x);
    config_->Write("Y", y);
    config_->Write("WIDTH", width);
    config_->Write("HEIGHT", height);

    log_window_->GetSize(&width, &height);
    log_window_->GetPosition(&x, &y);
    config_->SetPath("/LogWindow");
    config_->Write("X", x);
    config_->Write("Y", y);
    config_->Write("WIDTH", width);
    config_->Write("HEIGHT", height);
}




void IDZ80::SetupLabels()
{
    Labels_->io_labels = new LabelListCtrl(this, IO_LIST,"PORT", log_window_);
    Labels_->var_labels = new LabelListCtrl(this, VAR_LIST,"VAR", log_window_);
    Labels_->prog_labels = new LabelListCtrl(this, PRG_LIST,"PROGRAM", log_window_);
    Labels_->constant_labels = new LabelListCtrl(this, CONST_LIST,"CONST", log_window_);
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
		panel_log_->AppendText("*** First Idle Event failed to unbind !\n\n");
	#endif

    app_root_dir_ = wxGetCwd();
    app_resource_dir_ = app_root_dir_ + "\\" + ResourceDir;
    fileopen_last_dir_ = "";

    SetupMenuItemStatus();

//    process_ = new ProcessData(this, log_window_);
//    codeview_ = new CodeView(this, process_, log_window_);
//    project_ = new ProjectManagerXML(this);
//    Programs_ = new RawDataManager(log_window_);


//    Labels_ = new LabelManager();
//    SetupLabels();

//    SetupAUIPanes();
//    ReadStoredConfiguration();
    LoadMnemonicsDB();


    status_bar_->SetStatusText(app_root_dir_);

    /*
	if (m_commandline.GetCount() > 1)
	{
		OpenProgramFile(m_commandline[1]);
	}
    */

//    codeview_->Enable(false);

	log_window_->Show();

	//TODO: Implement Log in project class
	//project_->SetLog(panel_log_);

//	SetupMenuEvents();


    if (maximize_main_window_)
        Maximize();

//    NewProjectDialog dialog(this);
//    dialog.ShowModal();

/*
    if (Programs_->First()->IsLoaded())
    {
        wxMenuBar *mb;
        mb = GetMenuBar();
        mb->Enable(idMenuToolsDasmAll, true);
        mb->Enable(idMenuFileInfo, true);
        mb->Enable(idMenuFileClose, true);
        mb->Enable(idMenuToolsGenCode, false);
        mb->Enable(idMenuToolsAutoLabel, false);
    }
*/
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

    if (aui_mgr_->GetPane("VarLabels").IsShown())
        mb->Check(idMenuViewVarLabels, true);
    else
        mb->Check(idMenuViewVarLabels, false);

    if (aui_mgr_->GetPane("ProgLabels").IsShown())
        mb->Check(idMenuViewProgLabels, true);
    else
        mb->Check(idMenuViewProgLabels, false);

    if (aui_mgr_->GetPane("IOLabels").IsShown())
        mb->Check(idMenuViewIOLabels, true);
    else
        mb->Check(idMenuViewIOLabels, false);

    if (aui_mgr_->GetPane("ConstLabels").IsShown())
        mb->Check(idMenuViewConstLabels, true);
    else
        mb->Check(idMenuViewConstLabels, false);
}




bool IDZ80::LoadMnemonicsDB()
{
    wxString s;
    bool ret = false;

    s = app_resource_dir_ + "\\z80_instructions.xml";

    panel_log_->SetDefaultStyle(wxTextAttr(*wxBLACK));
    panel_log_->AppendText("Opening mnemonic file:\n");
    panel_log_->SetDefaultStyle(wxTextAttr(*wxRED));
    panel_log_->AppendText(s);
    panel_log_->AppendText("\n");

    if(Mnemonics_ == 0)
        Mnemonics_ = new MnemonicContainer(panel_log_);
    else
        Mnemonics_->Clear();
    MnemonicXMLFile mnemonic(Mnemonics_, panel_log_);

    ret = mnemonic.Open(s);

    if (ret)
    {
        panel_log_->SetDefaultStyle(wxTextAttr(*wxBLACK));
        panel_log_->AppendText("Mnemonics OK !\n");
        LogIt("Mnemonics OK !\n");
    }
    else
    {
        panel_log_->SetDefaultStyle(wxTextAttr(*wxRED));
        panel_log_->AppendText("Mnemonics FAILED !\n");
    }

    return ret;
}




void IDZ80::SetupIcon()
{
    wxIcon icon(wxICON(IDI_APPICON));  // IDI_APPICON in resources.rc

    // Check if the icon loaded successfully
    if (icon.IsOk()) {
        SetIcon(icon);
    } else {
        LogIt("Failed to load application icon from resources.\n");
    }
}




void IDZ80::SetupStatusBar()
{
	status_bar_ = new wxStatusBar(this, ID_STATUSBAR1, 0, "ID_STATUSBAR");
	int __wxStatusBarWidths_1[1] = { -10 };
	int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
	status_bar_->SetFieldsCount(1,__wxStatusBarWidths_1);
	status_bar_->SetStatusStyles(1,__wxStatusBarStyles_1);
	SetStatusBar(status_bar_);
}




void IDZ80::SetupPanelLog()
{
	panel_log_ = new wxTextCtrl(this, ID_TEXTCTRL1, "Text", wxPoint(114,460), wxDefaultSize, wxTE_MULTILINE|wxTE_RICH, wxDefaultValidator, "ID_TEXTCTRL1");
	wxFont PanelLogFont(8, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, false, "Courier New", wxFONTENCODING_DEFAULT);
	panel_log_->SetFont(PanelLogFont);
}




void IDZ80::SetupAUIPanes()
{
//    if ((codeview_ != 0) && (Labels_ != 0))
    {
//        aui_mgr_->AddPane(codeview_, wxAuiPaneInfo().Name("MainWindow").Caption("Disassembly Window").CaptionVisible().CenterPane().PaneBorder().MinSize(170,170)/*.DockFixed()*/.FloatingSize(170,170)/*.Fixed()*/);
        aui_mgr_->AddPane(Labels_->var_labels,
        wxAuiPaneInfo().Name("VarLabels").Caption("Var Labels").CaptionVisible().Left()/*.TopDockable(false).BottomDockable(false)*/.PaneBorder().MinSize(170,170)/*.DockFixed()*/.FloatingSize(170,170)/*.Fixed()*/);
        aui_mgr_->AddPane(Labels_->prog_labels,
        wxAuiPaneInfo().Name("ProgLabels").Caption("Program Labels").CaptionVisible().Left()/*.TopDockable(false).BottomDockable(false)*/.PaneBorder().MinSize(170,170)/*.DockFixed()*/.FloatingSize(170,170)/*.Fixed()*/);
        aui_mgr_->AddPane(Labels_->io_labels,
        wxAuiPaneInfo().Name("IOLabels").Caption("IO Labels").CaptionVisible().Right()/*.TopDockable(false).BottomDockable(false)*/.PaneBorder().MinSize(170,170)/*.DockFixed()*/.FloatingSize(170,170)/*.Fixed()*/);
        aui_mgr_->AddPane(Labels_->constant_labels,
        wxAuiPaneInfo().Name("ConstLabels").Caption("Constant Labels").CaptionVisible().Right()/*.TopDockable(false).BottomDockable(false)*/.PaneBorder().MinSize(170,170)/*.DockFixed()*/.FloatingSize(170,170)/*.Fixed()*/);

        SetupAUIStoredConfiguration();
    }
}



void IDZ80::SetupAUIStoredConfiguration()
{
    config_ = new wxConfig("IDZ80");

    wxString cfg;

    config_->SetPath("/AUI");
    if (config_->Read("Perspective", &cfg))
        aui_mgr_->LoadPerspective(cfg, true);
    else
        aui_mgr_->Update();
}



void IDZ80::ReadStoredConfiguration()
{
    long x, y, width, height;

    config_ = new wxConfig("IDZ80");

    config_->SetPath("/Directory");
    if (config_->Read("Last", &fileopen_last_dir_))
    {
        wxDir dir;
        if (!dir.Exists(fileopen_last_dir_))
            fileopen_last_dir_ = app_root_dir_;
    }
    else
        fileopen_last_dir_ = app_root_dir_;

    config_->SetPath("/MainWindow");
    if (config_->Read("X", &x) &&
        config_->Read("Y", &y) &&
        config_->Read("WIDTH", &width) &&
        config_->Read("HEIGHT", &height))
    {
        SetPosition(wxPoint(x, y));
        SetSize(width, height);
        maximize_main_window_ = false;
    }

    config_->SetPath("/LogWindow");
    if (config_->Read("X", &x) &&
        config_->Read("Y", &y) &&
        config_->Read("WIDTH", &width) &&
        config_->Read("HEIGHT", &height))
    {
        log_window_->SetPosition(wxPoint(x, y));
        log_window_->SetSize(width, height);
    }
}




void IDZ80::SetupMenuEvents()
{
	aui_mgr_->Bind(wxEVT_AUI_PANE_CLOSE, &IDZ80::OnAuiPaneClose, this);
	Bind(wxEVT_MENU, &IDZ80::OnMenuFileOpen, this, idMenuFileOpenProject);
	Bind(wxEVT_MENU, &IDZ80::OnMenuFileOpen, this, idMenuFileOpenArchive);
	Bind(wxEVT_MENU, &IDZ80::OnMenuFileOpen, this, idMenuFileOpen);
	Bind(wxEVT_MENU, &IDZ80::OnMenuFileSaveProject, this, idMenuFileSave);
	Bind(wxEVT_MENU, &IDZ80::OnMenuFileSaveAsProject, this, idMenuFileSaveAs);
	Bind(wxEVT_MENU, &IDZ80::OnMenuFileClose, this, idMenuFileClose);
	Bind(wxEVT_MENU, &IDZ80::OnMenuFileInfo, this, idMenuFileInfo);
	Bind(wxEVT_MENU, &IDZ80::OnMenuFileQuit, this, idMenuFileQuit);
	Bind(wxEVT_MENU, &IDZ80::OnMenuViewDisassemblyWindow, this, idMenuViewDasmWindow);
	Bind(wxEVT_MENU, &IDZ80::OnMenuViewProgramLabels, this, idMenuViewProgLabels);
	Bind(wxEVT_MENU, &IDZ80::OnMenuViewVarLabels, this, idMenuViewVarLabels);
	Bind(wxEVT_MENU, &IDZ80::OnMenuViewIOLabels, this, idMenuViewIOLabels);
	Bind(wxEVT_MENU, &IDZ80::OnMenuViewConstLabels, this, idMenuViewConstLabels);
	Bind(wxEVT_MENU, &IDZ80::OnMenuToolsDisAsm, this, idMenuToolsDasmAll);
	Bind(wxEVT_MENU, &IDZ80::OnMenuToolAutoLabel, this, idMenuToolsAutoLabel);
	Bind(wxEVT_MENU, &IDZ80::OnMenuToolsGenCode, this, idMenuToolsGenCode);
	Bind(wxEVT_MENU, &IDZ80::OnMenuToolMnemonicXML, this, IdMenuMnemonicXML);

	#ifdef IDZ80DEBUG
	Bind(wxEVT_MENU, &IDZ80::OnMenuMnemonicsLoad, this, idMenuMnemLoad);
	Bind(wxEVT_MENU, &IDZ80::OnMenuMnemonicsInfo, this, idMenuMnemInfo);
	#endif

	Bind(wxEVT_MENU, &IDZ80::OnMenuSettingsColor, this, idMenuSettingsColors);
	Bind(wxEVT_MENU, &IDZ80::OnMenuSettingsBlur, this, idMenuSettingsBlur);

	Bind(wxEVT_MENU, &IDZ80::OnMenuHelpAbout, this, IdMenuHelpAbout);
}


void IDZ80::SetupMenu()
{
	wxMenu      *main_menu;
	wxMenu      *sub_menu;
	wxMenuItem  *menu_item;
	wxMenuBar   *main_menu_bar;

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
	main_menu->AppendSeparator();

	menu_item = new wxMenuItem(main_menu, IdMenuMnemonicXML, "Mnemonic XML", wxEmptyString, wxITEM_NORMAL);
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
}




void IDZ80::OpenProgramFile(wxString filename)
{
    /* ----    OLD     ---------
	static bool simulateexecution = false;
	wxString    info,
				caption;
	FileSettingsDialog config(process->Program);

    if (filename.IsEmpty())
        filename = DialogLoadProgramFile();

    if ((!filename.IsEmpty()) && process->Program->Open(filename))
    {
        if (process->Program->HasBasic())
        {
            wxMessageBox("This ROM file has a program written in BASIC. IDZ80 don't support it.", "File type unsupported !");
            process->Program->Close();
            return;
        }
		PanelLog->SetDefaultStyle(wxTextAttr(*wxBLACK));
		PanelLog->AppendText("Opened file:\n");
		PanelLog->SetDefaultStyle(wxTextAttr(*wxRED));
		PanelLog->AppendText(filename);
		PanelLog->AppendText("\n");
		PanelLog->SetDefaultStyle(wxTextAttr(*wxBLACK));
		PanelLog->AppendText("File size: ");
		PanelLog->SetDefaultStyle(wxTextAttr(*wxRED));
		info.Printf("%d bytes\n",process->Program->GetSize());
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
                process->LoadSystemLabels(m_currentDir + "/resource/Labels.txt");
            }

            process->Program->StartAddress = config.GetStartAddress();
            process->Program->ExecAddress = config.GetExecAddress();
            process->Program->EndAddress = config.GetEndAddress();

            process->Disassembled->AddOrgAddress(0, process->Program->StartAddress);

            m_project->New();
            if (config.WantsAutoDisassembly())
            {
                simulateexecution = config.WantsSimulateExecution();

                wxCommandEvent ev_dasm(wxEVT_MENU, idMenuToolsDasmAll);
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
	*/

}


/*
wxString IDZ80::DialogLoadProgramFile()
{
    wxString caption = "Choose a file";
    wxString wildcard = "Program files (*.ROM, *.COM, *.BIN)|*.rom;*.com;*.bin|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, fileopen_last_dir_, wxEmptyString, wildcard, wxFD_OPEN);
    wxString ret = "";

    if (dialog.ShowModal() == wxID_OK)
    {
        ret = dialog.GetPath();
        fileopen_last_dir_ = dialog.GetDirectory();
    }

    return ret;
}
*/



//bool IDZ80::OpenProjectFile(const wxString filename)
void IDZ80::OpenProjectFile()
{
/*
	wxString filename;

	filename = DialogLoadProjectFile();
	if ((!filename.IsEmpty()) && project_->Open(filename, app_resource_dir_ + "Labels.txt"))
    {
//		codeview_->Enable(true);
//		codeview_->Plot();
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
*/
}



wxString IDZ80::DialogLoadProjectFile()
{
    wxString caption = "Choose a project";
    wxString wildcard = "Project files (*.dap)|*.dap|All files (*.*)|*.*";
//    wxFileDialog dialog(this, caption, fileopen_last_dir_, wxEmptyString, wildcard, wxFD_OPEN);
    wxString ret = "";

/*    if (dialog.ShowModal() == wxID_OK)
    {
        ret = dialog.GetPath();
        fileopen_last_dir_ = dialog.GetDirectory();
    } */
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
    wxString str = "Must fill in";
//    str.Printf("Num of Mnemonics: %d\nMemory allocated: %d bytes\nSyscalls = %d\nSysVars = %d\nSysIO = %d ",
//    process->Mnemonics->GetCount(), process->Mnemonics->GetAllocated(), process->sys_calls->GetCount(),
//    process->sys_vars->GetCount(), process->sys_io->GetCount());
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
    /*
    wxSize  psize,ldsize;
    wxPoint ldpos;
    int     w,h,x,y;
    */
    //SortedIntArray  entries(CompareSortedInt);
    bool *simulateexecution;

    simulateexecution = (bool *)event.GetClientData();
//    codeview_->Enable(false);
/*
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
*/
/*
    process_->DisassembleFirst(*simulateexecution);
    process_->InitData();
    process_->InsertLineLabelsInSourceCode();
    process_->prog_labels->SortAddress(true);
    process_->io_labels->SortAddress(true);
    process_->var_labels->SortAddress(true);
    process_->constant_labels->SortAddress(true);

    codeview_->Enable(true);
*/
    #ifdef IDZ80DEBUG
    wxString stemp;
    stemp.Printf("Used memory (dasmed)= %d bytes\n", process_->Disassembled->GetUsedMem());
    panel_log_->AppendText(stemp);

    panel_log_->AppendText(wxString::Format("Program Label = %d items, Var Label = %d items, IO Labels = %d items.\n",
                                          process_->prog_labels->GetCount(), process_->var_labels->GetCount(), process_->io_labels->GetCount()));
    #endif

//    delete GaugeLd;
//    codeview_->Plot();

    wxMenuBar *mb;
    mb = GetMenuBar();
    mb->Enable(idMenuToolsAutoLabel, true);
    mb->Enable(idMenuFileSave, true);
    mb->Enable(idMenuFileSaveAs, true);
    mb->Enable(idMenuToolsGenCode, true);

//    process_->Program->GetEntries(entries);

//    if (!entries.IsEmpty())
//        codeview_->CenterAddress(entries[0]);
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
        aui_mgr_->GetPane("MainWindow").Show();
    }
    else
    {
        mb->Check(idMenuViewDasmWindow,false);
        aui_mgr_->GetPane("MainWindow").Hide();
    }
    aui_mgr_->Update();
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
        aui_mgr_->GetPane("ProgLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewProgLabels, false);
        aui_mgr_->GetPane("ProgLabels").Hide();
    }
    aui_mgr_->Update();
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
        aui_mgr_->GetPane("VarLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewVarLabels,false);
        aui_mgr_->GetPane("VarLabels").Hide();
    }
    aui_mgr_->Update();
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
        aui_mgr_->GetPane("IOLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewIOLabels,false);
        aui_mgr_->GetPane("IOLabels").Hide();
    }
    aui_mgr_->Update();
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
        aui_mgr_->GetPane("ConstLabels").Show();
    }
    else
    {
        mb->Check(idMenuViewConstLabels,false);
        aui_mgr_->GetPane("ConstLabels").Hide();
    }
    aui_mgr_->Update();
}




void IDZ80::OnMenuToolAutoLabel(wxCommandEvent& event)
{
    /*
    process_->AutoLabel();
    process_->InsertLineLabelsInSourceCode();
    codeview_->ClearSelection();
    codeview_->Refresh();
    */
}



void IDZ80::OnMenuSettingsColor(wxCommandEvent& event)
{
    LogIt("Change colors !");
}


void IDZ80::OnMenuSettingsBlur(wxCommandEvent& event)
{
    LogIt("Event: Make Blur...");
//    codeview_->TestBlur();
}



bool IDZ80::SaveAs()
{
    wxString fname, caption, wildcard,
            defaultFilename = wxEmptyString;
    bool    ret;

    ret = false;
    caption = "Save project as";
    wildcard = "Project files (*.dap)|*.dap|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, fileopen_last_dir_, defaultFilename,wildcard,
                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    if (dialog.ShowModal() == wxID_OK)
    {
        fname = dialog.GetPath();
//        if (project_->Save(fname))
//            ret = true;
    }
    return ret;
}





void IDZ80::Clear_all()
{
//    codeview_->Clear();
//    codeview_->Enable(false);
//    process_->Clear();
}




void IDZ80::OnMenuFileSaveProject(wxCommandEvent& event)
{
/*
    if (project_->HasName())
    {
        if (!project_->Save())
            panel_log_->AppendText("Project NOT saved !\n");
        else
            panel_log_->AppendText("Project saved !\n");
    }
    else
        if (SaveAs())
        {
            UpdateTitle(project_->GetFilename());
        }
*/
}




void IDZ80::OnMenuFileSaveAsProject(wxCommandEvent& event)
{
/*
    if (SaveAs())
    {
        UpdateTitle(project_->GetFilename());
    }
*/
}




void IDZ80::OnMenuFileInfo(wxCommandEvent& event)
{
//    ShowFileInfo dialog(this);
//    dialog.SendInfo(process_);
//  dialog.ShowModal();
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
//    codeGenerator *bakeCode;
    topsizer->Add(textCode, 1, wxEXPAND | wxALL, 10);

    dasmwin->SetSizer(topsizer);

//    defaultFilename = project_->GetFilename() << ".mac";

    caption = "Save source code as";
    wildcard = "Source code files (*.mac)|*.mac|All files (*.*)|*.*";
    wxFileDialog dialog(this, caption, fileopen_last_dir_, defaultFilename,wildcard,
                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    if (dialog.ShowModal() == wxID_OK)
    {
        fname = dialog.GetPath();
/*        bakeCode = new codeGenerator(process_);
        //TODO: If viewing the final code is necessary. Replace with scintilla.
        //Since wxTextCtrl doesnt handle more than 32Kb, this is used only for debugging.
        textCode->AppendText(bakeCode->GenerateCode(fname, cfM80));
        dasmwin->ShowModal();

        delete bakeCode; */
    }
    dasmwin->Destroy();
}



void IDZ80::OnMenuToolMnemonicXML(wxCommandEvent& event)
{
    LogIt("Mnemonic XML!!");
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



