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

#include <wx/filefn.h>
#include <wx/filename.h>

#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>



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
const long IDZ80::idMenuToolsDasmAll = wxNewId();
const long IDZ80::idMenuToolsAutoLabel = wxNewId();
const long IDZ80::idMenuToolsGenCode = wxNewId();
const long IDZ80::idMenuMnemLoad = wxNewId();
const long IDZ80::idMenuMnemInfo = wxNewId();
const long IDZ80::idMenuHelpContents = wxNewId();
const long IDZ80::IdMenuHelpAbout = wxNewId();
const long IDZ80::ID_STATUSBAR1 = wxNewId();



const long IDZ80::ID_VARLABELPANE=wxNewId();

BEGIN_EVENT_TABLE(IDZ80,wxFrame)

END_EVENT_TABLE()

IDZ80::IDZ80(wxWindow* parent, wxArrayString &arraystr, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize s;
    wxPoint pt;
    //int i;


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
	AuiManager1->AddPane(PanelLog, wxAuiPaneInfo().Name("MessageLog").Caption(_("Message Log")).CaptionVisible().Bottom());
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
	MenuItem13 = new wxMenuItem(Menu1, idMenuFileSaveAs, _("&Save As...\tAlt+s"), wxEmptyString, wxITEM_NORMAL);
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
// test begin
	MenuItem11 = new wxMenuItem(Menu3, idMenuViewConstLabels, "Constant Labels", wxEmptyString, wxITEM_CHECK);
	Menu3->Append(MenuItem11);
// test end
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
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuMnemonicsLoad, this, idMenuMnemLoad);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuMnemonicsInfo, this, idMenuMnemInfo);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &IDZ80::OnMenuHelpAbout, this, IdMenuHelpAbout);
	Bind(wxEVT_IDLE, &IDZ80::OnFirstIdle, this);


    /*
     *  Get stored configuration
     */
    config = new wxConfig(_("IDZ80"));
    m_currentDir = LocalPath; // ::wxGetCwd();

    process = new ProcessData(this);
    codeview = new CodeView(this,process);
    m_project = new ProjectManager(process,codeview);

    AuiManager1->AddPane(codeview, wxAuiPaneInfo().Name("MainWindow").Caption("Disassembly Window").CaptionVisible().CenterPane().MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());
    AuiManager1->AddPane(process->var_labels,
    wxAuiPaneInfo().Name("VarLabels").Caption("Var Labels").CaptionVisible().Left().TopDockable(false).BottomDockable(false).MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());
    AuiManager1->AddPane(process->prog_labels,
    wxAuiPaneInfo().Name("ProgLabels").Caption("Program Labels").CaptionVisible().Left().TopDockable(false).BottomDockable(false).MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());
    AuiManager1->AddPane(process->io_labels,
    wxAuiPaneInfo().Name("IOLabels").Caption("IO Labels").CaptionVisible().Right().TopDockable(false).BottomDockable(false).MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());
    AuiManager1->AddPane(process->constant_labels,
    wxAuiPaneInfo().Name("ConstLabels").Caption("Constant Labels").CaptionVisible().Right().TopDockable(false).BottomDockable(false).MinSize(170,170).DockFixed().FloatingSize(170,170).Fixed());


    wxString cfg;
    if (config->Read("/AUI_Perspective",&cfg))
        AuiManager1->LoadPerspective(cfg, true);
    else

        AuiManager1->Update();

    if (!config->Read("/Lastdir", &m_lastDir))
            m_lastDir = m_currentDir;

    Maximize();

    process->SetLog(PanelLog);
    m_project->SetLog(PanelLog);
    codeview->DebugLog(PanelLog);
    LoadMnemonicsDB();

    // Enable/Check submenus
    wxMenuBar *mb;
    mb = GetMenuBar();
    mb->Enable(idMenuToolsDasmAll,false);
    mb->Enable(idMenuFileInfo,false);
    mb->Enable(idMenuToolsAutoLabel,false);
    mb->Check(idMenuViewDasmWindow,true);
    mb->Enable(idMenuFileSave,false);
    mb->Enable(idMenuFileSaveAs,false);
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

    codeview->Enable(false);
    /*
    m_commandline = arraystr;
    if (!m_commandline.IsEmpty())
	{
		PanelLog->AppendText(_("Command line arguments :\n"));
		for (i = 0; i < m_commandline.GetCount(); i++)
			PanelLog->AppendText(wxString::Format(_("-> %s <%s>\n"), m_commandline[i].c_str(), wxPathOnly(m_commandline[i])));

	}
    */
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
}







/*
 *	Process things after main window shows up
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

    StatusBar1->SetStatusText(m_currentDir);

	if (m_commandline.GetCount() > 1)
	{
		OpenProgramFile(m_commandline[1]);
	}


	PanelLog->AppendText(wxString::Format("byte = %d, word = %d, int = %d, long = %d, long long = %d\n", sizeof(byte), sizeof(word), sizeof(uint), sizeof(long), sizeof(long long)));
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
    }

    else
    {
        PanelLog->SetDefaultStyle(wxTextAttr(*wxRED));
        PanelLog->AppendText("Mnemonics FAILED !\n");
    }

    return ret;
}





bool IDZ80::OpenProgramFile(const wxString filename)
{
	bool		ret;
	wxString	info,
				caption;
	FileTypeDialog config(this);

	ret = process->Program->Open(filename);
	if (!ret)
	{
		caption.Printf("Could not open '%s' !", filename.c_str());
		wxMessageBox(caption, "Error...");
	}
	else
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

		config.SetData(*process->Program);
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

            /*FIXME: If user change the addresses, it won't appear here, because
             * they are not converted from textcntrl objects to the integer
             * variables of FileTypeDialog.
             */
            process->Program->StartAddress = config.GetStartAddress();
            process->Program->ExecAddress = config.GetExecAddress();
            process->Program->EndAddress = config.GetEndAddress();

            process->m_Dasm->AddOrgAddress(0, process->Program->StartAddress);

            m_project->New();
            if (config.cb_autodisassemble->IsChecked())
            {
                wxCommandEvent ev_dasm(wxEVT_COMMAND_MENU_SELECTED, idMenuToolsDasmAll);
                AddPendingEvent(ev_dasm);
            }
        }
        else
        {
            process->Program->Close();
            ret = false;
            StatusBar1->SetStatusText("Cancelled by user !");
        }
	}
    return ret;
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
		mb->Enable(idMenuFileSaveAs, true);
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

    int     debugint;
    SortedIntArray  debugarray(CompareSortedInt);


    if (event.GetId() == idMenuFileOpenArchive)
    {
        caption = "Choose a file";
        wildcard = "Program files (*.ROM, *.COM, *.BIN)|*.rom;*.com;*.bin|All files (*.*)|*.*";
    }
    else
        if (event.GetId() == idMenuFileOpenProject)
        {
            caption = "Choose a project";
            wildcard = "Project files (*.dap)|*.dap|All files (*.*)|*.*";
            project = true;
        }
        else
            unknown = true;

    if (!unknown)
    {
        wxFileDialog dialog(this, caption, m_lastDir, defaultFilename,wildcard, wxFD_OPEN);

        if (dialog.ShowModal() == wxID_OK)
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
            {
				Clear_all();
				if (OpenProgramFile(fname) && process->SetupSystemLabels())
                {
                    process->LoadSystemLabels("X:/idz80/Labels.txt");
                }

				if ((process->Program->isCartridge()) && (process->Program->GetEntries(debugarray)))
                    for(debugint = 0; debugint < debugarray.GetCount(); debugint++)
                    {
                        PanelLog->AppendText(wxString::Format("Entries item %d: %.4X\n", debugint, debugarray[debugint]));
                    }
			}

		}
        else
            StatusBar1->SetStatusText("Cancelled by user !");

	}
    else
        StatusBar1->SetStatusText("Unknown file !");
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
    process->DisassembleFirst();
    process->InitData();
    process->processLabel();

    codeview->Enable(true);

    #ifdef IDZ80DEBUG
    wxString stemp;
    stemp.Printf("Used memory (dasmed)= %d bytes\n", process->m_Dasm->GetUsedMem());
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
    if (event.pane->name == _("ConstLabels"))
        mb->Check(idMenuViewConstLabels,false);
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
        fname=dialog.GetPath();
        if (m_project->Save(fname,true))
            ret = true;
    }
    return ret;
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

    UpdateTitle(_(""));

    Clear_all();
}





void IDZ80::OnMenuToolsGenCode(wxCommandEvent& event)
{
    wxString fname, caption, wildcard,
            defaultFilename;

    wxDialog *dasmwin = new wxDialog(0, wxID_ANY, "Disassembled Code", wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU, _T("TextCodeBox"));
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl *textCode = new wxTextCtrl(dasmwin, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    codeGenerator *bakeCode;
    topsizer->Add(textCode, 1, wxEXPAND | wxALL,10);

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
		info << _(" - ") << str;
	SetTitle(info);
}



