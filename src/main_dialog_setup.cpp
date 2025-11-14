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
    m_labels->io_labels = new LabelListCtrl(this, IO_LIST,"PORT", m_log_window);
    m_labels->var_labels = new LabelListCtrl(this, VAR_LIST,"VAR", m_log_window);
    m_labels->prog_labels = new LabelListCtrl(this, PRG_LIST,"PROGRAM", m_log_window);
    m_labels->constant_labels = new LabelListCtrl(this, CONST_LIST,"CONST", m_log_window);
}



void IDZ80::SetupNotebook()
{
    wxSize client_size = GetClientSize();

    m_notebook = new wxAuiNotebook(this, wxID_ANY,
                                    wxPoint(client_size.x, client_size.y),
                                    FromDIP(wxSize(430,200)),
                                    wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_SCROLL_BUTTONS);
   m_notebook->Freeze();
   m_notebook->AddPage( /*new wxTextCtrl( m_notebook, wxID_ANY, "Some text",
                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER)*/ new wxPanel(this), "wxPanel 1", true );

   m_notebook->AddPage( new wxTextCtrl( m_notebook, wxID_ANY, "Some more text",
                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER) , "wxTextCtrl 2", true );

   m_notebook->AddPage( new wxTextCtrl( m_notebook, wxID_ANY, "Some more text",
                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER) , "wxTextCtrl 3" );

   m_notebook->AddPage( new wxTextCtrl( m_notebook, wxID_ANY, "Some more text",
                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER) , "wxTextCtrl 4", true );
   m_notebook->Thaw();
   m_notebook->Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED, IDZ80::OnAuiNotebookChanged, this);
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

    if (m_aui_mgr->GetPane("VarLabels").IsShown())
        mb->Check(idMenuViewVarLabels, true);
    else
        mb->Check(idMenuViewVarLabels, false);

    if (m_aui_mgr->GetPane("ProgLabels").IsShown())
        mb->Check(idMenuViewProgLabels, true);
    else
        mb->Check(idMenuViewProgLabels, false);

    if (m_aui_mgr->GetPane("IOLabels").IsShown())
        mb->Check(idMenuViewIOLabels, true);
    else
        mb->Check(idMenuViewIOLabels, false);

    if (m_aui_mgr->GetPane("ConstLabels").IsShown())
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
	m_status_bar = new wxStatusBar(this, wxID_ANY, 0, "ID_STATUSBAR");
	int __wxStatusBarWidths_1[1] = { -10 };
	int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
	m_status_bar->SetFieldsCount(1,__wxStatusBarWidths_1);
	m_status_bar->SetStatusStyles(1,__wxStatusBarStyles_1);
	SetStatusBar(m_status_bar);
}



void IDZ80::SetupPanelLog()
{
	m_panel_log = new wxTextCtrl(this, wxID_ANY, "", wxPoint(114,460), wxDefaultSize, wxTE_MULTILINE|wxTE_RICH, wxDefaultValidator, "MessageLog");

    wxFontInfo fontInfo(8);
    fontInfo.Family(wxFONTFAMILY_SWISS)
            .Style(wxFONTSTYLE_NORMAL)
            .Weight(wxFONTWEIGHT_NORMAL)
            .FaceName("Courier New");
    wxFont PanelLogFont(fontInfo);

	m_panel_log->SetFont(PanelLogFont);
}



void IDZ80::SetupAUIPanes()
{
//    if ((codeview_ != 0) && (Labels_ != 0))
    {
//        m_aui_mgr->AddPane(codeview_, wxAuiPaneInfo().Name("MainWindow").Caption("Disassembly Window").CaptionVisible().CenterPane().PaneBorder().MinSize(170,170)/*.DockFixed()*/.FloatingSize(170,170)/*.Fixed()*/);
        m_aui_mgr->AddPane(m_notebook, wxAuiPaneInfo().Name("MainWindow").Caption("Disassembly Window").CaptionVisible().CenterPane().PaneBorder().MinSize(170,170).FloatingSize(170,170));
        m_aui_mgr->AddPane(m_labels->var_labels,
        wxAuiPaneInfo().Name("VarLabels").Caption("Var Labels").CaptionVisible().Left()/*.TopDockable(false).BottomDockable(false)*/.PaneBorder().MinSize(170,170)/*.DockFixed()*/.FloatingSize(170,170)/*.Fixed()*/);
        m_aui_mgr->AddPane(m_labels->prog_labels,
        wxAuiPaneInfo().Name("ProgLabels").Caption("Program Labels").CaptionVisible().Left()/*.TopDockable(false).BottomDockable(false)*/.PaneBorder().MinSize(170,170)/*.DockFixed()*/.FloatingSize(170,170)/*.Fixed()*/);
        m_aui_mgr->AddPane(m_labels->io_labels,
        wxAuiPaneInfo().Name("IOLabels").Caption("IO Labels").CaptionVisible().Right()/*.TopDockable(false).BottomDockable(false)*/.PaneBorder().MinSize(170,170)/*.DockFixed()*/.FloatingSize(170,170)/*.Fixed()*/);
        m_aui_mgr->AddPane(m_labels->constant_labels,
        wxAuiPaneInfo().Name("ConstLabels").Caption("Constant Labels").CaptionVisible().Right()/*.TopDockable(false).BottomDockable(false)*/.PaneBorder().MinSize(170,170)/*.DockFixed()*/.FloatingSize(170,170)/*.Fixed()*/);

        SetupAUIStoredConfiguration();
    }
}



void IDZ80::SetupAUIStoredConfiguration()
{
    m_config = new wxConfig("IDZ80");

    wxString cfg;

    m_config->SetPath("/AUI");
    if (m_config->Read("Perspective", &cfg))
        m_aui_mgr->LoadPerspective(cfg, true);
    else
        m_aui_mgr->Update();
}



void IDZ80::SetupMenuEvents()
{
	m_aui_mgr->Bind(wxEVT_AUI_PANE_CLOSE, &IDZ80::OnAuiPaneClose, this);
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
    m_config = new wxConfig("IDZ80");
    wxString cfg;
    int    x, y, width, height;

    cfg = m_aui_mgr->SavePerspective();
    m_config->SetPath("/AUI");
    m_config->Write("Perspective", cfg);

    m_config->SetPath("/Directory");
    m_config->Write("Last", fileopen_last_dir_);

    GetSize(&width, &height);
    GetPosition(&x, &y);
    m_config->SetPath("/MainWindow");
    m_config->Write("X", x);
    m_config->Write("Y", y);
    m_config->Write("WIDTH", width);
    m_config->Write("HEIGHT", height);

    m_log_window->GetSize(&width, &height);
    m_log_window->GetPosition(&x, &y);
    m_config->SetPath("/LogWindow");
    m_config->Write("X", x);
    m_config->Write("Y", y);
    m_config->Write("WIDTH", width);
    m_config->Write("HEIGHT", height);
}



void IDZ80::ReadStoredConfiguration()
{
    long x, y, width, height;

    m_config = new wxConfig("IDZ80");

    m_config->SetPath("/Directory");
    if (m_config->Read("Last", &fileopen_last_dir_))
    {
        wxDir dir;
        if (!dir.Exists(fileopen_last_dir_))
            fileopen_last_dir_ = app_root_dir_;
    }
    else
        fileopen_last_dir_ = app_root_dir_;

    m_config->SetPath("/MainWindow");
    if (m_config->Read("X", &x) &&
        m_config->Read("Y", &y) &&
        m_config->Read("WIDTH", &width) &&
        m_config->Read("HEIGHT", &height))
    {
        SetPosition(wxPoint(x, y));
        SetSize(width, height);
        m_maximize_main_window = false;
    }

    m_config->SetPath("/LogWindow");
    if (m_config->Read("X", &x) &&
        m_config->Read("Y", &y) &&
        m_config->Read("WIDTH", &width) &&
        m_config->Read("HEIGHT", &height))
    {
        m_log_window->SetPosition(wxPoint(x, y));
        m_log_window->SetSize(width, height);
    }
}

