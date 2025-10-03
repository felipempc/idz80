/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Main module - Setup and configuration routines
 **************************************************************/

#include <wx/dir.h>

#include "main_dialog.h"



/// @brief Create objects to each type of labels
void IDZ80::SetupLabels()
{
    Labels_->io_labels = new LabelListCtrl(this, IO_LIST,"PORT", log_window_);
    Labels_->var_labels = new LabelListCtrl(this, VAR_LIST,"VAR", log_window_);
    Labels_->prog_labels = new LabelListCtrl(this, PRG_LIST,"PROGRAM", log_window_);
    Labels_->constant_labels = new LabelListCtrl(this, CONST_LIST,"CONST", log_window_);
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

