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

#include "main_dialog.h"
#include "fileinfo_dialog.h"
#include "systemlabels.h"
#include "file_settings_dialog.h"
#include "mnemonicxmlfile.h"
#include "newproject_dialog.h"

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
const long IDZ80::IdMenuMnemonicXML = wxNewId();



IDZ80::IDZ80(wxWindow* parent, wxArrayString &arraystr)
{
    m_aui_mgr = 0;
    m_panel_log = 0;
    m_status_bar = 0;
    m_log_window = 0;
//    process_ = 0;
//    codeview_ = 0;
    m_config = 0;
//    project_ = 0;
	m_maximize_main_window = true;
//	CodeViewLines_ = 0;
    Disassembled_ = 0;
    Programs_ = 0;
    Mnemonics_ = 0;
    Labels_ = 0;

	Create(parent, wxID_ANY, "Interactive Disassembler Z80", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, "id");

    m_log_window = new LogWindow(this, "IDZ80 - Log Window");
    SetTextLog(m_log_window);
    ModuleName = "Main";

    SetupIcon();

	SetClientSize(wxSize(600,465));
	Move(wxDefaultPosition);

	SetupPanelLog();
	m_aui_mgr = new wxAuiManager(this, wxAUI_MGR_DEFAULT);
	m_aui_mgr->AddPane(m_panel_log, wxAuiPaneInfo().Name("MessageLog").Caption("Message Log").CaptionVisible().Bottom());
	m_aui_mgr->Update();

    SetupMenu();
    SetupStatusBar();

	Bind(wxEVT_IDLE, &IDZ80::OnFirstIdle, this);

}




IDZ80::~IDZ80()
{
    StoreConfiguration();
    m_aui_mgr->UnInit();
//    delete project_;
    delete m_config;
//    delete codeview_;
//    delete process_;
}




bool IDZ80::LoadMnemonicsDB()
{
    wxString s;
    bool ret = false;

    s = app_resource_dir_ + "\\z80_instructions.xml";

    m_panel_log->SetDefaultStyle(wxTextAttr(*wxBLACK));
    m_panel_log->AppendText("Opening mnemonic file:\n");
    m_panel_log->SetDefaultStyle(wxTextAttr(*wxRED));
    m_panel_log->AppendText(s);
    m_panel_log->AppendText("\n");

    if(Mnemonics_ == 0)
        Mnemonics_ = new MnemonicContainer(m_panel_log);
    else
        Mnemonics_->Clear();
    MnemonicXMLFile mnemonic(Mnemonics_, m_panel_log);

    ret = mnemonic.Open(s);

    if (ret)
    {
        m_panel_log->SetDefaultStyle(wxTextAttr(*wxBLACK));
        m_panel_log->AppendText("Mnemonics OK !\n");
        LogIt("Mnemonics OK !\n");
    }
    else
    {
        m_panel_log->SetDefaultStyle(wxTextAttr(*wxRED));
        m_panel_log->AppendText("Mnemonics FAILED !\n");
    }

    return ret;
}



void IDZ80::OpenProgramFile(wxString filename)
{
    NewProjectDialog dialog(this);
    
    if (dialog.ShowModal() == wxID_OK) {
        wxMenuBar *mb;
        mb = GetMenuBar();
        mb->Enable(idMenuToolsDasmAll, true);
        mb->Enable(idMenuFileInfo, true);
        mb->Enable(idMenuFileClose, true);
        mb->Enable(idMenuToolsGenCode, false);
        mb->Enable(idMenuToolsAutoLabel, false);

    }



    /* ----    OLD     ---------
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



void IDZ80::UpdateTitle(const wxString str)
{
	wxString info = GetTitle();
	info = info.BeforeFirst('-');
	info.Trim();
	if (!str.IsEmpty())
		info << " - " << str;
	SetTitle(info);
}



